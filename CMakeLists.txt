cmake_minimum_required(VERSION 3.10)
project(RagStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VCPKG_INSTALLED_DIR "/home/yang/soft/vcpkg/installed/x64-linux")
# 查找必要的包
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
set(MINIO_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/include")
set(MINIO_LIB_DIR "${VCPKG_INSTALLED_DIR}/lib")
# 包含目录
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${MINIO_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/third_party/webserver)
include_directories(${PROJECT_SOURCE_DIR}/third_party/faiss)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/orm/rs)

include_directories(/usr/include/jsoncpp)

add_subdirectory(third_party/webserver)


file(GLOB_RECURSE src_files src/*.cc orm/rs/*.cc)

add_library(rag_store SHARED ${src_files})
force_redefine_file_macro_for_sources(rag_store)
target_link_libraries(rag_store PRIVATE 
    ${CURL_LIBRARIES}
    ${MINIO_LIB_DIR}/libminiocpp.a
    ${MINIO_LIB_DIR}/libpugixml.a
    ${MINIO_LIB_DIR}/libINIReader.a
    ${MINIO_LIB_DIR}/libinih.a
    ${MINIO_LIB_DIR}/libcurlpp.a
    ${MINIO_LIB_DIR}/libcurl.a
    ${MINIO_LIB_DIR}/libz.a
    ${MINIO_LIB_DIR}/libssl.a
    ${MINIO_LIB_DIR}/libcrypto.a
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/module)