# cmd_line_parse Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(cmd_line_parse VERSION 1.0.0.0 DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "grocsoftware")

####
# cmd_line_parse directory cmake include Files
####

####
# cmd line parse base Files
####
set (cmd_line_parseInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/cmd_line_parse.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/cmd_line_parse_api.h
    )

set (cmd_line_parseSrc
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse_api_lib.cpp
    )


####
# cmd_line_parse library
####
add_library(${PROJECT_NAME} STATIC ${cmd_line_parseSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# cmd_line_parse Unit testing
####
set (cmd_line_parseUnitTestSrc
    ${cmd_line_parseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_test.cpp
    )

set (cmd_line_parseMockIncude
    )

set (cmd_line_parseMockSrc
    )

set (cmd_line_parseUnitTestMock
    )

set (cmd_line_parseExternalLib
    varg
    parser_base
    )

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${cmd_line_parseUnitTestSrc} ${cmd_line_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test ${cmd_line_parseExternalLib} ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
