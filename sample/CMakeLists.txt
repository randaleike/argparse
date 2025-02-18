# Sample code CMake file
cmake_minimum_required(VERSION 3.14)
project(sample_code VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Argument input parser sample code executable" HOMEPAGE_URL "https://github.com/randaleike/argparse")

        # Sample code CMake file
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(stdlib_compile_options "")
    set(stdlib_c_link_options "")
    set(pthread_lib "")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
    set(stdlib_compile_options "-std=c++17")
    set(stdlib_c_link_options "stdc++") #needs stdc++ because libraries use it
    set(pthread_lib "pthread")
endif()

####
# Sample code argparse library include paths
####
set (samples_cxxlib_include
     ${MASTER_PROJECT_BASE_DIR}/varg/inc
     ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
     ${MASTER_PROJECT_BASE_DIR}/cmd_line_parse/inc
     ${MASTER_PROJECT_BASE_DIR}/envparse/inc
     )

set (samples_clib_include
     ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
     ${MASTER_PROJECT_BASE_DIR}/cmd_line_parse/inc_clib
     )

####
# Sample code executable argparse library list
####
set(sample_argparse_libs
    varg
    cmd_line_parse
    envparse
    )

####
# Sample code executable build
####
set(cxx_sample_files sample1 sample2 sample3 sample4 sample5)

foreach(samplefile ${cxx_sample_files})
    add_executable(${samplefile} ${CMAKE_CURRENT_SOURCE_DIR}/${samplefile}.cpp)
    target_include_directories(${samplefile} PUBLIC ${samples_cxxlib_include})
    target_compile_options(${samplefile} PRIVATE ${stdlib_compile_options})
    target_link_directories(${samplefile} PRIVATE ${CMAKE_BINARY_DIR})
    target_link_libraries(${samplefile} PRIVATE ${sample_argparse_libs})
endforeach()

set(c_sample_files sample6)
foreach(samplefile ${c_sample_files})
    add_executable(${samplefile} ${CMAKE_CURRENT_SOURCE_DIR}/${samplefile}.c)
    target_include_directories(${samplefile} PUBLIC ${samples_clib_include})
    target_link_libraries(${samplefile} PUBLIC ${sample_argparse_libs} ${stdlib_c_link_options})
endforeach()

add_custom_target(samples
    COMMENT "Sample code build"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${cxx_sample_files} ${c_sample_files}
    )

####
# Unit testing
####
enable_testing()

add_executable(samples-unittest ${CMAKE_CURRENT_SOURCE_DIR}/test/sample_unittest.cpp)
target_include_directories(samples-unittest SYSTEM PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(samples-unittest PUBLIC GTest::gtest_main ${pthread_lib})
add_dependencies(samples-unittest ${cxx_sample_files} ${c_sample_files})

include(GoogleTest)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    gtest_add_tests(TARGET samples-unittest WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE} TEST_LIST sampleAllTests)
    set_tests_properties(${sampleAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
else()
    gtest_add_tests(TARGET samples-unittest WORKING_DIRECTORY ${CMAKE_BINARY_DIR} TEST_LIST sampleAllTests)
endif()

add_custom_target(run-samples-unittest
    COMMENT "Samples unittest execution"
    COMMAND samples-unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/samples-unittest
    )
