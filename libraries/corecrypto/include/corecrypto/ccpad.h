/*
 * Copyright (c) 2010,2011,2012,2014,2015,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCPAD_H_
#define _CORECRYPTO_CCPAD_H_

#include <corecrypto/ccmode.h>

// CTS1,2,3 are defined in Addendum to 800-38A,
// "Cipher Modes of Operation: Three Variants of Ciphertext Stealing for CBC Mode"
// CTS3 is also known as "CTS" in RFC3962

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts1_decrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts1_encrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);
/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts2_decrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts2_encrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);
/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts3_decrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_cts3_encrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                       size_t nbytes, const void *in, void *out);

/* Contract is nbytes is non zero and a multiple of block_size. Furthermore in is nbytes long and out is nbytes long.  Returns number of bytes written to out (technically we always write nbytes to out but the returned value is the number of bytes decrypted after removal of padding.

    To be safe we remove the entire offending block if the pkcs7 padding checks failed.  However we purposely don't report the failure to decode the padding since any use of this error leads to potential security exploits.  So currently there is no way to distinguish between a full block of padding and bad padding.
 */
size_t ccpad_pkcs7_decrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                           size_t nbytes, const void *in, void *out);

/* Contract is in is nbytes long.  Writes (nbytes / block_size) + 1 times block_size to out.  In other words, out must be nbytes rounded down to the closest multiple of block_size plus block_size bytes. */
size_t ccpad_pkcs7_encrypt(const struct ccmode_cbc *cbc, cccbc_ctx *ctx, cccbc_iv *iv,
                         size_t nbytes, const void *in, void *out);

/* Contract is 'don't break CommonCrypto functionality that allows PKCS7 padding with ECB mode'.  This is basically the same routines above, without an IV, because calling
   crypt with an IV makes ecb cry (and crash) */

size_t ccpad_pkcs7_ecb_decrypt(const struct ccmode_ecb *ecb, ccecb_ctx *ecb_key,
                               size_t nbytes, const void *in, void *out);

size_t ccpad_pkcs7_ecb_encrypt(const struct ccmode_ecb *ecb, ccecb_ctx *ctx,
                             size_t nbytes, const void *in, void *out);

/* Function common to ccpad_pkcs7_ecb_decrypt and ccpad_pkcs7_decrypt */
size_t ccpad_pkcs7_decode(const size_t block_size, const uint8_t* last_block);

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
size_t ccpad_xts_decrypt(const struct ccmode_xts *xts, ccxts_ctx *ctx, ccxts_tweak *tweak,
                       size_t nbytes, const void *in, void *out);

/* Contract is nbytes is at least 1 block + 1 byte.  Also in is nbytes long out is nbytes long. */
void ccpad_xts_encrypt(const struct ccmode_xts *xts, ccxts_ctx *ctx, ccxts_tweak *tweak,
                       size_t nbytes, const void *in, void *out);

#endif /* _CORECRYPTO_CCPAD_H_ */
