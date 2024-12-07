/*
 * Copyright (c) 2013,2014,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_cccmac_H_
#define _CORECRYPTO_cccmac_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccmode.h>
#include <corecrypto/ccaes.h>

#define CMAC_BLOCKSIZE   16

#if CORECRYPTO_USE_TRANSPARENT_UNION
struct cccmac_ctx {
    uint8_t b[8];
} CC_ALIGNED(8);

typedef struct cccmac_ctx_hdr {
    uint8_t k1[CMAC_BLOCKSIZE];
    uint8_t k2[CMAC_BLOCKSIZE];
    uint8_t block[CMAC_BLOCKSIZE];
    size_t  block_nbytes;      // Number of byte occupied in block buf
    size_t  cumulated_nbytes;  // Total size processed
    const struct ccmode_cbc *cbc;
    uint8_t ctx[8];
} CC_ALIGNED(8) cccmac_ctx_hdr;


typedef union {
    struct cccmac_ctx *b;
    cccmac_ctx_hdr *hdr;
} cccmac_ctx_t __attribute__((transparent_union));
#define cccmac_hdr_size sizeof(struct cccmac_ctx_hdr)

#else

struct cccmac_ctx {
    uint8_t k1[CMAC_BLOCKSIZE];
    uint8_t k2[CMAC_BLOCKSIZE];
    uint8_t block[CMAC_BLOCKSIZE];
    size_t  block_nbytes; // Number of byte occupied in block
    size_t  cumulated_nbytes;  // Total size processed
    const struct ccmode_cbc *cbc;
    uint8_t ctx[8];
} CC_ALIGNED(8);// cccmac_ctx_hdr;

typedef struct cccmac_ctx* cccmac_ctx_t;

#define cccmac_hdr_size sizeof(struct cccmac_ctx)

#endif


#define cccmac_iv_size(_mode_)  ((_mode_)->block_size)
#define cccmac_cbc_size(_mode_) ((_mode_)->size)

#define cccmac_ctx_size(_mode_) (cccmac_hdr_size + cccmac_iv_size(_mode_) + cccmac_cbc_size(_mode_))
#define cccmac_ctx_n(_mode_)  ccn_nof_size(cccmac_ctx_size(_mode_))

#define cccmac_mode_decl(_mode_, _name_) cc_ctx_decl(struct cccmac_ctx, cccmac_ctx_size(_mode_), _name_)
#define cccmac_mode_clear(_mode_, _name_) cc_clear(cccmac_ctx_size(_mode_), _name_)

#if CORECRYPTO_USE_TRANSPARENT_UNION
/* Return a cccbc_ctx * which can be accesed with the macros in ccmode.h */
#define cccmac_mode_ctx_start(_mode_, HC)     (((HC).hdr)->ctx)
#define CCCMAC_HDR(HC)      (((cccmac_ctx_t)(HC)).hdr)
#else
/* Return a cccbc_ctx * which can be accesed with the macros in ccmode.h */
#define cccmac_mode_ctx_start(_mode_, HC)    (HC->ctx)
#define CCCMAC_HDR(HC)      (HC)
#endif

#define cccmac_mode_sym_ctx(_mode_, HC)     (cccbc_ctx *)(cccmac_mode_ctx_start(_mode_, HC))
#define cccmac_mode_iv(_mode_, HC)     (cccbc_iv *)(cccmac_mode_ctx_start(_mode_, HC)+cccmac_cbc_size(_mode_))
#define cccmac_k1(HC)       (CCCMAC_HDR(HC)->k1)
#define cccmac_k2(HC)       (CCCMAC_HDR(HC)->k2)
#define cccmac_block(HC)    (CCCMAC_HDR(HC)->block)
#define cccmac_cbc(HC)      (CCCMAC_HDR(HC)->cbc)
#define cccmac_block_nbytes(HC)        (CCCMAC_HDR(HC)->block_nbytes)
#define cccmac_cumulated_nbytes(HC)    (CCCMAC_HDR(HC)->cumulated_nbytes)


/* CMAC as defined in NIST SP800-38B - 2005 */

/* =============================================================================

                                ONE SHOT

 ==============================================================================*/

/*!
 @function   cccmac_one_shot_generate
 @abstract   CMAC generation in one call

 @param   cbc          CBC and block cipher specification
 @param   key_nbytes   Length of the key in bytes
 @param   key          Pointer to the key of length key_nbytes
 @param   data_nbytes  Length of the data in bytes
 @param   data         Pointer to the data in bytes
 @param   mac_nbytes   Length in byte of the mac, > 0
 @param   mac          Output of length cbc->block_size

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */
int cccmac_one_shot_generate(const struct ccmode_cbc *cbc,
                        size_t key_nbytes, const void *key,
                        size_t data_nbytes, const void *data,
                        size_t mac_nbytes, void *mac);

/*!
 @function   cccmac_one_shot_verify
 @abstract   CMAC verification in one call

 @param   cbc          CBC and block cipher specification
 @param   key_nbytes  Length of the key in bytes
 @param   key          Pointer to the key of length key_nbytes
 @param   data_nbytes Length of the data in bytes
 @param   data         Pointer to the data in bytes
 @param   expected_mac_nbytes  Length in byte of the mac, > 0
 @param   expected_mac Mac value expected

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */
int cccmac_one_shot_verify(const struct ccmode_cbc *cbc,
                           size_t key_nbytes, const void *key,
                           size_t data_nbytes, const void *data,
                           size_t expected_mac_nbytes, const void *expected_mac);

/* =============================================================================

                               STREAMING
 
                        Init - Update - Final

==============================================================================*/

/*!
 @function   cccmac_init
 @abstract   Init CMAC context with CBC mode and key

 @param   cbc         CBC and block cipher specification
 @param   ctx         Context use to store internal state
 @param   key_nbytes  Length of the key in bytes
 @param   key         Full key

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */

int cccmac_init(const struct ccmode_cbc *cbc,
                cccmac_ctx_t ctx,
                size_t key_nbytes, const void *key);

/*!
 @function   cccmac_update
 @abstract   Process data

 @param   ctx          Context use to store internal state
 @param   data_nbytes Length in byte of the data
 @param   data         Data to process

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */

int cccmac_update(cccmac_ctx_t ctx,
                  size_t data_nbytes, const void *data);

/*!
 @function   cccmac_final_generate
 @abstract   Final step for generation

 @param   ctx          Context use to store internal state
 @param   mac_nbytes   Length in byte of the mac, > 0
 @param   mac          Output of length mac_nbytes

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */
int cccmac_final_generate(cccmac_ctx_t ctx,
                     size_t mac_nbytes, void *mac);

/*!
 @function   cccmac_final_verify
 @abstract   Final step and verification

 @param   ctx          Context use to store internal state
 @param   expected_mac_nbytes  Length in byte of the mac, > 0
 @param   expected_mac Mac value expected

 @result     0 iff successful.

 @discussion Only supports CMAC_BLOCKSIZE block ciphers
 */
int cccmac_final_verify(cccmac_ctx_t ctx,
                        size_t expected_mac_nbytes, const void *expected_mac);

#endif /* _CORECRYPTO_cccmac_H_ */
