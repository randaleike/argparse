# parser_base Library CMake file
cmake_minimum_required(VERSION 3.14)
project(parser_base VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Parser Base Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# parser_base directory cmake include Files
####

####
# parser_base Files
####
set (parser_baseInclude
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
    )

set (parser_baseSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/parser_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/parser_string_list.cpp
    )

####
# parser_base library
####
add_library(${PROJECT_NAME} STATIC ${parser_baseSrc})
target_include_directories(${PROJECT_NAME} PRIVATE ${parser_baseInclude})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# parser_base Unit testing
####
set (parser_baseTest
    ${CMAKE_CURRENT_LIST_DIR}/test/parser_string_list_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/parser_base_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/parser_base_unittest.cpp
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
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test ${parser_baseUnitTest} ${parser_baseUnitTestMock} ${parser_baseTest})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${parser_baseInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${parser_baseExternalLib} GTest::gtest_main)

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST parserBaseAllTests)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${parserBaseAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()
