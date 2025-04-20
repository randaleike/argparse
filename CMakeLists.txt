# Arg parser Library CMake file
cmake_minimum_required(VERSION 3.14)
#cmake_policy(SET CMP0048 NEW)
project(argparse VERSION 0.9.2.0 LANGUAGES C CXX
        DESCRIPTION "Executable argument input parser utilities" HOMEPAGE_URL "https://github.com/randaleike/argparse")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy --config-file ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/varg/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/varg/inc_clib
    ${CMAKE_CURRENT_SOURCE_DIR}/language/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/parser_base/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/cmd_line_parse/inc_clib
    ${CMAKE_CURRENT_SOURCE_DIR}/envparse/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/envparse/inc_clib
    ${CMAKE_CURRENT_SOURCE_DIR}/config_json_parse/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/config_json_parse/_clib
    ${CMAKE_CURRENT_SOURCE_DIR}/config_xml_parse/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/config_xml_parse/inc_clib
    )
set(MASTER_PROJECT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include(GNUInstallDirs)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif()


####
# Gtest setup
####
if(DEFINED ENV{GITHUB_ACTION})
    set(GTEST_GITHUB_REPO https://randaleike:$ENV{GTEST_FETCH}@github.com/google/googletest)
else()
    set(GTEST_GITHUB_REPO https://github.com/google/googletest)
endif()

include(CTest)
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY ${GTEST_GITHUB_REPO}
    GIT_TAG v1.15.2)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    #set(gtest_force_shared_crt ON CACHE BOOL "Windows use msvcrt.dll" FORCE)
endif()
FetchContent_MakeAvailable(googletest)
set (GTEST_INCLUDE_DIR ${googletest_SOURCE_DIR}/googletest/include)
set (GMOCK_INCLUDE_DIR ${googletest_SOURCE_DIR}/googlemock/include)
set (GTEST_DLL_LIBRARY_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${CMAKE_BUILD_TYPE})
set (GTEST_LIB_LIBRARY_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_BUILD_TYPE})
set (GTEST_LIBRARIES GTest::gmock_main GTest::gtest_main)
#message ("googletest_DLL: ${GTEST_DLL_LIBRARY_PATH}")
#message ("googletest_LIB: ${GTEST_LIB_LIBRARY_PATH}")
#message ("googletest BINdir: ${googletest_BINARY_DIR}")
#message ("googletest SRCdir: ${googletest_SOURCE_DIR}")
#LIST (APPEND CMAKE_PROGRAM_PATH ${GTEST_LIB_LIBRARY_PATH})

####
# varg Files
####
add_subdirectory (varg)

####
# Parser Base base Files
####
add_subdirectory (language)
add_subdirectory (parser_base)
add_subdirectory (cmd_line_parse)
add_subdirectory (envparse)
add_subdirectory (config_json_parse)
add_subdirectory (config_xml_parse)

####
# Argparse Library build
####
set (argparse_libs
    $<TARGET_FILE:envparse>
    $<TARGET_FILE:cmd_line_parse>
    $<TARGET_FILE:varg>
    )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_program(LIB_EXE NAMES lib.exe)
    set(C_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.lib)
    add_custom_target(${PROJECT_NAME}
                    COMMAND ${LIB_EXE} /out:${C_LIB} $<TARGET_FILE:varg> $<TARGET_FILE:cmd_line_parse> $<TARGET_FILE:envparse>
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    DEPENDS ${argparse_libs}
                    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_program(LIB_EXE NAMES ar)
    set(C_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a)
    add_custom_target(${PROJECT_NAME}
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:varg>
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:cmd_line_parse>
                      COMMAND ${LIB_EXE} -x $<TARGET_FILE:envparse>
                      COMMAND ${LIB_EXE} rcs ${C_LIB} *.o
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                      DEPENDS ${argparse_libs}
                      )
endif()


####
# Library unit testing
####
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (UNITTEST_WORKING_DIR ${CMAKE_BINARY_DIR})
    set (UNITTEST_FILE_EXT "")
    set (UNITTEST_EXE_CMD "./")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set (UNITTEST_WORKING_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set (UNITTEST_FILE_EXT ".exe")
    set (UNITTEST_EXE_CMD ".\\")
endif()

set(libUnittestList
    varg_test
    parser_base_test
    cmd_line_parse_test
    envparse_test
    config_json_parse_test
    config_xml_parse_test
    )

add_custom_target(build-unittest
    COMMENT "Build library unittest"
    WORKING_DIRECTORY ${UNITTEST_WORKING_DIR}
    DEPENDS varg_test
    DEPENDS parser_base_test
    DEPENDS cmd_line_parse_test
    DEPENDS envparse_test
    DEPENDS config_json_parse_test
    DEPENDS config_xml_parse_test
    )

add_custom_target(run-unittest
    COMMENT "Run library unittest"
    COMMAND ${UNITTEST_EXE_CMD}varg_test${UNITTEST_FILE_EXT}
    COMMAND ${UNITTEST_EXE_CMD}parser_base_test${UNITTEST_FILE_EXT}
    COMMAND ${UNITTEST_EXE_CMD}cmd_line_parse_test${UNITTEST_FILE_EXT}
    COMMAND ${UNITTEST_EXE_CMD}envparse_test${UNITTEST_FILE_EXT}
    COMMAND ${UNITTEST_EXE_CMD}config_json_parse_test${UNITTEST_FILE_EXT}
    COMMAND ${UNITTEST_EXE_CMD}config_xml_parse_test${UNITTEST_FILE_EXT}
    WORKING_DIRECTORY ${UNITTEST_WORKING_DIR}
    DEPENDS build-unittest
    )

add_custom_target(build-coverage-reports
    COMMENT "Build library unittest coverage report"
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/coverage
    COMMAND gcovr --html-details ${CMAKE_BINARY_DIR}/coverage/unittest-coverage.html
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

####
# Install package
####
include(CPack)
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")

if (WIN32)
    set(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_SOURCE_GENERATOR "TGZ")
endif()

####
# argparse doxygen generation
####
if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    option(BUILD_DOC "Build Documentation configuration" ON)
    set(DOXYGEN_CPPIN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-cpp.in)
    set(DOXYGEN_CPPOUT ${CMAKE_BINARY_DIR}/Doxyfile-cpp)
    configure_file(${DOXYGEN_CPPIN} ${DOXYGEN_CPPOUT} @ONLY)

    set(DOXYGEN_CIN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-c.in)
    set(DOXYGEN_COUT ${CMAKE_BINARY_DIR}/Doxyfile-c)
    configure_file(${DOXYGEN_CIN} ${DOXYGEN_COUT} @ONLY)

    if((DEFINED ENV{GITHUB_ACTION}) AND (${CMAKE_SYSTEM_NAME} MATCHES "Windows"))
    else()
        find_package(Doxygen)
        message("Doxygen build started")
        add_custom_target(gendoc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CPPOUT}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_COUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
        endif()
endif()

####
# Samples generation
####
add_subdirectory(sample)