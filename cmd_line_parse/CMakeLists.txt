# cmd_line_parse Library CMake file
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(cmd_line_parse VERSION 0.9.0.0 LANGUAGES C CXX 
        DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
    
####
# cmd_line_parse directory cmake include Files
####

####
# cmd line parse base Files
####
set (cmd_line_includes
     ${MASTER_PROJECT_BASE_DIR}/varg/inc
     ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
     ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc_clib)

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

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(${PROJECT_NAME}_test ${cmd_line_parseUnitTestSrc} ${cmd_line_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PRIVATE ${cmd_line_includes} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test ${cmd_line_parseExternalLib} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
