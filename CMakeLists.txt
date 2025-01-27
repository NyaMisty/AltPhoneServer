cmake_minimum_required(VERSION 3.4.1...3.17.2)

project(AltServer)

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(NO_USBMUXD_STUB ON CACHE BOOL "NO_USBMUXD_STUB Option")
if (NO_USBMUXD_STUB)
    add_definitions(-DNO_USBMUXD_STUB)
endif()

message(STATUS "NO_USBMUXD_STUB: " ${NO_USBMUXD_STUB})

if (UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H -DDEBUG -O0 -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H -DDEBUG -O0 -g")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# AltSign
# add_subdirectory(upstream_repo/AltSign)
add_subdirectory(makefiles/AltSign-build)

# main
FILE(GLOB main_src
        src/AltServerMain.cpp
        src/AnisetteDataManager.cpp
        src/WiredConnection.cpp
        shims/windows_shim.cpp

        #upstream_repo/AltServer/AltServer.cpp
        upstream_repo/AltServer/AltServerApp.cpp
        #upstream_repo/AltServer/AnisetteDataManager.cpp
        upstream_repo/AltServer/ClientConnection.cpp
        upstream_repo/AltServer/ConnectionManager.cpp
        upstream_repo/AltServer/DebugConnection.cpp
        upstream_repo/AltServer/DeveloperDiskManager.cpp
        upstream_repo/AltServer/DeviceManager.cpp
        upstream_repo/AltServer/InstalledApp.cpp
        upstream_repo/AltServer/NotificationConnection.cpp
        upstream_repo/AltServer/ServerError.cpp
        #upstream_repo/AltServer/WiredConnection.cpp
        upstream_repo/AltServer/WirelessConnection.cpp)

# libimobiledevice
FILE(GLOB libimobiledevice_src
    libraries/libimobiledevice/src/*.c
    libraries/libimobiledevice/common/*.c
    libraries/libimobiledevice-glue/src/*.c
    libraries/libusbmuxd/src/*.c
    libraries/libusbmuxd/common/*.c
    )

# libplist
FILE(GLOB libplist_src 
            libraries/libplist/src/*.c
            libraries/libplist/libcnary/node.c
            libraries/libplist/libcnary/node_list.c)

# dnssd_loader
set(dnssd_loader_src 
    libraries/dnssd_loader/dnssd_loader.cpp
)

add_executable(AltServer
    ${main_src}
    ${libimobiledevice_src}
    ${libplist_src}
    ${dnssd_loader_src}
)

target_include_directories(AltServer PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    libraries
    libraries/AltSign
    libraries/libimobiledevice
    libraries/libimobiledevice/include
    libraries/libusbmuxd/include
    libraries/libplist/include
    libraries/libplist/libcnary/include
    libraries/libplist/src
    libraries/libusbmuxd/common
    )

if (UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads)
  target_link_libraries(AltServer PRIVATE Threads::Threads)
endif()

target_compile_options(AltServer PRIVATE -fpermissive -include common.h)
target_link_libraries(AltServer PRIVATE AltSign ssl crypto z cpprest boost_system boost_filesystem uuid c++)

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
    target_include_directories(AltServer PUBLIC
            ${HOMEBREW_PREFIX}/include
            ${OPENSSL_INCLUDE_DIR}
            libraries/libimobiledevice-glue/include/
            libraries/corecrypto/include/corecrypto
            upstream_repo/AltServer
            upstream_repo/AltSign
            src/
            shims/
            )

    target_link_directories(AltServer PUBLIC
            ${HOMEBREW_PREFIX}/opt/openssl/lib
            ${HOMEBREW_PREFIX}/lib
            )
    target_link_libraries(AltServer PRIVATE "-framework Foundation")
endif()