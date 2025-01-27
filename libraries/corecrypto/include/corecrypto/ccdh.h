/*
 * Copyright (c) 2011,2012,2013,2015,2016,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCDH_H_
#define _CORECRYPTO_CCDH_H_

#include <corecrypto/ccrng.h>
#include <corecrypto/cczp.h>
#include <stdbool.h>

/* Error codes */
#define CCDH_ERROR_DEFAULT                  -1
#define CCDH_GENERATE_KEY_TOO_MANY_TRIES    -2
#define CCDH_NOT_SUPPORTED_CONFIGURATION    -3
#define CCDH_SAFETY_CHECK                   -4
#define CCDH_PUBLIC_KEY_MISSING             -5
#define CCDH_INVALID_DOMAIN_PARAMETER       -6
#define CCDH_INVALID_INPUT                  -7
#define CCDH_DOMAIN_PARAMETER_MISMATCH      -8
#define CCDH_GENERATE_KEY_CONSISTENCY       -9

#if CORECRYPTO_USE_TRANSPARENT_UNION

cc_aligned_struct(16) ccdh_gp;

/* A ccdh_gp_t is a pointer to a set of DH parameters. 
   The first entry is a (short) prime field. */
typedef union {
    cczp_t  zp;
    ccdh_gp *gp;   
} __attribute__((transparent_union)) ccdh_gp_t;

/* A ccdh_const_gp_t is a const pointer to a set of DH parameters.
 The first entry is a const prime field. */
typedef union {
    cczp_const_t  zp;
    const ccdh_gp  *gp;       
    ccdh_gp_t      _ncgp;      // Non const variant
} __attribute__((transparent_union)) ccdh_const_gp_t;


/* Every dh context (a public or private key) starts with this. */
struct ccdh_ctx_header {
    ccdh_const_gp_t     gp;
    uint8_t             pad[16 - sizeof(ccdh_const_gp_t *)];
} CC_ALIGNED(16);

struct ccdh_ctx_body {
    struct ccdh_ctx_header  hdr;
    cc_unit              xy[];
} CC_ALIGNED(16);

struct ccdh_ctx_public {
    struct ccdh_ctx_header  hdr;
    cc_unit pub[];
} CC_ALIGNED(16);

/* The ccdh_full_ctx_decl macro allocates an array of ccdh_full_ctx */
typedef struct ccdh_full_ctx {
    struct ccdh_ctx_header  hdr;
} CC_ALIGNED(16) ccdh_full_ctx;

/* The ccdh_pub_ctx_decl macro allocates an array of ccdh_pub_ctx */
typedef struct ccdh_pub_ctx {
    struct ccdh_ctx_header  hdr;
} CC_ALIGNED(16) ccdh_pub_ctx;

/* A ccdh_full_ctx_t is a pointer to a dh key pair.  It should be
   allocated to be sizeof(ccdh_full_ctx_decl()) bytes. Each of the
   ccns within a dh key is always ccdh_ctx_n() cc_units long. */
typedef union {
    ccdh_full_ctx *_full;              // Constructor
    struct ccdh_ctx_header *hdr;
    struct ccdh_ctx_body *body;
    struct ccdh_ctx_public *pub;
} __attribute__((transparent_union)) ccdh_full_ctx_t;

typedef union {
    ccdh_pub_ctx *_pub;              // Constructor
    ccdh_full_ctx *_full;            // Constructor
    struct ccdh_ctx_header *hdr;
    struct ccdh_ctx_body *body;
    struct ccdh_ctx_public *pub;
    ccdh_full_ctx_t fullt;           // Conversion constructor fk->pk
} __attribute__((transparent_union)) ccdh_pub_ctx_t;

#else  //================================================= NO transparent union

struct ccdh_gp {
    __CCZP_ELEMENTS_DEFINITIONS()
} CC_ALIGNED(CCN_UNIT_SIZE);

/* A ccdh_gp_t is a pointer to a set of DH parameters.
 The first entry is a (short) prime field. */
typedef struct ccdh_gp *ccdh_gp_t;

/* A ccdh_const_gp_t is a const pointer to a set of DH parameters.
 The first entry is a const prime field. */
typedef const struct ccdh_gp *ccdh_const_gp_t;

/* The ccdh_full_ctx_decl macro allocates an array of ccdh_full_ctx */
struct  ccdh_full_ctx {
    ccdh_const_gp_t     gp;
    uint8_t             pad[16 - sizeof(ccdh_const_gp_t *)];
    cc_unit             xy[];
} CC_ALIGNED(16) ;

/* The ccdh_pub_ctx_decl macro allocates an array of ccdh_pub_ctx */
struct  ccdh_pub_ctx {
    ccdh_const_gp_t     gp;
    uint8_t             pad[16 - sizeof(ccdh_const_gp_t *)];
    cc_unit             xy[];
} CC_ALIGNED(16) ;

/* A ccdh_full_ctx_t is a pointer to a dh key pair.  It should be
 allocated to be sizeof(ccdh_full_ctx_decl()) bytes. Each of the
 ccns within a dh key is always ccdh_ctx_n() cc_units long. */

typedef struct ccdh_full_ctx *ccdh_full_ctx_t;
typedef struct ccdh_pub_ctx *ccdh_pub_ctx_t;

#endif //================================================= NO transparent union

#if CORECRYPTO_USE_TRANSPARENT_UNION
    /* Return the size of an ccdh_full_ctx where each ccn is _size_ bytes. */
    /* Full has x and y */
    #define ccdh_full_ctx_size(_size_)  (sizeof(struct ccdh_ctx_header) + 2 * (_size_))
    /* Pub has only y */
    #define ccdh_pub_ctx_size(_size_)   (sizeof(struct ccdh_ctx_header) + 1 * (_size_))

    /* Declare a fully scheduled dh key.  Size is the size in bytes each ccn in
     the key.  For example to declare (on the stack or in a struct) a 1024 bit
     dh public key named foo use ccdh_pub_ctx_decl(ccn_sizeof(1024), foo). */
    #define ccdh_full_ctx_decl(_size_, _name_)  cc_ctx_decl(ccdh_full_ctx, ccdh_full_ctx_size(_size_), _name_)
    #define ccdh_pub_ctx_decl(_size_, _name_)   cc_ctx_decl(ccdh_pub_ctx, ccdh_pub_ctx_size(_size_), _name_)
#else
    /* Return the size of an ccdh_full_ctx where each ccn is _size_ bytes. */
    /* Full has x and y */
    #define ccdh_full_ctx_size(_size_)  (sizeof(struct ccdh_full_ctx) + 2 * (_size_))
    /* Pub has only y */
    #define ccdh_pub_ctx_size(_size_)   (sizeof(struct ccdh_pub_ctx) + 1 * (_size_))

    /* Declare a fully scheduled dh key.  Size is the size in bytes each ccn in
     the key.  For example to declare (on the stack or in a struct) a 1024 bit
     dh public key named foo use ccdh_pub_ctx_decl(ccn_sizeof(1024), foo). */
    #define ccdh_full_ctx_decl(_size_, _name_)  cc_ctx_decl(struct ccdh_full_ctx, ccdh_full_ctx_size(_size_), _name_)
    #define ccdh_pub_ctx_decl(_size_, _name_)   cc_ctx_decl(struct ccdh_pub_ctx, ccdh_pub_ctx_size(_size_), _name_)
#endif

#define ccdh_pub_ctx_clear(_size_, _name_)   cc_clear(ccdh_pub_ctx_size(_size_), _name_)
#define ccdh_full_ctx_clear(_size_, _name_)  cc_clear(ccdh_full_ctx_size(_size_), _name_)
/* Declare storage for a fully scheduled dh key for a given set of dh parameters. */
#define ccdh_full_ctx_decl_gp(_gp_, _name_) ccdh_full_ctx_decl(ccdh_ccn_size(_gp_), _name_)
#define ccdh_pub_ctx_decl_gp(_gp_, _name_)  ccdh_pub_ctx_decl(ccdh_ccn_size(_gp_), _name_)

/* Return the length of the prime for gp in bits. */
#define ccdh_gp_prime_bitlen(GP)  (ccn_bitlen(ccdh_gp_n(GP), ccdh_gp_prime(GP)))

/* Return the sizeof the prime for gp. */
#define ccdh_gp_prime_size(GP)  (ccdh_ccn_size(GP))

#if CORECRYPTO_USE_TRANSPARENT_UNION
/* Group parameters accessors */
/* If you set the structure manually, you must set it to zero to be
 future proof */
#define CCDH_GP_N(_gp_)         (CCZP_N((_gp_).zp))
#define CCDH_GP_PRIME(_gp_)     (CCZP_PRIME((_gp_).zp))
#define CCDH_GP_ZP(_gp_)        ((_gp_).zp)
static inline cczp_const_t ccdh_gp_zp(ccdh_const_gp_t gp) { return gp.zp;}
#define CCDH_GP_RECIP(_gp_)     (CCZP_RECIP((_gp_).zp))
/* l must be chosen wisely to avoid the private key to be recoverable with the Pohlig-Hellman algorithm for example. "Small" l is only possible for special groups for example when p is a safe prime. */
#else
/* Group parameters accessors */
/* If you set the structure manually, you must set it to zero to be
 future proof */
#define CCDH_GP_N(_gp_)         (CCZP_N(_gp_))
#define CCDH_GP_PRIME(_gp_)     (CCZP_PRIME(_gp_))
#define CCDH_GP_ZP(_gp_)        ((cczp_t)(_gp_))
static inline cczp_const_t ccdh_gp_zp(ccdh_const_gp_t gp) { return (cczp_const_t) gp;}
#define CCDH_GP_RECIP(_gp_)     (CCZP_RECIP((_gp_)))
#endif

#define CCDH_GP_G(_gp_)         (CCDH_GP_RECIP(_gp_) + 1 + ccdh_gp_n(_gp_))  // recip size is n+1
#define CCDH_GP_L(_gp_)         (*((CCDH_GP_Q(_gp_) + ccdh_gp_n(_gp_)))) // Size of the private key in bit.
#define CCDH_GP_Q(_gp_)         (CCDH_GP_G(_gp_) + ccdh_gp_n(_gp_))          // generator size is n
/* l must be chosen wisely to avoid the private key to be recoverable with the Pohlig-Hellman algorithm for example. "Small" l is only possible for special groups for example when p is a safe prime. */

/* Return the size of a ccdh_gp where the prime is of _size_ bytes. */
#define ccdh_gp_size(_size_) (cczp_size(_size_) + 2 * (_size_) + ccn_sizeof_n(1))

#if CORECRYPTO_USE_TRANSPARENT_UNION

/* Declare a gp  */
#define ccdh_gp_decl(_size_, _name_)  cc_ctx_decl(ccdh_gp, ccdh_gp_size(_size_), _name_)

/* lvalue accessors to ccdh_ctx fields. (only a ccdh_full_ctx_t has y). */
/* gp: group parameter */
#define ccdh_ctx_gp(KEY)     (((ccdh_pub_ctx_t)(KEY)).hdr->gp)
//we do not call ccdh_ctx_public(), because  ccdh_ctx_gp may be called with ccdh_pub_ctx_t KEY
/* n: size of group */
#define ccdh_ctx_n(KEY)      (ccdh_gp_n(ccdh_ctx_gp(KEY)))
/* prime: group prime */
#define ccdh_ctx_prime(KEY)  (ccdh_gp_prime(ccdh_ctx_gp(KEY)))
/* y: the public key */
#define ccdh_ctx_y(KEY)  (((ccdh_pub_ctx_t)(KEY)).body->xy)
/* x: the private key */
#define ccdh_ctx_x(KEY)  (((ccdh_full_ctx_t)(KEY)).body->xy + 1 * ccdh_ctx_n(KEY))
#else

/* Declare a gp  */
#define ccdh_gp_decl(_size_, _name_)  cc_ctx_decl(struct ccdh_gp, ccdh_gp_size(_size_), _name_)

/* lvalue accessors to ccdh_ctx fields. (only a ccdh_full_ctx_t has y). */
/* gp: group parameter */
#define ccdh_ctx_gp(KEY)     (((ccdh_pub_ctx_t)(KEY))->gp)
/* n: size of group */
#define ccdh_ctx_n(KEY)      (ccdh_gp_n(ccdh_ctx_gp(KEY)))
/* prime: group prime */
#define ccdh_ctx_prime(KEY)  (ccdh_gp_prime(ccdh_ctx_gp(KEY)))
/* y: the public key */
#define ccdh_ctx_y(KEY)    ((KEY)->xy)
/* x: the private key */
#define ccdh_ctx_x(KEY)    (ccdh_ctx_y(KEY) + 1 * ccdh_ctx_n(KEY))  

#endif

CC_INLINE
ccdh_pub_ctx_t ccdh_ctx_public(ccdh_full_ctx_t key) {
    return (ccdh_pub_ctx_t)key;
}

#if CORECRYPTO_USE_TRANSPARENT_UNION

/* Callers must call this function to initialze a ccdh_full_ctx or
   ccdh_pub_ctx before using most of the macros in this file. */ 
CC_INLINE CC_NONNULL_TU((1))
void ccdh_ctx_init(ccdh_const_gp_t gp, ccdh_pub_ctx_t key) {
    key.hdr->gp = gp;
}

/* rvalue accessors to ccdh_ctx fields. */

/* Return count (n) of a ccn for gp. */
CC_CONST CC_INLINE CC_NONNULL_TU((1))
cc_size ccdh_gp_n(ccdh_const_gp_t gp) {
    return cczp_n(gp.zp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_prime(ccdh_const_gp_t gp) {
    return cczp_prime(gp.zp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_recip(ccdh_const_gp_t gp) {
    return cczp_recip(gp.zp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit ccdh_gp_options(ccdh_const_gp_t gp) {
    return cczp_options(gp.zp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_g(ccdh_const_gp_t gp) {
    return CCDH_GP_G(gp._ncgp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_order(ccdh_const_gp_t gp) {
    return CCDH_GP_Q(gp._ncgp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
cc_size ccdh_gp_l(ccdh_const_gp_t gp) {
    return CCDH_GP_L(gp._ncgp);
}
/* Return sizeof a ccn for gp. */
CC_CONST CC_INLINE CC_NONNULL_TU((1))
size_t ccdh_ccn_size(ccdh_const_gp_t gp) {
    return ccn_sizeof_n(cczp_n(gp.zp));
}

#else //=================================================================================
/* Callers must call this function to initialze a ccdh_full_ctx or
 ccdh_pub_ctx before using most of the macros in this file. */
CC_INLINE CC_NONNULL_TU((1))
void ccdh_ctx_init(ccdh_const_gp_t gp, ccdh_pub_ctx_t key) {
    key->gp = gp;
}

/* rvalue accessors to ccdh_ctx fields. */

/* Return count (n) of a ccn for gp. */
CC_CONST CC_INLINE CC_NONNULL_TU((1))
cc_size ccdh_gp_n(ccdh_const_gp_t gp) {
    return cczp_n((cczp_const_t)gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_prime(ccdh_const_gp_t gp) {
    return cczp_prime((cczp_const_t)gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_recip(ccdh_const_gp_t gp) {
    return cczp_recip((cczp_const_t)gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit ccdh_gp_options(ccdh_const_gp_t gp) {
    return cczp_options((cczp_const_t)gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_g(ccdh_const_gp_t gp) {
    return CCDH_GP_G(gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
const cc_unit *ccdh_gp_order(ccdh_const_gp_t gp) {
    return CCDH_GP_Q(gp);
}

CC_CONST CC_INLINE CC_NONNULL_TU((1))
cc_size ccdh_gp_l(ccdh_const_gp_t gp) {
    return CCDH_GP_L((ccdh_const_gp_t)gp);
}

/* Return sizeof a ccn for gp. */
CC_CONST CC_INLINE CC_NONNULL_TU((1))
size_t ccdh_ccn_size(ccdh_const_gp_t gp) {
    return ccn_sizeof_n(CCZP_N(gp));
}

#endif//=================================================================================

CC_INLINE CC_NONNULL_TU((1))
size_t ccdh_gp_order_bitlen(ccdh_const_gp_t gp) {
    return ccn_bitlen(ccdh_gp_n(gp),ccdh_gp_order(gp));
}



/* DH group parameter initialization */

/*
 * Group paramters must be well chosen to avoid serious security issues.
 *  a) ccdh_init_gp with l>0 is to be used for group parameter where p is a safe prime.
 *     l should be at least twice the security level desired (128bit security => l=256).
 *     If you are not sure, set l=0, it is slow but it is safe against attacks using the
 *     Pohlig-Hellman algorithm for example.
 *  b) ccdh_init_gp_with_order is to be used when the group prime is not a safe prime:
 *     the order is necessary to avoid small subgroup attacks and generate the private key
 *     efficiently
 *  c) ccdh_init_gp_with_order to set the group from byte.
 *           If the group prime is not a safe prime, the order MUST be provided to avoid small subgroup attacks
 *           If the group prime is a safe prime, l should be at least twice the security level desired (128bit security => l=256).
 *                  If you are not sure, set l=0, it is slow but it is safe against attacks using the
 *                  Pohlig-Hellman algorithm for example.
 */
CC_NONNULL_TU((1)) CC_NONNULL((3, 4))
int ccdh_init_gp(ccdh_gp_t gp, cc_size n,
                 const cc_unit *p,
                 const cc_unit *g,
                 cc_size l);

CC_NONNULL_TU((1)) CC_NONNULL((3, 4, 5))
int ccdh_init_gp_with_order(ccdh_gp_t gp, cc_size n,
                            const cc_unit *p,
                            const cc_unit *g,
                            const cc_unit *q);

CC_NONNULL_TU((1)) CC_NONNULL((4, 6))
int ccdh_init_gp_from_bytes(ccdh_gp_t gp, cc_size n,
                            size_t p_len, const uint8_t *p,
                            size_t g_len, const uint8_t *g,
                            size_t q_len, const uint8_t *q,
                            cc_size l);

/* 
 * Generate a DH private/public key pair from the group parameter 
 */
CC_NONNULL_TU((1)) CC_NONNULL2
int ccdh_generate_key(ccdh_const_gp_t gp, struct ccrng_state *rng,
                      ccdh_full_ctx_t key);

/* 
 * Compute an DH shared secret between private_key and public_key after validation the public key.
 * Returns the result in computed_key, which must be an array of ccdh_ctx_n(private_key) cc_units
 * DEPRECATED - use ccdh_compute_key
 */

CC_NONNULL_TU((1,2)) CC_NONNULL3
int ccdh_compute_key(ccdh_full_ctx_t private_key, ccdh_pub_ctx_t public_key,
                     cc_unit *computed_key);

/* Leading bytes of computed_shared_secret (a.k.a. Z) that contain all zero bits 
 are stripped before it is used as the shared secret. Match common specs such as TLS */
CC_NONNULL_TU((1,2)) CC_NONNULL4
int ccdh_compute_shared_secret(ccdh_full_ctx_t private_key,
                               ccdh_pub_ctx_t public_key,
                               size_t *computed_shared_secret_len,
                               uint8_t *computed_shared_secret,
                               struct ccrng_state *blinding_rng);


/* Import a public key. The imported key is an Octet String, as defined in PKCS#3 */
CC_NONNULL_TU((1)) CC_NONNULL3
int ccdh_import_pub(ccdh_const_gp_t gp, size_t in_len, const uint8_t *in,
                    ccdh_pub_ctx_t key);

/* Import a private key. The imported key is an Octet String, as defined in PKCS#3 */
CC_NONNULL_TU((1)) CC_NONNULL3
int ccdh_import_priv(ccdh_const_gp_t gp, size_t in_len, const uint8_t *in,
                     ccdh_full_ctx_t key);

/* Import a private key. The imported key is an Octet String, as defined in PKCS#3 */
CC_NONNULL_TU((1)) CC_NONNULL3
int ccdh_import_full(ccdh_const_gp_t gp,
                     size_t in_priv_len, const uint8_t *in_priv,
                     size_t in_pub_len,  const uint8_t *in_pub,
                     ccdh_full_ctx_t key);

/* Return the sizeof a buffer needed to exported public key to. */
CC_INLINE CC_CONST CC_NONNULL_TU((1))
size_t ccdh_export_pub_size(ccdh_pub_ctx_t key) {
    return ccdh_gp_prime_size(ccdh_ctx_gp(key));
}

/* Export public key to out. Out must be ccdh_export_pub_size(key) bytes long.
   The key is exported as an Octet String, as defined in PKCS#3 */
CC_NONNULL_TU((1)) CC_NONNULL2
void ccdh_export_pub(ccdh_pub_ctx_t key, void *out);

/* 
 * ASN.1/DER glue from PKCS #3 :
 * prime p, generator g, and optional privateValueLength l
 */

CC_NONNULL_TU((1))
size_t ccder_encode_dhparams_size(const ccdh_const_gp_t gp);

CC_NONNULL_TU((1)) CC_NONNULL((2)) CC_NONNULL((3))
uint8_t * ccder_encode_dhparams(const ccdh_const_gp_t gp, uint8_t *der, uint8_t *der_end);

/* CCZP_N(gpfoo.zp) must be set before decoding */
CC_NONNULL_TU((1)) CC_NONNULL((2))
const uint8_t *ccder_decode_dhparams(ccdh_gp_t gp, const uint8_t *der, const uint8_t *der_end);

/* returns the n needed for ccdh_gp_decl/heap allocation of a ccdh_gp_t, can be larger then the actual size used */
CC_NONNULL((1))
cc_size ccder_decode_dhparam_n(const uint8_t *der, const uint8_t *der_end);

#endif /* _CORECRYPTO_CCDH_H_ */
