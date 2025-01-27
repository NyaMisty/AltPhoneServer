/*
 * Copyright (c) 2010,2011,2013,2014,2015,2016,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCPERF_H_
#define _CORECRYPTO_CCPERF_H_

#include <corecrypto/ccn.h>
#include <corecrypto/ccrng.h>
#include "cctime.h"
#include "ccstats.h"
#include <stdlib.h>

/** RNG instance used for perf tests */
extern struct ccrng_state *rng;


struct ccperf_test {
    const char *name;
};

/** perf family **/

enum ccperf_size_kind {
    ccperf_size_bytes = 0,
    ccperf_size_bits,
    ccperf_size_iterations,
    ccperf_size_units,
};

struct ccperf_family {
    const char *name;
    double(*func)(size_t loops, size_t size, const void *test);
    struct ccperf_test **tests;
    size_t ntests;
    size_t *sizes;
    size_t nsizes;
    size_t loops;
    enum ccperf_size_kind size_kind;
    size_t nruns;
    double run_time;
};

// Stop iterating after reaching timeout
#define RUN_TIMEOUT 10.0

#define numof(_x_) (sizeof(_x_)/sizeof(_x_[0]))

/* Some macros used by family factories */

#define F_ARGS(_f) _f##_perf_tests
#define F_FUNC(_f) perf_##_f
#define F_SZ(_f) sizeof(F_ARGS(_f)[0])
#define F_N(_f) numof(F_ARGS(_f))

#define F_GET_ALL(_family, _f)                                                  \
do {                                                                            \
    _family.name = #_f;                                                         \
    _family.func=F_FUNC(_f);                                                    \
    ccperf_family_select(&_family, F_N(_f), F_ARGS(_f), F_SZ(_f), argc, argv);  \
    _family.loops=1;                                                            \
} while(0)

#define F_GET_ALL2(_family, _f, _func)                                          \
do {                                                                            \
    _family.name = #_func;                                                      \
    _family.func=F_FUNC(_func);                                                 \
    ccperf_family_select(&_family, F_N(_f), F_ARGS(_f), F_SZ(_f), argc, argv);  \
    _family.loops=1;                                                            \
} while(0)

#define F_SIZES(_family, _n, _first) ccperf_family_set_pot_sizes(&_family, (_n), (_first))

#define F_SIZES_FROM_ARRAY(_family, _const_array) {\
    _family.nsizes=sizeof(_const_array)/sizeof(_const_array[0]); \
    _family.sizes=malloc(sizeof(_const_array)); \
    memcpy(_family.sizes,_const_array,sizeof(_const_array));\
    }

#define F_DEFINE(_fam, _oper, _kind, _n, _first)                                \
static struct ccperf_family _fam##_##_oper##_family;                            \
struct ccperf_family *ccperf_family_##_fam##_##_oper(int argc, char *argv[])    \
{                                                                               \
    ccperf_family_##_fam##_once(argc, argv);                                    \
    F_GET_ALL2(_fam##_##_oper##_family, _fam, _fam##_##_oper);                  \
    F_SIZES(_fam##_##_oper##_family, _n, _first);                               \
    _fam##_##_oper##_family.size_kind=_kind;                                    \
    return &_fam##_##_oper##_family;                                            \
}


/* family factories */
struct ccperf_family *ccperf_family_ccecb_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccecb_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccecb_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccbc_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccbc_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccbc_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb8_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb8_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb8_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccfb_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccctr_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccctr_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccctr_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_gmac(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_set_iv(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_finalize(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccgcm_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_cbcmac(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_set_iv(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_finalize(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccccm_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccofb_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccofb_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccofb_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccomac_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccomac_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccomac_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccxts_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccxts_set_tweak(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccxts_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccxts_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccchacha_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccchacha_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccchacha_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccpoly_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccpoly_update(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccpoly_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccchachapoly_encrypt_and_sign(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccchachapoly_decrypt_and_verify(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccsiv_init(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccsiv_aad_or_nonce(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccsiv_one_shot(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccdigest(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cchmac(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccn(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cczp(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccec(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccec25519(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccrsa(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccpbkdf2(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccsrp(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccansikdf(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cccmac(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccrng(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccdrbg(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccdh_generate_key(int argc, char *argv[]);
struct ccperf_family *ccperf_family_ccdh_compute_shared_secret(int argc, char *argv[]);
struct ccperf_family *ccperf_family_cczp_inv(int argc, char *argv[]);

/* utility functions */

double histogram_sieve(struct ccperf_family *f, size_t size, const void *arg);

void ccperf_family_select(struct ccperf_family *f, size_t ntests, void *tests, size_t testsz, int argc, char **argv);
void ccperf_family_set_pot_sizes(struct ccperf_family *f, size_t nsizes, size_t first);
int ccperf_main(int argc, char **argv);

#endif /* _CORECRYPTO_CCPERF_H_ */
