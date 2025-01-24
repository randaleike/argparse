# Sample code CMake file
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0048 NEW)
project(sample_code VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Argument input parser sample code executable" HOMEPAGE_URL "https://github.com/randaleike/argparse")

        # Sample code CMake file
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
endif()

set (SAMPLE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sample)

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
    add_executable(${samplefile} EXCLUDE_FROM_ALL ${SAMPLE_SOURCE_DIR}/${samplefile}.cpp)
    target_include_directories(${samplefile} PUBLIC ${samples_cxxlib_include})
    target_compile_options(${samplefile} PRIVATE -std=c++17)
    target_link_libraries(${samplefile} ${sample_argparse_libs})
endforeach()

set(c_sample_files sample6)
foreach(samplefile ${c_sample_files})
    add_executable(${samplefile} EXCLUDE_FROM_ALL ${SAMPLE_SOURCE_DIR}/${samplefile}.c)
    target_include_directories(${samplefile} PUBLIC ${samples_clib_include})
    target_link_libraries(${samplefile} ${sample_argparse_libs} stdc++) #needs stdc++ because libraries use it
endforeach()

add_custom_target(samples
    COMMENT "Sample code build"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${cxx_sample_files} ${c_sample_files}
    )

####
# Unit testing
####
add_executable(samples-unittest EXCLUDE_FROM_ALL ${SAMPLE_SOURCE_DIR}/test/sample_unittest.cpp)
target_include_directories(samples-unittest SYSTEM PRIVATE ${GTEST_INCLUDE_DIR})
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(samples-unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(samples-unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
endif()
add_dependencies(samples-unittest ${cxx_sample_files} ${c_sample_files})

add_custom_target(run-samples-unittest
    COMMENT "Samples unittest execution"
    COMMAND samples-unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/samples-unittest
    )
