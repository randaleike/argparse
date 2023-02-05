# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(argparse VERSION 1.0.0.0 DESCRIPTION "Command Line Parser Utility" HOMEPAGE_URL "grocsoftware")

# Base includes
include(GNUInstallDirs)
include(CTest)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# include directories
include_directories(
    varg_intf/inc
    varg/inc
    listvarg/inc
    parser_base/inc
    cmd_line_parse/inc
    envparse/inc
    )

####
# Unit testing
####
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIR})

####
# varg Files
####
add_subdirectory (varg)

####
# parser Files
####
add_subdirectory (parser_base)
add_subdirectory (cmd_line_parse)
add_subdirectory (envparse)

####
# Argparse Library build
####
set (argparse_libs
    varg
    parser_base
    cmd_line_parse
    )

set(C_LIB ${CMAKE_BINARY_DIR}/libargparse.a)
add_custom_target(argparse
    COMMAND ar -x $<TARGET_FILE:varg>
    COMMAND ar -x $<TARGET_FILE:parser_base>
    COMMAND ar -x $<TARGET_FILE:cmd_line_parse>
    COMMAND ar -x $<TARGET_FILE:envparse>
    COMMAND ar rcs ${C_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${argparse_libs}
    )

####
# Install package
####
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

####
# cmd_line_parse sample generation
####
set (cmd_line_sample_include
    -I${CMAKE_CURRENT_SOURCE_DIR}/varg/inc
    -I${CMAKE_CURRENT_SOURCE_DIR}/parser_base/inc
    -I${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc
    )

set (cmd_line_sample_libs
    ${CMAKE_BINARY_DIR}/libcmd_line_parse.a
    ${CMAKE_BINARY_DIR}/libvarg.a
    )

add_custom_target(samples
    COMMENT "Samples build"
    COMMAND ${CMAKE_CXX_COMPILER} -o sample1.exe ${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/sample/sample1.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS argparse
    )
