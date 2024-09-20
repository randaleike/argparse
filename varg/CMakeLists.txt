# varg Library CMake file
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0048 NEW)
project(varg VERSION 0.9.2.0 DESCRIPTION "Varg Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

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
    ${CMAKE_CURRENT_LIST_DIR}/inc/varg_intf.h
    ${CMAKE_CURRENT_LIST_DIR}/inc/listvarg.h
    )

set (vargSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/varg_intf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/varg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/vargincrement.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/listvarg.cpp
    )

####
# varg library
####
add_library(${PROJECT_NAME} STATIC ${vargSrc})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# varg Unit testing
####
set (vargUnitTestSrc
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_intf_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_basic_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/vargincrement_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/vargenum_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/listvarg_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_test.cpp
    )

set (vargMockInclude
    )

set (vargMockSrc
    )

#find_package(PkgConfig)
#pkg_search_module(GTEST REQUIRED gtest_main)

add_executable(${PROJECT_NAME}_test ${vargSrc} ${vargUnitTestSrc} ${vargMockSrc})
target_include_directories(${PROJECT_NAME}_test PRIVATE ${include_directories})
target_link_libraries(${PROJECT_NAME}_test ${GTEST_LDFLAGS})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
