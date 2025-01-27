/*
 * Copyright (c) 2015,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCMODE_SIV_PRIV_H_
#define _CORECRYPTO_CCMODE_SIV_PRIV_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccn.h>
#include <corecrypto/ccmode_siv.h>

#include <corecrypto/cccmac.h>

// Maximum size for the key is 512
#define CCSIV_MAX_BLOCK_BYTESIZE 128/8

// Maximum size for the key is 512
#define CCSIV_MAX_KEY_BYTESIZE   512/8

struct _ccmode_siv_ctx {
    const struct ccmode_siv *siv;
    size_t  key_bytesize;
    cc_unit state;
    cc_unit k1[ccn_nof_size(CCSIV_MAX_KEY_BYTESIZE/2)]; // cmac key
    cc_unit k2[ccn_nof_size(CCSIV_MAX_KEY_BYTESIZE/2)]; // ctr key
    cc_unit block[ccn_nof_size(CCSIV_MAX_BLOCK_BYTESIZE)];
};

int ccmode_siv_init(const struct ccmode_siv *siv, ccsiv_ctx *ctx,
                    size_t rawkey_byte_len, const uint8_t *rawkey);

// Authentication of the adata
int ccmode_siv_auth(ccsiv_ctx *ctx,
                    size_t nbytes, const uint8_t *in);

// Authentication of the last vector (the encrypted part)
int ccmode_siv_auth_last(ccsiv_ctx *ctx,
                         size_t nbytes, const uint8_t *in, uint8_t* V);

int ccmode_siv_encrypt(ccsiv_ctx *ctx,
                       size_t nbytes, const uint8_t *in, uint8_t *out);

int ccmode_siv_decrypt(ccsiv_ctx *ctx,
                       size_t nbytes, const uint8_t *in, uint8_t *out);

int ccmode_siv_reset(ccsiv_ctx *ctx);

/* Macros for accessing a CCMODE_SIV.
 {
 const struct ccmode_siv *siv;
 uint8_t key[512/8];
 uint8_t d[512/8];
 cc_unit cmac_ctx[cbc->n];
 cc_unit ctr_ctx[ctr->n];
 } */
#define _CCMODE_SIV_CTX(K) ((struct _ccmode_siv_ctx *)(K))
#define _CCMODE_SIV_CBC_MODE(K) (_CCMODE_SIV_CTX(K)->siv->cbc)
#define _CCMODE_SIV_CTR_MODE(K) (_CCMODE_SIV_CTX(K)->siv->ctr)
#define _CCMODE_SIV_STATE(K)    (_CCMODE_SIV_CTX(K)->state)
#define _CCMODE_SIV_KEYSIZE(K)  (_CCMODE_SIV_CTX(K)->key_bytesize)
#define _CCMODE_SIV_K1(K)       ((uint8_t *)_CCMODE_SIV_CTX(K)->k1)
#define _CCMODE_SIV_K2(K)       ((uint8_t *)_CCMODE_SIV_CTX(K)->k2)
#define _CCMODE_SIV_D(K)        ((uint8_t *)_CCMODE_SIV_CTX(K)->block)





#endif /* _CORECRYPTO_CCMODE_SIV_PRIV_H_ */
