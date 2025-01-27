cmake_minimum_required(VERSION 3.1)
project(imobiledevice-glue LANGUAGES C)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

set(ROOT "${ALTSERVER_DIR}/libraries/libimobiledevice-glue")
file(GLOB src "${ROOT}/src/*.c" "${ROOT}/common/*.c")

add_library(${PROJECT_NAME} STATIC ${src})
add_library(imobiledevice::glue ALIAS ${PROJECT_NAME})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE "${ROOT}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}/include")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PUBLIC plist::plist)