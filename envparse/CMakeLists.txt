# envparse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(envparse VERSION 0.9.2.0 DESCRIPTION "Command Line Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

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
# envparse base Files
####
set (envparseInclude
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
    ${MASTER_PROJECT_BASE_DIR}/language/inc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc_clib
    )

set (envparse_baseSrc
    ${CMAKE_CURRENT_LIST_DIR}/src/envparse.cpp
    )

set (envparseSrc
    ${languageStringsSrc}
    ${parserBaseSrc}
    ${envparse_baseSrc}
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
    ${parserBaseSrc}
    ${envparse_baseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/envparse_test.cpp
    )

set (envparseUnittestIncude
    ${envparseInclude}
    ${MASTER_PROJECT_BASE_DIR}/varg/inc
    ${MASTER_PROJECT_BASE_DIR}/varg/mock
    ${MASTER_PROJECT_BASE_DIR}/language/inc
    ${MASTER_PROJECT_BASE_DIR}/language/mock
    )

set (envparseMockSrc
    ${MASTER_PROJECT_BASE_DIR}/language/mock/mock_ParserStringListInterface.cpp
    )

set (envparseExternalLib
    varg
    )

enable_testing()

add_executable(${PROJECT_NAME}_test  ${envparseUnitTestSrc} ${envparseMockSrc})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${envparseUnittestIncude} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${envparseExternalLib} ${GTEST_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(${PROJECT_NAME}_test PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST envparseAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${envparseAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()
