# varg Library CMake file
cmake_minimum_required(VERSION 3.14)
project(varg VERSION 0.9.2.0 LANGUAGES C CXX DESCRIPTION "Varg Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

####
# varg directory cmake include Files
####

####
# varg Files
####
set (vargInclude
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc_clib
    )

set (vargSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/varg_intf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/varg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/listvarg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/varg_clib.cpp
    )

####
# varg library
####
add_library(${PROJECT_NAME} STATIC ${vargSrc})
target_include_directories(${PROJECT_NAME} PRIVATE ${vargInclude})
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
    ${CMAKE_CURRENT_LIST_DIR}/test/vargptr_basic_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/vargcarray_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_clib_api_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/varg_test.cpp
    )

set (vargMockInclude
    )

set (vargMockSrc
    )

set (varargExtLibs
)

enable_testing()

enable_testing()

add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL ${vargSrc} ${vargUnitTestSrc} ${vargMockSrc})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${vargInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${varargExtLibs} GTest::gtest_main)
target_compile_options(${PROJECT_NAME}_test PUBLIC )

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST vargAllTests)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${vargAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()
