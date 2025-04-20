# ParserStringListInterface Library CMake file
cmake_minimum_required(VERSION 3.14)
project(ParserStringListInterface VERSION 1.0.1.0 LANGUAGES C CXX DESCRIPTION "ParserStringListInterface Library" HOMEPAGE_URL "https://github.com/randaleike/argparse")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

####
# ParserStringListInterface Files
####
set (ParserStringListInterface_baseInclude
     ${CMAKE_CURRENT_LIST_DIR}/inc
     )

set (ParserStringListInterface_baseSrc
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterface.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceEnglish.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceSpanish.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceFrench.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/ParserStringListInterfaceSimplifiedchinese.cpp
     )

####
# ParserStringListInterface Unit tests
####
enable_testing()
include(GoogleTest)

####
# english string unit test build
####
add_executable(ParserStringListInterfaceEnglish_test  src/ParserStringListInterfaceEnglish.cpp test/ParserStringListInterfaceEnglish_test.cpp)
target_include_directories(ParserStringListInterfaceEnglish_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
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
# spanish string unit test build
####
add_executable(ParserStringListInterfaceSpanish_test  src/ParserStringListInterfaceSpanish.cpp test/ParserStringListInterfaceSpanish_test.cpp)
target_include_directories(ParserStringListInterfaceSpanish_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
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
# french string unit test build
####
add_executable(ParserStringListInterfaceFrench_test  src/ParserStringListInterfaceFrench.cpp test/ParserStringListInterfaceFrench_test.cpp)
target_include_directories(ParserStringListInterfaceFrench_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
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
# SimplifiedChinese string unit test build
####
add_executable(ParserStringListInterfaceSimplifiedchinese_test  src/ParserStringListInterfaceSimplifiedchinese.cpp test/ParserStringListInterfaceSimplifiedchinese_test.cpp)
target_include_directories(ParserStringListInterfaceSimplifiedchinese_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
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
# OS selection Unit test unit test build
####
add_executable(LocalLanguageSelect_Linux_test  ${ParserStringListInterface_baseSrc} test/LocalLanguageSelect_Linux_test.cpp)
target_include_directories(LocalLanguageSelect_Linux_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(LocalLanguageSelect_Linux_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(LocalLanguageSelect_Linux_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(LocalLanguageSelect_Linux_test PRIVATE --coverage)
    target_link_options(LocalLanguageSelect_Linux_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET LocalLanguageSelect_Linux_test TEST_LIST LocalLanguageSelect_Linux_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${LocalLanguageSelect_Linux_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# OS selection Unit test unit test build
####
add_executable(LocalLanguageSelect_Windows_test  ${ParserStringListInterface_baseSrc} test/LocalLanguageSelect_Windows_test.cpp)
target_include_directories(LocalLanguageSelect_Windows_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
target_link_libraries(LocalLanguageSelect_Windows_test PRIVATE ${GTEST_LIBRARIES})
target_compile_options(LocalLanguageSelect_Windows_test PUBLIC -DGTEST_LINKED_AS_SHARED_LIBRARY=1)
if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
    target_compile_options(LocalLanguageSelect_Windows_test PRIVATE --coverage)
    target_link_options(LocalLanguageSelect_Windows_test PRIVATE --coverage)
endif()

gtest_add_tests (TARGET LocalLanguageSelect_Windows_test TEST_LIST LocalLanguageSelect_Windows_testAllTests)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_tests_properties(${LocalLanguageSelect_Windows_testAllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
endif()

####
# ParserStringListInterface_baseUnittest unit test build
####
add_executable(ParserStringListInterface_test  ${ParserStringListInterface_baseSrc} test/ParserStringListInterface_test.cpp)
target_include_directories(ParserStringListInterface_test PUBLIC ${ParserStringListInterface_baseInclude} ${GTEST_INCLUDE_DIR})
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
