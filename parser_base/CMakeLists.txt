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

set (parser_baseMockInc
    ${parser_baseInclude}
    ${MASTER_PROJECT_BASE_DIR}/varg/mock
    )

set (parser_baseTestMock
    )

set (parser_baseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test ${parser_baseSrc} ${parser_baseTestMock} ${parser_baseTest})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${parser_baseInclude} ${parser_baseMockInc} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${parser_baseExternalLib} ${GTEST_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(${PROJECT_NAME}_test PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST parserBaseAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${parserBaseAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

