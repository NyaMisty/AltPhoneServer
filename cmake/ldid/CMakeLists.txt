cmake_minimum_required(VERSION 3.1)
project(ldid)

# -------------------------------------------------------------------------------------------------------------------------------
# Setup library                          
# -------------------------------------------------------------------------------------------------------------------------------

file(GLOB c_src "${ALTSERVER_DIR}/upstream_patched/ldid/*.c")
set_source_files_properties(${c_src} PROPERTIES LANGUAGE C)

file(GLOB src "${ALTSERVER_DIR}/upstream_patched/ldid/*.cpp")
set_source_files_properties(${src} PROPERTIES LANGUAGE CXX)

add_library(${PROJECT_NAME} STATIC ${c_src} ${src})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------------------------------------------

target_include_directories(${PROJECT_NAME} PRIVATE "${ALTSERVER_DIR}/shims")
target_include_directories(${PROJECT_NAME} PUBLIC "${ALTSERVER_DIR}/upstream_patched/ldid")

# --------------------------------------------------------------------------------------------------------
# Link required libraries
# --------------------------------------------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} PUBLIC plist::plist)