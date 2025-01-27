/*
 * Copyright (c) 2010,2011,2012,2014,2015,2016,2018 Apple Inc. All rights reserved.
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

#ifndef _CORECRYPTO_CCDIGEST_H_
#define _CORECRYPTO_CCDIGEST_H_

#include <corecrypto/cc.h>
#include <corecrypto/ccn.h>

/* To malloc a digest context for a given di, use malloc(ccdigest_di_size(di))
   and assign the result to a pointer to a struct ccdigest_ctx. */
#if CORECRYPTO_USE_TRANSPARENT_UNION
struct ccdigest_ctx {
    union {
        uint8_t u8;
        uint32_t u32;
        uint64_t u64;
        cc_unit ccn;
    } state;
} CC_ALIGNED(8);

typedef union {
    struct ccdigest_ctx *hdr;
} ccdigest_ctx_t __attribute__((transparent_union));

struct ccdigest_state {
    union {
        uint8_t u8;
        uint32_t u32;
        uint64_t u64;
        cc_unit ccn;
    } state;
} CC_ALIGNED(8);

typedef union {
    struct ccdigest_state *hdr;
    struct ccdigest_ctx *_ctx;
    ccdigest_ctx_t _ctxt;
} ccdigest_state_t __attribute__((transparent_union));
#else //=======================================================
struct ccdigest_ctx {
    union {
        uint8_t u8;
        uint32_t u32;
        uint64_t u64;
        cc_unit ccn;
    } state;
} CC_ALIGNED(8);

typedef struct ccdigest_ctx *ccdigest_ctx_t ;

struct ccdigest_state {
    union {
        uint8_t u8;
        uint32_t u32;
        uint64_t u64;
        cc_unit ccn;
    } state;
} CC_ALIGNED(8);

typedef struct ccdigest_state *ccdigest_state_t;
#endif //=======================================================


struct ccdigest_info {
    size_t output_size;
    size_t state_size;
    size_t block_size;
    size_t oid_size;
    const unsigned char *oid;
    const void *initial_state;
    void(*compress)(ccdigest_state_t state, size_t nblocks,
                    const void *data);
    void(*final)(const struct ccdigest_info *di, ccdigest_ctx_t ctx,
                 unsigned char *digest);
};

/* Return sizeof a ccdigest_ctx for a given size_t _state_size_ and
   size_t _block_size_. */
#define ccdigest_ctx_size(_state_size_, _block_size_)  ((_state_size_) + sizeof(uint64_t) + (_block_size_) + sizeof(unsigned int))
/* Return sizeof a ccdigest_ctx for a given struct ccdigest_info *_di_. */
#define ccdigest_di_size(_di_)  (ccdigest_ctx_size((_di_)->state_size, (_di_)->block_size))

/* Declare a ccdigest_ctx for a given size_t _state_size_ and
   size_t _block_size_, named _name_.  Can be used in structs or on the
   stack. */
#define ccdigest_ctx_decl(_state_size_, _block_size_, _name_)  cc_ctx_decl(struct ccdigest_ctx, ccdigest_ctx_size(_state_size_, _block_size_), _name_)
#define ccdigest_ctx_clear(_state_size_, _block_size_, _name_) cc_clear(ccdigest_ctx_size(_state_size_, _block_size_), _name_)
/* Declare a ccdigest_ctx for a given size_t _state_size_ and
   size_t _block_size_, named _name_.  Can be used on the stack. */
#define ccdigest_di_decl(_di_, _name_)  cc_ctx_decl(struct ccdigest_ctx, ccdigest_di_size(_di_), _name_)
#define ccdigest_di_clear(_di_, _name_) cc_clear(ccdigest_di_size(_di_), _name_)

/* Digest context field accessors.  Consider the implementation private. */
#if CORECRYPTO_USE_TRANSPARENT_UNION
#define ccdigest_state(_di_, _ctx_)      ((struct ccdigest_state *)(&((ccdigest_ctx_t)(_ctx_)).hdr->state.u8 + sizeof(uint64_t)))
#else
#define ccdigest_state(_di_, _ctx_)      ((struct ccdigest_state *)(&((ccdigest_ctx_t)(_ctx_))->state.u8 + sizeof(uint64_t)))
#endif

#define ccdigest_state_u8(_di_, _ctx_)   ccdigest_u8(ccdigest_state((_di_), (_ctx_)))
#define ccdigest_state_u32(_di_, _ctx_)  ccdigest_u32(ccdigest_state((_di_), (_ctx_)))
#define ccdigest_state_u64(_di_, _ctx_)  ccdigest_u64(ccdigest_state((_di_), (_ctx_)))
#define ccdigest_state_ccn(_di_, _ctx_)  ccdigest_ccn(ccdigest_state((_di_), (_ctx_)))

#if CORECRYPTO_USE_TRANSPARENT_UNION
#define ccdigest_nbits(_di_, _ctx_)      (((uint64_t *)(&((ccdigest_ctx_t)(_ctx_)).hdr->state.u8))[0])
#define ccdigest_data(_di_, _ctx_)       (&((ccdigest_ctx_t)(_ctx_)).hdr->state.u8 + (_di_)->state_size + sizeof(uint64_t))
#define ccdigest_num(_di_, _ctx_)        (((unsigned int *)(&((ccdigest_ctx_t)(_ctx_)).hdr->state.u8 + (_di_)->state_size + sizeof(uint64_t) + (_di_)->block_size))[0])
#else
#define ccdigest_nbits(_di_, _ctx_)      (((uint64_t *)(&((ccdigest_ctx_t)(_ctx_))->state.u8))[0])
#define ccdigest_data(_di_, _ctx_)       (&((ccdigest_ctx_t)(_ctx_))->state.u8 + (_di_)->state_size + sizeof(uint64_t))
#define ccdigest_num(_di_, _ctx_)        (((unsigned int *)(&((ccdigest_ctx_t)(_ctx_))->state.u8 + (_di_)->state_size + sizeof(uint64_t) + (_di_)->block_size))[0])
#endif

#if CORECRYPTO_USE_TRANSPARENT_UNION
/* Digest state field accessors.  Consider the implementation private. */
#define ccdigest_u8(_state_)             (&((ccdigest_state_t)(_state_)).hdr->state.u8)
#define ccdigest_u32(_state_)            (&((ccdigest_state_t)(_state_)).hdr->state.u32)
#define ccdigest_u64(_state_)            (&((ccdigest_state_t)(_state_)).hdr->state.u64)
#define ccdigest_ccn(_state_)            (&((ccdigest_state_t)(_state_)).hdr->state.ccn)
#else
/* Digest state field accessors.  Consider the implementation private. */
#define ccdigest_u8(_state_)             (&((ccdigest_state_t)(_state_))->state.u8)
#define ccdigest_u32(_state_)            (&((ccdigest_state_t)(_state_))->state.u32)
#define ccdigest_u64(_state_)            (&((ccdigest_state_t)(_state_))->state.u64)
#define ccdigest_ccn(_state_)            (&((ccdigest_state_t)(_state_))->state.ccn)
#endif

/* We could just use memcpy instead of this special macro, but this allows us
   to use the optimized ccn_set() assembly routine if we have one, which for
   32 bit arm is about 200% quicker than generic memcpy(). */
#if CCN_SET_ASM && CCN_UNIT_SIZE <= 4
#define ccdigest_copy_state(_di_, _dst_, _src_) ccn_set((_di_)->state_size / CCN_UNIT_SIZE, _dst_, _src_)
#else
#define ccdigest_copy_state(_di_, _dst_, _src_) CC_MEMCPY(_dst_, _src_, (_di_)->state_size)
#endif

void ccdigest_init(const struct ccdigest_info *di, ccdigest_ctx_t ctx);
void ccdigest_update(const struct ccdigest_info *di, ccdigest_ctx_t ctx,
                     size_t len, const void *data);

CC_INLINE
void ccdigest_final(const struct ccdigest_info *di, ccdigest_ctx_t ctx, unsigned char *digest)
{
    di->final(di,ctx,digest);
}

void ccdigest(const struct ccdigest_info *di, size_t len,
              const void *data, void *digest);

/* test functions */
int ccdigest_test(const struct ccdigest_info *di, size_t len,
              const void *data, const void *digest);

int ccdigest_test_chunk(const struct ccdigest_info *di, size_t len,
                        const void *data, const void *digest, size_t chunk);

struct ccdigest_vector {
    size_t len;
    const void *message;
    const void *digest;
};

int ccdigest_test_vector(const struct ccdigest_info *di, const struct ccdigest_vector *v);
int ccdigest_test_chunk_vector(const struct ccdigest_info *di, const struct ccdigest_vector *v, size_t chunk);


#define OID_DEF(_VALUE_)  ((const unsigned char *)_VALUE_)

#define CC_DIGEST_OID_MD2       OID_DEF("\x06\x08\x2A\x86\x48\x86\xF7\x0D\x02\x02")
#define CC_DIGEST_OID_MD4       OID_DEF("\x06\x08\x2A\x86\x48\x86\xF7\x0D\x02\x04")
#define CC_DIGEST_OID_MD5       OID_DEF("\x06\x08\x2A\x86\x48\x86\xF7\x0D\x02\x05")
#define CC_DIGEST_OID_SHA1      OID_DEF("\x06\x05\x2b\x0e\x03\x02\x1a")
#define CC_DIGEST_OID_SHA224    OID_DEF("\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x04")
#define CC_DIGEST_OID_SHA256    OID_DEF("\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01")
#define CC_DIGEST_OID_SHA384    OID_DEF("\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x02")
#define CC_DIGEST_OID_SHA512    OID_DEF("\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x03")
#define CC_DIGEST_OID_RMD128    OID_DEF("\x06\x06\x28\xCF\x06\x03\x00\x32")
#define CC_DIGEST_OID_RMD160    OID_DEF("\x06\x05\x2B\x24\x03\x02\x01")
#define CC_DIGEST_OID_RMD256    OID_DEF("\x06\x05\x2B\x24\x03\x02\x03")
#define CC_DIGEST_OID_RMD320    OID_DEF(NULL)

#endif /* _CORECRYPTO_CCDIGEST_H_ */
