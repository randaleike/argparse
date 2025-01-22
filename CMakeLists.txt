# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_CLANG_TIDY clang-tidy --config-file ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)
cmake_policy(SET CMP0048 NEW)
project(argparse VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Executable argument input parser utilities" HOMEPAGE_URL "https://github.com/randaleike/argparse")

        # control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Base includes
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
endif()

#include_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}/parser_base/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/varg/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/varg/inc_clib
#    ${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc_clib
#    ${CMAKE_CURRENT_SOURCE_DIR}/envparse/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/envparse/inc_clib
#    ${CMAKE_CURRENT_SOURCE_DIR}/config_json_parse/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/config_json_parse/_clib
#    ${CMAKE_CURRENT_SOURCE_DIR}/config_xml_parse/inc
#    ${CMAKE_CURRENT_SOURCE_DIR}/config_xml_parse/inc_clib
#    )

set(MASTER_PROJECT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    

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
    envparse
    )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_program(LIB_EXE NAMES lib.exe)
    set(C_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.lib)
    add_custom_target(${PROJECT_NAME}
                    COMMAND ${LIB_EXE} /out:${C_LIB} $<TARGET_FILE:varg> $<TARGET_FILE:parser_base> $<TARGET_FILE:cmd_line_parse> $<TARGET_FILE:envparse>
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS ${argparse_libs}
                    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_program(LIB_EXE NAMES ${LIB_EXE})
    set(C_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a)
    add_custom_target(${PROJECT_NAME}
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:varg>
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:parser_base>
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:cmd_line_parse>
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:envparse>
                      COMMAND ${LIB_EXE} rcs ${C_LIB} *.o
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                      DEPENDS ${argparse_libs}
                      )
endif()
    
####
# sample Files
####
add_subdirectory (sample)

####
# Library unit testing
####
add_custom_target(unittest
    COMMENT "Run unittest"
    COMMAND ./varg_test
    COMMAND ./cmd_line_parse_test
    COMMAND ./envparse_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/varg_test
    DEPENDS ${CMAKE_BINARY_DIR}/cmd_line_parse_test
    DEPENDS ${CMAKE_BINARY_DIR}/envparse_test
    )

add_custom_target(run-samples-unittest
    COMMENT "Samples unittest execution"
    COMMAND samples-unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/samples-unittest
)

####
# Install package
####
include(CPack)
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")

if (WIN32)
    set(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_SOURCE_GENERATOR "TGZ")
endif()

####
# argparse doxygen generation
####
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    option(BUILD_DOC "Build Documentation configuration" ON)
    #find_package(Doxygen REQUIRED dot)
    set(DOXYGEN_CPPIN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-cpp.in)
    set(DOXYGEN_CPPOUT ${CMAKE_BINARY_DIR}/Doxyfile-cpp)
    configure_file(${DOXYGEN_CPPIN} ${DOXYGEN_CPPOUT} @ONLY)

    set(DOXYGEN_CIN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-c.in)
    set(DOXYGEN_COUT ${CMAKE_BINARY_DIR}/Doxyfile-c)
    configure_file(${DOXYGEN_CIN} ${DOXYGEN_COUT} @ONLY)

    #message("Doxygen build started")
    #add_custom_target( doc_cpp_${PROJECT_NAME}
    #    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CPPOUT}
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    #    COMMENT "Generating CPP API documentation with Doxygen"
    #    VERBATIM )

    #add_custom_target( doc_c_${PROJECT_NAME}
    #    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_COUT}
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    #    COMMENT "Generating C API documentation with Doxygen"
    #    VERBATIM )
endif()
