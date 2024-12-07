cmake_minimum_required(VERSION 3.1)
project(plist)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

set(ROOT "${ALTSERVER_DIR}/libraries/libplist")

file(GLOB c_src "${ROOT}/src/*.c" "${ROOT}/libcnary/*.c")
list(FILTER c_src EXCLUDE REGEX "cnary\.c")
set_source_files_properties(${c_src} PROPERTIES LANGUAGE C)

file(GLOB src "${ROOT}/src/*.cpp" "${ROOT}/libcnary/*.cpp")
set_source_files_properties(${src} PROPERTIES LANGUAGE CXX)

add_library(${PROJECT_NAME} STATIC ${c_src} ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE "${ROOT}/src")
target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${ROOT}/libcnary/include")