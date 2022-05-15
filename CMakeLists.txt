cmake_minimum_required(VERSION 3.10)

#set the project name
project(store VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_BUILD_TYPE "Release")

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

if(MSVC)
 add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#add the executable
add_executable(store
main.cxx
)

target_link_libraries(store PUBLIC 	"${PROJECT_SOURCE_DIR}/libcurl-x64.lib"	)

# add the binary tree to the search path for include files
target_include_directories(store PUBLIC ${PROJECT_SOURCE_DIR}/curl
			)