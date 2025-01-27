/*
 * Copyright (c) 2015,2016,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCMODE_SIV_H_
#define _CORECRYPTO_CCMODE_SIV_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccmode.h>
#include <corecrypto/ccmode_impl.h>

#include <corecrypto/cccmac.h>

/* This provide an implementation of SIV
 as specified in https://tools.ietf.org/html/rfc5297
 also in http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/siv/siv.pdf
 Counter Mode where IV is based on CMAC
 */

cc_aligned_struct(16) ccsiv_ctx;

struct ccmode_siv {
    size_t size;        /* first argument to ccsiv_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_siv *siv, ccsiv_ctx *ctx,
                 size_t key_len, const uint8_t *key);
    int (*set_nonce)(ccsiv_ctx *ctx,  size_t nbytes, const uint8_t *in);  // could just be ccm with NULL out
    int (*auth)(ccsiv_ctx *ctx,  size_t nbytes, const uint8_t *in);  // could just be ccm with NULL out
    int (*crypt)(ccsiv_ctx *ctx, size_t nbytes, const uint8_t *in, uint8_t *out);
    int (*reset)(ccsiv_ctx *ctx);
    const struct ccmode_cbc *cbc;
    const struct ccmode_ctr *ctr;
};

#define ccsiv_ctx_decl(_size_, _name_)  cc_ctx_decl(ccsiv_ctx, _size_, _name_)
#define ccsiv_ctx_clear(_size_, _name_) cc_clear(_size_, _name_)

// Functions

CC_INLINE size_t ccsiv_context_size(const struct ccmode_siv *mode)
{
    return mode->size;
}

CC_INLINE size_t ccsiv_block_size(const struct ccmode_siv *mode)
{
    return mode->block_size;
}

CC_INLINE size_t ccsiv_ciphertext_size(const struct ccmode_siv *mode,
                                       size_t plaintext_size)
{
    return plaintext_size+mode->cbc->block_size;
}

CC_INLINE size_t ccsiv_plaintext_size(const struct ccmode_siv *mode,
                                       size_t ciphertext_size)
{
    if (ciphertext_size<mode->cbc->block_size) {
        return 0; // error
    }
    return ciphertext_size-mode->cbc->block_size;
}

// Supported key sizes are 32, 48, 64 bytes
CC_INLINE int ccsiv_init(const struct ccmode_siv *mode, ccsiv_ctx *ctx,
                          size_t key_byte_len, const uint8_t *key)
{
    return mode->init(mode, ctx, key_byte_len, key);
}

// Process nonce. it is actually just an authenticated data
CC_INLINE int ccsiv_set_nonce(const struct ccmode_siv *mode, ccsiv_ctx *ctx,
                         size_t nbytes, const uint8_t *in)
{
    return mode->set_nonce(ctx, nbytes, in);
}

// Process authenticated data. Taken into account for authentication but not
// encrypted
CC_INLINE int ccsiv_aad(const struct ccmode_siv *mode, ccsiv_ctx *ctx,
                            size_t nbytes, const uint8_t *in)
{
    return mode->auth(ctx, nbytes, in);
}

// Encryption data. Authenticated and encrypted.
// Encrypt/Decrypt can only be called once
CC_INLINE int ccsiv_crypt(const struct ccmode_siv *mode, ccsiv_ctx *ctx,
                            size_t nbytes, const uint8_t *in, uint8_t *out)
{
    return mode->crypt(ctx, nbytes, in, out);
}

// Clear all context for reuse.
// Key is clear to avoid leaking it
CC_INLINE int ccsiv_reset(const struct ccmode_siv *mode, ccsiv_ctx *ctx)
{
    return mode->reset(ctx);
}

// One shot with only one vector of adata
CC_INLINE int ccsiv_one_shot(const struct ccmode_siv *mode,
                              size_t key_len, const uint8_t *key,
                              unsigned nonce_nbytes, const uint8_t* nonce,
                              unsigned adata_nbytes, const uint8_t* adata,
                              size_t in_nbytes, const uint8_t *in, uint8_t *out)
{
    int rc;
    ccsiv_ctx_decl(mode->size, ctx);
    rc=mode->init(mode, ctx, key_len, key);
    if (rc) {return rc;}
    rc=mode->set_nonce(ctx, nonce_nbytes, nonce);
    if (rc) {return rc;}
    rc=mode->auth(ctx, adata_nbytes, adata);
    if (rc) {return rc;}
    rc=mode->crypt(ctx, in_nbytes, in, out);
    if (rc) {return rc;}
    ccsiv_ctx_clear(mode->size, ctx);
    return rc;
}

void ccmode_factory_siv_encrypt(struct ccmode_siv *siv,
                                const struct ccmode_cbc *cbc,
                                const struct ccmode_ctr *ctr);

void ccmode_factory_siv_decrypt(struct ccmode_siv *siv,
                                const struct ccmode_cbc *cbc,
                                const struct ccmode_ctr *ctr);


#endif /* _CORECRYPTO_CCMODE_H_ */
