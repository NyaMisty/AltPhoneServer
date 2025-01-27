cmake_minimum_required(VERSION 3.1)
project(altserver LANGUAGES CXX)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

file(GLOB src "${ALTSERVER_DIR}/upstream_patched/AltServer/*.c*")

add_library(${PROJECT_NAME} STATIC ${c_src} ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------------------
# Use shims
# --------------------------------------------------------------------------------------------------------

file(GLOB shims "${ALTSERVER_DIR}/shims/*.c*")
target_sources(${PROJECT_NAME} PRIVATE ${shims})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE "${ALTSERVER_DIR}/shims")
target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/upstream_patched/AltServer")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PUBLIC imobiledevice::imobiledevice dnssd::dnssd altsign::altsign)