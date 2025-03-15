# config_xml_parse Library CMake file
cmake_minimum_required(VERSION 3.14)
project(config_xml_parse VERSION 0.2.0.0 LANGUAGES C CXX
        DESCRIPTION "JSON Config File Parser Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

####
# config_xml_parse directory cmake include Files
####
set (config_xml_includes
     ${MASTER_PROJECT_BASE_DIR}/varg/inc
     ${MASTER_PROJECT_BASE_DIR}/varg/inc_clib
     ${MASTER_PROJECT_BASE_DIR}/parser_base/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc
     ${CMAKE_CURRENT_LIST_DIR}/inc_clib)

####
# config_xml_parse base Files
####
set (config_xml_parseSrc
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_base.cpp
    ${MASTER_PROJECT_BASE_DIR}/parser_base/src/parser_string_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/config_xml_parse.cpp
    )

####
# config_xml_parse library
####
add_library(${PROJECT_NAME} STATIC ${config_xml_parseSrc})
target_include_directories(${PROJECT_NAME} PUBLIC ${config_xml_includes})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# config_xml_parse Unit testing
####
set (config_xml_parseUnitTestSrc
    ${config_xml_parseSrc}
    ${CMAKE_CURRENT_LIST_DIR}/test/config_xml_parse_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/config_xml_parse_unittest.cpp
    )

set (config_xml_parseMockIncude
    ${config_xml_includes}
    ${MASTER_PROJECT_BASE_DIR}/varg/mock
    )

set (config_xml_parseMockSrc
    )

set (config_xml_parseUnitTestMock
    )

set (config_xml_parseExternalLib
    varg
    )

file(COPY ${CMAKE_CURRENT_LIST_DIR}/test/test-config.xml DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()

add_executable(${PROJECT_NAME}_test  ${config_xml_parseUnitTestSrc} ${config_xml_parseUnitTestMock} )
target_include_directories(${PROJECT_NAME}_test PUBLIC ${config_xml_parseMockIncude} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${config_xml_parseExternalLib} ${GTEST_LIBRARIES})
target_compile_options(${PROJECT_NAME}_test PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(${PROJECT_NAME}_test PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}_test PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test TEST_LIST configXmlAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${configXmlAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

