/*
 * Copyright (c) 2010,2011,2012,2015,2016,2018 Apple Inc. All rights reserved.
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
#ifndef _CORECRYPTO_CCASN1_H_
#define _CORECRYPTO_CCASN1_H_

#include <corecrypto/cc.h>
#include <stdbool.h>
#include <string.h>

/* ASN.1 types for on the fly ASN.1 BER/DER encoding/decoding. Don't use
   these with the ccder interface, use the CCDER_ types instead. */
enum {
    CCASN1_EOL               = 0x00,
    CCASN1_BOOLEAN           = 0x01,
    CCASN1_INTEGER           = 0x02,
    CCASN1_BIT_STRING        = 0x03,
    CCASN1_OCTET_STRING      = 0x04,
    CCASN1_NULL              = 0x05,
    CCASN1_OBJECT_IDENTIFIER = 0x06,
    CCASN1_OBJECT_DESCRIPTOR = 0x07,
    /* External or instance-of 0x08 */
    CCASN1_REAL              = 0x09,
    CCASN1_ENUMERATED        = 0x0a,
    CCASN1_EMBEDDED_PDV      = 0x0b,
    CCASN1_UTF8_STRING       = 0x0c,
    /*                         0x0d */
    /*                         0x0e */
    /*                         0x0f */
    CCASN1_SEQUENCE          = 0x10,
    CCASN1_SET               = 0x11,
    CCASN1_NUMERIC_STRING    = 0x12,
    CCASN1_PRINTABLE_STRING  = 0x13,
    CCASN1_T61_STRING        = 0x14,
    CCASN1_VIDEOTEX_STRING   = 0x15,
    CCASN1_IA5_STRING        = 0x16,
    CCASN1_UTC_TIME          = 0x17,
    CCASN1_GENERALIZED_TIME  = 0x18,
    CCASN1_GRAPHIC_STRING    = 0x19,
    CCASN1_VISIBLE_STRING    = 0x1a,
    CCASN1_GENERAL_STRING    = 0x1b,
    CCASN1_UNIVERSAL_STRING  = 0x1c,
    /*                         0x1d */
    CCASN1_BMP_STRING        = 0x1e,
    CCASN1_HIGH_TAG_NUMBER   = 0x1f,
    CCASN1_TELETEX_STRING    = CCASN1_T61_STRING,

    CCASN1_TAG_MASK			 = 0xff,
    CCASN1_TAGNUM_MASK		 = 0x1f,

    CCASN1_METHOD_MASK		 = 0x20,
    CCASN1_PRIMITIVE		 = 0x00,
    CCASN1_CONSTRUCTED		 = 0x20,

    CCASN1_CLASS_MASK		 = 0xc0,
    CCASN1_UNIVERSAL		 = 0x00,
    CCASN1_APPLICATION		 = 0x40,
    CCASN1_CONTEXT_SPECIFIC	 = 0x80,
    CCASN1_PRIVATE			 = 0xc0,

    CCASN1_CONSTRUCTED_SET = CCASN1_SET | CCASN1_CONSTRUCTED,
    CCASN1_CONSTRUCTED_SEQUENCE = CCASN1_SEQUENCE | CCASN1_CONSTRUCTED,
};

#if CORECRYPTO_USE_TRANSPARENT_UNION
typedef union {
    const unsigned char * oid;
} __attribute__((transparent_union)) ccoid_t;
#define CCOID(x) ((x).oid)
#else
    typedef const unsigned char * ccoid_t;
#define CCOID(oid) (oid)
#endif

/* Returns *der iff *der points to a DER encoded oid that fits within *der_len. */
ccoid_t ccoid_for_der(size_t *der_len, const uint8_t **der);

/* Returns the size of an oid including it's tag and length. */
CC_INLINE CC_PURE CC_NONNULL_TU((1))
size_t ccoid_size(ccoid_t oid) {
    return 2 + CCOID(oid)[1];
}

CC_INLINE CC_PURE CC_NONNULL_TU((1)) CC_NONNULL_TU((2))
bool ccoid_equal(ccoid_t oid1, ccoid_t oid2) {
    return  (ccoid_size(oid1) == ccoid_size(oid2)
            && memcmp(CCOID(oid1), CCOID(oid2), ccoid_size(oid1))== 0);
}

#endif /* _CORECRYPTO_CCASN1_H_ */
