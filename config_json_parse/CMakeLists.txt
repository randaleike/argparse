# config_json_parse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(config_json_parse VERSION 0.2.0.0 LANGUAGES C CXX
        DESCRIPTION "JSON Config File Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# config_json_parse directory cmake include Files
####
set (config_json_includes
     ${MASTER_PROJECT_BASE_DIR}/varg/inc
     ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
     ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc_clib)

####
# config_json_parse base Files
####
set (config_json_parseSrc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_base.cpp
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/config_json_parse.cpp
    )


####
# config_json_parse library
####
add_library(${PROJECT_NAME} STATIC ${config_json_parseSrc})
target_include_directories(${PROJECT_NAME} PUBLIC ${config_json_includes})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# config_json_parse Unit testing
####
set (config_json_parseUnitTestSrc
    ${config_json_parseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/config_json_parse_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/config_json_parse_unittest.cpp
    )

set (config_json_parseMockIncude
    )

set (config_json_parseMockSrc
    )

set (config_json_parseUnitTestMock
    )

set (config_json_parseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL ${config_json_parseUnitTestSrc} ${config_json_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PUBLIC ${config_json_includes} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${config_json_parseExternalLib} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
