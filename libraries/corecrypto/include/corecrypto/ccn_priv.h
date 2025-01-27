/*
 * Copyright (c) 2014,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef corecrypto_ccn_priv_h
#define corecrypto_ccn_priv_h

#include <corecrypto/ccn.h>
#include <corecrypto/cc_memory.h>
#include <corecrypto/cc_priv.h>

#define CCN_MUL_WS_WORKSPACE_N(n) (4*((n)+1))

#if CCN_DEDICATED_SQR
#define CCN_SQR_WS_WORKSPACE_N(n) (2*(n))
#else
#define CCN_SQR_WS_WORKSPACE_N(n) CCN_MUL_WS_WORKSPACE_N((n))
#endif

// Same ccn_make_recip, takes a ws
//#define CCN_MAKE_RECIP_WORKSPACE_SIZE(n) (3*(n))  /* shift sub */
#define CCN_MAKE_RECIP_WORKSPACE_SIZE(n) (7*n+CCN_MUL_WS_WORKSPACE_N(n+1)) /* newtonraphson */
void ccn_make_recip_ws(cc_ws_t ws, cc_size nd, cc_unit *recip, const cc_unit *d);

// Same ccn_div_euclid, takes a ws
//#define CCN_DIV_EUCLID_WORKSPACE_SIZE(na,nd) (nd+1 + CC_MAX(CCN_MAKE_RECIP_WORKSPACE_SIZE(nd),CCN_DIV_USE_RECIP_WORKSPACE_SIZE(na,nd)))
#define CCN_DIV_EUCLID_WORKSPACE_SIZE(na,nd) (nd+1 + 4+CCN_DIV_USE_RECIP_WORKSPACE_SIZE(na,nd))
int ccn_div_euclid_ws(cc_ws_t ws, cc_size nq, cc_unit *q, cc_size nr, cc_unit *r, cc_size na, const cc_unit *a, cc_size nd, const cc_unit *d);

/*!
 @brief ccn_div_use_recip(nq, q, nr, r, na, a, nd, d) computes q=a/d and r=a%d
 @discussion q and rcan be NULL. Reads na from a and nd from d. Writes nq in q and nr in r. nq and nr must be large enough to accomodate results, otherwise error is retuned. Execution time depends on the size of a. Computation is perfomed on of fixedsize and the leadig zeros of a of q are are also used in the computation.
 @param nq length of array q that hold the quotients. The maximum length of quotient is the actual length of dividend a
 @param q  returned quotient. If nq is larger than needed, it is filled with leading zeros. If it is smaller, error is returned. q can be set to NULL, if not needed.
 @param nr length of array r that hold the remainder. The maximum length of remainder is the actual length of divisor d
 @param r  returned remainder. If nr is larger than needed, it is filled with leading zeros. Ifi is smaller error is returned. r can be set to NULL if not required.
 @param na length of dividend. Dividend may have leading zeros.
 @param a  input Dividend
 @param nd length of input divisor. Divisor may have leading zeros.
 @param d  input Divisor
 @param recip_d The reciprocal of d, of length nd+1.

 @return  returns 0 if successful, negative of error.
 */
CC_NONNULL((7, 9, 10))
#define CCN_DIV_USE_RECIP_WORKSPACE_SIZE(na,nd) (6*(CC_MAX(2*nd,na)) + 4 + na)
int ccn_div_use_recip_ws(cc_ws_t ws, cc_size nq, cc_unit *q, cc_size nr, cc_unit *r, cc_size na, const cc_unit *a, cc_size nd, const cc_unit *d, const cc_unit *recip_d);


/*!
 @brief ccn_make_recip_newtonraphson(cc_size nd, cc_unit *recip, const cc_unit *d) computes the reciprocal of d: recip = 2^2b/d where b=bitlen(d)
 @discussion This use the NewtonRaphson method.
 @param nd      length of array d
 @param recip   returned reciprocal of size nd+1
 @param d       input integer
 */

CC_NONNULL((3, 4))
void ccn_make_recip_newtonraphson(cc_ws_t ws, cc_size nd, cc_unit *recip, const cc_unit *d);


/*!
 @brief ccn_make_recip_shift_sub(cc_size nd, cc_unit *recip, const cc_unit *d) computes the reciprocal of d: recip = 2^2b/d where b=bitlen(d)
 @discussion  This use the Shift/Sub (binary division) method.
 @param nd      length of array d
 @param recip   returned reciprocal of size nd+1
 @param d       input integer
 */
CC_NONNULL((3, 4))
void ccn_make_recip_shift_sub(cc_ws_t ws, cc_size nd, cc_unit *recip, const cc_unit *d);


#if defined(_WIN32)
 #include <windows.h>
 #include <intrin.h>
 #if (CCN_UNIT_SIZE==8)
  CC_INLINE cc_size clz64_win( uint64_t value )
  {
    DWORD leading_zero;
    _BitScanReverse64( &leading_zero, value );
    return 63 - leading_zero;
  }
  #define  clz_win(x) clz64_win(x)
 #elif (CCN_UNIT_SIZE<=4)
  CC_INLINE cc_size clz32_win( uint32_t value )
  {
    DWORD leading_zero;
    _BitScanReverse( &leading_zero, value );
    return 31 - leading_zero;
  }
  #define  clz_win(x) clz32_win(x)
  #endif
#else //for clang * GNU
 #if (CCN_UNIT_SIZE==8)
  #define CC_BUILTIN_CLZ __builtin_clzll
 #elif (CCN_UNIT_SIZE<=4)
  #define CC_BUILTIN_CLZ __builtin_clzl
 #endif
#endif

/*  Counts leading zeros in data, under the assumption that data is non-zero. */
CC_INLINE CC_CONST cc_size cc_clz_nonzero(cc_unit data) {
    cc_assert(data != 0);
#if defined(_WIN32)
    return clz_win(data);
#elif defined __x86_64__ || defined __i386__
    /*  On i386 and x86_64, we know clang and GCC will generate BSR for
     *  __builtin_clzl.  This instruction IS NOT constant time on all micro-
     *  architectures, but it *is* constant time on all micro-architectures that
     *  have been used by Apple, and we expect that to continue to be the case.
     *
     *  When building for x86_64h with clang, this produces LZCNT, which is exactly
     *  what we want.                                                             */
    return CC_BUILTIN_CLZ(data);
#elif defined __arm64__ || defined __arm__
    /*  On arm and arm64, we know that clang and GCC generate the constant-time CLZ
     *  instruction from __builtin_clzl( ).                                       */
    return CC_BUILTIN_CLZ(data);
#elif defined __GNUC__
#   warning Using __builtin_clzl( ) on an unknown architecture; it may not be constant-time.
    /*  If you find yourself seeing this warning, file a radar for someone to
     *  check whether or not __builtin_clzl( ) generates a constant-time
     *  implementation on the architecture you are targeting.  If it does, append
     *  the name of that architecture to the list of "safe" architectures above.  */
    return CC_BUILTIN_CLZ(data);
#else
    #warning Using a non-constant time implementation of cc_clz_nonzero( ).
    /*  If you find yourself seeing this warning, the function ccn_bitlen will not
     *  be constant time in the resulting build of corecrypto.  Please file a radar
     *  for someone to implement a constant-time cc_clz_nonzero for your targeted
     *  architecture and compiler pair.                                           */
    cc_unit mask = CC_UNIT_C(1) << (CCN_UNIT_BITS - 1);
    size_t b = 0;
    while ((data & mask >> b) == 0) b++;
    return b;
#endif
}

// perform division the division a/d, whne size n of a are d are the same. Returns remainder r and
// a cc_unit quotient. To be used only in xgcd. Exported here for test purpose only.
CC_NONNULL((2, 3, 4))
cc_unit ccn_div_equal_size(cc_size n, cc_unit *r, const cc_unit *a, const cc_unit *d);

// Conditionally swap the content of r0 and r1 buffers in constant time
// r0:r1 <- r1*k1 + s0*(k1-1)
void ccn_cond_swap(cc_size n,
                   int ki,
                   cc_unit *r0,
                   cc_unit *r1);


#endif
