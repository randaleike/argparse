# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(argparse VERSION 1.0.0.0 DESCRIPTION "Command Line Parser Utility" HOMEPAGE_URL "grocsoftware")

# Base includes
include(GNUInstallDirs)
include(CTest)

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
add_subdirectory (varg_intf)
add_subdirectory (varg)
add_subdirectory (listvarg)

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
    vargintf
    varg
    listvarg
    parser_base
    cmd_line_parse
    )

set(C_LIB ${CMAKE_BINARY_DIR}/libargparse.a)
add_custom_target(argparse
    COMMAND ar -x $<TARGET_FILE:vargintf>
    COMMAND ar -x $<TARGET_FILE:varg>
    COMMAND ar -x $<TARGET_FILE:listvarg>
    COMMAND ar -x $<TARGET_FILE:parser_base>
    COMMAND ar -x $<TARGET_FILE:cmd_line_parse>
    COMMAND ar -x $<TARGET_FILE:envparse>
    COMMAND ar rcs ${C_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${argparse_libs}
    )
