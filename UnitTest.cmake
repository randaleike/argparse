###
# Set the test name from the base name
###
macro (setTestName baseName)
    set (testName ${baseName}_test)
endmacro()

#########################################################################################
#########################################################################################
# Unit test coverage
#########################################################################################
#########################################################################################
function(coverage baseName)
    # Value intialization
    setTestName (${baseName})
    set (objectDir ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${testName}.dir)
    #message ("Setup ${testName} unit test coverage generation in ${objectDir}/coverage")

    # Cleaning coverage
    add_custom_target(${testName}_cleancov
        COMMENT "Cleaning ${testName} coverage files"
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/coverage
        COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/coverage/*
        )

    # Generate coverage reports
    if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
        add_custom_target(${testName}_gencov_report
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/coverage
            COMMENT "Generating ${testName} coverage files"
            COMMAND echo "Generate coverage files to ${CMAKE_CURRENT_BINARY_DIR}/coverage"
            COMMAND gcov -b ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${objectDir}/src/*
            COMMAND echo "Coverage files have been output to ${CMAKE_CURRENT_BINARY_DIR}/coverage"
            COMMAND echo "Capture coverage data to ${CMAKE_CURRENT_BINARY_DIR}/coverage/${testName}-coverage.info"
            COMMAND lcov --capture --directory ${objectDir}/ -o ${testName}-coverage.info
            COMMAND echo "Generate html data to ${CMAKE_CURRENT_BINARY_DIR}/coverage/test-coverage"
            COMMAND genhtml ${testName}-coverage.info --output-directory test-coverage
            COMMAND echo "Coverage report generation complete"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage
            )
    else()
        add_custom_target(${baseName}_gencov_report
            COMMENT "No ${testName} coverage files only exist in Linux Debug builds"
            )
    endif()
endfunction()

#########################################################################################
#########################################################################################
# Unit test build configuration
#########################################################################################
#########################################################################################
function(unittest baseName sourceFileList testFileList mockFileList includePaths externalLibs)
    # Value intialization
    set (testName ${baseName}_test)
    #message ("Setup ${testName} unit test build")

    # Enable testing
    include(CTest)
    include(GoogleTest)
    enable_testing()

    # Add unir test executable
    add_executable(${testName} ${sourceFileList} ${testFileList} ${mockFileList})
    target_include_directories(${testName} PUBLIC ${includePaths} ${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
    target_link_libraries(${testName} PUBLIC ${externalLibs} GTest::gtest_main)
    if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug"))
        target_compile_options(${testName} PRIVATE --coverage)
        target_link_options(${testName} PRIVATE --coverage)
    endif()

    gtest_add_tests(TARGET ${testName} TEST_LIST ${baseName}AllTests)

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set_tests_properties(${${baseName}AllTests} PROPERTIES ENVIRONMENT "PATH=$<SHELL_PATH:$<TARGET_FILE_DIR:gtest>>$<SEMICOLON>$ENV{PATH}")
    endif()

endfunction()