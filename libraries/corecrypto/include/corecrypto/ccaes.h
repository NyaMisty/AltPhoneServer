/*
 * Copyright (c) 2010,2011,2012,2013,2015,2016,2017,2018 Apple Inc. All rights reserved.
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
#ifndef _CORECRYPTO_CCAES_H_
#define _CORECRYPTO_CCAES_H_

#include <corecrypto/cc_config.h>
#include <corecrypto/ccmode.h>

#define CCAES_BLOCK_SIZE 16
#define CCAES_KEY_SIZE_128 16
#define CCAES_KEY_SIZE_192 24
#define CCAES_KEY_SIZE_256 32

#define CCAES_CTR_MAX_PARALLEL_NBLOCKS 8

extern const struct ccmode_ecb ccaes_ltc_ecb_decrypt_mode;
extern const struct ccmode_ecb ccaes_ltc_ecb_encrypt_mode;

extern const struct ccmode_cbc ccaes_gladman_cbc_encrypt_mode;
extern const struct ccmode_cbc ccaes_gladman_cbc_decrypt_mode;

#if  CCAES_ARM_ASM
extern const struct ccmode_ecb ccaes_arm_ecb_encrypt_mode;
extern const struct ccmode_ecb ccaes_arm_ecb_decrypt_mode;

extern const struct ccmode_cbc ccaes_arm_cbc_encrypt_mode;
extern const struct ccmode_cbc ccaes_arm_cbc_decrypt_mode;

extern const struct ccmode_xts ccaes_arm_xts_encrypt_mode;
extern const struct ccmode_xts ccaes_arm_xts_decrypt_mode;

extern const struct ccmode_cfb ccaes_arm_cfb_encrypt_mode;
extern const struct ccmode_cfb ccaes_arm_cfb_decrypt_mode;

extern const struct ccmode_ofb ccaes_arm_ofb_crypt_mode;

#endif

#if CCAES_MUX
extern const struct ccmode_cbc ccaes_ios_hardware_cbc_encrypt_mode;
extern const struct ccmode_cbc ccaes_ios_hardware_cbc_decrypt_mode;

extern const struct ccmode_ctr ccaes_ios_hardware_ctr_crypt_mode;

extern const struct ccmode_cbc *ccaes_ios_mux_cbc_encrypt_mode(void);
extern const struct ccmode_cbc *ccaes_ios_mux_cbc_decrypt_mode(void);

extern const struct ccmode_ctr *ccaes_ios_mux_ctr_crypt_mode(void);

#endif

#if  CCAES_INTEL_ASM
//extern const struct ccmode_ecb ccaes_intel_ecb_encrypt_mode;
//extern const struct ccmode_ecb ccaes_intel_ecb_decrypt_mode;

extern const struct ccmode_ecb ccaes_intel_ecb_encrypt_opt_mode;
extern const struct ccmode_ecb ccaes_intel_ecb_encrypt_aesni_mode;

extern const struct ccmode_ecb ccaes_intel_ecb_decrypt_opt_mode;
extern const struct ccmode_ecb ccaes_intel_ecb_decrypt_aesni_mode;

//extern const struct ccmode_cbc ccaes_intel_cbc_encrypt_mode;
//extern const struct ccmode_cbc ccaes_intel_cbc_decrypt_mode;

extern const struct ccmode_cbc ccaes_intel_cbc_encrypt_opt_mode;
extern const struct ccmode_cbc ccaes_intel_cbc_encrypt_aesni_mode;

extern const struct ccmode_cbc ccaes_intel_cbc_decrypt_opt_mode;
extern const struct ccmode_cbc ccaes_intel_cbc_decrypt_aesni_mode;

//extern const struct ccmode_xts ccaes_intel_xts_encrypt_mode;
//extern const struct ccmode_xts ccaes_intel_xts_decrypt_mode;

extern const struct ccmode_xts ccaes_intel_xts_encrypt_opt_mode;
extern const struct ccmode_xts ccaes_intel_xts_encrypt_aesni_mode;

extern const struct ccmode_xts ccaes_intel_xts_decrypt_opt_mode;
extern const struct ccmode_xts ccaes_intel_xts_decrypt_aesni_mode;
#endif

#if CC_USE_L4
extern const struct ccmode_cbc ccaes_skg_cbc_encrypt_mode;
extern const struct ccmode_cbc ccaes_skg_cbc_decrypt_mode;

extern const struct ccmode_ecb ccaes_skg_ecb_encrypt_mode;
extern const struct ccmode_ecb ccaes_skg_ecb_decrypt_mode;

extern const struct ccmode_ecb ccaes_trng_ecb_encrypt_mode;
#endif

/* Implementation Selectors: */
const struct ccmode_ecb *ccaes_ecb_encrypt_mode(void);
const struct ccmode_cbc *ccaes_cbc_encrypt_mode(void);
const struct ccmode_cfb *ccaes_cfb_encrypt_mode(void);
const struct ccmode_cfb8 *ccaes_cfb8_encrypt_mode(void);
const struct ccmode_xts *ccaes_xts_encrypt_mode(void);
const struct ccmode_gcm *ccaes_gcm_encrypt_mode(void);
const struct ccmode_ccm *ccaes_ccm_encrypt_mode(void);

const struct ccmode_ecb *ccaes_ecb_decrypt_mode(void);
const struct ccmode_cbc *ccaes_cbc_decrypt_mode(void);
const struct ccmode_cfb *ccaes_cfb_decrypt_mode(void);
const struct ccmode_cfb8 *ccaes_cfb8_decrypt_mode(void);
const struct ccmode_xts *ccaes_xts_decrypt_mode(void);
const struct ccmode_gcm *ccaes_gcm_decrypt_mode(void);
const struct ccmode_ccm *ccaes_ccm_decrypt_mode(void);

const struct ccmode_ctr *ccaes_ctr_crypt_mode(void);
const struct ccmode_ofb *ccaes_ofb_crypt_mode(void);

const struct ccmode_siv *ccaes_siv_encrypt_mode(void);
const struct ccmode_siv *ccaes_siv_decrypt_mode(void);

#endif /* _CORECRYPTO_CCAES_H_ */
