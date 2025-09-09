cmake_minimum_required(VERSION 3.14)
project(ParserStringListInterface VERSION 0.5.0 LANGUAGES C CXX DESCRIPTION "C++ international language support dor the argparse library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

####
# ParserStringListInterface include directories
####
set (ParserStringListInterfaceInclude
     ${CMAKE_CURRENT_LIST_DIR}/inc
    )

####
# ParserStringListInterface source files
####
set (ParserStringListInterfaceSources
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterface.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceEnglish.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceSpanish.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceFrench.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceSimplifiedchinese.cpp
    )

####
# ParserStringListInterface library
####
add_library(${PROJECT_NAME} OBJECT ${ParserStringListInterfaceSources})
target_include_directories(${PROJECT_NAME} PRIVATE ${ParserStringListInterfaceInclude})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

####
# Enable testing
####
enable_testing()
include(GoogleTest)

####
# ParserStringListInterfaceEnglish_test unit test build
####
add_executable(ParserStringListInterfaceEnglish_test src/ParserStringListInterfaceEnglish.cpp test/ParserStringListInterfaceEnglish_test.cpp)
target_include_directories(ParserStringListInterfaceEnglish_test PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(ParserStringListInterfaceEnglish_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(ParserStringListInterfaceEnglish_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(ParserStringListInterfaceEnglish_test PRIVATE --coverage)
    target_link_options(ParserStringListInterfaceEnglish_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET ParserStringListInterfaceEnglish_test TEST_LIST ParserStringListInterfaceEnglish_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${ParserStringListInterfaceEnglish_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# ParserStringListInterfaceSpanish_test unit test build
####
add_executable(ParserStringListInterfaceSpanish_test src/ParserStringListInterfaceSpanish.cpp test/ParserStringListInterfaceSpanish_test.cpp)
target_include_directories(ParserStringListInterfaceSpanish_test PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(ParserStringListInterfaceSpanish_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(ParserStringListInterfaceSpanish_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(ParserStringListInterfaceSpanish_test PRIVATE --coverage)
    target_link_options(ParserStringListInterfaceSpanish_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET ParserStringListInterfaceSpanish_test TEST_LIST ParserStringListInterfaceSpanish_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${ParserStringListInterfaceSpanish_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# ParserStringListInterfaceFrench_test unit test build
####
add_executable(ParserStringListInterfaceFrench_test src/ParserStringListInterfaceFrench.cpp test/ParserStringListInterfaceFrench_test.cpp)
target_include_directories(ParserStringListInterfaceFrench_test PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(ParserStringListInterfaceFrench_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(ParserStringListInterfaceFrench_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(ParserStringListInterfaceFrench_test PRIVATE --coverage)
    target_link_options(ParserStringListInterfaceFrench_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET ParserStringListInterfaceFrench_test TEST_LIST ParserStringListInterfaceFrench_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${ParserStringListInterfaceFrench_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# ParserStringListInterfaceSimplifiedchinese_test unit test build
####
add_executable(ParserStringListInterfaceSimplifiedchinese_test src/ParserStringListInterfaceSimplifiedchinese.cpp test/ParserStringListInterfaceSimplifiedchinese_test.cpp)
target_include_directories(ParserStringListInterfaceSimplifiedchinese_test PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(ParserStringListInterfaceSimplifiedchinese_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(ParserStringListInterfaceSimplifiedchinese_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(ParserStringListInterfaceSimplifiedchinese_test PRIVATE --coverage)
    target_link_options(ParserStringListInterfaceSimplifiedchinese_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET ParserStringListInterfaceSimplifiedchinese_test TEST_LIST ParserStringListInterfaceSimplifiedchinese_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${ParserStringListInterfaceSimplifiedchinese_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# LocalLanguageSelect_Linux unit test build
####
add_executable(LocalLanguageSelect_Linux ${ParserStringListInterfaceSources} test/LocalLanguageSelect_Linux_test.cpp)
target_include_directories(LocalLanguageSelect_Linux PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(LocalLanguageSelect_Linux PRIVATE ${GTEST_LIBRARIES})
target_compile_options(LocalLanguageSelect_Linux PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(LocalLanguageSelect_Linux PRIVATE --coverage)
    target_link_options(LocalLanguageSelect_Linux PRIVATE --coverage)
endif()

gtest_add_tests (TARGET LocalLanguageSelect_Linux TEST_LIST LocalLanguageSelect_LinuxAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${LocalLanguageSelect_LinuxAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# LocalLanguageSelect_Windows unit test build
####
add_executable(LocalLanguageSelect_Windows ${ParserStringListInterfaceSources} test/LocalLanguageSelect_Windows_test.cpp)
target_include_directories(LocalLanguageSelect_Windows PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(LocalLanguageSelect_Windows PRIVATE ${GTEST_LIBRARIES})
target_compile_options(LocalLanguageSelect_Windows PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(LocalLanguageSelect_Windows PRIVATE --coverage)
    target_link_options(LocalLanguageSelect_Windows PRIVATE --coverage)
endif()

gtest_add_tests (TARGET LocalLanguageSelect_Windows TEST_LIST LocalLanguageSelect_WindowsAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${LocalLanguageSelect_WindowsAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# ParserStringListInterface_test unit test build
####
add_executable(ParserStringListInterface_test ${ParserStringListInterfaceSources} test/ParserStringListInterface_test.cpp)
target_include_directories(ParserStringListInterface_test PUBLIC ${ParserStringListInterfaceInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(ParserStringListInterface_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(ParserStringListInterface_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(ParserStringListInterface_test PRIVATE --coverage)
    target_link_options(ParserStringListInterface_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET ParserStringListInterface_test TEST_LIST ParserStringListInterface_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${ParserStringListInterface_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

