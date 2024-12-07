/*
 * Copyright (c) 2012,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCWRAP_H_
#define _CORECRYPTO_CCWRAP_H_

#include <stdint.h>

#include <corecrypto/ccmode.h>

#define CCWRAP_IV 0xA6A6A6A6A6A6A6A6
#define CCWRAP_SEMIBLOCK 8

// chosen somewhat arbitrarily
// corresponds to 65536 bytes of key material and one eight-byte IV
#define CCWRAP_MAXSEMIBLOCKS 8193

/*!

 @function   ccwrap_wrapped_size
 
 @param      data_size  The size of the unwrapped key
 
 @result     The size of the key after wrapping

 */
CC_INLINE size_t ccwrap_wrapped_size(const size_t data_size)
{
    return (data_size + CCWRAP_SEMIBLOCK);
}

/*!
 
 @function   ccwrap_unwrapped_size
 
 @param      data_size  The size of the wrapped key
 
 @result     The size of the key after unwrapping
 
 @discussion If the input is illegal (i.e. it is smaller than the overhead imposed by wrapping), the result will be zero.
 
 */
CC_INLINE size_t ccwrap_unwrapped_size(const size_t data_size)
{
    if (data_size < CCWRAP_SEMIBLOCK) {
        // data is malformed and possibly malicious
        // just avoid underflow for now
        // actually detect and handle error in ccwrap_auth_decrypt
        return 0;
    }

    return (data_size - CCWRAP_SEMIBLOCK);
}


/*!
 @function   ccwrap_auth_decrypt
 @abstract   Recover the wrapped key.
 
 @param      ecb_mode   Definition of an ECB implementation
 @param      ctx        An instance of the implementation
 @param      nbytes     Length in bytes of the wrapped key
 @param      in         Pointer to the wrapped key
 @param      obytes     Return parameter describing the size of the unwrapped key
 @param      out        Return parameter pointing to the unwrapped key
 
 @result     0 iff successful; -1 otherwise.
 
 @discussion The out buffer should be allocated by the caller based 
 on the result of ccwrap_unwrapped_size(nbytes). On a successful 
 invocation, *obytes == ccwrap_unwrapped_size(nbytes); the caller needn't 
 verify this invariant.
 */
int ccwrap_auth_decrypt(const struct ccmode_ecb *ecb_mode, ccecb_ctx *ctx,
                        size_t nbytes, const void *in,
                        size_t *obytes, void *out);

/*!
 @function   ccwrap_auth_encrypt
 @abstract   Wrap a key.
 
 @param      ecb_mode   Definition of an ECB implementation
 @param      ctx        An instance of the implementation
 @param      nbytes     Length in bytes of the key
 @param      in         Pointer to the key
 @param      obytes     Return parameter describing the size of the wrapped key
 @param      out        Return parameter pointing to the wrapped key
 
 @result     0 iff successful; -1 otherwise.
 
 @discussion The out buffer should be allocated by the caller based
 on the result of ccwrap_wrapped_size(nbytes). On a successful
 invocation, *obytes == ccwrap_wrapped_size(nbytes); the caller needn't
 verify this invariant.
 */
int ccwrap_auth_encrypt(const struct ccmode_ecb *ecb_mode, ccecb_ctx *ctx,
                        size_t nbytes, const void *in,
                        size_t *obytes, void *out);

/*
	This was originally implemented according to the “AES Key Wrap Specification”
	formalized in RFC 3394.
 
	The following publications track changes made over time:

   [AES-KW1] National Institute of Standards and Technology, AES Key
         Wrap Specification, 17 November 2001.
         http://csrc.nist.gov/groups/ST/toolkit/documents/kms/
         AES_key_wrap.pdf

   [AES-KW2] Schaad, J. and R. Housley, "Advanced Encryption Standard
             (AES) Key Wrap Algorithm", RFC 3394, September 2002.
 
	Note: block size is required to be 128 bits.
 
    This implementation wraps plaintexts between two and (CCWRAP_MAXSEMIBLOCKS-1) 
    semiblocks in length to produce ciphertexts between three and 
    CCWRAP_MAXSEMIBLOCKS semiblocks in length. All other inputs are rejected.

	While only the original unpadded algorithm is implemented at this time, the 
    following documents include specifications for padded versions allowing 
    plaintexts of arbitrary length:

        http://tools.ietf.org/html/rfc5649

        NIST SP800-38F

*/

#endif /* _CORECRYPTO_CCWRAP_H_ */
