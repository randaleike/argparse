# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.12)
project(sample-code VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Argparse sample code" HOMEPAGE_URL "https://github.com/randaleike/argparse")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

####
# Sample lib include paths
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
# Sample lib includes
####
set (samples_cxxlibs
    $<TARGET_FILE:envparse>
    $<TARGET_FILE:cmd_line_parse>
    $<TARGET_FILE:varg>
    )

set (samples_clibs
    $<TARGET_FILE:envparse>
    $<TARGET_FILE:cmd_line_parse>
    $<TARGET_FILE:varg>
    )

####
# Sample code executable build
####
set(cxx_sample_files sample1 sample2 sample3 sample4 sample5)

foreach(samplefile ${cxx_sample_files})
    add_executable(${samplefile} ${CMAKE_CURRENT_SOURCE_DIR}/${samplefile}.cpp)
    target_include_directories(${samplefile} PUBLIC ${samples_cxxlib_include})
    target_link_libraries(${samplefile} ${samples_cxxlibs})
endforeach()

#target_compile_options(sample5 PRIVATE -std=c++17)

set(c_sample_files sample6)
foreach(samplefile ${c_sample_files})
    add_executable(${samplefile} ${CMAKE_CURRENT_SOURCE_DIR}/${samplefile}.c)
    target_include_directories(${samplefile} PUBLIC ${samples_clib_include})
    target_link_libraries(${samplefile} ${samples_clibs})
endforeach()

####
# Unit testing
####
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(samples-unittest ${CMAKE_CURRENT_SOURCE_DIR}/test/sample_unittest.cpp)
#target_include_directories(samples_unittest SYSTEM PRIVATE ${GTEST_INCLUDE_DIR})
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(samples-unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(samples-unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
endif()
#add_dependencies(samples-unittest ${cxx_sample_files} ${c_sample_files} ${samples_cxxlibs} ${samples_clibs})
