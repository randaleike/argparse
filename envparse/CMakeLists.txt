# envparse Library CMake file
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(envparse VERSION 0.9.2.0 DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

####
# envparse directory cmake include Files
####

####
# envparse base Files
####
set (envparseInclude
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
    ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc_clib
    )

set (envparseSrc
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/envparse.cpp
    )


####
# envparse library
####
add_library(${PROJECT_NAME} STATIC ${envparseSrc})
target_include_directories(${PROJECT_NAME} PRIVATE ${envparseInclude})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# envparse Unit testing
####
set (envparseUnitTestSrc
    ${envparseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/envparse_test.cpp
    )

set (envparseMockIncude
    )

set (envparseMockSrc
    )

set (envparseUnitTestMock
    )

set (envparseExternalLib
    varg
    )

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(${PROJECT_NAME}_test ${envparseUnitTestSrc} ${envparseUnitTestMock})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${envparseInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test ${envparseExternalLib} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
