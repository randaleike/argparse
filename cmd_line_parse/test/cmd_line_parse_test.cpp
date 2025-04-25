/*
 Copyright (c) 2022-2025 Randal Eike

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
 * @file cmd_line_parse_test.cpp
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <cstddef>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "varg_intf_mock.h"
#include "cmd_line_parse_test.h"

//======================================================================================
// Public Interface testing
//======================================================================================

TEST(cmd_line_parse, parseTestFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program", false, false);

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, testvar.parse(2, argv));   // NOLINT
}

TEST(cmd_line_parse, parseTestFlagFailure)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getInvalidAssignmentMessage("-f")).WillOnce(Return("mock \"-f\" invalid assignment"));

    StrictMock<argparser::mock_varg_intf> testflgvarg;

    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f=false";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();

    EXPECT_STREQ("mock \"-f\" invalid assignment\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyWithAssign)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("5")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=5";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, testvar.parse(2, argv));   // NOLINT
}

TEST(cmd_line_parse, parseTestKeyAssignMissing)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getMissingAssignmentMessage("-i")).WillOnce(Return("mock \"-i\" missing assignment value"));

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));  // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\" missing assignment value\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyAssignFail)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "foo"))
        .WillOnce(Return("mock \"-i\", \"foo\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("foo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=foo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"foo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyRangeFail)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "1"))
        .WillOnce(Return("mock \"-i\", \"1\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("1")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e));

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=1";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"1\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyNullPtrFail)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "1"))
        .WillOnce(Return("mock \"-i\", \"1\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("1")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_STORAGE_NULLPTR_e));

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=1";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"1\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyAssignNextArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18";
    parserchar* argv[] = {progname, opt1, val1};
    EXPECT_EQ(3, testvar.parse(3, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parsePositional)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testposvarg;
    EXPECT_CALL(testposvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "42";
    parserchar* argv[] = {progname, opt1};
    EXPECT_EQ(2, testvar.parse(2, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parsePositionalFailed)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("postst", "goo"))
        .WillOnce(Return("mock \"postst\", \"goo\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testposvarg;
    EXPECT_CALL(testposvarg, setValue(::testing::StrEq("goo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "goo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"postst\", \"goo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parsePositionalRangeFailed)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("postst", "3"))
        .WillOnce(Return("mock \"postst\", \"3\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testposvarg;
    EXPECT_CALL(testposvarg, setValue(::testing::StrEq("3")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e));

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "3";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"postst\", \"3\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parsePositionalNullPtrFailed)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("postst", "3"))
        .WillOnce(Return("mock \"postst\", \"3\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testposvarg;
    EXPECT_CALL(testposvarg, setValue(::testing::StrEq("3")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_STORAGE_NULLPTR_e));

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "3";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"postst\", \"3\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestMultiple)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("5")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testposvarg;
    EXPECT_CALL(testposvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar opt2[] = "-i";
    parserchar val2[] = "5";
    parserchar opt3[] = "42";
    parserchar* argv[] = {progname, opt1, opt2, val2, opt3};
    EXPECT_EQ(5, testvar.parse(5, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, addKeyArgInvalidNargs)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getNotListTypeMessage(2)).WillOnce(Return("mock only list type arguments can have an argument count of 2"));

    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    testvar.addKeyArgument(&testvarg, "tstint", "-i,--val", "This is the test key argument", 2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock only list type arguments can have an argument count of 2\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddListArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    EXPECT_EQ(3, testvar.parse(3, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddListArgMultipleArgv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("19")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "19";
    parserchar val2[] = "21";
    parserchar val3[] = "42";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    EXPECT_EQ(5, testvar.parse(5, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddListArgTooFew)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getMissingListAssignmentMessage("-i", 3, 2))
        .WillOnce(Return("mock \"-i\" missing assignment. Expected: 3 found: 2 arguments"));

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddListArgTooMany)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getTooManyAssignmentMessage("-i", 3, 4))
        .WillOnce(Return("Mock \"-i\" too many assignment values. Expected: 3 found: 4 arguments"));

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,42,56";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock \"-i\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddDynamicListArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    EXPECT_EQ(3, testvar.parse(3, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListArg2Vals)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    EXPECT_EQ(3, testvar.parse(3, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListArg2ValsMultiArgv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    EXPECT_EQ(4, testvar.parse(4, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListArgMultiArgvSwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    EXPECT_EQ(5, testvar.parse(5, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1ArgcTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    EXPECT_EQ(4, testvar.parse(4, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1SwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    EXPECT_EQ(5, testvar.parse(5, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1LongSwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("14")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("15")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("16")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "14";
    parserchar val4[] = "15";
    parserchar val5[] = "16";
    parserchar opt2[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3, val4, val5, opt2};
    EXPECT_EQ(8, testvar.parse(8, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1FailAssignment)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "foo"))
        .WillOnce(Return("mock \"-i\", \"foo\" assignment failed"));    //NOLINT

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("foo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "foo";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(5, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"foo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1FailNullPtrAssignment)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "81"))
        .WillOnce(Return("mock \"-i\", \"81\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_STORAGE_NULLPTR_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "67";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(5, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"81\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1FailRangeAssignment)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("-i", "10"))
        .WillOnce(Return("mock \"-i\", \"10\" assignment failed")); //NOLINT

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("1")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("10")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "1";
    parserchar val2[] = "10";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(5, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i\", \"10\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestDualSingleCharFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    testvar.disableHelpDisplayOnError();

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-if";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, testvar.parse(2, argv));    // NOLINT
}

TEST(cmd_line_parse, parseTestTripleSingleCharFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg2;
    EXPECT_CALL(testflgvarg2, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar* argv[] = {progname, opt1};
    EXPECT_EQ(2, testvar.parse(2, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestDualSingleCharFlagVal)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testvalvarg2;
    EXPECT_CALL(testvalvarg2, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg=42";
    parserchar* argv[] = {progname, opt1};
    EXPECT_EQ(2, testvar.parse(2, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestDualSingleCharFlagValNextargv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testvalvarg2;
    EXPECT_CALL(testvalvarg2, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    EXPECT_EQ(3, testvar.parse(3, argv));
    // NOLINTEND
}

TEST(cmd_line_parse, parseTestMissingRequired)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getMissingArgumentMessage("-i|--val"))
        .WillOnce(Return("mock \"-i|--val\" required argument missing"));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testvalvarg2;
    EXPECT_CALL(testvalvarg2, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument", true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-fg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"-i|--val\" required argument missing\n", output.c_str());
}

TEST(cmd_line_parse, parseTestMultiplePositionalArgs)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testsubarg;
    EXPECT_CALL(testsubarg, setValue(::testing::StrEq("foo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    EXPECT_CALL(testflgvarg1, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflgvarg2;
    EXPECT_CALL(testflgvarg2, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testoutname;
    EXPECT_CALL(testoutname, setValue(::testing::StrEq("george")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");
    testvar.addPositionalArgument(&testoutname, "outputname", "This is the test output name argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar opt5[] = "george";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4, opt5};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse
    EXPECT_EQ(argc, testvar.parse(argc, argv));        // NOLINT
}

TEST(cmd_line_parse, parseTestTwoPhasedParse)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testsubarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    StrictMock<argparser::mock_varg_intf> testflgvarg2;

    ::testing::ExpectationSet subcmd = EXPECT_CALL(testsubarg, setValue(::testing::StrEq("foo")))
                                        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testflgvarg, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testflgvarg1, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testflgvarg2, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    EXPECT_EQ(2, testvar.parse(argc, argv, 1, 2));     // NOLINT

    // Phase 2
    EXPECT_EQ(argc, testvar.parse(argc, argv, 2));     // NOLINT
}


TEST(cmd_line_parse, parseTestThreePhasedParse)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testsubarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    StrictMock<argparser::mock_varg_intf> testflgvarg2;
    StrictMock<argparser::mock_varg_intf> testoutname;

    ::testing::ExpectationSet subcmd = EXPECT_CALL(testsubarg, setValue(::testing::StrEq("foo")))
                                        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    ::testing::ExpectationSet flags;
    flags += EXPECT_CALL(testflgvarg, setValue())
                .After(subcmd)
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflgvarg1, setValue())
                .After(subcmd)
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflgvarg2, setValue())
                .After(subcmd)
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testoutname, setValue(::testing::StrEq("george")))
        .After(subcmd, flags)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");
    testvar.addPositionalArgument(&testoutname, "outputname", "This is the test output name argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar opt5[] = "george";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4, opt5};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    EXPECT_EQ(2, testvar.parse(argc, argv, 1, 2));              // NOLINT

    // Phase 2
    EXPECT_EQ(argc-1, testvar.parse(argc, argv, 2, argc-1));    // NOLINT

    // Phase 3
    EXPECT_EQ(argc, testvar.parse(argc, argv, argc-1));         // NOLINT
}

TEST(cmd_line_parse, parseTestTwoPhasedTwoParserPositionalStop)
{
    argparser::cmd_line_parse testvar("testprog [global options] subcommand [subcommand options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    StrictMock<argparser::mock_varg_intf> testflgvarg1;
    StrictMock<argparser::mock_varg_intf> testflgvarg2;
    StrictMock<argparser::mock_varg_intf> testsubarg;

    ::testing::ExpectationSet flags;
    flags += EXPECT_CALL(testflgvarg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflgvarg1, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflgvarg2, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    ::testing::ExpectationSet subcmd = EXPECT_CALL(testsubarg, setValue(::testing::StrEq("cmd1")))
                                        .After(flags)
                                        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar opt2[] = "-f";
    parserchar opt3[] = "-g";
    parserchar opt4[] = "cmd1";
    parserchar opt5[] = "-x";
    parserchar opt6[] = "-y";
    parserchar opt7[] = "13";
    parserchar opt8[] = "-o";
    parserchar opt9[] = "myfile";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4, opt5, opt6, opt7, opt8, opt9};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    testvar.setPositionalNameStop("subcmd");
    EXPECT_EQ(5, testvar.parse(argc, argv));       // NOLINT

    // Phase 2, Parser 2
    argparser::cmd_line_parse cmd1Parser("testprog [global options] cmd1 [subcommand options]", "Description of the subcommand test program");
    StrictMock<argparser::mock_varg_intf> testCmd1flgvarg;
    StrictMock<argparser::mock_varg_intf> testCmd1Valuevarg;
    StrictMock<argparser::mock_varg_intf> testCmd1OutArg;

    EXPECT_CALL(testCmd1flgvarg, setValue())
        .After(flags, subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testCmd1Valuevarg, setValue(::testing::StrEq("13")))
        .After(flags, subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    EXPECT_CALL(testCmd1OutArg, setValue(::testing::StrEq("myfile")))
        .After(flags, subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    cmd1Parser.addFlagArgument(&testCmd1flgvarg, "testCmd1flg", "-x", "This is the cmd1 test flag argument");
    cmd1Parser.addKeyArgument(&testCmd1Valuevarg, "testCmd1Value", "-y", "This is the cmd1 test value argument");
    cmd1Parser.addKeyArgument(&testCmd1OutArg, "testCmd1OutArg", "-o, --output", "This is the cmd1 test output argument");

    EXPECT_EQ(argc, cmd1Parser.parse(argc, argv, 5));      // NOLINT
}

TEST(cmd_line_parse, parseTestMissingRequiredSubcommand)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getMissingArgumentMessage("subcommand"))
        .WillOnce(Return("mock \"subcommand\" required argument missing"));

    StrictMock<argparser::mock_varg_intf> flagArg;
    StrictMock<argparser::mock_varg_intf> subcommand;

    testvar.addFlagArgument(&flagArg, "version", "-V,--version", "Example of a simple true/false flag argument");
    testvar.addPositionalArgument(&subcommand, "subcommand", "Example of a positional argument as subcommand", 1, true); // sub command argument

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar* argv[] = {progname};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();

    EXPECT_EQ(-1, testvar.parse(argc, argv, 1, 2));    // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock \"subcommand\" required argument missing\n", output.c_str());
}

TEST(cmd_line_parse, ArgumentsWithKeyDash)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> flagArg;
    EXPECT_CALL(flagArg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> keyArg;
    EXPECT_CALL(keyArg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(keyArg, setValue(::testing::StrEq("47")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> posArg;
    EXPECT_CALL(posArg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&flagArg, "flag", "-f,--flag", "This is the flag argument");
    testvar.addKeyArgument(&keyArg, "mykey", "-k,--key", "Mykey value help text");
    testvar.addPositionalArgument(&posArg, "myPosArg", "Position value help text");

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar opt[] = "-f";
    parserchar opt1[] = "-k=42";
    parserchar opt3[] = "13";
    parserchar* argv[] = {progname, opt, opt1, opt3};
    int argc = sizeof(argv) / sizeof(argv[0]);
    EXPECT_EQ(argc, testvar.parse(argc, argv));

    parserchar opt11[] = "--flag";
    parserchar opt12[] = "--key=47";
    parserchar opt13[] = "32";
    parserchar* argv2[] = {progname, opt11, opt12, opt13};
    argc = sizeof(argv2) / sizeof(argv2[0]);
    EXPECT_EQ(argc, testvar.parse(argc, argv2));
    // NOLINTEND
}

TEST(cmd_line_parse, ArgumentsWithKeySlash)
{
    parserstr usage = "winutil [options]";
    parserstr description = "test slash keymarker";
    argparser::cmd_line_parse testvar(usage, description);
    testvar.setKeyPrefixCharacter('/');
    testvar.disableSingleCharListArgs();

    StrictMock<argparser::mock_varg_intf> flagArg;
    EXPECT_CALL(flagArg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> keyArg;
    EXPECT_CALL(keyArg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(keyArg, setValue(::testing::StrEq("47")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> posArg;
    EXPECT_CALL(posArg, setValue(::testing::StrEq("13")))
        .Times(1)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addFlagArgument(&flagArg, "flag", "/f,/flag", "This is the flag argument");
    testvar.addKeyArgument(&keyArg, "mykey", "/k,/key", "Mykey value help text");
    testvar.addPositionalArgument(&posArg, "myPosArg", "Position value help text");

    // NOLINTBEGIN
    parserchar progname[] = "winutil";
    parserchar opt[] = "/f";
    parserchar opt1[] = "/k=42";
    parserchar opt3[] = "13";
    parserchar* argv[] = {progname, opt, opt1, opt3};
    int argc = sizeof(argv) / sizeof(argv[0]);
    EXPECT_EQ(argc, testvar.parse(argc, argv));

    parserchar opt11[] = "/flag";
    parserchar opt12[] = "/key=47";
    parserchar opt13[] = "32";
    parserchar* argv2[] = {progname, opt11, opt12, opt13};
    argc = sizeof(argv2) / sizeof(argv2[0]);
    EXPECT_EQ(argc, testvar.parse(argc, argv2));
    // NOLINTEND
}

TEST(cmd_line_parse, AssignPositionalListValue)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> posarg;
    EXPECT_CALL(posarg, isList())
        .WillOnce(Return(true));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("1")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("2")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("3")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("4")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&posarg, "myposition", "List positional argument", 4);

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar posValArg[] = "1,2,3,4";
    parserchar* argv[] = {progname, posValArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    EXPECT_EQ(2, testvar.parse(argc, argv));    // NOLINT
}

TEST(cmd_line_parse, AssignPositionalListValueNeg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> posarg;
    EXPECT_CALL(posarg, isList())
        .WillOnce(Return(true));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("1")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("2")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(posarg, setValue(::testing::StrEq("3")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addPositionalArgument(&posarg, "myposition", "List positional argument", -4);

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar posValArg[] = "1,2,3";
    parserchar* argv[] = {progname, posValArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    EXPECT_EQ(2, testvar.parse(argc, argv));    // NOLINT
}

TEST(cmd_line_parse, AssignPositionalNotListNargsNot1)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getNotListTypeMessage(-4))
        .WillOnce(Return("mock only list type arguments can have an argument count of -4"));

    StrictMock<argparser::mock_varg_intf> posarg;
    EXPECT_CALL(posarg, isList())
        .WillOnce(Return(false));

    testing::internal::CaptureStderr();
    testvar.addPositionalArgument(&posarg, "myposition", "List positional argument", -4);
    parserstr output = testing::internal::GetCapturedStderr();

    EXPECT_STREQ("mock only list type arguments can have an argument count of -4\n", output.c_str());
}

TEST(cmd_line_parse, parsePositionArgWithEmptyList)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getUnknownArgumentMessage("posValue"))
        .WillOnce(Return("mock unknown argument: posValue"));

    // NOLINTBEGIN
    parserchar progname[] = "testprog";
    parserchar posValArg[] = "posValue";
    parserchar* argv[] = {progname, posValArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);
    testvar.disableHelpDisplayOnError();

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, testvar.parse(argc, argv));    // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock unknown argument: posValue\n", output.c_str());
}

/** @} */
