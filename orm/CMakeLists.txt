cmake_minimum_required(VERSION 3.0)
project(orm_data)

set(LIB_SRC
    rs/file_chunks_info.cc
    rs/file_downloads_info.cc
    rs/files_info.cc
    rs/user_file_relations_info.cc
    rs/users_info.cc
)
add_library(orm_data ${LIB_SRC})
force_redefine_file_macro_for_sources(orm_data)
