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

#ifndef corecrypto_cczp_priv_h
#define corecrypto_cczp_priv_h

#include <corecrypto/ccn_priv.h>
#include <stdbool.h>

#define CCZP_MOD_WORKSPACE_N(n)  (6*((n)+1)+CCN_MUL_WS_WORKSPACE_N(n))
#define CCZP_MUL_WORKSPACE_N(n)  (2*(n) + CCZP_MOD_WORKSPACE_N(n))
#define CCZP_POWER_SSMA_WORKSPACE_N(n)  (5*(n) + CCZP_MUL_WORKSPACE_N(n))
#define CCZP_SQR_WORKSPACE_N(n)  (CCZP_MUL_WORKSPACE_N(n))
#define CCZP_ADD_WORKSPACE_N(n)  (1*n)
#define CCZP_SUB_WORKSPACE_N(n)  (1*n)

#define CCZP_MONTGOMERY  1

/*
 * Same as cczp_init with workspace
 */
void cczp_init_ws(cc_ws_t ws, cczp_t zp);

/*
 * Same as cczp_modn with workspace
 */
int cczp_modn_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *r, cc_size ns, const cc_unit *s);


/* Compute r = m ^ e (mod p), where p=cczp_prime(zp). Writes n=cczp_n(zp) units to r and
 reads n units units from m and e. If r and m are not identical
 they must not overlap. r and e must not overlap nor be identical.
 Before calling this function either cczp_init(zp) must have been called
 or both CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must
 be initialized some other way. 
 
 Use this function with PUBLIC values only, it may leak the parameters 
 in timing / Simple power analysis
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
int
cczp_power_fast(cczp_const_t zp, cc_unit *r, const cc_unit *s, const cc_unit *e);

/*
 * Montgomery representation support
 */
CC_NONNULL_TU((1))
CC_INLINE
bool cczp_is_montgomery(cczp_const_t zp) {
    return (CCZP_OPS(zp) & CCZP_MONTGOMERY) == CCZP_MONTGOMERY ;
}

/*
 * to Montgomery representation
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
CC_INLINE
void cczp_convert_to_montgomery(cczp_const_t zp,cc_unit *r, const cc_unit *x) {
    //ccn_lprint(cczp_n(zp)+1, "Montgomery cst:",cczp_recip(zp));q
    cc_assert(cczp_is_montgomery(zp));
    cczp_mul(zp,r,x,cczp_recip(zp));
}

/*
 * from Montgomery representation
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
CC_INLINE
void cczp_convert_from_montgomery(cczp_const_t zp,cc_unit *r, const cc_unit *x) {
    cc_unit t2[2*cczp_n(zp)];//vla
    ccn_setn(2*cczp_n(zp), t2, cczp_n(zp),x);
    cczp_mod_prime(zp)(NULL, zp, r, t2);
}

#if CORECRYPTO_USE_TRANSPARENT_UNION
    #define cczp_const_decl(zp, ini) cczp_const_t (zp); (zp).zp = (ini);
#else
    #define  cczp_const_decl(zp, ini) cczp_const_t (zp) = (ini);
#endif

#endif
