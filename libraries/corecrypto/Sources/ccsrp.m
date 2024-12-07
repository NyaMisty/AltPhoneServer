//
//  ccsrp.c
//  AltSign
//
//  Created by Riley Testut on 6/25/20.
//  Copyright © 2020 Riley Testut. All rights reserved.
//

#include "ccsrp.h"

#import <Foundation/Foundation.h>

cc_unit *srp_ccn(void *srp)
{
    // Memory layout of ccsrp_ctx changed between iOS 13/macOS 10.15 and iOS 14/macOS 11.
    // Dynamically cast to correct memory layout to ensure we access valid memory.
    
#if TARGET_OS_IPHONE
    if ([[NSProcessInfo processInfo] isOperatingSystemAtLeastVersion:(NSOperatingSystemVersion){ 14, 0, 0 }])
#else
    if ([[NSProcessInfo processInfo] isOperatingSystemAtLeastVersion:(NSOperatingSystemVersion){ 10, 16, 0 }])
#endif
    {
        return SRP_CCN((ccsrp_ctx_t)(srp));
    }
    else
    {
        return SRP_CCN((ccsrp_ctx_t_legacy)(srp));
    }
}
