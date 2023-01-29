# varg Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(varg VERSION 1.0.0.0 DESCRIPTION "Varg Library" HOMEPAGE_URL "grocsoftware")

####
# varg directory cmake include Files
####

####
# varg Files
####

set (vargInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc/varg.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/vargincrement.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/vargenum.h
    )

set (vargSrc
    ${CMAKE_CURRENT_LIST_DIR}/../varg_intf/src/varg_intf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/varg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/vargincrement.cpp
    )

####
# varg library
####
add_library(${PROJECT_NAME} STATIC ${vargSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# varg Unit testing
####
set (vargTest
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_test.cpp
    )

set (vargMockInclude
    )

set (vargMockSrc
    )

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${vargIntfSrc} ${vargSrc} ${vargUnitTestSrc} ${vargTest})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test vargintf ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
