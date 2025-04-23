# cmd_line_parse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(cmd_line_parse VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")


####
# Language String Files
####
include(${MASTER_PROJECT_BASE_DIR}/language/language_files.cmake)

####
# Parser Base base Files
####
include(${MASTER_PROJECT_BASE_DIR}/parser_base/parser_base.cmake)

####
# cmd_line_parse directory cmake include Files
####
set (cmd_line_includes
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
    ${MASTER_PROJECT_BASE_DIR}/language/inc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc_clib
    )

####
# cmd line parse base Files
####
set (cmd_line_baseSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/cmd_line_parse_api_lib.cpp
    )

set (cmd_line_parseSrc
    ${languageStringsSrc}
    ${parserBaseSrc}
    ${cmd_line_baseSrc}
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
    ${parserBaseSrc}
    ${cmd_line_baseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_test_class.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_misc_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_api_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/cmd_line_parse_unittest.cpp
    )

set (cmd_line_parseUnittestIncude
    ${cmd_line_includes}
    ${MASTER_PROJECT_BASE_DIR}/varg/mock
    ${MASTER_PROJECT_BASE_DIR}/language/mock
    )

set (cmd_line_parseMockSrc
    ${MASTER_PROJECT_BASE_DIR}/language/mock/mock_ParserStringListInterface.cpp
    )

set (cmd_line_parseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test  ${cmd_line_parseUnitTestSrc} ${cmd_line_parseMockSrc})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${cmd_line_parseUnittestIncude} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${cmd_line_parseExternalLib} ${GTEST_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
target_compile_options(${PROJECT_NAME}_test PRIVATE -DCONSTRUCTOR_GET_HELP_STRING=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(${PROJECT_NAME}_test PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST cmdLineParseAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${cmdLineParseAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()
