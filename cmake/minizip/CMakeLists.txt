cmake_minimum_required(VERSION 3.1)
project(minizip LANGUAGES C)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

file(GLOB src "${ALTSERVER_DIR}/upstream_patched/AltSign/Dependencies/minizip/*.c")
list(FILTER src EXCLUDE REGEX "win|miniunz|minizip\.c")


add_library(${PROJECT_NAME} STATIC ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/upstream_patched/AltSign/Dependencies/minizip")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ZLIB::ZLIB)