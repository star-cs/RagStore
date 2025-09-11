cmake_minimum_required(VERSION 3.5)
project(RagStore)

set(CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/webserver)
include_directories(${PROJECT_SOURCE_DIR}/third_party/faiss)
include_directories(${PROJECT_SOURCE_DIR}/third_party/minio/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(/usr/include/jsoncpp)

add_subdirectory(third_party/webserver)
# add_subdirectory(third_party/minio)
# add_subdirectory(third_party/faiss)

include_directories(${PROJECT_SOURCE_DIR}/orm/rs)

file(GLOB_RECURSE src_files src/*.cc orm/rs/*.cc)

add_library(rag_store SHARED ${src_files})
force_redefine_file_macro_for_sources(rag_store)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/module)