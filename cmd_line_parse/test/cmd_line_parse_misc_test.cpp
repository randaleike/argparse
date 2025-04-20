/*
 Copyright (c) 2022-2024 Randal Eike

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * @file cmd_line_parse_misc_test.cpp
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <cstddef>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <parser_base.h>
#include "varg_intf_mock.h"
#include "parser_string_list.h"
#include "cmd_line_parse.h"
#include "mock_ParserStringListInterface.h"

const size_t defaultArgWidth = 14;
const size_t defaultColWidth = 80;
const size_t testArgWidth    = 17;
const int    testValue       = 10;

using ::testing::StrictMock;
using ::testing::Return;

TEST(cmd_line_parse, SetPrognameTestStr)
{
    argparser::cmd_line_parse testvar;
    parserstr progname = "clprogname";
    EXPECT_TRUE(testvar.setProgramName(progname));
}

TEST(cmd_line_parse, SetPrognameTestCharPtr)
{
    argparser::cmd_line_parse testvar;
    EXPECT_TRUE(testvar.setProgramName("clprogchar"));
}

TEST(cmd_line_parse, SetPrognameFailTestStr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    parserstr progname = "clprogname";
    EXPECT_FALSE(testvar.setProgramName(progname));
}

TEST(cmd_line_parse, SetPrognameFailTestCharPtr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    EXPECT_FALSE(testvar.setProgramName("clprogchar"));
}

TEST(cmd_line_parse, enableUnknownArgIgnore)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> flagArg;
    EXPECT_CALL(flagArg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> keyarg;
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&flagArg, "flag", "-f,--flag", "Example of a simple true/false flag argument");
    testvar.addKeyArgument(&keyarg, "key", "-v,--value", "Example of a key argument", 1, true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar vargflg[] = "-f";
    parserchar posArg[] = "-v=15";
    parserchar unknownArg[] = "--test";
    parserchar* argv[] = {progname, unknownArg, vargflg, posArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Unknown argument --test\n", output.c_str());

    testvar.enableUnknowArgumentIgnore();
    EXPECT_EQ(4, testvar.parse(argc, argv));    // NOLINT
}

TEST(cmd_line_parse, AssignKeyFlagValue_testDebugStr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program",
                                      false, true,
                                      debugVerbosityLevel_e::veryVerboseDebug);

    StrictMock<argparser::mock_varg_intf> keyarg;
    EXPECT_CALL(keyarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    testvar.addFlagArgument(&keyarg, "flag", "-f,--flag", "Example of a flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar keyArg[] = "-f";
    parserchar* argv[] = {progname, keyArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr erroutput = testing::internal::GetCapturedStderr();

    EXPECT_STREQ("\"-f \" assignment failed\n", erroutput.c_str());
#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Parsing key arg: -f\n";
    expected += "match found, name = flag\n";
    expected += "match found, nargs = 0\n";
    expected += "cmd_line_parse::assignKeyFlagValue return status: 1\n";
    expected += "Parsing error status = 1\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif //(ENABLE_DEBUG_STRING_CHECK)
}

TEST(cmd_line_parse, AssignKeyListValue_testDebugStr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program",
                                      false, true,
                                      debugVerbosityLevel_e::veryVerboseDebug);

    StrictMock<argparser::mock_varg_intf> keyarg;
    EXPECT_CALL(keyarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("12")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("5")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&keyarg, "goober", "-f,--flag", "Example of a flag argument", 3);

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar keyArg[] = "-f";
    parserchar keyVal1Arg[] = "15";
    parserchar keyVal2Arg[] = "12";
    parserchar keyVal3Arg[] = "5";
    parserchar* argv[] = {progname, keyArg, keyVal1Arg, keyVal2Arg, keyVal3Arg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStdout();
    EXPECT_EQ(5, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStdout();

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Parsing key arg: -f\n";
    expected += "match found, name = goober\n";
    expected += "match found, nargs = 3\n";
    expected += "Initial value string: \n";
    expected += "Value string post get: \n";
    expected += "Initial value count: 1 Required Count(abs): 3 Narg: 3\n";
    expected += "Current arg index: 3 of 5\n";
    expected += "Next value string: 12\n";
    expected += "Next addCount: 1\n";
    expected += "Next value string: 5\n";
    expected += "Next addCount: 1\n";
    expected += "Assignment string count: 3\n";
    expected += "Parsing error status = 0\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif //(ENABLE_DEBUG_STRING_CHECK)
}

TEST(cmd_line_parse, AssignKeyValue_testDebugStr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program",
                                      false, true,
                                      debugVerbosityLevel_e::veryVerboseDebug);

    StrictMock<argparser::mock_varg_intf> keyarg;
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&keyarg, "goober", "-f,--flag", "Example of a flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar keyArg[] = "-f=15";
    parserchar* argv[] = {progname, keyArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStdout();
    EXPECT_EQ(2, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStdout();

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Parsing key arg: -f\n";
    expected += "Value string: 15\n";
    expected += "match found, name = goober\n";
    expected += "match found, nargs = 1\n";
    expected += "Initial value string: 15\n";
    expected += "Value string post get: 15\n";
    expected += "Initial value count: 1 Required Count(abs): 1 Narg: 1\n";
    expected += "Current arg index: 2 of 2\n";
    expected += "Assignment string count: 1\n";
    expected += "Parsing error status = 0\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif //(ENABLE_DEBUG_STRING_CHECK)
}

TEST(cmd_line_parse, AssignPositionalValue_testDebugStr)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program",
                                      false, true,
                                      debugVerbosityLevel_e::veryVerboseDebug);

    StrictMock<argparser::mock_varg_intf> posarg;
    EXPECT_CALL(posarg, setValue(::testing::StrEq("posValue")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&posarg, "myposition", "Goober positional argument");

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar posValArg[] = "posValue";
    parserchar* argv[] = {progname, posValArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStdout();
    EXPECT_EQ(2, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStdout();

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Positional Argument Name: myposition, position: 1\n";
    expected += "Current Parsing Position: 1\n";
    expected += "Initial value string: posValue\n";
    expected += "Value string post get: posValue\n";
    expected += "Initial value count: 1 Required Count(abs): 1 Narg: 1\n";
    expected += "Current arg index: 2 of 2\n";
    expected += "Assignment string count: 1\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif //(ENABLE_DEBUG_STRING_CHECK)
}

/** @} */
