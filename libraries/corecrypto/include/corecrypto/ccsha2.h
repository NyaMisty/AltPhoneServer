/*
 * Copyright (c) 2010,2011,2012,2014,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCSHA2_H_
#define _CORECRYPTO_CCSHA2_H_

#include <corecrypto/ccdigest.h>

/* sha2 selectors */
const struct ccdigest_info *ccsha224_di(void);
const struct ccdigest_info *ccsha256_di(void);
const struct ccdigest_info *ccsha384_di(void);
const struct ccdigest_info *ccsha512_di(void);

/* TODO: Placeholders */
#define ccoid_sha224 ((unsigned char *)"\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x04")
#define ccoid_sha224_len 11

#define ccoid_sha256 ((unsigned char *)"\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01")
#define ccoid_sha256_len 11

#define ccoid_sha384 ((unsigned char *)"\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x02")
#define ccoid_sha384_len 11

#define ccoid_sha512 ((unsigned char *)"\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x03")
#define ccoid_sha512_len 11


/* SHA256 */
#define CCSHA256_BLOCK_SIZE  64
#define	CCSHA256_OUTPUT_SIZE 32
#define	CCSHA256_STATE_SIZE  32
extern const struct ccdigest_info ccsha256_ltc_di;
extern const struct ccdigest_info ccsha256_v6m_di;
#if  CCSHA2_VNG_INTEL
#if defined __x86_64__
extern const struct ccdigest_info ccsha224_vng_intel_AVX2_di;
extern const struct ccdigest_info ccsha224_vng_intel_AVX1_di;
extern const struct ccdigest_info ccsha256_vng_intel_AVX2_di;
extern const struct ccdigest_info ccsha256_vng_intel_AVX1_di;
extern const struct ccdigest_info ccsha384_vng_intel_AVX2_di;
extern const struct ccdigest_info ccsha384_vng_intel_AVX1_di;
extern const struct ccdigest_info ccsha384_vng_intel_SupplementalSSE3_di;
extern const struct ccdigest_info ccsha512_vng_intel_AVX2_di;
extern const struct ccdigest_info ccsha512_vng_intel_AVX1_di;
extern const struct ccdigest_info ccsha512_vng_intel_SupplementalSSE3_di;
#endif
extern const struct ccdigest_info ccsha224_vng_intel_SupplementalSSE3_di;
extern const struct ccdigest_info ccsha256_vng_intel_SupplementalSSE3_di;
#endif
#if  CCSHA2_VNG_ARMV7NEON
extern const struct ccdigest_info ccsha224_vng_armv7neon_di;
extern const struct ccdigest_info ccsha256_vng_armv7neon_di;
extern const struct ccdigest_info ccsha384_vng_arm64_di;
extern const struct ccdigest_info ccsha384_vng_armv7neon_di;
extern const struct ccdigest_info ccsha512_vng_arm64_di;
extern const struct ccdigest_info ccsha512_vng_armv7neon_di;
#endif
extern const uint32_t ccsha256_K[64];
extern const uint64_t ccsha512_K[80];

/* SHA224 */
#define	CCSHA224_OUTPUT_SIZE 28
extern const struct ccdigest_info ccsha224_ltc_di;

/* SHA512 */
#define CCSHA512_BLOCK_SIZE  128
#define	CCSHA512_OUTPUT_SIZE  64
#define	CCSHA512_STATE_SIZE   64
extern const struct ccdigest_info ccsha512_ltc_di;

/* SHA384 */
#define	CCSHA384_OUTPUT_SIZE  48
extern const struct ccdigest_info ccsha384_ltc_di;

#endif /* _CORECRYPTO_CCSHA2_H_ */
