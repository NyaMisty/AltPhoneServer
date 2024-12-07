/*
 * Copyright (c) 2014,2015,2016,2017,2018 Apple Inc. All rights reserved.
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

/* Elliptic Curve Integrated Encryption Scheme implementation using AES-GCM for
 encryption/authentication.
 Terminology borrowed from http://www.secg.org/index.php?action=secg,docs_secg
   sharedinfo_1 is diversifier for the KDF:
        In original design of DHIES, it needs to be set to the ephemeral public 
            key to address malleability concerns, which are limited for ECIES.
        Even on ECIES, the mission of the public key appears to loosen the 
        security bounds of certain security proofs (cf p28 http://shoup.net/papers/iso-2_1.pdf)
        Use option ECIES_EPH_PUBKEY_IN_SHAREDINFO1 to achieve this.
        Still considered optional per standards SEC1 and x9.63
   sharedinfo_2 is diversifier for the MAC
        Potential security threat when attacker controled.
 */

#ifndef corecrypto_ccecies_h
#define corecrypto_ccecies_h

// bit mask 
#define ECIES_EPH_PUBKEY_IN_SHAREDINFO1      1
#define ECIES_EXPORT_PUB_STANDARD            2
#define ECIES_EXPORT_PUB_COMPACT             4
//#define ECIES_EXPORT_PUB_COMPRESSES        8 // not supported
#define ECIES_LEGACY_IV                      16

#include <corecrypto/cc.h>
#include <corecrypto/ccec.h>
#include <corecrypto/ccrng.h>
#include <corecrypto/ccmode.h>

typedef struct ccecies_gcm {
    const struct ccdigest_info *di;
    struct ccrng_state *rng;
    const struct ccmode_gcm *gcm;
    uint32_t key_length;
    uint32_t mac_length;
    uint32_t options; // bit mask
} *ccecies_gcm_t;

/* set the structure from the argument passed */
CC_NONNULL((1, 2, 3, 4))
void
ccecies_encrypt_gcm_setup(ccecies_gcm_t ecies,
                              const struct ccdigest_info *di,
                              struct ccrng_state *rng,
                              const struct ccmode_gcm *aes_gcm_enc,
                              uint32_t cipher_key_size,
                              uint32_t mac_tag_nbytes,
                              uint32_t options
                              );

/* Return the size of the ciphertext from ccecies_encrypt_aes_gcm
 It requires ecies to have been initialized with the setup function */
CC_NONNULL_TU((1)) CC_NONNULL((2))
size_t
ccecies_encrypt_gcm_ciphertext_size(ccec_pub_ctx_t public_key,
                                    ccecies_gcm_t ecies,
                                    size_t plaintext_nbytes
                              );

/* Encrypt using the provided public key and elliptic curve info
 It requires ecies to have been initialized with the setup function.
 ciphertext_nbytes must be at least "ccecies_encrypt_gcm_cipher_size" bytes 
 If ECIES_EPH_PUBKEY_IN_SHAREDINFO1 is set, sharedinfo1_nbytes is ignored */
CC_NONNULL_TU((1)) CC_NONNULL((2, 4, 9, 10))
int
ccecies_encrypt_gcm( ccec_pub_ctx_t public_key,
                         const ccecies_gcm_t ecies,
                         size_t plaintext_nbytes,   const uint8_t *plaintext,
                         size_t sharedinfo1_nbytes, const void *sharedinfo_1,
                         size_t sharedinfo2_nbytes, const void *sharedinfo_2,
                         size_t *encrypted_blob_nbytes,  uint8_t *encrypted_blob
                         );

/* set the structure from the argument passed */
CC_NONNULL((1, 2, 3))
void
ccecies_decrypt_gcm_setup(ccecies_gcm_t ecies,
                                   const struct ccdigest_info *di,
                                   const struct ccmode_gcm *aes_gcm_dec,
                                   uint32_t cipher_key_nbytes,
                                   uint32_t mac_tag_nbytes,
                                   uint32_t options
                                   );

/* Return the size of the plaintext output of ccecies_decrypt_aes_gcm
 It requires ecies to have been initialized with the setup function */
CC_NONNULL_TU((1)) CC_NONNULL((2))
size_t
ccecies_decrypt_gcm_plaintext_size(ccec_full_ctx_t full_key,
                                   ccecies_gcm_t ecies,
                                   size_t ciphertext_nbytes
                                   );


/* Decrypt using the provided private key and elliptic curve info
 It requires ecies to have been initialized with the setup function.
 ciphertext_nbytes must be at least "ccecies_encrypt_gcm_cipher_size" bytes
 If ECIES_EPH_PUBKEY_IN_SHAREDINFO1 is set, sharedinfo1_nbytes is ignored */
CC_NONNULL_TU((1)) CC_NONNULL((2, 4, 9, 10))
int
ccecies_decrypt_gcm(ccec_full_ctx_t full_key,
                        const ccecies_gcm_t ecies,
                        size_t encrypted_blob_nbytes,   const uint8_t *encrypted_blob,
                        size_t sharedinfo1_nbytes, const void *sharedinfo_1,
                        size_t sharedinfo2_nbytes, const void *sharedinfo_2,
                        size_t *plaintext_nbytes, uint8_t *plaintext
                        );




/* Return the size of the returned/expected exported public key */
CC_NONNULL_TU((1)) CC_NONNULL((2))
size_t
ccecies_pub_key_size(ccec_pub_ctx_t public_key,
                     ccecies_gcm_t ecies);

/* Encrypt using the provided public key and elliptic curve info
 It requires ecies to have been initialized with the setup function.
 ciphertext_nbytes must be at least "ccecies_encrypt_gcm_cipher_size" bytes
 If ECIES_EPH_PUBKEY_IN_SHAREDINFO1 is set, sharedinfo1_nbytes is ignored
 Composite because ciphertext, mac and publickey are separate output */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4, 5, 7))
int
ccecies_encrypt_gcm_composite(ccec_pub_ctx_t public_key,
                              const ccecies_gcm_t ecies,
                              uint8_t *exported_public_key, /* output - length from ccecies_pub_key_size */
                              uint8_t *ciphertext,          /* output - length same as plaintext_nbytes */
                              uint8_t *mac_tag,             /* output - length ecies->mac_length */
                              size_t plaintext_nbytes,   const uint8_t *plaintext,
                              size_t sharedinfo1_nbytes, const void *sharedinfo_1,
                              size_t sharedinfo2_nbytes, const void *sharedinfo_2
                              );

/* Decrypt using the provided private key and elliptic curve info
 It requires ecies to have been initialized with the setup function.
 ciphertext_nbytes must be at least "ccecies_encrypt_gcm_cipher_size" bytes
 If ECIES_EPH_PUBKEY_IN_SHAREDINFO1 is set, sharedinfo1_nbytes is ignored
 Composite because ciphertext, mac and publickey are separate input */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 10, 11))
int
ccecies_decrypt_gcm_composite(ccec_full_ctx_t full_key,
                              const ccecies_gcm_t ecies,
                              uint8_t *plaintext,           /* output - length same as ciphertext_nbytes */
                              size_t sharedinfo1_nbytes, const void *sharedinfo_1,
                              size_t sharedinfo2_nbytes, const void *sharedinfo_2,
                              size_t ciphertext_nbytes, const uint8_t *ciphertext,
                              const uint8_t *imported_public_key, /* expect length from ccecies_pub_key_size */
                              const uint8_t *mac_tag              /* expect length ecies->mac_nbytesgth */
                              );


#endif
