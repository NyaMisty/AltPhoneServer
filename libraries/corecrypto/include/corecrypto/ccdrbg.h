/*
 * Copyright (c) 2010,2011,2012,2014,2015,2016,2017,2018 Apple Inc. All rights reserved.
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

/*!
 @header corecrypto/ccdrbg.h
 @abstract The functions provided in ccdrbg.h implement high-level accessors
 to cryptographically secure random numbers.

 */

#ifndef _CORECRYPTO_CCDRBG_H_
#define _CORECRYPTO_CCDRBG_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccdrbg_impl.h>

/* error codes */
#define CCDRBG_STATUS_OK 0
#define CCDRBG_STATUS_ERROR (-1)
#define CCDRBG_STATUS_NEED_RESEED (-2)
#define CCDRBG_STATUS_PARAM_ERROR (-3)
// If this value is returned, the caller must abort or panic the process for security reasons.
// for example in the case of catastrophic error in
// http://csrc.nist.gov/publications/drafts/800-90/sp800_90a_r1_draft.pdf
// ccdrbg calls abort() or panic(), if they are available in the system.
#define CCDRBG_STATUS_ABORT (-4)
/*
 * The maximum length of the entropy_input,  additional_input (max_additional_input_length) , personalization string 
 * (max_personalization_string_length) and max_number_of_bits_per_request  are implementation dependent
 * but shall fit in a 32 bit register and be be less than or equal to the specified maximum length for the 
 * selected DRBG mechanism (NIST 800-90A Section 10).
 */

#define CCDRBG_MAX_ENTROPY_SIZE         ((uint32_t)1<<16)
#define CCDRBG_MAX_ADDITIONALINPUT_SIZE ((uint32_t)1<<16)
#define CCDRBG_MAX_PSINPUT_SIZE         ((uint32_t)1<<16)
#define CCDRBG_MAX_REQUEST_SIZE         ((uint32_t)1<<16) //this is the absolute maximum in NIST 800-90A
#define CCDRBG_RESEED_INTERVAL          ((uint64_t)1<<30) // must be able to fit the NIST maximum of 2^48


/*
 * The entropyLength is forced to be greater or equal than the security strength.
 * Nonce is not forced. It either needs to have 0.5*security strength entropy. Or, a vale that is repeated
 * less than a 0.5*security strength bit random string.
 * see below or NIST  800-90A for the definition of security strength
 */

CC_INLINE int ccdrbg_init(const struct ccdrbg_info *info,
			struct ccdrbg_state *drbg,
            size_t entropyLength, const void* entropy,
            size_t nonceLength, const void* nonce,
            size_t psLength, const void* ps)
{
	return info->init(info, drbg, entropyLength, entropy, nonceLength, nonce, psLength, ps);
}

/*
 *  The entropyLength is forced to be greater or equal than the security strength.
 */
CC_INLINE int ccdrbg_reseed(const struct ccdrbg_info *info,
       struct ccdrbg_state *drbg,
       size_t entropyLength, const void *entropy,
       size_t additionalLength, const void *additional)
{
    return info->reseed(drbg, entropyLength, entropy, additionalLength, additional);
}


CC_INLINE int ccdrbg_generate(const struct ccdrbg_info *info,
         struct ccdrbg_state *drbg,
         size_t dataOutLength, void *dataOut,
         size_t additionalLength, const void *additional)
{
    return info->generate(drbg, dataOutLength, dataOut, additionalLength, additional);
}

CC_INLINE void ccdrbg_done(const struct ccdrbg_info *info,
		struct ccdrbg_state *drbg)
{
	info->done(drbg);
}

CC_INLINE size_t ccdrbg_context_size(const struct ccdrbg_info *info)
{
    return info->size;
}


/*
 * NIST SP 800-90 CTR_DRBG
 * the maximum security strengh of drbg equals to the block size of the corresponding ECB.
 */
struct ccdrbg_nistctr_custom {
    const struct ccmode_ctr *ctr_info;
    size_t keylen;
    int strictFIPS;
    int use_df;
};

void ccdrbg_factory_nistctr(struct ccdrbg_info *info, const struct ccdrbg_nistctr_custom *custom);

/*
 * NIST SP 800-90 HMAC_DRBG
 * the maximum security strengh of drbg is half of output size of the input hash function and it internally is limited to 256 bits
 */
extern struct ccdrbg_info ccdrbg_nistdigest_info;

struct ccdrbg_nisthmac_custom {
    const struct ccdigest_info *di;
    int strictFIPS;
};

void ccdrbg_factory_nisthmac(struct ccdrbg_info *info, const struct ccdrbg_nisthmac_custom *custom);

/*
 * NIST SP 800-90 TRNG DRBG
 *
 * Call into the SEP DRBG and perform a SP 800-90 test operation.
 */
void ccdrbg_factory_trng(struct ccdrbg_info *info);

/* Required length of the various TRNG entropy and personalization inputs. */
#define CCDRBG_TRNG_VECTOR_LEN     48

/*
 * Dummy DRBG
 */
extern struct ccdrbg_info ccdrbg_dummy_info;

#endif /* _CORECRYPTO_CCDRBG_H_ */
