# varg Library CMake file
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0048 NEW)
project(varg VERSION 0.9.2.0 LANGUAGES C CXX DESCRIPTION "Varg Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
endif()

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

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()
message("gtest lib=${GTEST_LIBRARIES}")
message("gtest_main lib=${GTEST_MAIN_LIBRARIES}")
add_executable(${PROJECT_NAME}_test EXCLUDE_FROM_ALL ${vargSrc} ${vargUnitTestSrc} ${vargMockSrc})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${vargInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PUBLIC ${GTEST_CFLAGS})
gtest_discover_tests(${PROJECT_NAME}_test)
