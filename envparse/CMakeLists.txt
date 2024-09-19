# envparse Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(envparse VERSION 0.9.2.0 DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# envparse directory cmake include Files
####

####
# envparse base Files
####
set (envparseInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/envparse.h
    )

set (envparseSrc
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/envparse.cpp
    )


####
# envparse library
####
add_library(${PROJECT_NAME} STATIC ${envparseSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# envparse Unit testing
####
set (envparseUnitTestSrc
    ${envparseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/envparse_test.cpp
    )

set (envparseMockIncude
    )

set (envparseMockSrc
    )

set (envparseUnitTestMock
    )

set (envparseExternalLib
    varg
    parser_base
    )

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${envparseUnitTestSrc} ${envparseUnitTestMock})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test ${envparseExternalLib} ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
