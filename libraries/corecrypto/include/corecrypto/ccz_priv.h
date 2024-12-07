/*
 * Copyright (c) 2011,2012,2015,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCZ_PRIV_H_
#define _CORECRYPTO_CCZ_PRIV_H_

#include <corecrypto/ccz.h>
#include <corecrypto/ccn.h>
#include <corecrypto/cc_priv.h>
#include <stdlib.h>  /* For abs() */

#ifndef CCZ_PREC
#define CCZ_PREC                 32     /* default units of precision */
#endif

/* Error codes. */
enum {
    CCZ_OK = 0,
    CCZ_MEM,
};

#define ccz_zp_decl(_ccz_, _name_) \
    cczp_decl_n(ccz_n(_ccz_), _name_); \
    CCZP_N(_name_) = ccz_n(_ccz_); \
    ccn_set(ccz_n(_ccz_), CCZP_PRIME(_name_), _ccz_->u);


CC_INLINE CC_CONST CC_NONNULL_ALL
int ccz_sign(const ccz *s) {
    return s->sac < 0 ? -1 : 1;
}

CC_INLINE CC_NONNULL1
void ccz_set_sign(ccz *r, int sign)
{
    if (ccz_sign(r) != sign)
        r->sac = -r->sac;
}

CC_INLINE CC_CONST CC_NONNULL_ALL
cc_size ccz_n(const ccz *s) {
    return s->n;
}

CC_INLINE CC_NONNULL1
void ccz_set_n(ccz *r, cc_size n) {
    r->n = n;
}

CC_INLINE CC_CONST CC_NONNULL_ALL
cc_size ccz_capacity(const ccz *s) {
    return (cc_size)abs(s->sac);
}

CC_INLINE CC_NONNULL1
void ccz_set_capacity(ccz *r, cc_size capacity)
{
    if (ccz_capacity(r) < capacity) {
        size_t ncapacity = capacity + (CCZ_PREC * 2) - (capacity % CCZ_PREC);
        cc_unit *t;
        if (ccz_capacity(r))
            t = r->isa->ccz_realloc(r->isa->ctx, ccn_sizeof_n(ccz_capacity(r)), r->u, ccn_sizeof_n(ncapacity));
        else
            t = r->isa->ccz_alloc(r->isa->ctx, ccn_sizeof_n(ncapacity));

        r->sac = r->sac < 0 ? -(int)ncapacity : (int)ncapacity;
        r->u = t;
    }
}

/* From ccn_write_radix. */
extern const char *ccn_radix_digit_map;

#endif /* _CORECRYPTO_CCZ_PRIV_H_ */
