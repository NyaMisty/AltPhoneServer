cmake_minimum_required(VERSION 3.1)
project(imobiledevice LANGUAGES C)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

set(ROOT "${ALTSERVER_DIR}/libraries/libimobiledevice")
file(GLOB src "${ROOT}/src/*.c" "${ROOT}/common/*.c")

add_library(${PROJECT_NAME} STATIC ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/libraries")
target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}/include" "${ROOT}/common")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imobiledevice::glue usbmuxd::usbmuxd OpenSSL::SSL)