cmake_minimum_required(VERSION 3.10)

#set the project name
project(storelib VERSION 1.0)

add_library(storelib
main.cxx
)
cmake_path(GET PROJECT_SOURCE_DIR PARENT_PATH PARENT_DIR)
message(STATUS ${PARENT_DIR})
target_include_directories(storelib PUBLIC ${PARENT_DIR}/curl )