/*
 * Copyright (c) 2010,2011,2012,2014,2015,2016,2017,2018 Apple Inc. All rights reserved.
 *
 * corecrypto Internal Use License Agreement
 *
 * IMPORTANT:  This Apple corecrypto software is supplied to you by Apple Inc. ("Apple")
 * in consideration of your agreement to the following terms, and your download or use
 * of this Apple software constitutes acceptance of these terms.  If you do not agree
 * with these terms, please do not download or use this Apple software.
 *
 * 1.    As used in this Agreement, the term "Apple Software" collectively means and
 * includes all of the Apple corecrypto materials provided by Apple here, including
 * but not limited to the Apple corecrypto software, frameworks, libraries, documentation
 * and other Apple-created materials. In consideration of your agreement to abide by the
 * following terms, conditioned upon your compliance with these terms and subject to
 * these terms, Apple grants you, for a period of ninety (90) days from the date you
 * download the Apple Software, a limited, non-exclusive, non-sublicensable license
 * under Apple’s copyrights in the Apple Software to make a reasonable number of copies
 * of, compile, and run the Apple Software internally within your organization only on
 * devices and computers you own or control, for the sole purpose of verifying the
 * security characteristics and correct functioning of the Apple Software; provided
 * that you must retain this notice and the following text and disclaimers in all
 * copies of the Apple Software that you make. You may not, directly or indirectly,
 * redistribute the Apple Software or any portions thereof. The Apple Software is only
 * licensed and intended for use as expressly stated above and may not be used for other
 * purposes or in other contexts without Apple's prior written permission.  Except as
 * expressly stated in this notice, no other rights or licenses, express or implied, are
 * granted by Apple herein.
 *
 * 2.    The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES
 * OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING
 * THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS,
 * SYSTEMS, OR SERVICES. APPLE DOES NOT WARRANT THAT THE APPLE SOFTWARE WILL MEET YOUR
 * REQUIREMENTS, THAT THE OPERATION OF THE APPLE SOFTWARE WILL BE UNINTERRUPTED OR
 * ERROR-FREE, THAT DEFECTS IN THE APPLE SOFTWARE WILL BE CORRECTED, OR THAT THE APPLE
 * SOFTWARE WILL BE COMPATIBLE WITH FUTURE APPLE PRODUCTS, SOFTWARE OR SERVICES. NO ORAL
 * OR WRITTEN INFORMATION OR ADVICE GIVEN BY APPLE OR AN APPLE AUTHORIZED REPRESENTATIVE
 * WILL CREATE A WARRANTY.
 *
 * 3.    IN NO EVENT SHALL APPLE BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING
 * IN ANY WAY OUT OF THE USE, REPRODUCTION, COMPILATION OR OPERATION OF THE APPLE
 * SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING
 * NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * 4.    This Agreement is effective until terminated. Your rights under this Agreement will
 * terminate automatically without notice from Apple if you fail to comply with any term(s)
 * of this Agreement.  Upon termination, you agree to cease all use of the Apple Software
 * and destroy all copies, full or partial, of the Apple Software. This Agreement will be
 * governed and construed in accordance with the laws of the State of California, without
 * regard to its choice of law rules.
 *
 * You may report security issues about Apple products to product-security@apple.com,
 * as described here:  https://www.apple.com/support/security/.  Non-security bugs and
 * enhancement requests can be made via https://bugreport.apple.com as described
 * here: https://developer.apple.com/bug-reporting/
 *
 * EA1350
 * 10/5/15
 */

#ifndef _CORECRYPTO_CCHMAC_H_
#define _CORECRYPTO_CCHMAC_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccdigest.h>

/* An hmac_ctx_t is normally allocated as an array of these. */
struct cchmac_ctx {
    uint8_t b[8];
} CC_ALIGNED(8);

#if CORECRYPTO_USE_TRANSPARENT_UNION
typedef union {
    struct cchmac_ctx *hdr;
    ccdigest_ctx_t digest;
} cchmac_ctx_t __attribute__((transparent_union));
#else
typedef struct cchmac_ctx* cchmac_ctx_t;
#endif

#define cchmac_ctx_size(STATE_SIZE, BLOCK_SIZE)  (ccdigest_ctx_size(STATE_SIZE, BLOCK_SIZE) + (STATE_SIZE))
#define cchmac_di_size(_di_)  (cchmac_ctx_size((_di_)->state_size, (_di_)->block_size))

#define cchmac_ctx_n(STATE_SIZE, BLOCK_SIZE)  ccn_nof_size(cchmac_ctx_size((STATE_SIZE), (BLOCK_SIZE)))

#define cchmac_ctx_decl(STATE_SIZE, BLOCK_SIZE, _name_) cc_ctx_decl(struct cchmac_ctx, cchmac_ctx_size(STATE_SIZE, BLOCK_SIZE), _name_)
#define cchmac_ctx_clear(STATE_SIZE, BLOCK_SIZE, _name_) cc_clear(cchmac_ctx_size(STATE_SIZE, BLOCK_SIZE), _name_)
#define cchmac_di_decl(_di_, _name_) cchmac_ctx_decl((_di_)->state_size, (_di_)->block_size, _name_)
#define cchmac_di_clear(_di_, _name_) cchmac_ctx_clear((_di_)->state_size, (_di_)->block_size, _name_)

/* Return a ccdigest_ctx_t which can be accesed with the macros in ccdigest.h */
#if CORECRYPTO_USE_TRANSPARENT_UNION
#define cchmac_digest_ctx(_di_, HC)    (((cchmac_ctx_t)(HC)).digest)
#else
#define cchmac_digest_ctx(_di_, HC)    ((ccdigest_ctx_t)(HC))
#endif

/* Accesors for ostate fields, this is all cchmac_ctx_t adds to the ccdigest_ctx_t. */
#if CORECRYPTO_USE_TRANSPARENT_UNION
#define cchmac_ostate(_di_, HC)    ((struct ccdigest_state *)(((cchmac_ctx_t)(HC)).hdr->b + ccdigest_di_size(_di_)))
#else
#define cchmac_ostate(_di_, HC)    ((struct ccdigest_state *)(((cchmac_ctx_t)(HC))->b + ccdigest_di_size(_di_)))
#endif
#define cchmac_ostate8(_di_, HC)   (ccdigest_u8(cchmac_ostate(_di_, HC)))
#define cchmac_ostate32(_di_, HC)  (ccdigest_u32(cchmac_ostate(_di_, HC)))
#define cchmac_ostate64(_di_, HC)  (ccdigest_u64(cchmac_ostate(_di_, HC)))
#define cchmac_ostateccn(_di_, HC) (ccdigest_ccn(cchmac_ostate(_di_, HC)))

/* Convenience accessors for ccdigest_ctx_t fields. */
#if CORECRYPTO_USE_TRANSPARENT_UNION
#define cchmac_istate(_di_, HC)    ccdigest_state(_di_, ((cchmac_ctx_t)(HC)).digest)
#else
#define cchmac_istate(_di_, HC)    ccdigest_state(_di_, ((ccdigest_ctx_t)(HC)))
#endif
#define cchmac_istate8(_di_, HC)   ccdigest_u8(cchmac_istate(_di_, HC))
#define cchmac_istate32(_di_, HC)  ccdigest_u32(cchmac_istate(_di_, HC))
#define cchmac_istate64(_di_, HC)  ccdigest_u64(cchmac_istate(_di_, HC))
#define cchmac_istateccn(_di_, HC) ccdigest_ccn(cchmac_istate(_di_, HC))

#if CORECRYPTO_USE_TRANSPARENT_UNION
#define cchmac_data(_di_, HC)      ccdigest_data(_di_, ((cchmac_ctx_t)(HC)).digest)
#define cchmac_num(_di_, HC)       ccdigest_num(_di_, ((cchmac_ctx_t)(HC)).digest)
#define cchmac_nbits(_di_, HC)     ccdigest_nbits(_di_, ((cchmac_ctx_t)(HC)).digest)
#else
#define cchmac_data(_di_, HC)      ccdigest_data(_di_, ((ccdigest_ctx_t)(HC)))
#define cchmac_num(_di_, HC)       ccdigest_num(_di_, ((ccdigest_ctx_t)(HC)))
#define cchmac_nbits(_di_, HC)     ccdigest_nbits(_di_, ((ccdigest_ctx_t)(HC)))
#endif

void cchmac_init(const struct ccdigest_info *di, cchmac_ctx_t ctx,
                 size_t key_len, const void *key);
void cchmac_update(const struct ccdigest_info *di, cchmac_ctx_t ctx,
                   size_t data_len, const void *data);
void cchmac_final(const struct ccdigest_info *di, cchmac_ctx_t ctx,
                  unsigned char *mac);

void cchmac(const struct ccdigest_info *di, size_t key_len,
            const void *key, size_t data_len, const void *data,
            unsigned char *mac);

/* Test functions */

struct cchmac_test_input {
    const struct ccdigest_info *di;
    size_t key_len;
    const void *key;
    size_t data_len;
    const void *data;
    size_t mac_len;
    const void *expected_mac;
};

int cchmac_test(const struct cchmac_test_input *input);
int cchmac_test_chunks(const struct cchmac_test_input *input, size_t chunk_size);


#endif /* _CORECRYPTO_CCHMAC_H_ */
