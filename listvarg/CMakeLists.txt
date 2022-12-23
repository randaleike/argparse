# listvarg Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(listvarg VERSION 1.0.0.0 DESCRIPTION "List Varg Library" HOMEPAGE_URL "grocsoftware")

####
# listvarg directory cmake include Files
####

####
# listvarg Files
####
set (listvargInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/listvarg.h
    )

set (listvargSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/listvarg.cpp
    )

####
# varg library
####
add_library(${PROJECT_NAME} STATIC ${listvargSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# listvarg Unit testing
####
set (listvargTest
    ${CMAKE_CURRENT_LIST_DIR}/test/listvarg_test.cpp
    )

set (listvargMockIncude
    )

set (listvargMockSrc
    )

set (listvargUnitTestMock
    )

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${vargIntfSrc} ${listvargSrc} ${listvargUnitTestMock} ${listvargTest})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test vargintf ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
