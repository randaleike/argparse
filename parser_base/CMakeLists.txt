# parser_base Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(parser_base VERSION 1.0.0.0 DESCRIPTION "Parser Base Library" HOMEPAGE_URL "grocsoftware")

####
# parser_base directory cmake include Files
####

####
# parser_base Files
####
set (parser_baseInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/parser_base.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/parser_string_list.h
    )

set (parser_baseSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/parser_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/parser_string_list.cpp
    )

####
# parser_base library
####
add_library(${PROJECT_NAME} STATIC ${parser_baseSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# parser_base Unit testing
####
set (parser_baseTest
    ${CMAKE_CURRENT_LIST_DIR}/test/parser_base_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/parser_string_list_test.cpp
    )

set (parser_baseMockIncude
    )

set (parser_baseMockSrc
    )

set (parser_baseUnitTest
    ${vargIntfSrc}
    ${parser_baseSrc}
    )

set (parser_baseUnitTestMock
    )

set (parser_baseExternalLib
    vargintf
    varg
    listvarg
    )

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${parser_baseUnitTest} ${parser_baseUnitTestMock} ${parser_baseTest})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test ${parser_baseExternalLib} ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
