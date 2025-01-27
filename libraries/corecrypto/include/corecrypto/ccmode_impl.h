/*
 * Copyright (c) 2012,2015,2016,2017,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCMODE_IMPL_H_
#define _CORECRYPTO_CCMODE_IMPL_H_

#include <corecrypto/cc.h>

/* ECB mode. */
cc_aligned_struct(16) ccecb_ctx;


/* Actual symmetric algorithm implementation should provide you one of these. */
struct ccmode_ecb {
    size_t size;        /* first argument to ccecb_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_ecb *ecb, ccecb_ctx *ctx,
                size_t key_nbytes, const void *key);
    int (*ecb)(const ccecb_ctx *ctx, size_t nblocks, const void *in,
               void *out);
};

/*!
 * @brief corecrypto symmetrical encryption and decryption modes
 *
 * corecrypto supports 6 stateless en(de)cryption modes and 2 stateful authenticated en(de)cryption modes
 * stateless modes CBC, CFB, CFB8, CTR, OFB, XTS: They provide 3 interface functions that do not return errors codes
 *   1- ccmod_xxx_init()
 *   2- ccmod_xxx_decrypt()
 *   3- ccmod_xxx_encrypt()
 * 
 * stateful modes CCM and GCM: They provide 7 interface functions that return error codes if a function is called out of state
 *   1- ccmod_xxx_init()
 *   2- ccmod_xxx_setiv()
 *   3- ccmod_xxx_aad()
 *   4- ccmod_xxx_decrypt()
 *   5- ccmod_xxx_encrypt()
 *   6- ccmod_xxx_finalize()
 *   7- ccmod_xxx_reset()
 *
 *  the correct call sequences are:
 *
 *  calls to 1, 2 and 6 arerequired
 *  2 and 3 can be called as mant times as needed
 *  calls to 3, 4, 5 can be skipped
 *
 *  1, 2*n, 3*n, 4|5, 6
 *  1, 2*n,    , 4|5, 6
 *  1, 2*n,    ,    , 6
 *  1, 2*n, 3*n,    , 6
 */

// 1- CBC mode, stateless
cc_aligned_struct(16) cccbc_ctx;
cc_aligned_struct(16) cccbc_iv;

struct ccmode_cbc {
    size_t size;        /* first argument to cccbc_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_cbc *cbc, cccbc_ctx *ctx,
                size_t key_len, const void *key);
    /* cbc encrypt or decrypt nblocks from in to out, iv will be used and updated. */
    int (*cbc)(const cccbc_ctx *ctx, cccbc_iv *iv,
               size_t nblocks, const void *in, void *out);
    const void *custom;
};

// 2- CFB mode, stateless
cc_aligned_struct(16) cccfb_ctx;

struct ccmode_cfb {
    size_t size;        /* first argument to cccfb_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_cfb *cfb, cccfb_ctx *ctx,
                size_t key_len, const void *key, const void *iv);
    int (*cfb)(cccfb_ctx *ctx, size_t nbytes, const void *in, void *out);
    const void *custom;
};

// 3- CFB8 mode, stateless
cc_aligned_struct(16) cccfb8_ctx;

struct ccmode_cfb8 {
    size_t size;        /* first argument to cccfb8_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_cfb8 *cfb8, cccfb8_ctx *ctx,
                size_t key_len, const void *key, const void *iv);
    int (*cfb8)(cccfb8_ctx *ctx, size_t nbytes, const void *in, void *out);
    const void *custom;
};

// 4- CTR mode, stateless
cc_aligned_struct(16) ccctr_ctx;

struct ccmode_ctr {
    size_t size;        /* first argument to ccctr_ctx_decl(). */
    size_t block_size;  /* for historical reasons, this is set to 1 */
    size_t ecb_block_size;  /* the actual block size of the underlying cipher */
    int (*init)(const struct ccmode_ctr *mode, ccctr_ctx *ctx,
                size_t key_len, const void *key, const void *iv);
    int (*setctr)(const struct ccmode_ctr *mode, ccctr_ctx *ctx, const void *ctr);
    int (*ctr)(ccctr_ctx *ctx, size_t nbytes, const void *in, void *out);
    const void *custom;
};

// 5- OFB mode, stateless
cc_aligned_struct(16) ccofb_ctx;

struct ccmode_ofb {
    size_t size;        /* first argument to ccofb_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_ofb *ofb, ccofb_ctx *ctx,
                size_t key_len, const void *key, const void *iv);
    int (*ofb)(ccofb_ctx *ctx, size_t nbytes, const void *in, void *out);
    const void *custom;
};

// 6- XTS mode, stateless
cc_aligned_struct(16) ccxts_ctx;
cc_aligned_struct(16) ccxts_tweak;

struct ccmode_xts {
    size_t size;        /* first argument to ccxts_ctx_decl(). Size of the ctx data structure */
    size_t tweak_size;  /* first argument to ccxts_tweak_decl(). Size of the tweak structure, not the expected tweak size */
    size_t block_size;

    /* Create a xts key from a xts mode object.  
     key must point to at least 'size' bytes of free storage.
     tweak_key must point to at least 'tweak_size' bytes of free storage.
     key and tweak_key must differ.
     Returns nonzero on failure.
     */
    int (*init)(const struct ccmode_xts *xts, ccxts_ctx *ctx,
                size_t key_nbytes, const void *data_key, const void *tweak_key);
    
    void (*key_sched)(const struct ccmode_xts *xts, ccxts_ctx *ctx,
                      size_t key_nbytes, const void *data_key, const void *tweak_key);

    /* Set the tweak (sector number), the block within the sector zero. */
    int (*set_tweak)(const ccxts_ctx *ctx, ccxts_tweak *tweak, const void *iv);

    /* Encrypt blocks for a sector, clients must call set_tweak before calling
       this function. Return a pointer to the tweak buffer */
    void *(*xts)(const ccxts_ctx *ctx, ccxts_tweak *tweak,
                 size_t nblocks, const void *in, void *out);

    const void *custom;
    const void *custom1;
};

//7- GCM mode, statful
cc_aligned_struct(16) ccgcm_ctx;
#define  CCMODE_GCM_DECRYPTOR 78647
#define  CCMODE_GCM_ENCRYPTOR 4073947

struct ccmode_gcm {
    size_t size;        /* first argument to ccgcm_ctx_decl(). */
    int encdec;        //is it encrypt or decrypt object
    size_t block_size;
    int (*init)(const struct ccmode_gcm *gcm, ccgcm_ctx *ctx,
                 size_t key_nbytes, const void *key);
    int (*set_iv)(ccgcm_ctx *ctx, size_t iv_nbytes, const void *iv);
    int (*gmac)(ccgcm_ctx *ctx, size_t nbytes, const void *in);  // could just be gcm with NULL out
    int (*gcm)(ccgcm_ctx *ctx, size_t nbytes, const void *in, void *out);
    int (*finalize)(ccgcm_ctx *key, size_t tag_nbytes, void *tag);
    int (*reset)(ccgcm_ctx *ctx);
    const void *custom;
};

//8- GCM mode, statful
cc_aligned_struct(16) ccccm_ctx;
cc_aligned_struct(16) ccccm_nonce;

struct ccmode_ccm {
    size_t size;        /* first argument to ccccm_ctx_decl(). */
    size_t nonce_size;  /* first argument to ccccm_nonce_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_ccm *ccm, ccccm_ctx *ctx,
                 size_t key_len, const void *key);
    int (*set_iv)(ccccm_ctx *ctx, ccccm_nonce *nonce_ctx, size_t nonce_len, const void *nonce,
                   size_t mac_size, size_t auth_len, size_t data_len);
    int (*cbcmac)(ccccm_ctx *ctx, ccccm_nonce *nonce_ctx, size_t nbytes, const void *in);  // could just be ccm with NULL out
    int (*ccm)(ccccm_ctx *ctx, ccccm_nonce *nonce_ctx, size_t nbytes, const void *in, void *out);
    int (*finalize)(ccccm_ctx *key, ccccm_nonce *nonce_ctx, void *mac);
    int (*reset)(ccccm_ctx *key, ccccm_nonce *nonce_ctx);
    const void *custom;
};


/* OMAC mode. */
cc_aligned_struct(16) ccomac_ctx;

struct ccmode_omac {
    size_t size;        /* first argument to ccomac_ctx_decl(). */
    size_t block_size;
    int (*init)(const struct ccmode_omac *omac, ccomac_ctx *ctx,
                size_t tweak_len, size_t key_len, const void *key);
    int (*omac)(ccomac_ctx *ctx, size_t nblocks,
                const void *tweak, const void *in, void *out);
    const void *custom;
};

#endif /* _CORECRYPTO_CCMODE_IMPL_H_ */
