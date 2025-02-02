# cmd_line_parse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(cmd_line_parse VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# cmd_line_parse directory cmake include Files
####
set (cmd_line_includes
     ${MASTER_PROJECT_BASE_DIR}/varg/inc
     ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
     ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc_clib)

####
# cmd line parse base Files
####

set (cmd_line_parseSrc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_base.cpp
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse_api_lib.cpp
    )


####
# cmd_line_parse library
####
add_library(${PROJECT_NAME} STATIC ${cmd_line_parseSrc})
target_include_directories(${PROJECT_NAME} PRIVATE ${cmd_line_includes})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# cmd_line_parse Unit testing
####
set (cmd_line_parseUnitTestSrc
    ${cmd_line_parseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_api_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_unittest.cpp
    )

set (cmd_line_parseMockIncude
    )

set (cmd_line_parseMockSrc
    )

set (cmd_line_parseUnitTestMock
    )

set (cmd_line_parseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL ${cmd_line_parseUnitTestSrc} ${cmd_line_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PRIVATE ${cmd_line_includes} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${cmd_line_parseExternalLib} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test DISCOVERY_MODE PRE_TEST)
