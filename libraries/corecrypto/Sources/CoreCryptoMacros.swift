//
//  CoreCryptoMacros.swift
//  AltSign
//
//  Created by Riley Testut on 8/20/20.
//  Copyright © 2020 Riley Testut. All rights reserved.
//

@_exported import CCoreCrypto

/// Reimplementations of various CoreCrypto macros that are unavailable in Swift.

public func ccdigest_ctx_size(_ stateSize: Int, _ blockSize: Int) -> Int
{
    (stateSize + MemoryLayout<UInt64>.size + blockSize + MemoryLayout<UInt32>.size)
}

public func ccdigest_di_size(_ digestInfo: UnsafePointer<ccdigest_info>) -> Int
{
    ccdigest_ctx_size(digestInfo.pointee.state_size, digestInfo.pointee.block_size)
}

public func ccsrp_gpbuf_size(_ group: ccdh_const_gp_t) -> Int
{
    ccdh_ccn_size(group) * 4
}

public func ccsrp_dibuf_size(_ digestInfo: UnsafePointer<ccdigest_info>) -> Int
{
    digestInfo.pointee.output_size * 4
}

public func ccsrp_sizeof_srp(_ digestInfo: UnsafePointer<ccdigest_info>, _ group: ccdh_const_gp_t) -> Int
{
    MemoryLayout<ccsrp_ctx>.size + ccsrp_gpbuf_size(group) + ccsrp_dibuf_size(digestInfo)
}

public func cchmac_ctx_size(_ stateSize: Int, _ blockSize: Int) -> Int
{
    ccdigest_ctx_size(stateSize, blockSize) + stateSize
}

public func cchmac_di_size(_ digestInfo: UnsafePointer<ccdigest_info>) -> Int
{
    if #available(iOS 14, macOS 11, *)
    {
        // CoreCrypto headers aren't accurate for iOS 14 or macOS 11 yet,
        // so return bigger buffer size to ensure against invalid memory access.
        return cchmac_ctx_size(digestInfo.pointee.state_size, digestInfo.pointee.block_size) * 2
    }
    else
    {
        return cchmac_ctx_size(digestInfo.pointee.state_size, digestInfo.pointee.block_size)
    }
}
