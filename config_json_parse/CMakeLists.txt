# config_json_parse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(config_json_parse VERSION 0.2.0.0 LANGUAGES C CXX
        DESCRIPTION "JSON Config File Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# config_json_parse_test setup files
####
configure_file(${CMAKE_CURRENT_LIST_DIR}/test/test_config_json.h.in
                ${CMAKE_CURRENT_LIST_DIR}/test/test_config_json.h
                @ONLY)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/test/test-config.json DESTINATION ${CMAKE_BINARY_DIR})

####
# Varg Files
####
include(${MASTER_PROJECT_BASE_DIR}/varg/varg.cmake)

####
# Language String Files
####
include(${MASTER_PROJECT_BASE_DIR}/language/language_files.cmake)

####
# Parser Base base Files
####
include(${MASTER_PROJECT_BASE_DIR}/parser_base/parser_base.cmake)

####
# config_json_parse directory cmake include Files
####
set (config_json_includes
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
    ${MASTER_PROJECT_BASE_DIR}/language/inc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc_clib
    )

####
# config_json_parse base Files
####
set (config_json_parse_baseSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/config_json_parse.cpp
    )

set (config_json_parseSrc
    ${languageStringsSrc}
    ${parserBaseSrc}
    ${config_json_parse_baseSrc}
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
    ${parserBaseSrc}
    ${config_json_parse_baseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/config_json_parse_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/config_json_parse_unittest.cpp
    )

set (config_json_parseUnittestIncude
    ${config_json_includes}
    ${MASTER_PROJECT_BASE_DIR}/varg/mock
    ${MASTER_PROJECT_BASE_DIR}/language/mock
    )

set (config_json_parseMockSrc
    ${MASTER_PROJECT_BASE_DIR}/language/mock/mock_ParserStringListInterface.cpp
    )

set (config_json_parseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test  ${config_json_parseUnitTestSrc} ${config_json_parseMockSrc} )
target_include_directories(${PROJECT_NAME}_test PUBLIC ${config_json_parseUnittestIncude} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${config_json_parseExternalLib} ${GTEST_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(${PROJECT_NAME}_test PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST configJsonAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${configJsonAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()
