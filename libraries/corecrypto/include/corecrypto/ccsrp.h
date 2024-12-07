/*
 * Copyright (c) 2012,2013,2014,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef SRP_H
#define SRP_H

#include <corecrypto/ccn.h>
#include <corecrypto/cczp.h>
#include <corecrypto/ccdigest.h>
#include <corecrypto/ccdh.h>
#include <corecrypto/ccrng.h>

/* Aliases for DH-style group params for SRP */
#if CORECRYPTO_USE_TRANSPARENT_UNION
typedef ccdh_gp ccsrp_gp;
#else
typedef struct ccdh_gp ccsrp_gp;
#endif
#define CCSRP_HDR_PAD 32

#if CORECRYPTO_USE_TRANSPARENT_UNION
typedef ccdh_gp_t ccsrp_gp_t;
typedef ccdh_const_gp_t ccsrp_const_gp_t;

struct ccsrp_ctx_header {
    const struct ccdigest_info *di;
    ccsrp_const_gp_t gp;
    struct ccrng_state *blinding_rng;
    struct {
        unsigned int authenticated:1;
        unsigned int noUsernameInX:1;
        unsigned int sessionkey:1;
        unsigned int variant:16;
    } flags;
    uint8_t           pad[CCSRP_HDR_PAD - (sizeof(struct ccdigest_info *)+
                                            sizeof(ccsrp_const_gp_t)+
                                            sizeof(struct ccrng_state *)+
                                            sizeof(bool))];
    cc_unit              ccn[1];
} CC_ALIGNED(16);

typedef struct ccsrp_ctx {
    struct ccsrp_ctx_header  hdr;
} CC_ALIGNED(16) ccsrp_ctx;

struct ccsrp_ctx_body {
    struct ccsrp_ctx_header  hdr;
    cc_unit              ccn[];
} CC_ALIGNED(16);

typedef union {
    ccsrp_ctx *_full;              // Constructor
    struct ccsrp_ctx_header *hdr;
    struct ccsrp_ctx_body *body;
} __attribute__((transparent_union)) ccsrp_ctx_t;
#else
typedef ccdh_gp_t ccsrp_gp_t;
typedef ccdh_const_gp_t ccsrp_const_gp_t;

struct ccsrp_ctx {
    const struct ccdigest_info *di;
    ccsrp_const_gp_t gp;
    struct ccrng_state *blinding_rng;
    struct {
        unsigned int authenticated:1;
        unsigned int noUsernameInX:1;
        unsigned int sessionkey:1;
        unsigned int variant:16;
    } flags;
    cc_unit              ccn[1];
}  CC_ALIGNED(16);
typedef struct ccsrp_ctx *ccsrp_ctx_t;

struct ccsrp_ctx_legacy {
    const struct ccdigest_info *di;
    ccsrp_const_gp_t gp;
    struct ccrng_state *blinding_rng;
    struct {
        unsigned int authenticated:1;
        unsigned int noUsernameInX:1;
        unsigned int sessionkey:1;
        unsigned int variant:16;
    } flags;
    uint8_t           pad[CCSRP_HDR_PAD - (sizeof(struct ccdigest_info *)+
                                           sizeof(ccsrp_const_gp_t)+
                                           sizeof(struct ccrng_state *)+
                                           sizeof(bool))];
    cc_unit              ccn[1];
}  CC_ALIGNED(16);
typedef struct ccsrp_ctx_legacy *ccsrp_ctx_t_legacy;

cc_unit *srp_ccn(void *srp);

#endif

#define ccsrp_gpbuf_size(_gp_) (ccdh_ccn_size(_gp_)*4)
#define ccsrp_dibuf_size(_di_) ((_di_)->output_size*4)

/* Size of the context structure for the di and gp combo */
#define ccsrp_sizeof_srp(_di_,_gp_) sizeof(struct ccsrp_ctx)+\
    ccsrp_gpbuf_size(_gp_)+ccsrp_dibuf_size(_di_)

/* Use this to declare a context on the stack 
 Use ccsrp_ctx_clear when done to prevent exposing key material */
#define ccsrp_ctx_decl(_di_, _gp_, _name_) \
    cc_ctx_decl(struct ccsrp_ctx, ccsrp_sizeof_srp(_di_,_gp_), _name_)

#define ccsrp_ctx_clear(_di_, _gp_, _name_) \
    cc_clear(ccsrp_sizeof_srp(_di_,_gp_), _name_)

/*
 Accessors to the context structure.
 */
#if CORECRYPTO_USE_TRANSPARENT_UNION
#define HDR(srp) ((srp).hdr)
#else
#define HDR(srp) (srp)
#endif

#define SRP_DI(srp) (HDR(srp)->di)
#define SRP_GP(srp) (HDR(srp)->gp)
#define SRP_FLG(srp) (HDR(srp)->flags)
#define SRP_CCN(srp) (HDR(srp)->ccn)
#define SRP_RNG(srp) (HDR(srp)->blinding_rng)

#define ccsrp_ctx_gp(KEY)  SRP_GP((ccsrp_ctx_t)(KEY))
#define ccsrp_ctx_di(KEY)  SRP_DI((ccsrp_ctx_t)(KEY))
#define ccsrp_ctx_zp(KEY)   ccdh_gp_zp((ccsrp_ctx_gp(KEY)))
#define ccsrp_ctx_gp_g(KEY)   (ccdh_gp_g(ccsrp_ctx_gp(KEY)))
#define ccsrp_ctx_gp_l(KEY)   (ccdh_gp_l(ccsrp_ctx_gp(KEY)))
#define ccsrp_ctx_n(KEY)      (ccdh_gp_n(ccsrp_ctx_gp(KEY)))
#define ccsrp_ctx_prime(KEY)  (ccdh_gp_prime(ccsrp_ctx_gp(KEY)))
#define ccsrp_ctx_ccn(KEY)    srp_ccn(KEY)
#define ccsrp_ctx_pki_key(KEY,_N_) (ccsrp_ctx_ccn(KEY) + ccsrp_ctx_n(KEY) * _N_)
#define ccsrp_ctx_public(KEY)           (ccsrp_ctx_pki_key(KEY,0))
#define ccsrp_ctx_private(KEY)          (ccsrp_ctx_pki_key(KEY,1))
#define ccsrp_ctx_v(KEY)                (ccsrp_ctx_pki_key(KEY,2))
#define ccsrp_ctx_S(KEY)                (ccsrp_ctx_pki_key(KEY,3))
#define ccsrp_ctx_K(KEY)    ((uint8_t *)(ccsrp_ctx_pki_key(KEY,4)))
#define ccsrp_ctx_M(KEY)    (uint8_t *)(ccsrp_ctx_K(KEY)+\
2*ccsrp_ctx_di(KEY)->output_size)
#define ccsrp_ctx_HAMK(KEY) (uint8_t *)(ccsrp_ctx_K(KEY)+\
3*ccsrp_ctx_di(KEY)->output_size)

/* Session Keys and M and HAMK are returned in this many bytes */
#define ccsrp_ctx_keysize(KEY)   ccsrp_get_session_key_length(KEY)

#define ccsrp_ctx_M_HAMK_size(KEY)   (ccsrp_ctx_di(KEY)->output_size)

/* The public keys and the verifier are returned in this many bytes */
#define ccsrp_ctx_sizeof_n(KEY)   (ccn_sizeof_n(ccsrp_ctx_n(KEY)))

/******************************************************************************
 *  Error codes
 *****************************************************************************/

#define CCSRP_ERROR_DEFAULT                 CCDH_ERROR_DEFAULT
#define CCSRP_GENERATE_KEY_TOO_MANY_TRIES   CCDH_GENERATE_KEY_TOO_MANY_TRIES
#define CCSRP_NOT_SUPPORTED_CONFIGURATION   CCDH_NOT_SUPPORTED_CONFIGURATION
#define CCSRP_SAFETY_CHECK                  CCDH_SAFETY_CHECK
#define CCSRP_PUBLIC_KEY_MISSING            CCDH_PUBLIC_KEY_MISSING
#define CCSRP_INVALID_DOMAIN_PARAMETER      CCDH_INVALID_DOMAIN_PARAMETER

/******************************************************************************
 *  Variant (main difference is key derivation after DH di
 *****************************************************************************/
/* OPTION
     [0..2]: KDF to compute K from S
     [3..5]: Variant (value of k in the computation of B)
     [6..7]: Padding in Hash (leading zeroes hashed or skipped in hashes) */

// Do Not use these flags directly. Please use one of the "combo" flags.
// and request a new combo flag is needed.

// Selection of KDF for the session key
#define CCSRP_OPTION_KDF_MASK           (7<<0)
// K = H(S), size of K is the size of the digest output
#define CCSRP_OPTION_KDF_HASH           (0<<0)
// K = MGF1(S), size of K is TWICE the size of the digest output
#define CCSRP_OPTION_KDF_MGF1           (1<<0)
// K = H_Interleave(S), size of K is TWICE the size of the digest output
#define CCSRP_OPTION_KDF_INTERLEAVED    (2<<0)

// Selection of the variant for internal computation
#define CCSRP_OPTION_VARIANT_MASK       (7<<3)
// k = HASH(N | PAD(g)) and u = HASH(PAD(A) | PAD(B))
#define CCSRP_OPTION_VARIANT_SRP6a      (0<<3)
// K = 1 and u=MSB32bit(HASH(PAD(B))
#define CCSRP_OPTION_VARIANT_RFC2945    (1<<3)

// Selection of leading zeroes in integer hashes
#define CCSRP_OPTION_PAD_MASK           (3<<6)
// Skip zeroes of A and B during hashes for the computation of k, U and X
#define CCSRP_OPTION_PAD_SKIP_ZEROES_k_U_X     (1<<6)
// Skip leading zeroes when hashing A,B in M and HAMK only
// This is a hack to be compatible with AppleSRP implementation
#define CCSRP_OPTION_PAD_SKIP_ZEROES_TOKEN     (2<<6)


// Higher level combos:
//  Corecrypto default
#define CCSRP_OPTION_SRP6a_HASH      (CCSRP_OPTION_VARIANT_SRP6a \
                                            | CCSRP_OPTION_KDF_HASH)

// Improved SRP6a (with MGF1) compatible with SRP
// The domain parameter (g) is hashed on the exact number of bytes instead hashing
// modlen bytes.
#define CCSRP_OPTION_SRP6a_MGF1     (CCSRP_OPTION_VARIANT_SRP6a  \
                                            | CCSRP_OPTION_KDF_MGF1 \
                                            | CCSRP_OPTION_PAD_SKIP_ZEROES_TOKEN)

//  TLS-SRP. Not recommended except when interoperability is required
#define CCSRP_OPTION_RFC2945_INTERLEAVED    (CCSRP_OPTION_VARIANT_RFC2945  \
                                            | CCSRP_OPTION_KDF_INTERLEAVED \
                                            | CCSRP_OPTION_PAD_SKIP_ZEROES_k_U_X \
                                            | CCSRP_OPTION_PAD_SKIP_ZEROES_TOKEN)




/*!
 @function   ccsrp_ctx_init_option
 @abstract   Initialize the SRP context

 @param  srp            SRP
 @param  di             handle on the digest to be used (ex. ccsha1_di())
 @param  gp             handle on DH group parameters (requires group with no small subgroups)
 @param  option         Define variant, key derivation and padding of integers being hashed.
 @param  blinding_rng   For randomization of internal computations, rng may be used for as long as the "srp" context is used.

 @result 0 if no error
 */
/* Init context structures with this function */
CC_NONNULL_TU((1,3)) CC_NONNULL((2))
CC_INLINE int
ccsrp_ctx_init_option(ccsrp_ctx_t srp,
                      const struct ccdigest_info *di,
                      ccsrp_const_gp_t gp,
                      uint32_t option,
                      struct ccrng_state *blinding_rng) {
    cc_zero(ccsrp_sizeof_srp(di, gp),HDR(srp));
    SRP_DI(srp) = di;
    SRP_GP(srp) = gp;
    SRP_FLG(srp).authenticated = false;
    SRP_FLG(srp).sessionkey = false;
    SRP_RNG(srp)=blinding_rng;
    // Option is a bit mask. If not a power of two, it's an error.
    SRP_FLG(srp).variant = 0xFFFF & option;
    return 0; // Success
}

// Legacy function, initialize for the RFC5054 variant.
CC_NONNULL_TU((1,3)) CC_NONNULL((2))
CC_INLINE void
ccsrp_ctx_init(ccsrp_ctx_t srp, const struct ccdigest_info *di, ccsrp_const_gp_t gp) {
    ccsrp_ctx_init_option(srp,di,gp,CCSRP_OPTION_SRP6a_HASH,ccrng(NULL));
}

/******************************************************************************
 *  Salt and Verification Generation - used to setup an account.
 *****************************************************************************/

/*!
 @function   ccsrp_generate_salt_and_verification
 @abstract   Generate the salt and the verification token to be used for future 
                authentications

 @param      srp        SRP
 @param      rng        handle on rng for ephemeral key generation
 @param      username   identity
 @param      password_len length in byte of the password
 @param      password   password of length password_len
 @param      salt_len   length in byte of the salt
 @param      salt       salt of length salt_len (output)
 @param      verifier   password verifier known to the server (output)

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,3,5,7,8))
int
ccsrp_generate_salt_and_verification(ccsrp_ctx_t srp, struct ccrng_state *rng,
                                     const char *username,
                                     size_t password_len, const void *password,
                                     size_t salt_len, void *salt,
                                     void *verifier);

/*!
 @function   ccsrp_generate_verifier
 @abstract   Generate the verification token to be used for future
 authentications

 @param      srp        SRP
 @param      username   identity
 @param      password_len length in byte of the password
 @param      password   password of length password_len
 @param      salt_len   length in byte of the salt
 @param      salt       salt of length salt_len (input)
 @param      verifier   password verifier known to the server (output)

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,4,6,7))
int
ccsrp_generate_verifier(ccsrp_ctx_t srp,
			const char *username,
			size_t password_len, const void *password, 
			size_t salt_len, const void *salt,
			void *verifier);

/******************************************************************************
 *  Server Side Routines
 *****************************************************************************/

/*!
 @function   ccsrp_server_generate_public_key
 @abstract   Generate the server value B. 
             If A is known, call directly ccsrp_server_start_authentication

 @param      srp        SRP
 @param      rng        handle on rng for ephemeral key generation
 @param      username   identity
 @param      verifier   password verifier known to the server
 @param      B_bytes    Value B which is the challenge to send to the client (output)

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,3,4))
int
ccsrp_server_generate_public_key(ccsrp_ctx_t srp, struct ccrng_state *rng,
                                 const void *verifier, void *B_bytes);

/*!
 @function   ccsrp_server_compute_session
 @abstract   Generate the session key material and tokens for authentication

 @param      srp        SRP
 @param      username   identity
 @param      salt_len   length in byte of the salt
 @param      salt       salt of length salt_len
 @param      A_bytes    Ephemeral public key received from the client

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,4,5))
int
ccsrp_server_compute_session(ccsrp_ctx_t srp,
                              const char *username,
                              size_t salt_len, const void *salt,
                              const void *A_bytes);

/*!
 @function   ccsrp_server_start_authentication
 @abstract   Performs in one shot the server public key and the session key material

 @param      srp        SRP
 @param      rng        handle on rng for ephemeral key generation
 @param      username   identity
 @param      salt_len   length in byte of the salt
 @param      salt       salt of length salt_len
 @param      verifier   password verifier known to the server
 @param      A_bytes    Ephemeral public key received from the client
 @param      B_bytes    Value B which is the challenge to send to the client (output)

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,3,5,6,7,8))
int
ccsrp_server_start_authentication(ccsrp_ctx_t srp, struct ccrng_state *rng,
                                  const char *username,
                                  size_t salt_len, const void *salt,
                                  const void *verifier, const void *A_bytes,
                                  void *B_bytes);

/*!
 @function   ccsrp_server_verify_session
 @abstract   Verify that the token received from the client is correct and that
            therefore authentication succeeded.

 @param      srp            SRP
 @param      user_M         Authentication token received from the client
 @param      HAMK_bytes     Authentication token generated to be sent to the client (output)

 @result true if client is authenticated, false otherwise (fail or incomplete protocol)
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,3))
bool
ccsrp_server_verify_session(ccsrp_ctx_t srp, const void *user_M,
                            void *HAMK_bytes);

/******************************************************************************
 *  Client Side Routines
 *****************************************************************************/

/*!
 @function   ccsrp_client_start_authentication
 @abstract   Initiate protocol with an ephemeral public key

 @param      srp        SRP
 @param      rng        handle on random for key generation
 @param      A_bytes    Output public key to send to the server

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,3))
int
ccsrp_client_start_authentication(ccsrp_ctx_t srp, struct ccrng_state *rng,
                                  void *A_bytes);

/*!
 @function   ccsrp_client_process_challenge
 @abstract   Process the challenge receive from the server

 @param      srp        SRP
 @param      username   identity
 @param      password_len length in byte of the password
 @param      password   password of length password_len
 @param      salt_len   length in byte of the salt
 @param      salt       salt of length salt_len
 @param      B_bytes    Value B received from the server
 @param      M_bytes    Response to the challenge (output)

 @result 0 if no error
 */
CC_NONNULL_TU((1)) CC_NONNULL((2,4,6,7,8))
int
ccsrp_client_process_challenge(ccsrp_ctx_t srp,
                               const char *username,
                               size_t password_len, const void *password,
                               size_t salt_len, const void *salt,
                               const void *B_bytes,
                               void *M_bytes);

/*!
 @function   ccsrp_client_verify_session
 @abstract   Verify that the token received from the server is correct and that
             therefore authentication succeeded.

 @param      srp            SRP
 @param      HAMK_bytes     Authentication token received from the server

 @result true if authenticated, false otherwise (fail or incomplete protocol)
 */
CC_NONNULL_TU((1)) CC_NONNULL((2))
bool
ccsrp_client_verify_session(ccsrp_ctx_t srp, const uint8_t *HAMK_bytes);

CC_NONNULL_TU((1))
CC_INLINE bool
ccsrp_client_set_noUsernameInX(ccsrp_ctx_t srp, bool flag)
{
    return HDR(srp)->flags.noUsernameInX = !!flag;
}


/******************************************************************************
 *  Functions for both sides
 *****************************************************************************/

/*!
 @function   ccsrp_is_authenticated
 @abstract   Returns whether authentication was successful

 @param      srp        SRP

 @result true if authenticated, false otherwise (fail or incomplete protocol)
 */
CC_NONNULL_TU((1))
CC_INLINE bool
ccsrp_is_authenticated(ccsrp_ctx_t srp) {
	return HDR(srp)->flags.authenticated;
}

/*!
 @function   ccsrp_exchange_size
 @abstract   Returns the size of the public keys exchanged

 @param      srp        SRP

 @result The length of the public key in bytes
 */
CC_NONNULL_TU((1))
CC_INLINE size_t
ccsrp_exchange_size(ccsrp_ctx_t srp) {
    return ccsrp_ctx_sizeof_n(srp);
}

/*!
 @function   ccsrp_session_size
 @abstract   Returns the size of the session authentication tokens M and HAMK

 @param      srp        SRP

 @result The length of M and HAMK in bytes
 */
CC_NONNULL_TU((1))
CC_INLINE size_t
ccsrp_session_size(ccsrp_ctx_t srp) {
    /* Session Keys and M and HAMK are returned in this many bytes */
    return   (ccsrp_ctx_di(srp)->output_size);
}

/*!
 @function   ccsrp_get_session_key_length
 @abstract   Returns the size of the session key K, which depends on the variant

 @param      srp        SRP

 @result The length of K
 */
CC_NONNULL_TU((1))
CC_INLINE size_t
ccsrp_get_session_key_length(ccsrp_ctx_t srp) {
    if ((HDR(srp)->flags.variant & CCSRP_OPTION_KDF_MASK) == CCSRP_OPTION_KDF_HASH) {
        return (ccsrp_ctx_di(srp)->output_size);
    }
    else if ((    (HDR(srp)->flags.variant & CCSRP_OPTION_KDF_MASK) == CCSRP_OPTION_KDF_INTERLEAVED)
             ||  ((HDR(srp)->flags.variant & CCSRP_OPTION_KDF_MASK) == CCSRP_OPTION_KDF_MGF1)) {
        return 2*(ccsrp_ctx_di(srp)->output_size);
    }
    return 0; // Error
}

/*!
 @function   ccsrp_get_session_key
 @abstract   Returns a pointer to the session key and its size

 @param      srp            SRP context
 @param      key_length     pointer to output the size of the session key

 @result pointer to the session key in SRP context. NULL if the key has not been
            computed yet
 */
CC_NONNULL_TU((1)) CC_NONNULL((2))
CC_INLINE const void *
ccsrp_get_session_key(ccsrp_ctx_t srp, size_t *key_length) {
    *key_length = ccsrp_get_session_key_length(srp);
    if (HDR(srp)->flags.sessionkey) {
        return ccsrp_ctx_K(srp);
    } else {
        return NULL;
    }
}

/*!
 @function   ccsrp_get_premaster_secret
 @abstract   Returns a pointer to the premaster secret key
                use for TLS-SRP
                This value is not a cryptographic key. A KDF is needed before
                use.

 @param      srp            SRP context

 @result pointer to the premaster secret in SRP context.
        The secret is an array of ccsrp_ctx_n(srp) cc_units */
CC_NONNULL_TU((1))
CC_INLINE cc_unit *
ccsrp_get_premaster_secret(ccsrp_ctx_t srp) {
    if (HDR(srp)->flags.sessionkey) {
        return ccsrp_ctx_S(srp);
    } else {
        return NULL;
    }
}

/******************************************************************************
 *  Component Test Interface
 *****************************************************************************/

int
ccsrp_test_calculations(const struct ccdigest_info *di, ccsrp_const_gp_t gp,
                        struct ccrng_state *blinding_rng,
                        const char *username, uint32_t options,
                        size_t password_len, const void *password,
                        size_t salt_len, const void *salt,
                        size_t k_len, const void *k,
                        size_t x_len, const void *x,
                        size_t v_len, const void *v,
                        size_t a_len, const void *a,
                        size_t b_len, const void *b,
                        size_t A_len, const void *A,
                        size_t B_len, const void *B,
                        size_t u_len, const void *u,
                        size_t S_len, const void *S,
                        size_t K_len, const void *K,
                        size_t M_len, const void *M,
                        size_t HAMK_len, const void *HAMK
                        );

#endif /* Include Guard */

