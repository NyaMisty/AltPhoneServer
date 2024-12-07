cmake_minimum_required(VERSION 3.1)
project(altsign LANGUAGES CXX)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

file(GLOB src "${ALTSERVER_DIR}/upstream_patched/AltSign/*.c*")

add_library(${PROJECT_NAME} STATIC ${c_src} ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/shims")
target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/upstream_patched/AltSign")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(cpprestsdk GIT_REPOSITORY "https://github.com/microsoft/cpprestsdk" GIT_TAG "2.10.18")
FetchContent_MakeAvailable(cpprestsdk)

target_link_libraries(${PROJECT_NAME} PUBLIC plist::plist ldid::ldid minizip::minizip corecrypto::corecrypto cpprest)