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

#ifndef _CORECRYPTO_CCZP_H_
#define _CORECRYPTO_CCZP_H_

#include <corecrypto/ccn.h>
#include <corecrypto/ccrng.h>

/* 
 Don't use cczp_hd struct directly, except in static tables such as eliptic curve parameter definitions.
 
 Declare cczp objects using cczp_decl_n(). It allocates cc_unit arrays of the length returned by either cczp_nof_n() or cczp_short_nof_n().
*/

struct cczp;
#if CORECRYPTO_USE_TRANSPARENT_UNION

typedef union {
    cc_unit *u;
    struct cczp *zp;
    //cczp_const_t czp; //for automatic type cast
    //struct cczp_prime *prime;
} cczp_t __attribute__((transparent_union));

typedef union {
    const cc_unit *u;
    const struct cczp *zp;
    //const struct cczp_prime *prime;
    cczp_t _nczp;
} cczp_const_t __attribute__((transparent_union));

#else
    typedef struct cczp* cczp_t;
    typedef const struct cczp* cczp_const_t;
#endif
typedef void (*ccmod_func_t)(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *s);

// keep cczp_hd and cczp structures consistent
// cczp_hd is typecasted to cczp to read EC curve params
// options field is to specify Montgomery arithmetic, bit field, etc
// make sure n is the first element see ccrsa_ctx_n macro 
#define __CCZP_HEADER_ELEMENTS_DEFINITIONS(pre) \
cc_size pre ## n;\
cc_unit pre ## options;\
ccmod_func_t pre ## mod_prime;

#define __CCZP_ELEMENTS_DEFINITIONS(pre) \
__CCZP_HEADER_ELEMENTS_DEFINITIONS(pre) \
cc_unit pre ## ccn[];

//cczp_hd must be defined separetly without variable length array ccn[], because it is used in sructures such as ccdh_gp_decl_n
struct cczp_hd{
    __CCZP_HEADER_ELEMENTS_DEFINITIONS()
}  CC_ALIGNED(CCN_UNIT_SIZE);

struct cczp {
    __CCZP_ELEMENTS_DEFINITIONS()
} CC_ALIGNED(CCN_UNIT_SIZE);


/* Return the size of an cczp where each ccn is _size_ bytes. */
#define cczp_size(_size_) (sizeof(struct cczp) + ccn_sizeof_n(1) + 2 * (_size_))

/* Return number of units that a struct cczp needs to be in units for a prime
   size of N units.  This is large enough for all operations.  */
#define cczp_nof_n(_n_) (ccn_nof_size(sizeof(struct cczp)) + 1 + 2 * (_n_))

/* Return number of units that a struct cczp needs to be in units for a prime
   size of _n_ units.  The _short variant does not have room for CCZP_RECIP,
   so it can not be used with cczp_mod, cczp_mul, cczp_sqr. It can be used
   with cczp_add, cczp_sub, cczp_div2, cczp_mod_inv. */
#define cczp_short_nof_n(_n_) (ccn_nof_size(sizeof(struct cczp)) + (_n_))

#define cczp_decl_n(_n_, _name_)  cc_ctx_decl(struct cczp, ccn_sizeof_n(cczp_nof_n(_n_)), _name_)
#define cczp_short_decl_n(_n_, _name_) cc_ctx_decl(struct cczp_short, ccn_sizeof_n(cczp_short_nof_n(_n_)), _name_)

#define cczp_clear_n(_n_, _name_)  cc_clear(ccn_sizeof_n(cczp_nof_n(_n_)), _name_)
#define cczp_short_clear_n(_n_, _name_)  cc_clear(ccn_sizeof_n(cczp_short_nof_n(_n_)), _name_)

#if CORECRYPTO_USE_TRANSPARENT_UNION 
  #define CCZP_N(ZP) (((cczp_t)(ZP)).zp->n)
  #define CCZP_MOD(ZP) (((cczp_t)(ZP)).zp->mod_prime)
  #define CCZP_PRIME(ZP) (((cczp_t)(ZP)).zp->ccn)
  #define CCZP_RECIP(ZP) (((cczp_t)(ZP)).zp->ccn + cczp_n(ZP))
  #define CCZP_OPS(ZP) ((ZP).zp->options)
  #define CCZP_MOD_PRIME(ZP) CCZP_MOD(ZP)

CC_CONST CC_NONNULL_TU((1))
static inline cc_size cczp_n(cczp_const_t zp) {
    return zp.zp->n;
}

CC_CONST CC_NONNULL_TU((1))
static inline cc_unit cczp_options(cczp_const_t zp) {
    return zp.zp->options;
}

CC_CONST CC_NONNULL_TU((1))
static inline ccmod_func_t cczp_mod_prime(cczp_const_t zp) {
    return zp.zp->mod_prime;
}

CC_CONST CC_NONNULL_TU((1))
static inline const cc_unit *cczp_prime(cczp_const_t zp) {
    return zp.zp->ccn;
}

/* Return a pointer to the Reciprocal or Montgomery constant of zp, which is
 allocated cczp_n(zp) + 1 units long. */
CC_CONST CC_NONNULL_TU((1))

static inline const cc_unit *cczp_recip(cczp_const_t zp) {
    return zp.zp->ccn + zp.zp->n;
}

#else
  #define CCZP_N(ZP)     ((ZP)->n)
  #define CCZP_MOD(ZP)   ((ZP)->mod_prime)
  #define CCZP_MOD_PRIME(ZP) CCZP_MOD(ZP)
  #define CCZP_PRIME(ZP) ((ZP)->ccn)
  #define CCZP_RECIP(ZP) ((ZP)->ccn + CCZP_N(ZP))
  #define CCZP_OPS(ZP)   ((ZP)->options)
CC_CONST CC_NONNULL_TU((1))
static inline cc_size cczp_n(cczp_const_t zp) {
    return zp->n;
}

CC_CONST CC_NONNULL_TU((1))
static inline cc_unit cczp_options(cczp_const_t zp) {
    return zp->options;
}

CC_CONST CC_NONNULL_TU((1))
static inline ccmod_func_t cczp_mod_prime(cczp_const_t zp) {
    return zp->mod_prime;
}

CC_CONST CC_NONNULL_TU((1))
static inline const cc_unit *cczp_prime(cczp_const_t zp) {
    return zp->ccn;
}

/* Return a pointer to the Reciprocal or Montgomery constant of zp, which is
 allocated cczp_n(zp) + 1 units long. */
CC_CONST CC_NONNULL_TU((1))

static inline const cc_unit *cczp_recip(cczp_const_t zp) {
    return zp->ccn + zp->n;
}

#endif


CC_CONST CC_NONNULL_TU((1))
CC_INLINE size_t cczp_bitlen(cczp_const_t zp) {
    return ccn_bitlen(cczp_n(zp), cczp_prime(zp));
}


/* Ensure both cczp_mod_prime(zp) and cczp_recip(zp) are valid. cczp_n and
   cczp_prime must have been previously initialized. */
CC_NONNULL_TU((1))
int cczp_init(cczp_t zp);

/* Compute r = s2n mod cczp_prime(zp). Will write cczp_n(zp)
 units to r and reads 2 * cczp_n(zp) units units from s2n. If r and s2n are not
 identical they must not overlap.  Before calling this function either
 cczp_init(zp) must have been called or both CCZP_MOD_PRIME((cc_unit *)zp)
 and CCZP_RECIP((cc_unit *)zp) must be initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
void cczp_mod(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *s2n);

/* Compute r = sn mod cczp_prime(zp), Will write cczp_n(zp)
 units to r and reads sn units units from s. If r and s are not
 identical they must not overlap.  Before calling this function either
 cczp_init(zp) must have been called or both CCZP_MOD_PRIME((cc_unit *)zp)
 and CCZP_RECIP((cc_unit *)zp) must be initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 4))
int cczp_modn(cczp_const_t zp, cc_unit *r, cc_size ns, const cc_unit *s);

/* Compute r = x * y mod cczp_prime(zp). Will write cczp_n(zp) units to r
   and reads cczp_n(zp) units units from both x and y. If r and x are not
   identical they must not overlap, The same holds for r and y.  Before
   calling this function either cczp_init(zp) must have been called or both
   CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must be
   initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
void cczp_mul(cczp_const_t zp, cc_unit *t, const cc_unit *x, const cc_unit *y);

CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4, 5))
void cczp_mul_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *t, const cc_unit *x, const cc_unit *y);

/* Compute r = x * x mod cczp_prime(zp). Will write cczp_n(zp) units to r
   and reads cczp_n(zp) units from x. If r and x are not identical they must
   not overlap. Before calling this function either cczp_init(zp) must have
   been called or both CCZP_MOD_PRIME((cc_unit *)zp) and
   CCZP_RECIP((cc_unit *)zp) must be initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
void cczp_sqr(cczp_const_t zp, cc_unit *r, const cc_unit *x);

CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
void cczp_sqr_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *x);

/* Compute r = x^(1/2) mod cczp_prime(zp). Will write cczp_n(zp) units to r
 and reads cczp_n(zp) units from x. If r and x are not identical they must
 not overlap. Before calling this function either cczp_init(zp) must have
 been called or both CCZP_MOD_PRIME((cc_unit *)zp) and
 CCZP_RECIP((cc_unit *)zp) must be initialized some other way. 
 Only support prime = 3 mod 4 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
int cczp_sqrt(cczp_const_t zp, cc_unit *r, const cc_unit *x);

/* Compute r = m ^ e mod cczp_prime(zp), using Montgomery ladder.
   - writes cczp_n(zp) units to r
   - reads  cczp_n(zp) units units from m and e
   - if r and m are not identical they must not overlap. 
   - r and e must not overlap nor be identical.
   - before calling this function either cczp_init(zp) must have been called
   or both CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must
   be initialized some other way.
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
int cczp_power(cczp_const_t zp, cc_unit *r, const cc_unit *m,
               const cc_unit *e);

/* Compute r = m ^ e mod cczp_prime(zp), using Square Square Multiply Always.
 - writes cczp_n(zp) units to r
 - reads  cczp_n(zp) units units from m and e
 - if r and m are not identical they must not overlap.
 - r and e must not overlap nor be identical.
 - before calling this function either cczp_init(zp) must have been called
 or both CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must
 be initialized some other way. 
 
 Important: This function is intented to be constant time but is more likely
    to leak information due to memory cache. Only used with randomized input
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
int cczp_power_ssma(cczp_const_t zp, cc_unit *r, const cc_unit *m,
                const cc_unit *e);

int cczp_power_ssma_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *s, const cc_unit *e);

/* Compute r = m ^ e mod cczp_prime(zp). Will write cczp_n(zp) units to r and
 reads cczp_n(zp) units units from m.  Reads ebitlen bits from e.
 m must be <= to cczp_prime(zp).  If r and m are not identical they must not
 overlap. r and e must not overlap nor be identical.
 Before calling this function either cczp_init(zp) must have been called
 or both CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must
 be initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 5))
int cczp_powern(cczp_const_t zp, cc_unit *r, const cc_unit *s,
                size_t ebitlen, const cc_unit *e);

/* Compute r = x + y mod cczp_prime(zp). Will write cczp_n(zp) units to r and
   reads cczp_n(zp) units units from x and y. If r and x are not identical
   they must not overlap. Only cczp_n(zp) and cczp_prime(zp) need to be valid.
   Can be used with cczp_short_nof_n sized cc_unit array zp. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
void cczp_add(cczp_const_t zp, cc_unit *r, const cc_unit *x,
              const cc_unit *y);

CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4, 5))
void cczp_add_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *x,
                 const cc_unit *y);

/* Compute r = x - y mod cczp_prime(zp). Will write cczp_n(zp) units to r and
   reads cczp_n(zp) units units from x and y. If r and x are not identical
   they must not overlap. Only cczp_n(zp) and cczp_prime(zp) need to be valid.
   Can be used with cczp_short_nof_n sized cc_unit array zp. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
void cczp_sub(cczp_const_t zp, cc_unit *r, const cc_unit *x, const cc_unit *y);

CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4, 5))
void cczp_sub_ws(cc_ws_t ws, cczp_const_t zp, cc_unit *r, const cc_unit *x,
                 const cc_unit *y);

/* Compute r = x / 2 mod cczp_prime(zp). Will write cczp_n(zp) units to r and
   reads cczp_n(zp) units units from x. If r and x are not identical
   they must not overlap. Only cczp_n(zp) and cczp_prime(zp) need to be valid.
   Can be used with cczp_short_nof_n sized cc_unit array zp. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
void cczp_div2(cczp_const_t zp, cc_unit *r, const cc_unit *x);

/* Compute q = a_2n / cczp_prime(zd) (mod cczp_prime(zd)) . Will write cczp_n(zd)
   units to q and r. Will read 2 * cczp_n(zd) units units from a. If r and a
   are not identical they must not overlap. Before calling this function
   either cczp_init(zp) must have been called or both
   CCZP_MOD_PRIME((cc_unit *)zp) and CCZP_RECIP((cc_unit *)zp) must be
   initialized some other way. */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 4))
void cczp_div(cczp_const_t zd, cc_unit *q, cc_unit *r, const cc_unit *a_2n);


/*!
 @brief cczp_inv(zp, r, x) computes r = x^-1 (mod p) , where p=cczp_prime(zp).
 @discussion It is a general function and works for any p. It validates the inputs. r and x can overlap. It writes n =cczp_n(zp) units to r, and read n units units from x and p. The output r is overwriten only if the inverse is correctly computed. This function is not constant time in absolute sense, but it does not have data dependent 'if' statements in the code.
 @param zp  The input zp. cczp_n(zp) and cczp_prime(zp) need to be valid. cczp_init(zp) need not to be called before invoking cczp_inv().
 @param x input big integer
 @param r output big integer
 @return  0 if inverse exists and correctly computed.
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))

int cczp_inv(cczp_const_t zp, cc_unit *r, const cc_unit *x);

/*!
 @brief cczp_inv_odd(zp, r, x) computes r = x^-1 (mod p) , where p=cczp_prime(zp) is an odd number.
 @discussion  r and x can overlap.
 @param zp  The input zp. cczp_n(zp) and cczp_prime(zp) need to be valid. cczp_init(zp) need not to be called before invoking.
 @param x input big integer
 @param r output big integer
 @return  0 if successful
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
int cczp_inv_odd(cczp_const_t zp, cc_unit *r, const cc_unit *x);

/*!
 @brief cczp_inv_field(zp, r, x) computes r = x^-1 (mod p) , where p=cczp_prime(zp) is a prime number number.
 @discussion r and x must NOT overlap. The excution time of the function is independent to the value of the input x. It works only if p is a field. That is, when p is a prime. It supports Montgomery and non-Montgomery form of zp. It leaks the value of the prime and should only be used be used for public (not secret) primes (ex. Elliptic Curves)

 @param zp  The input zp. cczp_n(zp) and cczp_prime(zp) need to be valid. cczp_init(zp) need not to be called before invoking cczp_inv_field().
 @param x input big unteger
 @param r output big integer
 @return  0 if inverse exists and correctly computed.
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3))
int cczp_inv_field(cczp_const_t zp, cc_unit *r, const cc_unit *x);

#endif /* _CORECRYPTO_CCZP_H_ */
