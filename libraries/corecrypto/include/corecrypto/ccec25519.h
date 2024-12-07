/*
 * Copyright (c) 2014,2015,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCEC25519_H_
#define _CORECRYPTO_CCEC25519_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccrng.h>
#include <corecrypto/ccdigest.h>

typedef uint8_t ccec25519key[32];
typedef ccec25519key ccec25519secretkey;
typedef ccec25519key ccec25519pubkey;
typedef ccec25519key ccec25519base;

typedef uint8_t ccec25519signature[64];

/*!
    @function		cccurve25519
    @abstract		Perform Curve25519 Diffie-Hellman.
                    http://cr.yp.to/ecdh.html

    @param      out  Output shared secret or public key.
    @param      sk   Input secret key.
    @param      base Input basepoint (for computing a shared secret) 
                     or NULL (for computing a public key).
 */

void cccurve25519(ccec25519key out,
                  const ccec25519secretkey sk,
                  const ccec25519base base);

/*!
    @function	cccurve25519_make_priv
    @abstract	Generates a random, montgomery curve 25519 private key.

    @param      rng  An initialized random number generator
    @param      sk	 Receives 32-byte secret key.
 */

CC_INLINE void cccurve25519_make_priv(struct ccrng_state *rng,
                                      ccec25519secretkey sk) {
    ccrng_generate(rng,32,sk);
    sk[0] &= 248;
    sk[31] &= 127;
    sk[31] |= 64;
}

/*!
    @function	cccurve25519_make_pub
    @abstract	Creates a montgomery curve 25519 public key from a private key.

    @param      pk	Receives 32-byte public key.
    @param      sk	Receives 32-byte secret key.
 */

CC_INLINE void cccurve25519_make_pub(ccec25519pubkey pk,
                                     const ccec25519secretkey sk) {
    cccurve25519(pk, sk, NULL);
}

/*!
	@function	cccurve25519_make_key_pair
	@abstract	Generates a random, montgomery curve 25519 key pair.

    @param      rng  An initialized random number generator
	@param      pk	Receives 32-byte public key.
	@param      sk	Receives 32-byte secret key.
 */

CC_INLINE void cccurve25519_make_key_pair(struct ccrng_state *rng,
                                          ccec25519pubkey pk,
                                          ccec25519secretkey sk) {
    cccurve25519_make_priv(rng, sk);
    cccurve25519_make_pub(pk, sk);
}


/*!
	@function	cced25519_make_key_pair
	@abstract	Generates a random, Ed25519 key pair.

    @param      di   A valid descriptor for a 512 bit hash function for the platform
    @param      rng  An initialized random number generator
	@param      pk	Receives 32-byte public key.
	@param      sk	Receives 32-byte secret key.
 */
void cced25519_make_key_pair(const struct ccdigest_info *di,
                             struct ccrng_state *rng,
                             ccec25519pubkey pk,
                             ccec25519secretkey sk);

/*!
	@function	cced25519_sign
	@abstract	Signs a message using a secret key.
	
    @param      di      A valid descriptor for a 512 bit hash function for the platform
	@param		sig		Receives the 64-byte signature.
	@param		len		Number of bytes to sign.
	@param		msg		Data to sign.
	@param		pk		32-byte public key as generated by cced25519_make_key_pair().
	@param		sk		32-byte secret key as generated by cced25519_make_key_pair().
*/
void cced25519_sign(const struct ccdigest_info *di,
                    ccec25519signature sig,
                    size_t len, const void *msg,
                    const ccec25519pubkey pk,
                    const ccec25519secretkey sk);

/*!
	@function	cced25519_verify
	@abstract	Verifies a signed message using a public key.
	
    @param      di      A valid descriptor for a 512 bit hash function for the platform
	@param		len		Number of bytes of data to verify.
	@param		msg		Data to verify.
	@param		sig		64-byte signature to verify data against.
	@param		pk		32-byte public key. Should have been generated by the peer using
                        cced25519_make_key_pair().
	
	@result		0=Signed message is valid. Non-zero=Bad message.
*/
int cced25519_verify(const struct ccdigest_info *di,
                     size_t len, const void *msg,
                     const ccec25519signature sig,
                     const ccec25519pubkey pk);

#endif /* _CORECRYPTO_CCEC25519_H_ */
