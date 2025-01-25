# config_json_parse Library CMake file
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(config_json_parse VERSION 0.9.0.0 LANGUAGES C CXX
        DESCRIPTION "JSON Config File Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
endif()

####
# config_json_parse directory cmake include Files
####
set (cmd_line_includes
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
target_include_directories(${PROJECT_NAME} PRIVATE ${cmd_line_includes})
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

add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL ${config_json_parseUnitTestSrc} ${config_json_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PRIVATE ${cmd_line_includes} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${config_json_parseExternalLib} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
