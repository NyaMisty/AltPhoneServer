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

#ifndef _CORECRYPTO_CCEC_H_
#define _CORECRYPTO_CCEC_H_
#include <corecrypto/ccasn1.h>
#include <corecrypto/ccdigest.h>
#include <corecrypto/ccrng.h>
#include <corecrypto/cczp.h>
#include <stdbool.h>
#include <stdarg.h>


/* An ec_point. A ccec_projective_point_t is a point with x,y and z.
   A ccec_affine_point_t only has x and y. */

struct ccec_projective_point {
    cc_unit xyz[1];
} CC_ALIGNED(8);
typedef struct ccec_projective_point ccec_projective_point;

struct ccec_cp {
    __CCZP_ELEMENTS_DEFINITIONS()
} CC_ALIGNED(CCN_UNIT_SIZE);

#if CORECRYPTO_USE_TRANSPARENT_UNION
    struct ccec_point_hdr {
        cc_unit x;
    };

    typedef union {
        struct ccec_point_hdr *hdr;
        ccec_projective_point *_p;
    } __attribute__((transparent_union)) ccec_projective_point_t;

    typedef union {
        const struct ccec_point_hdr *hdr;
        ccec_projective_point_t point;
        ccec_projective_point *_p;
        const ccec_projective_point *_cp;
        const cc_unit *x;
    } __attribute__((transparent_union)) ccec_const_projective_point_t;

    #define ccec_affine_point_t ccec_projective_point_t
    #define ccec_affine_point ccec_projective_point
    #define ccec_const_affine_point_t ccec_const_projective_point_t

    typedef union {
        const struct ccec_cp *just_a_filler;
        const struct cczp   *zp; //security framework wants it this way
    } __attribute__((transparent_union)) ccec_const_cp_t;
#else

    struct ccec_affine_point {
        cc_unit xyz[1];
    } CC_ALIGNED(8);
    typedef struct ccec_affine_point ccec_affine_point;
    typedef ccec_projective_point* ccec_projective_point_t;
    typedef ccec_affine_point* ccec_affine_point_t;

    typedef const struct ccec_affine_point* ccec_const_affine_point_t;
    typedef const struct ccec_projective_point* ccec_const_projective_point_t;

    typedef const struct ccec_cp* ccec_const_cp_t;

#endif

/* Use ccec_full_ctx_decl to declare full ecc context */
struct ccec_full_ctx {
    ccec_const_cp_t  cp;
    uint8_t              pad[16 - sizeof(ccec_const_cp_t *)];
    struct ccec_projective_point point[];
} CC_ALIGNED(16) ;

struct ccec_pub_ctx {
    ccec_const_cp_t  cp;
    uint8_t              pad[16 - sizeof(ccec_const_cp_t *)];
    struct ccec_projective_point point[];
} CC_ALIGNED(16) ;

#if CORECRYPTO_USE_TRANSPARENT_UNION
#define ccec_ctx_public ccec_pub_ctx //this is just for compatibility
     typedef union {
         struct ccec_pub_ctx* pub;
         struct ccec_pub_ctx* _pub;
         struct ccec_pub_ctx* body;
         struct ccec_full_ctx *_full;
         struct ccec_full_ctx *full;
         struct ccec_full_ctx *fullt;
         struct ccec_full_ctx *hdr;
     } ccec_full_ctx_t __attribute__((transparent_union));
     #define ccec_full(x) ((x).full)
     typedef struct ccec_full_ctx ccec_full_ctx;

     typedef ccec_full_ctx_t ccec_pub_ctx_t;
     typedef struct ccec_pub_ctx ccec_pub_ctx;
#else
     typedef struct ccec_full_ctx* ccec_full_ctx_t;
     typedef struct ccec_pub_ctx* ccec_pub_ctx_t;
     CC_INLINE
     ccec_pub_ctx_t ccec_ctx_public(ccec_full_ctx_t fk) {
        return (ccec_pub_ctx_t) fk;
     }
#endif


/* Return the size of an ccec_full_ctx where each ccn is _size_ bytes. */
#define ccec_full_ctx_size(_size_)  (sizeof(struct ccec_full_ctx) + 4 * (_size_))
#define ccec_pub_ctx_size(_size_)   (sizeof(struct ccec_pub_ctx) + 3 * (_size_))

/* declare full and public context, when curve paramters cp are not known and will be assigned later*/
#define ccec_full_ctx_decl(_size_, _name_)  cc_ctx_decl( struct ccec_full_ctx, ccec_full_ctx_size(_size_), _name_)
#define ccec_full_ctx_clear(_size_, _name_) cc_clear(ccec_full_ctx_size(_size_), _name_)
#define ccec_pub_ctx_decl(_size_, _name_)   cc_ctx_decl( struct ccec_pub_ctx, ccec_pub_ctx_size(_size_), _name_)
#define ccec_pub_ctx_clear(_size_, _name_)  cc_clear(ccec_pub_ctx_size(_size_), _name_)

/* declare full and public context, when curve paramters cp are known */
#define ccec_full_ctx_decl_cp(_cp_, _name_)  ccec_full_ctx_decl(ccec_ccn_size(_cp_), _name_)
#define ccec_full_ctx_clear_cp(_cp_, _name_) ccec_full_ctx_clear(ccec_ccn_size(_cp_), _name_)
#define ccec_pub_ctx_decl_cp(_cp_, _name_)   ccec_pub_ctx_decl(ccec_ccn_size(_cp_), _name_)
#define ccec_pub_ctx_clear_cp(_cp_, _name_)  ccec_pub_ctx_clear(ccec_ccn_size(_cp_), _name_)

/* Declare storage for a projected or affine point respectively. */
#define ccec_point_size_n(_cp_)            (3 * ccec_cp_n(cp))
#define ccec_point_sizeof(_cp_)            ccn_sizeof_n(ccec_point_size_n(cp))
#define ccec_point_decl_cp(_cp_, _name_)   cc_ctx_decl(struct ccec_projective_point, ccec_point_sizeof(_cp_), _name_)
#define ccec_point_clear_cp(_cp_, _name_)  cc_clear(ccec_point_sizeof(_cp_), _name_)
#define ccec_affine_decl_cp(_cp_, _name_)  cc_ctx_decl(struct ccec_affine_point, 2 * ccec_ccn_size(_cp_), _name_)
#define ccec_affine_clear_cp(_cp_, _name_) cc_clear(2 * ccec_ccn_size(_cp_), _name_)

/* lvalue accessors to ccec_ctx fields. (only a ccec_full_ctx_t has K). */
#if CORECRYPTO_USE_TRANSPARENT_UNION
    #define ccec_ctx_cp(KEY)     (((ccec_pub_ctx_t)(KEY)).hdr->cp)
    CC_INLINE void ccec_ctx_init(ccec_const_cp_t cp, ccec_full_ctx_t key) {(key.full)->cp = cp;}
    //CC_INLINE struct ccec_projective_point *ccec_ctx_point(ccec_full_ctx_t key){return key.full->point;} // The public key as a projected point on the curve
    #define ccec_ctx_point(KEY)  ((ccec_projective_point_t)(((ccec_full_ctx_t)KEY).full->point))
    #define ccec_ctx_n(KEY)      (ccec_ctx_cp(KEY).zp->n)
    #define ccec_ctx_prime(KEY)  (ccec_ctx_cp(KEY).zp->ccn)

    CC_INLINE cc_size ccec_cp_n(ccec_const_cp_t cp) { return cp.zp->n; }
    //CC_INLINE cczp_const_t ccec_cp_zp(ccec_const_cp_t cp){ return cp.zp; }
    #define ccec_cp_zp(_cp_)     ((_cp_).zp)
    
#define ccec_ctx_x(KEY)      (ccec_ctx_point(KEY)._p->xyz) // The  x, y and z of the public key as a projected point on the curve.
#define ccec_ctx_y(KEY)      (ccec_ctx_point(KEY)._p->xyz+ 1 * ccec_ctx_n(KEY))
#define ccec_ctx_z(KEY)      (ccec_ctx_point(KEY)._p->xyz+ 2 * ccec_ctx_n(KEY))

#else
    /* Callers must use this macro to initialze a ccec_full_ctx or
    ccec_pub_ctx before using most of the macros in this file. */
    #define ccec_ctx_cp(KEY)     ((KEY)->cp)
    #define ccec_ctx_init(_cp_, _key_) ((_key_)->cp = (_cp_))
    #define ccec_ctx_point(KEY)  ((KEY)->point) // The public key as a projected point on the curve.
    #define ccec_ctx_n(KEY)      (ccec_ctx_cp(KEY)->n) // Return count (n) of a ccn for cp.
    #define ccec_ctx_prime(KEY)  (ccec_ctx_cp(KEY)->ccn)

    CC_CONST CC_INLINE cc_size ccec_cp_n(ccec_const_cp_t cp) { return cp->n; }
    CC_CONST CC_INLINE cczp_const_t ccec_cp_zp(ccec_const_cp_t cp){ return (cczp_const_t)cp; }

    #define ccec_ctx_x(KEY)      (ccec_ctx_point(KEY)->xyz) // The  x, y and z of the public key as a projected point on the curve.
    #define ccec_ctx_y(KEY)      (ccec_ctx_point(KEY)->xyz+ 1 * ccec_ctx_n(KEY))
    #define ccec_ctx_z(KEY)      (ccec_ctx_point(KEY)->xyz+ 2 * ccec_ctx_n(KEY))
#endif


/***************************************************************************/
/* EC Sizes                                                                */
/***************************************************************************/
/* Return the length of the prime for cp in bits. */
#if CORECRYPTO_USE_TRANSPARENT_UNION
    #define ccec_cp_prime_bitlen(CP) (ccn_bitlen((CP).zp->n, (CP).zp->ccn))
#else
    #define ccec_cp_prime_bitlen(CP) (ccn_bitlen((CP)->n, (CP)->ccn))
#endif
/* Return the sizeof the prime for cp. */
#define ccec_cp_prime_size(CP) ((ccec_cp_prime_bitlen(CP)+7)/8)
/* Return the ec keysize in bits. */
#define ccec_ctx_bitlen(KEY) (ccec_cp_prime_bitlen(ccec_ctx_cp(KEY)))
/* Return the ec keysize in bytes. */
#define ccec_ctx_size(KEY) (ccec_cp_prime_size(ccec_ctx_cp(KEY)))




/* The k of a full key which makes up the private key. 
   It is only accessible through full key
 */
CC_INLINE
cc_unit *ccec_ctx_k( ccec_full_ctx_t key) {
    return (ccec_ctx_x(key)+ 3 * ccec_ctx_n(key));
}

CC_INLINE
ccec_pub_ctx_t ccec_ctx_pub( ccec_full_ctx_t key) {
    return (ccec_pub_ctx_t) key;
}


/* Return sizeof a ccn for cp. */
CC_CONST CC_INLINE
size_t ccec_ccn_size(ccec_const_cp_t cp) {
    return ccn_sizeof_n(ccec_cp_n(cp));
}

/***************************************************************************/
/* EC Curve Parameters                                                     */
/***************************************************************************/

CC_CONST ccec_const_cp_t ccec_cp_192(void);
CC_CONST ccec_const_cp_t ccec_cp_224(void);
CC_CONST ccec_const_cp_t ccec_cp_256(void);
CC_CONST ccec_const_cp_t ccec_cp_384(void);
CC_CONST ccec_const_cp_t ccec_cp_521(void);

/***************************************************************************/
/* EC Wrap Params                                                          */
/***************************************************************************/

struct ccec_rfc6637_curve;
struct ccec_rfc6637_wrap;
struct ccec_rfc6637_unwrap;

extern struct ccec_rfc6637_wrap ccec_rfc6637_wrap_sha256_kek_aes128;
extern struct ccec_rfc6637_wrap ccec_rfc6637_wrap_sha512_kek_aes256;
extern struct ccec_rfc6637_unwrap ccec_rfc6637_unwrap_sha256_kek_aes128;
extern struct ccec_rfc6637_unwrap ccec_rfc6637_unwrap_sha512_kek_aes256;
extern struct ccec_rfc6637_curve ccec_rfc6637_dh_curve_p256;
extern struct ccec_rfc6637_curve ccec_rfc6637_dh_curve_p521;

/***************************************************************************/
/* EC Key Generation                                                       */
/***************************************************************************/

/*!
 @function   ccec_generate_key
 @abstract   Default - Currently invokes the FIPS version
    The behavior this function is not deterministic,
    the number of random bytes it consumes may vary
 @param      cp        Curve Parameters
 @param      rng       Random for the key generation as well as consistency signature
 @param      key       Full key containing the newly generated key pair
 @returns    0 if no error, an error code otherwise.
 */

CC_NONNULL_TU((1,3)) CC_NONNULL2
int ccec_generate_key(ccec_const_cp_t cp, struct ccrng_state *rng,
                      ccec_full_ctx_t key);

/*!
 @function   ccec_generate_key_legacy
 @abstract   NOT recommended: For legacy purposes in order to re-generate
    deterministic keys previously generated.
    2 * ccn_sizeof(ccec_cp_order_bitlen(cp)) of random bytes needed
 @param      cp        Curve Parameters
 @param      rng       Random for the key generation as well as consistency signature
 @param      key       Full key containing the newly generated key pair
 @returns    0 if no error, an error code otherwise.
 */

CC_NONNULL_TU((1,3)) CC_NONNULL2
int ccec_generate_key_legacy(ccec_const_cp_t cp,  struct ccrng_state *rng,
                             ccec_full_ctx_t key);

/*!
 @function   ccec_generate_key_fips
 @abstract   Guarantees FIPS compliant key pair. RECOMMENDED
    Use a non deterministic amount of random bytes
 @param      cp        Curve Parameters
 @param      rng       Random for the key generation as well as consistency signature
 @param      key       Full key containing the newly generated key pair
 @returns    0 if no error, an error code otherwise.
 */

CC_NONNULL_TU((1,3)) CC_NONNULL2
int ccec_generate_key_fips(ccec_const_cp_t cp,  struct ccrng_state *rng,
                           ccec_full_ctx_t key);

/*!
 @function   ccec_compact_generate_key
 @abstract   Generate a compact key pair according to
    https://tools.ietf.org/html/draft-jivsov-ecc-compact-05 and follows FIPS guideline
 @param      cp        Curve Parameters
 @param      rng       Random for the key generation as well as consistency signature
 @param      key       Full key containing the newly generated key pair
 @returns    0 if no error, an error code otherwise.
 */

/* Based on FIPS compliant version. Output a compact key */
/* Use a non deterministic amount of random bytes */
CC_NONNULL_TU((1,3)) CC_NONNULL2
int ccec_compact_generate_key(ccec_const_cp_t cp,  struct ccrng_state *rng,
                              ccec_full_ctx_t key);

/*!
 @function   ccec_generate_key_deterministic
 @abstract   Generate a key pair from the provided entropy buffer.
        requires cryptographic DRBG/KDF prior to calling
 @param      cp             Curve Parameters
 @param      entropy_len    Length in byte of the entropy buffer
 @param      entropy        Pointer to the entropy buffer of size entropy_len
 @param      rng            Real random for the signature and internal countermeasures
 @param      flag           Bitmask: options as explained below
 @param      key            Full key containing the newly generated key pair
 @returns    0 if no error, an error code otherwise.
 */
   
#define CCEC_GENKEY_DETERMINISTIC_FIPS     (1<<0)
/* FIPS consumes all of the entropy and requires a minimum of ceiling(qbitlen+64 / 8) bytes of entropy.
 It computes the secret key in [1,q-1] as (("entropy" mod (q-1)) + 1). "Entropy" is processed as a big endian number.
 Provided the entropy is FIPS compliant and no other option is set this method is FIPS compliant. 
 If COMPACT option is used, the key is not strictly FIPS compliant */

#define CCEC_GENKEY_DETERMINISTIC_PKA      (1<<1)
/* RAW consumes all of the entropy and requires a minimum of ceiling(qbitlen+64 / 8) bytes of entropy.
 It computes the secret key in [2,q-1] as (("entropy" mod (q-2)) + 2). "Entropy" is processed as a big endian number.
 Provided the entropy is FIPS compliant, this method is FIPS compliant. */

#define CCEC_GENKEY_DETERMINISTIC_LEGACY   (1<<2)
/* LEGACY requires a minimum of ccn_sizeof_n(n) byte of entropy, but ignores bytes after ccn_sizeof_n(n) */
/* Use them in the same sequence as the output of ccrng_generate that is used in ccec_generate_legacy */

#define CCEC_GENKEY_DETERMINISTIC_COMPACT  ((1<<3) | CCEC_GENKEY_DETERMINISTIC_FIPS)
/* generate key that is compatible with compact export format. Compatible with all of the options above */

#define CCEC_GENKEY_DETERMINISTIC_SECBKP   ((1<<4) | CCEC_GENKEY_DETERMINISTIC_COMPACT)
/* Compatibility flag for Secure Backup generated keys */

CC_NONNULL_TU((1,6)) CC_NONNULL((3,4))
int ccec_generate_key_deterministic(ccec_const_cp_t cp,
                                size_t entropy_len, const uint8_t *entropy,
                                struct ccrng_state *rng, // For masking and signature
                                uint32_t flags,
                                ccec_full_ctx_t key);    // Revisioning of the DRBG


/* Implementation per FIPS186-4 */
#define CCEC_GENERATE_KEY_DEFAULT_ERR     -1
#define CCEC_GENERATE_KEY_TOO_MANY_TRIES  -10
#define CCEC_GENERATE_KEY_MULT_FAIL       -11
#define CCEC_GENERATE_KEY_AFF_FAIL        -12
#define CCEC_GENERATE_KEY_CONSISTENCY     -13
#define CCEC_GENERATE_NOT_ON_CURVE        -14
#define CCEC_GENERATE_NOT_ENOUGH_ENTROPY  -15
#define CCEC_GENERATE_NOT_SUPPORTED       -16
#define CCEC_GENERATE_INVALID_INPUT       -17

/***************************************************************************/
/* EC SIGN/VERIFY  (ECDSA)                                                 */
/***************************************************************************/

/* Return the maximum buffer size needed to hold a signature for key. */
CC_INLINE CC_PURE CC_NONNULL_TU((1))
size_t ccec_sign_max_size(ccec_const_cp_t cp) {
    /* tag + 2 byte len + 2 * (tag + 1 byte len + optional leading zero + ccec_cp_prime_size) */
    return 3 + 2 * (3 + ccec_cp_prime_size(cp));
}

/*
    Signature in DER format 
*/
CC_NONNULL_TU((1)) CC_NONNULL((3, 4, 5, 6))
int ccec_sign(ccec_full_ctx_t key, size_t digest_len, const uint8_t *digest,
              size_t *sig_len, uint8_t *sig, struct ccrng_state *rng);
CC_NONNULL_TU((1)) CC_NONNULL((3, 5, 6))
int ccec_verify(ccec_pub_ctx_t key, size_t digest_len, const uint8_t *digest,
                size_t sig_len, const uint8_t *sig,  bool *valid);


/*
  Raw signature, big endian, padded to the key size.
 */
CC_NONNULL_TU((1))
size_t
ccec_signature_r_s_size(ccec_pub_ctx_t key);

CC_NONNULL_TU((1)) CC_NONNULL((3, 4, 5, 6))
int ccec_sign_composite(ccec_full_ctx_t key, size_t digest_len, const uint8_t *digest,
                        uint8_t *sig_r, uint8_t *sig_s, struct ccrng_state *rng);

CC_NONNULL_TU((1)) CC_NONNULL((3, 4, 5, 6))
int ccec_verify_composite(ccec_pub_ctx_t key, size_t digest_len, const uint8_t *digest,
                          uint8_t *sig_r, uint8_t *sig_s, bool *valid);

/***************************************************************************/
/* EC Diffie-Hellman                                                       */
/***************************************************************************/

/*
   Deprecated. Do not use.
   Migrate existing calls to ccecdh_compute_shared_secret
 */

/*!
 @function   ccec_compute_key
 @abstract   DEPRECATED. Use ccecdh_compute_shared_secret.
 */

CC_NONNULL_TU((1,2)) CC_NONNULL((3, 4))
int ccec_compute_key(ccec_full_ctx_t private_key, ccec_pub_ctx_t public_key,
                     size_t *computed_key_len, uint8_t *computed_key);

/*!
 @function   ccecdh_compute_shared_secret
 @abstract   Elliptic Curve Diffie-Hellman
 from ANSI X9.63 and NIST SP800-56A, section 5.7.1.2

 @param  private_key                Input: EC private key
 @param  public_key                 Input: EC public key
 @param  computed_shared_secret_len Input: Size of allocation for computed_shared_secret.
 Output: Effective size of data in computed_shared_secret
 @param  computed_shared_secret     Output: DH shared secret
 @param  masking_rng                Input: Handle on RNG to be used for the randomization of the computation

 @result 0 iff successful

 @discussion The shared secret MUST be transformed with a KDF function or at
 least Hash (SHA-256 or above) before being used.
 It shall not be used directly as a key.
 */

CC_NONNULL_TU((1,2)) CC_NONNULL((3, 4))
int ccecdh_compute_shared_secret(ccec_full_ctx_t private_key,
                                 ccec_pub_ctx_t public_key,
                                 size_t *computed_shared_secret_len, uint8_t *computed_shared_secret,
                                 struct ccrng_state *masking_rng);

/***************************************************************************/
/* EC WRAP/UNWRAP                                                          */
/***************************************************************************/

/*
 * Use rfc6637 style PGP wrapping for using EC keys
 */

CC_NONNULL_TU((1))
size_t ccec_rfc6637_wrap_key_size(ccec_pub_ctx_t public_key,
                                  unsigned long flags,
                                  size_t key_len);

/*
 * When CCEC_RFC6637_COMPACT_KEYS flag is used, the wrapping is NOT
 * compatible with RFC6637 so make sure the peer supports this mode
 * before using it.  It currently saves half of the public key size
 * which for P256 is 32 bytes which end up being about 1/4 of the
 * wrapping size.
 * Macros are bit masks
 */
#define CCEC_RFC6637_COMPACT_KEYS                 1
#define CCEC_RFC6637_DEBUG_KEYS                   2
#define CCEC_EXPORT_COMPACT_DIVERSIFIED_KEYS      4

/*!
 @function   ccec_rfc6637_wrap_key
 @abstract   Key wraping based on rfc6637

 @param  public_key     Input:  EC public key
 @param  wrapped_key    Output: Buffer for the wrapped key of length ccec_rfc6637_wrap_key_size
 @param  flags          Input:  Option flags
 @param  algid          Input:  Algorithm id
 @param  keylen         Input:  Length of the key to wrap (<=37 bytes)
 @param  key            Input:  Pointer to the key to wrap
 @param  curve          Input:  Definiton of the curve
 @param  curve          Input:  Definiton of the wrap
 @param  fingerprint    Input:  Point to a 20byte buffer used as fingerprint during wrapping.
 @param  rng            Input:  Handle on a RNG for ephemeral key generation and computation randomization

 @result 0 iff successful

 @discussion
    This implementation hides the length of the key to wrap.
    It only supports wrapping keys up to 37bytes.
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 6, 7, 8, 9, 10))
int
ccec_rfc6637_wrap_key(ccec_pub_ctx_t public_key,
                          void  *wrapped_key,
                          unsigned long flags,
                          uint8_t algid,
                          size_t key_len,
                          const void *key,
                          const struct ccec_rfc6637_curve *curve,
                          const struct ccec_rfc6637_wrap *wrap,
                          const uint8_t *fingerprint,
                          struct ccrng_state *rng);

/*!
 @function   ccec_diversify_pub
 @abstract   diversified public key with scalar r.
 r = entropy mod (q-1)) + 1, where entropy is interpreted as big endian.

 entropy_len must be greater or equal to ccec_diversify_min_entropy_len
 the entropy must be a well uniformly distributed number, such as random byte,
 output of a DRBG or output of a KDF.

 @param  cp                      Input:  Curve parameter
 @param  pub_key                 Input:  Original public key P.
 @param  entropy_len             Input:  byte length of the entropy
 @param  entropy                 Input:  point to the entropy
 @param  masking_rng             Input:  Random for randomizing the computation
 @param  diversified_generator   Output: New generator  (r.G).
 @param  diversified_pub_key     Output: New public key (r.P).

 @result 0 iff unwrapping was successful

 @discussion
 Diversified keys is the process of multiplying the generator and the public key
 by a same random number.
 This does not preserve properties of the key with respect to compact format
 However, this method is valid with compact points when using ECDH and when only X coordinate is used
 Therefore this is valid with ccec_rfc6637 wrap / unwrap.

 Compact here refers to https://datatracker.ietf.org/doc/draft-jivsov-ecc-compact/
 */
int ccec_diversify_pub(ccec_const_cp_t cp,
                       ccec_pub_ctx_t pub_key,
                       size_t entropy_len, const uint8_t *entropy,
                       struct ccrng_state *masking_rng,
                       ccec_pub_ctx_t  diversified_generator,
                       ccec_pub_ctx_t  diversified_pub_key
                       );

/*!
 @function   ccec_diversify_min_entropy_len
 @abstract   Minimum length of entropy to be passed to ccec_diversify_pub

 @param  cp                      Input:  Curve parameter

 @result Minimal entropy length in bytes to be used in ccec_diversify_pub

 */
size_t ccec_diversify_min_entropy_len(ccec_const_cp_t cp);

/*!
 @function   ccec_rfc6637_wrap_key_diversified
 @abstract   Key wraping based on rfc6637

 @param  generator      Input:  Generator, represented as a public key
 @param  public_key     Input:  EC public key
 @param  wrapped_key    Output: Buffer for the wrapped key of length ccec_rfc6637_wrap_key_size
 @param  flags          Input:  Option flags
 @param  algid          Input:  Algorithm id
 @param  keylen         Input:  Length of the key to wrap (<=38 bytes)
 @param  key            Input:  Pointer to the key to wrap
 @param  curve          Input:  Definiton of the curve
 @param  wrap           Input:  Definiton of the wrap
 @param  fingerprint    Input:  Point to a 20byte buffer used as fingerprint during wrapping.
 @param  rng            Input:  Handle on a RNG for ephemeral key generation and computation randomization

 @result 0 iff successful

 @discussion
    Diversified keys is the process of multiplying the generator and the public key
    by a same number.
    This implementation hides the length of the key to wrap.
    It only supports wrapping keys up to 37bytes.
 */

CC_NONNULL_TU((1,2)) CC_NONNULL((3, 7, 8, 9, 10, 11))
int
ccec_rfc6637_wrap_key_diversified(ccec_pub_ctx_t generator,
                                  ccec_pub_ctx_t public_key,
                                  void *wrapped_key,
                                  unsigned long flags,
                                  uint8_t symm_alg_id,
                                  size_t key_len,
                                  const void *key,
                                  const struct ccec_rfc6637_curve *curve,
                                  const struct ccec_rfc6637_wrap *wrap,
                                  const uint8_t *fingerprint, /* 20 bytes */
                                  struct ccrng_state *rng);

/*!
 @function   ccec_rfc6637_unwrap_key
 @abstract   Key unwraping based on rfc6637

 @param  private_key        Input:  Private key to unwrap the key
 @param  key_len            Input/Output:  Size of the allocated buffer / size of the key
 @param  key                Output: Buffer for the unwrapped key
 @param  flags              Input:  Option flags
 @param  symm_key_alg       Output: Algorithm id
 @param  curve              Input:  Definiton of the curve
 @param  wrap               Input:  Definiton of the unwrap
 @param  fingerprint        Input:  Point to a 20byte buffer used as fingerprint during wrapping.
 @param  wrapped_key_len    Input:  Size in byte of the wrapped key
 @param  wrapped_key        Input:  Pointer to the wrapped key

 @result 0 iff successful

 @discussion
 Diversified keys is the process of multiplying the generator and the public key
 by a same number.
 */
CC_NONNULL_TU((1)) CC_NONNULL((2, 3, 5, 6, 7, 8, 10))
int
ccec_rfc6637_unwrap_key(ccec_full_ctx_t private_key,
                            size_t *key_len,
                            void *key,
                            unsigned long flags,
                            uint8_t *symm_key_alg,
                            const struct ccec_rfc6637_curve *curve,
                            const struct ccec_rfc6637_unwrap *unwrap,
                            const uint8_t *fingerprint,
                            size_t wrapped_key_len,
                            const void  *wrapped_key);

/***************************************************************************/
/* EC Import/Export                                                        */
/***************************************************************************/

CC_NONNULL_TU((1,4)) CC_NONNULL((3))
int ccec_import_pub(ccec_const_cp_t cp, size_t in_len, const uint8_t *in, ccec_pub_ctx_t key);

/* Return the sizeof a buffer needed to exported public key to. */
CC_INLINE CC_CONST CC_NONNULL_TU((1))
size_t ccec_export_pub_size(ccec_pub_ctx_t key) {
    return 1 + 2 * ccec_cp_prime_size(ccec_ctx_cp(key));
}

/* Export key to out. Out must be ccec_export_pub_size(key) bytes long. */
CC_NONNULL_TU((1)) CC_NONNULL2
void ccec_export_pub(ccec_pub_ctx_t key, void *out);

/* ---------------------------------*/
/* x963							    */
/* ---------------------------------*/

/* Export 9.63 */
CC_INLINE CC_CONST CC_NONNULL_TU((2))
size_t ccec_x963_export_size(const int fullkey, ccec_pub_ctx_t key){
    return (((ccec_ctx_bitlen(key)+7)/8) * ((fullkey == 1) + 2)) + 1;
}

CC_NONNULL_TU((3)) CC_NONNULL2
void ccec_x963_export(const int fullkey, void *out, ccec_full_ctx_t key);

/* Import 9.63 */
size_t ccec_x963_import_pub_size(size_t in_len);

CC_NONNULL_TU((1,4)) CC_NONNULL3
/* Import an EC public key with x9.63 format */
int ccec_x963_import_pub(ccec_const_cp_t cp, size_t in_len, const uint8_t *in, ccec_pub_ctx_t key);

size_t ccec_x963_import_priv_size(size_t in_len);

CC_NONNULL_TU((1,4)) CC_NONNULL3
/* Import the full key (private and public part of the key) with x9.63 format */
int ccec_x963_import_priv(ccec_const_cp_t cp, size_t in_len, const uint8_t *in, ccec_full_ctx_t key);
/* ---------------------------------*/
/* Compact						    */
/* ---------------------------------*/

/* Compact here refers to https://datatracker.ietf.org/doc/draft-jivsov-ecc-compact/ */

/* Export Compact 
 Output as the same bitlen than p */
CC_NONNULL_TU((3)) CC_NONNULL2
void ccec_compact_export(const int fullkey, void *out, ccec_full_ctx_t key);

CC_INLINE CC_CONST CC_NONNULL_TU((2))
size_t ccec_compact_export_size(const int fullkey, ccec_pub_ctx_t key){
    return (((ccec_ctx_bitlen(key)+7)/8) * ((fullkey == 1) + 1));
}

/* Import Compact
 The public key is the x coordinate, in big endian, of length the byte length of p
 No preambule byte */
 
size_t ccec_compact_import_pub_size(size_t in_len);

CC_NONNULL_TU((1,4)) CC_NONNULL3
int ccec_compact_import_pub(ccec_const_cp_t cp, size_t in_len, const uint8_t *in, ccec_pub_ctx_t key);

size_t ccec_compact_import_priv_size(size_t in_len);

CC_NONNULL_TU((1,4)) CC_NONNULL3
int ccec_compact_import_priv(ccec_const_cp_t cp, size_t in_len, const uint8_t *in, ccec_full_ctx_t key);

/* ---------------------------------*/
/* DER (RFC 5915)                   */
/* ---------------------------------*/

/* Export EC priv to DER (RFC 5915) */
CC_NONNULL_TU((1))
size_t
ccec_der_export_priv_size(ccec_full_ctx_t key, ccoid_t key_oid, int includePublic);

CC_NONNULL_TU((1)) CC_NONNULL5
int
ccec_der_export_priv(ccec_full_ctx_t key, ccoid_t key_oid, int includePublic, size_t out_len, void *out);

/* import EC priv from DER (RFC 5915) */

CC_NONNULL((2,4))
int ccec_der_import_priv_keytype(size_t len, const uint8_t * data, ccoid_t *oid, size_t *n);

CC_NONNULL_TU((1,4)) CC_NONNULL((3))
int ccec_der_import_priv(ccec_const_cp_t cp, size_t length, const uint8_t *data, ccec_full_ctx_t full_key);

/* ---------------------------------*/
/* DER (custom) for diversified keys*/
/* ---------------------------------*/

/*!
 @function   ccec_der_export_diversified_pub_size
 @abstract   DER export of a diversified public key

 @param  diversified_generator Input:  Generator, represented as a public key
 @param  diversified_key       Input:  EC public key
 @param  flags                 Input:  Option flags (compact keys)

 @result sizeof a buffer needed to exported public key if successful, 0 otherwise.

 @discussion
 Diversified keys is the process of multiplying the generator and the public key
 by a same number.
 
 Compact here refers to https://datatracker.ietf.org/doc/draft-jivsov-ecc-compact/

 */
size_t ccec_der_export_diversified_pub_size(
                                            ccec_pub_ctx_t  diversified_generator,
                                            ccec_pub_ctx_t  diversified_key,
                                            unsigned long flags);
/*!
 @function   ccec_der_export_diversified_pub
 @abstract   DER export of a diversified public key

 @param  diversified_generator Input:  Generator, represented as a public key
 @param  diversified_key       Input:  EC public key
 @param  flags                 Input:  Option flags (compact keys)
 @param  der_len               Input:  Size of the destination buffer
 @param  der                   Output: Pointer to the destination buffer, must be ccec_export_pub_size(key) bytes long.

 @result NULL is error, pointer in the der buffer otherwise.

 @discussion
 Diversified keys is the process of multiplying the generator and the public key
 by a same number.

 Compact here refers to https://datatracker.ietf.org/doc/draft-jivsov-ecc-compact/
 */
uint8_t *ccec_der_export_diversified_pub(
                                    ccec_pub_ctx_t  diversified_generator,
                                    ccec_pub_ctx_t  diversified_key,
                                    unsigned long flags,
                                    size_t der_len, uint8_t *der);

/*!
 @function   ccec_der_export_diversified_pub
 @abstract   DER export of a diversified public key

 @param  diversified_generator Output:  Diversified generator, represented as a public key
 @param  diversified_key       Output:  Diversified EC public key
 @param  outflags              Output:  Output flags telling how the data was parsed.
 @param  der_len               Input:  Size of the destination buffer
 @param  der                   Output: Pointer to the destination buffer, must be ccec_export_pub_size(key) bytes long.

 @result 0 iff unwrapping was successful

 @discussion
 Diversified keys is the process of multiplying the generator and the public key
 by a same number. Currently the only valid output flag is CCEC_EXPORT_COMPACT_DIVERSIFIED_KEYS.
 The generator and the public point a required to be encoded in the same format, either standard
 or compact format. Mixing form is not allowed and that output is never generated
 by ccec_der_export_diversified_pub.

 Compact here refers to https://datatracker.ietf.org/doc/draft-jivsov-ecc-compact/
 */
int ccec_der_import_diversified_pub(
                                    ccec_const_cp_t cp,
                                    size_t length, const uint8_t *data,
                                    int *outflags,
                                    ccec_pub_ctx_t  diversified_generator,
                                    ccec_pub_ctx_t  diversified_key);

#define CCEC_IMPORT_DIVERSIFIED_KEYS_SUPPORT_FLAGS  1

/***************************************************************************/
/* EC Construction and Validation                                          */
/***************************************************************************/

CC_NONNULL_TU((1))
int ccec_get_pubkey_components(ccec_pub_ctx_t key, size_t *nbits,
                           uint8_t *x, size_t *xsize,
                           uint8_t *y, size_t *ysize);

CC_NONNULL_TU((1))
int ccec_get_fullkey_components(ccec_full_ctx_t key, size_t *nbits,
                            uint8_t *x, size_t *xsize,
                            uint8_t *y, size_t *ysize,
                            uint8_t *d, size_t *dsize);

CC_NONNULL_TU((6))
int ccec_make_pub(size_t nbits,
                  size_t xlength, const uint8_t *x,
                  size_t ylength, const uint8_t *y,
                  ccec_pub_ctx_t key);

CC_NONNULL_TU((8))
int ccec_make_priv(size_t nbits,
                   size_t xlength, const uint8_t *x,
                   size_t ylength, const uint8_t *y,
                   size_t klength, const uint8_t *k,
                   ccec_full_ctx_t key);

/*!
 @function   ccec_validate_pub
 @abstract   Perform validation of the public key
 @param  key elliptic curve public key
 @result true if the key is valid
 @discussion
 Perform the public key validation from FIPS: x,y are within range and
 the point is on the curve. Point at infinity is considered as invalid here.
 */
CC_NONNULL_TU((1))
bool ccec_validate_pub(ccec_pub_ctx_t key);

int ccec_keysize_is_supported(size_t keysize);

ccec_const_cp_t ccec_get_cp(size_t keysize);

CC_NONNULL_TU((1)) CC_NONNULL2
bool ccec_pairwise_consistency_check(const ccec_full_ctx_t full_key, struct ccrng_state *rng);

ccec_const_cp_t ccec_curve_for_length_lookup(size_t keylen, ...);

#endif /* _CORECRYPTO_CCEC_H_ */
