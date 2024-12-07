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
#ifndef _CORECRYPTO_FIPSPOST_H_
#define _CORECRYPTO_FIPSPOST_H_

#include <corecrypto/cc_config.h>

// Error list
#define CCERR_GENERIC_FAILURE   -1  // Configuration or unexpect issue
#define CCERR_INVALID_SLICE     -2
#define CCERR_FILE_ERROR        -3
#define CCERR_LIBRARY_ERROR     -4
#define CCERR_INTEGRITY_ERROR   -5
#define CCERR_KAT_FAILURE       -6  // Output of the algo is not as expect

// Boot-Arg fips_mode Flags
//
// FIPS_MODE_FLAG_FULL is the default value when no other value is set, which
// is the case for all production devices.
//
// When performing tests, if _FORCEFAIL is set to true, then the tests
// intentionally fail and log their failure. The kernelspace and userspace
// flags can be enabled independently.
//
// If it's not desired to panic, supply the _NOPANIC flag with the
// _FORCEFAIL flag.
//
// Additional logging can be enabled by supplying the _VERBOSE flag.
//
// _NOINTEG is used to ignore just the results of the module integrity
// check process, which is very useful when setting breakpoints in the
// kext for diagnostic or auditing purposes.
//
// Supplying _TRACE causes a trace buffer to be accumulated of the instrumented
// functions for only one execution of the POST.  As the POST finishes, the
// _TRACE flag is cleared from the fips_mode and no further tracing will occur.
#define FIPS_MODE_FLAG_DEBUG        (1 << 0)
#define FIPS_MODE_FLAG_FULL         (1 << 1)
#define FIPS_MODE_FLAG_DISABLE      (1 << 2)
#define FIPS_MODE_FLAG_VERBOSE      (1 << 3)
#define FIPS_MODE_FLAG_US_FORCEFAIL (1 << 4)
#define FIPS_MODE_FLAG_KS_FORCEFAIL (1 << 5)
#define FIPS_MODE_FLAG_NOINTEG      (1 << 6)
#define FIPS_MODE_FLAG_TRACE        (1 << 7)
#define FIPS_MODE_FLAG_NOPANIC      (1 << 8)

#define FIPS_MODE_IS_DEBUG(MODE)        ((MODE) & FIPS_MODE_FLAG_DEBUG)
#define FIPS_MODE_IS_FULL(MODE)         ((MODE) & FIPS_MODE_FLAG_FULL)
#define FIPS_MODE_IS_DISABLE(MODE)      ((MODE) & FIPS_MODE_FLAG_DISABLE)
#define FIPS_MODE_IS_VERBOSE(MODE)      ((MODE) & FIPS_MODE_FLAG_VERBOSE)
#define FIPS_MODE_IS_US_FORCEFAIL(MODE) ((MODE) & FIPS_MODE_FLAG_US_FORCEFAIL)
#define FIPS_MODE_IS_KS_FORCEFAIL(MODE) ((MODE) & FIPS_MODE_FLAG_KS_FORCEFAIL)
#define FIPS_MODE_IS_NOINTEG(MODE)      ((MODE) & FIPS_MODE_FLAG_NOINTEG)
#define FIPS_MODE_IS_TRACE(MODE)        ((MODE) & FIPS_MODE_FLAG_TRACE)
#define FIPS_MODE_IS_NOPANIC(MODE)      ((MODE) & FIPS_MODE_FLAG_NOPANIC)

#if CC_KERNEL
#define FIPS_MODE_FLAG_FORCEFAIL        FIPS_MODE_FLAG_KS_FORCEFAIL
#define FIPS_MODE_IS_FORCEFAIL(MODE)    FIPS_MODE_IS_KS_FORCEFAIL(MODE)
#else
#define FIPS_MODE_FLAG_FORCEFAIL        FIPS_MODE_FLAG_US_FORCEFAIL
#define FIPS_MODE_IS_FORCEFAIL(MODE)    FIPS_MODE_IS_US_FORCEFAIL(MODE)
#endif

struct mach_header;

/*
 * Entrypoint for all POST tests.
 */
int fipspost_post(int fips_mode, struct mach_header *pmach_header);

#endif /* _CORECRYPTO_FIPSPOST_H_ */
