cmake_minimum_required(VERSION 3.12)
project(altserver-linux VERSION 0.0.5)

set(ALTSERVER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# -------------------------------------------------------------------------------------------------------------------------------
# Options
# -------------------------------------------------------------------------------------------------------------------------------

option(altserver_cli "Whether or not to build the cli" ON)

# -------------------------------------------------------------------------------------------------------------------------------
# Patch upstream project
# -------------------------------------------------------------------------------------------------------------------------------

include(cmake/patch.cmake)

patch_prepare()

patch(altserver PATTERN "AltServer/*.[ch]*" EXCLUDE_PATTERN "AltServer/AltServer.cpp" "AltServer/WiredConnection.cpp" "AltServer/AnisetteDataManager.cpp")
patch(altsign PATTERN "AltSign/*.[ch]*")
patch(ldid PATTERN "ldid/*.[ch]*")

# -------------------------------------------------------------------------------------------------------------------------------
# Compile options
# -------------------------------------------------------------------------------------------------------------------------------

add_definitions(-w) # Ouch, we need this because cpprestsdk might cause fat errors otherwise.

# -------------------------------------------------------------------------------------------------------------------------------
# Force Include some shims
# -------------------------------------------------------------------------------------------------------------------------------

# This is really bad
add_definitions(-include "${ALTSERVER_DIR}/src/common.h" -include "${ALTSERVER_DIR}/shims/windows_shim.h")

# -------------------------------------------------------------------------------------------------------------------------------
# Setup Dependencies
# -------------------------------------------------------------------------------------------------------------------------------

# This is also pretty bad practice imo, but exporting all of the defines into cmake would also be retarded
add_definitions(-include "${ALTSERVER_DIR}/config/libimobiledevice.h" -include "${ALTSERVER_DIR}/config/dnssd.h")

add_subdirectory(cmake/ldid)
add_subdirectory(cmake/minizip)
add_subdirectory(cmake/libplist)
add_subdirectory(cmake/CoreCrypto)
add_subdirectory(libraries/dnssd_loader)
add_subdirectory(cmake/libimobiledevice)
add_subdirectory(cmake/libimobiledevice-glue)

add_subdirectory(cmake/AltSign)
add_subdirectory(cmake/AltServer)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup Executable
# -------------------------------------------------------------------------------------------------------------------------------

if (altserver_cli)
    file(GLOB src "src/*.cpp")
    add_executable(${PROJECT_NAME} ${src})

    target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)
    
    add_subdirectory(cmake/libusbmuxd)
    target_link_libraries(${PROJECT_NAME} PUBLIC usbmuxd uuid altserver altsign ${Boost_LIBRARIES} )
endif()
