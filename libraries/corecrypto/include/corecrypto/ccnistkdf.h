/*
 * Copyright (c) 2013,2015,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCNISTKDF_H_
#define _CORECRYPTO_CCNISTKDF_H_

#include <corecrypto/ccdigest.h>

/*! @function ccnistkdf_ctr_hmac
 
 @abstract          Perform a NIST SP800-108 KDF in Counter Mode with an HMAC PRF. 
                    http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf
 @discussion        This performs the transformation of password and salt through
                    an HMAC PRF of the callers slection (any Digest, typically SHA-256) 
                    returning dkLen bytes containing the entropy.
 
 
 @param prf         Pseudo-random function to be used
 @param kdkLen      Key Derivation Key Length
 @param kdk         Key derivation key, a key that is used as an input to a key derivation function
                    (along with other input data) to derive keying material.
 @param labelLen  	label length
 @param label	    A string that identifies the purpose for the derived keying material,
                    which is encoded as a binary string. The encoding method for the Label
                    is defined in a larger context, for example, in the protocol that uses a KDF.
 @param contextLen	context length
 @param context     A binary string containing the information related to the derived keying material.
                    It may include identities of parties who are deriving and/or using the derived
                    keying material and, optionally, a nonce known by the parties who derive the keys.
 @param dkLen       Derived Key Length
 @param dk          buffer for the results of the KDF transformation, must be dkLen big

 */

int ccnistkdf_ctr_hmac(const struct ccdigest_info *di,
                       size_t kdkLen, const void *kdk,
                       size_t labelLen, const void *label,
                       size_t contextLen, const void *context,
                       size_t dkLen, void *dk);

int ccnistkdf_ctr_hmac_fixed(const struct ccdigest_info *di,
                             size_t kdkLen, const void *kdk,
                             size_t fixedDataLen, const void *fixedData,
                             size_t dkLen, void *dk);

/*! @function ccnistkdf_fb_hmac
 
 @abstract          Perform a NIST SP800-108 KDF in Feedback Mode with an HMAC PRF. 
                    http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf
 
 @param di          digest info defining the digest type to use in the PRF.
 @param use_counter determines whether a counter will be used for each round through the PRF (0 = off).
 @param kdkLen      Key Derivation Key Length
 @param kdk         Key derivation key, a key that is used as an input to a key derivation function
                    (along with other input data) to derive keying material.
 @param labelLen  	label length
 @param label	    A string that identifies the purpose for the derived keying material,
                    which is encoded as a binary string. The encoding method for the Label
                    is defined in a larger context, for example, in the protocol that uses a KDF.
 @param contextLen	context length
 @param context     A binary string containing the information related to the derived keying material.
                    It may include identities of parties who are deriving and/or using the derived
                    keying material and, optionally, a nonce known by the parties who derive the keys.
 @param ivLen       iv length
 @param iv          A binary string that is used as an initial value in computing the first iteration in
                    the feedback mode. It can be either public or secret. It may be an empty string.
 @param dkLen       Derived Key Length
 @param dk          buffer for the results of the KDF transformation, must be dkLen big
 
 */

int ccnistkdf_fb_hmac(const struct ccdigest_info *di, int use_counter,
                      size_t kdkLen, const void *kdk,
                      size_t labelLen, const void *label,
                      size_t contextLen, const void *context,
                      size_t ivLen, const void *iv,
                      size_t dkLen, void *dk);

int ccnistkdf_fb_hmac_fixed(CC_UNUSED const struct ccdigest_info *di, int use_counter,
                            CC_UNUSED size_t kdkLen, CC_UNUSED const void *kdk,
                            CC_UNUSED size_t fixedDataLen, CC_UNUSED const void *fixedData,
                            CC_UNUSED size_t ivLen, CC_UNUSED const void *iv,
                            CC_UNUSED size_t dkLen, CC_UNUSED void *dk);

/*! @function ccnistkdf_dpi_hmac
 
 @abstract          Perform a NIST SP800-108 KDF in Double Pipeline Iteration Mode with an HMAC PRF.
                    http://csrc.nist.gov/publications/nistpubs/800-108/sp800-108.pdf
 
 @param di          digest info defining the digest type to use in the PRF.
 @param kdkLen		Key Derivation Key Length
 @param kdk         Key derivation key, a key that is used as an input to a key derivation function
                    (along with other input data) to derive keying material.
 @param labelLen  	label length
 @param label	    A string that identifies the purpose for the derived keying material,
                    which is encoded as a binary string. The encoding method for the Label
                    is defined in a larger context, for example, in the protocol that uses a KDF.
 @param contextLen	context length
 @param context     A binary string containing the information related to the derived keying material.
                    It may include identities of parties who are deriving and/or using the derived
                    keying material and, optionally, a nonce known by the parties who derive the keys.
 @param dkLen       Derived Key Length
 @param dk          buffer for the results of the KDF transformation, must be dkLen big
 
 */

int ccnistkdf_dpi_hmac(const struct ccdigest_info *di,
                       size_t kdkLen, const void *kdk,
                       size_t labelLen, const void *label,
                       size_t contextLen, const void *context,
                       size_t dkLen, void *dk);


#endif /* _CORECRYPTO_CCNISTKDF_H_ */
