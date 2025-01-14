# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_CLANG_TIDY clang-tidy --config-file ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)
cmake_policy(SET CMP0048 NEW)
project(argparse VERSION 0.9.2.0 DESCRIPTION "Executable argument input parser utilities" HOMEPAGE_URL "https://github.com/randaleike/argparse")

# Base includes
include(GNUInstallDirs)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# include directories
set(MY_PROJECT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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

set(C_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a)
add_custom_target(${PROJECT_NAME}
    COMMAND ar -x $<TARGET_FILE:varg>
    COMMAND ar -x $<TARGET_FILE:parser_base>
    COMMAND ar -x $<TARGET_FILE:cmd_line_parse>
    COMMAND ar -x $<TARGET_FILE:envparse>
    COMMAND ar rcs ${C_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${argparse_libs}
    )

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
# cmd_line_parse sample generation
####
set (cmd_line_sample_include
    -I${CMAKE_CURRENT_SOURCE_DIR}/varg/inc
    -I${CMAKE_CURRENT_SOURCE_DIR}/parser_base/inc
    -I${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc
    )

set (cmd_line_clib_sample_include
    -I${CMAKE_CURRENT_SOURCE_DIR}/varg/inc_clib
    -I${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc_clib
    )

set (cmd_line_sample_libs
    ${CMAKE_BINARY_DIR}/libenvparse.a
    ${CMAKE_BINARY_DIR}/libcmd_line_parse.a
    ${CMAKE_BINARY_DIR}/libvarg.a
    )

set (cmd_line_sample_clibs
    ${CMAKE_BINARY_DIR}/libcmd_line_parse.a
    ${CMAKE_BINARY_DIR}/libvarg.a
    )

add_custom_target(samples
    COMMENT "Samples build"
    COMMAND ${CMAKE_CXX_COMPILER} -o sample1.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample1.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} ${CMAKE_CXX_COMPILE_FLAG} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    COMMAND ${CMAKE_CXX_COMPILER} -o sample2.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample2.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} ${CMAKE_CXX_COMPILE_FLAG} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    COMMAND ${CMAKE_CXX_COMPILER} -o sample3.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample3.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} ${CMAKE_CXX_COMPILE_FLAG} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    COMMAND ${CMAKE_CXX_COMPILER} -o sample4.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample4.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} ${CMAKE_CXX_COMPILE_FLAG} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    COMMAND ${CMAKE_CXX_COMPILER} -o sample5.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample5.cpp ${cmd_line_sample_libs} ${cmd_line_sample_include} -std=c++17 ${CMAKE_CXX_COMPILE_FLAG} ${CMAKE_CXX_LINK_LIBRARY_FLAG}
    COMMAND ${CMAKE_CXX_COMPILER} -o sample6.exe ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample6.c ${cmd_line_sample_clibs} ${cmd_line_clib_sample_include} ${CMAKE_C_COMPILE_FLAG} ${CMAKE_C_LINK_LIBRARY_FLAG}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS argparse
    )

####
# sample code unit test generation
####
add_executable(samples-unittest ${CMAKE_CURRENT_SOURCE_DIR}/sample/sample_unittest.cpp)
add_dependencies(samples-unittest samples)
target_link_libraries(samples-unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_custom_target(run-samples-unittest
    COMMENT "Samples unittest execution"
    COMMAND samples-unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS samples-unittest
)

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
