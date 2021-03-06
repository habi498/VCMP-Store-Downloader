cmake_minimum_required(VERSION 3.10)

#set the project name
project(store VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_BUILD_TYPE "Release")


if(MSVC)
 add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(lib)

#add the executable
add_executable(store
main.cxx
)

add_executable(update
update.cxx
)
target_link_libraries(store PUBLIC 	"${PROJECT_SOURCE_DIR}/libcurl-x64.lib"	storelib)
target_link_libraries(update PUBLIC 	"${PROJECT_SOURCE_DIR}/libcurl-x64.lib" storelib	)

# add the binary tree to the search path for include files
target_include_directories(store PUBLIC ${PROJECT_SOURCE_DIR}/curl )
target_include_directories(update PUBLIC ${PROJECT_SOURCE_DIR}/curl )

add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dep/libcurl-x64.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )                 # <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dep/unzip.exe"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )                 # <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dep/README.txt"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )  		# <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/LICENSE.txt"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )  		# <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dep/cacert.pem"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )  		# <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/dep/curl-ca-bundle.crt"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )  		# <--this is out-file path
add_custom_command(TARGET store POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/Changelog.txt"      # <--this is in-file
        $<TARGET_FILE_DIR:store> )  		# <--this is out-file path


if(MAKE_GUI)

add_custom_command(
    TARGET store 
POST_BUILD
    COMMAND Ahk2Exe.exe /in store-gui.ahk /out store-gui.exe /icon icon.ico /base AutoHotkeySC.bin
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	store-gui.exe 
$<TARGET_FILE_DIR:store> 
COMMAND del store-gui.exe
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/store-gui-src/src+compiler
)
endif()

