cmake_minimum_required(VERSION 3.4.1...3.17.2)

project(AltSign)

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(AltSignSrcPath ../../upstream_repo/AltSign/)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-sse -include windows_shim.h")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

FILE(GLOB ALTSIGN_SOURCES ${AltSignSrcPath}/*.cpp)

list(APPEND ALTSIGN_SOURCES
    ${AltSignSrcPath}/Dependencies/minizip/ioapi.c
    ${AltSignSrcPath}/Dependencies/minizip/zip.c
    ${AltSignSrcPath}/Dependencies/minizip/unzip.c
    ${AltSignSrcPath}/../ldid/ldid.cpp
    ${AltSignSrcPath}/../ldid/lookup2.c
)

# corecrypto
if (APPLE)
    list(APPEND ALTSIGN_SOURCES ${AltSignSrcPath}/../../libraries/corecrypto/Sources/ccsrp.m)
    add_definitions(-DCORECRYPTO_DONOT_USE_TRANSPARENT_UNION=1)
endif()

# Static library
add_library(AltSign
    ${ALTSIGN_SOURCES}
)

# Help finding Homebrew's OpenSSL on macOS
if (APPLE)
    # This is for apple silicon (M1)
    set(HOMEBREW_PREFIX "/opt/homebrew"
        CACHE PATH "Path to Homebrew installation")

    # This is for apple x86
    # set(HOMEBREW_PREFIX "/usr/local"
    #     CACHE PATH "Path to Homebrew installation")

    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${HOMEBREW_PREFIX}/opt/openssl/lib)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${HOMEBREW_PREFIX}/opt/openssl/include)

    # This OPENSSL_FOUND check is to help find a cmake manually configured OpenSSL
    if (NOT OPENSSL_FOUND)
        find_package(OpenSSL REQUIRED)
    endif()
    message(STATUS "OpenSSL: " ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_PATH})

    add_definitions(${OPENSSL_DEFINITIONS})
    target_include_directories(AltSign PUBLIC
            ${HOMEBREW_PREFIX}/include
            ${OPENSSL_INCLUDE_DIR}
            ${AltSignSrcPath}/../../libraries/corecrypto/include/
            ${AltSignSrcPath}/../../libraries/corecrypto/include/corecrypto
            #Dependencies/corecrypto
            ${AltSignSrcPath}/Dependencies/minizip/
            ${AltSignSrcPath}/../libplist/include
            ${AltSignSrcPath}/../ldid/
            ${AltSignSrcPath}/../../shims
            )
endif()
