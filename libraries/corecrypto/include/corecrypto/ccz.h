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

#ifndef _CORECRYPTO_CCZ_H_
#define _CORECRYPTO_CCZ_H_

#include <corecrypto/ccn.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CCZ_INVALID_INPUT_ERROR -1
#define CCZ_INVALID_RADIX_ERROR -2

struct ccz {
    size_t n;
    struct ccz_class *isa;
    int sac;
    cc_unit *u;
};
typedef struct ccz ccz;

struct ccz_class {
	void *ctx;
	void *(*ccz_alloc)(void *, size_t);
	void *(*ccz_realloc)(void *, size_t, void *, size_t);
	void (*ccz_free)(void *, size_t, void *);
};

/* Return the size needed for a ccz big enough to hold cls type ccz's. */
CC_NONNULL_ALL
size_t ccz_size(struct ccz_class *cls);

/* Initialize a new ccz instance. */
CC_NONNULL_ALL
void ccz_init(struct ccz_class *cls, ccz *r);

/* Release the memory a ccz is holding on to. */
CC_NONNULL_ALL
void ccz_free(ccz *r);

/* r = 0, and clear memory accessed by r. */
CC_NONNULL_ALL
void ccz_zero(ccz *r);

/* r = s */
CC_NONNULL_ALL
void ccz_set(ccz *r, const ccz *s);

CC_NONNULL1
void ccz_seti(ccz *r, uint64_t v);

/* s == 0 -> return 0 | s > 0 -> return index (starting at 1) of most
 significant bit that is 1. */
CC_PURE CC_NONNULL_ALL
size_t ccz_bitlen(const ccz *s);

/* s == 0 -> return 0 | s > 0 -> return the number of bits which are zero
   before the first one bit from least to most significant bit. */
CC_PURE CC_NONNULL_ALL
size_t ccz_trailing_zeros(const ccz *s);

/* Return actual size in bytes needed to serialize s. */
CC_PURE CC_NONNULL1
size_t ccz_write_uint_size(const ccz *s);

/* Serialize s, to out.
 First byte of byte stream is the m.s. byte of s,
 regardless of the size of cc_unit.

 No assumption is made about the alignment of out.

 The out_size argument should be the value returned from ccz_write_uint_size,
 and is also the exact number of bytes this function will write to out.
 If out_size if less than the value returned by ccz_write_uint_size, only the
 first out_size non-zero most significant octets of s will be written. */
CC_NONNULL((1,3))
void ccz_write_uint(const ccz *s, size_t out_size, void *out);

/*  Return actual size in bytes needed to serialize s as int
 (adding leading zero if high bit is set). */
CC_PURE CC_NONNULL1
size_t ccz_write_int_size(const ccz *s);

/*  Serialize s, to out.
 First byte of byte stream is the m.s. byte of s,
 regardless of the size of cc_unit.

 No assumption is made about the alignment of out.

 The out_size argument should be the value returned from ccz_write_int_size,
 and is also the exact number of bytes this function will write to out.
 If out_size if less than the value returned by ccz_write_int_size, only the
 first out_size non-zero most significant octets of s will be written. */
CC_NONNULL((1,3))
void ccz_write_int(const ccz *s, size_t out_size, void *out);

/*  Return actual size in bytes needed to serialize s in base radix. Radix can be any value between 2 and 64.  */
CC_PURE CC_NONNULL1
size_t ccz_write_radix_size(const ccz *s, unsigned radix);

/* r = (data, len) treated as a big endian byte array, written in base radix. Radix can be any value between 2 and 64. */
/* Not constant time. Do not use for sensitive information. */
CC_NONNULL((1,3))
void ccz_write_radix(const ccz *s, size_t out_size, void *out, unsigned radix);

/* r = (data, len) treated as a big endian byte array. */
CC_NONNULL((1,3))
void ccz_read_uint(ccz *r, size_t data_size, const uint8_t *data);

/* r = (data, len) treated as a two's complement signed big endian byte
 array. Negative only iff high bit of first byte is set. */
CC_NONNULL((1,3))
void ccz_read_int(ccz *r, size_t data_size, const uint8_t *data);

/* r = (data, len) treated as a big endian byte array.  Return nonzero iff the passed in buffer isn't a valid base radix input string. Radix can be any value between 2 and 64. 
 Returns: 0 if no error
    CCZ_INVALID_INPUT_ERROR if the input is not valid for the select radar 
    CCZ_INVALID_RADIX_ERROR if the radix is not supported (>64) */
/* Not constant time. Do not use for sensitive information. */
CC_NONNULL((1,3))
int ccz_read_radix(ccz *r, size_t data_size, const char *data, unsigned radix);

CC_PURE CC_NONNULL_ALL
int ccz_cmp(const ccz *s, const ccz *t);

CC_PURE CC_NONNULL_ALL
int ccz_cmpi(const ccz *s, uint32_t v);

/* r = -r. */
CC_NONNULL_ALL
void ccz_neg(ccz *r);

/* r = s + t. */
CC_NONNULL_ALL
void ccz_add(ccz *r, const ccz *s, const ccz *t);

/* r = s + v. */
CC_NONNULL_ALL
void ccz_addi(ccz *r, const ccz *s, uint32_t v);

/* r = s - t. */
CC_NONNULL_ALL
void ccz_sub(ccz *r, const ccz *s, const ccz *t);

/* r = s - v. */
CC_NONNULL_ALL
void ccz_subi(ccz *r, const ccz *s, uint32_t v);

/* r = s * t  */
CC_NONNULL_ALL
void ccz_mul(ccz *r, const ccz *s, const ccz *t);

/* r = s * t  */
CC_NONNULL_ALL
void ccz_muli(ccz *r, const ccz *s, uint32_t v);

/* q = s / t, r = s % t */
CC_NONNULL((3,4))
void ccz_divmod(ccz *q, ccz *r, const ccz *s, const ccz *t);

/* r = s >> k  */
CC_NONNULL((1,2))
void ccz_lsr(ccz *r, const ccz *s, size_t k);

/* r = s << k */
CC_NONNULL((1,2))
void ccz_lsl(ccz *r, const ccz *s, size_t k);

/* r = s / 2 */
CC_INLINE CC_NONNULL_ALL
void ccz_div2(ccz *r, const ccz *s) {
    ccz_lsr(r, s, 1);
}

/* r = s % t */
CC_NONNULL_ALL
void ccz_mod(ccz *r, const ccz *s, const ccz *t);

/* r = s^2. */
CC_NONNULL_ALL
void ccz_sqr(ccz *r, const ccz *s);

/* r = gcd(s, t).  */
CC_NONNULL_ALL
void ccz_gcd(ccz *r, const ccz *s, const ccz *t);

/* r = lcm(s, t).  */
CC_NONNULL_ALL
void ccz_lcm(ccz *r, const ccz *s, const ccz *t);

/* r = (s * t) mod u.  */
CC_NONNULL_ALL
void ccz_mulmod(ccz *r, const ccz *s, const ccz *t, const ccz *u);

/* r = (s^2) mod t.  */
CC_NONNULL_ALL
int ccz_sqrmod(ccz *r, const ccz *s, const ccz *t);

/* r = (s^-1) mod t.  */
CC_NONNULL_ALL
int ccz_invmod(ccz *r, const ccz *s, const ccz *t);

/* r = (s^t) mod u.  */
CC_NONNULL_ALL
int ccz_expmod(ccz *r, const ccz *s, const ccz *t, const ccz *u);

/* Return the value of bit k in s. */
CC_PURE CC_NONNULL1
bool ccz_bit(const ccz *s, size_t k);

/* Set the value of bit k in r to value. */
CC_NONNULL1
void ccz_set_bit(ccz *r, size_t k, bool value);

/* Return true iff s a is likely prime.  Using rabin miller for depth.  */
CC_NONNULL_ALL
bool ccz_is_prime(const ccz *s, unsigned depth);

/* s == odd -> return true | s == even -> return false */
CC_INLINE CC_PURE CC_NONNULL_ALL
bool ccz_is_odd(const ccz *s) {
    return ccz_bit(s, 0);
}

/* s == 0 -> return true | s != 0 -> return false */
CC_PURE CC_NONNULL_ALL
bool ccz_is_zero(const ccz *s);

/* s == 1 -> return true | s != 1 -> return false */
CC_PURE CC_NONNULL_ALL
bool ccz_is_one(const ccz *s);

/* s < 0 -> return true | s >= 0 -> return false */
CC_PURE CC_NONNULL_ALL
bool ccz_is_negative(const ccz *s);



/* Debugging */
CC_NONNULL1
void ccz_print(const ccz *s);
CC_NONNULL2
void ccz_lprint(const char *label, const ccz *s);

/* Forward declaration so we don't depend on ccrng.h. */
struct ccrng_state;

/* Make a ccz with up to nbits sized random value. */
CC_NONNULL((1,3))
int ccz_random_bits(ccz *r, size_t nbits, struct ccrng_state *rng);

#endif /* _CORECRYPTO_CCZ_H_ */
