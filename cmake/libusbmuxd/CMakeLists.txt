cmake_minimum_required(VERSION 3.1)
project(usbmuxd LANGUAGES C)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

set(ROOT "${ALTSERVER_DIR}/libraries/libusbmuxd")

file(GLOB src "${ROOT}/src/*.c")
add_library(${PROJECT_NAME} STATIC ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}/include")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PUBLIC imobiledevice::imobiledevice)