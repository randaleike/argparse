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
 * @file cmd_line_parse_api_test.cpp
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <cmath>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "parser_string_list.h"
#include "varg_clib.h"
#include "cmd_line_parse_api.h"
#include "../src/cmd_line_parse_api_lib_private.h"
#include "parser_base.h"

//==================================================================
// Helper functions
//==================================================================
const size_t defaultArgWidth = 14;
const size_t defaultColWidth = 80;
const size_t testArgWidth    = 17;
const int    testValue       = 10;

parserstr getDefaultUsage_clib(parserstr inputuseage)
{
    argparser::BaseParserStringList parserStr;
    return parserStr.getUsageMessage() + "\n" + inputuseage;
}

parserstr getOptionArgMsg_clib()
{
    argparser::BaseParserStringList parserStr;
    return "\n"+parserStr.getSwitchArgumentsMessage()+"\n";
}

parserstr getOptionMsg_clib(parserstr keys, parserstr keyhelp, size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
{
    parserstr argMsg = " " + keys;
    while (argMsg.size() < argWidth)
    {
        argMsg += ' ';
    }

    parserstr helpmsg = keyhelp;
    while (helpmsg.size() < (consoleWidth - argWidth))
    {
        helpmsg += ' ';
    }

    return argMsg + helpmsg + "\n";
}

parserstr getDefaultHelpMsg_clib(size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
{
    return getOptionMsg_clib("-h,--help,-?", "show this help message and exit", argWidth, consoleWidth);
}

parserstr getEpilogStr_clib(parserstr epilog)
{
    return (epilog.empty() ? "\n" : "\n\n" + epilog + "\n");
}

parserstr getDescriptionStr_clib(parserstr descstr)
{
    return (descstr.empty() ? "" : "\n\n" + descstr + "\n");
}

parserstr getPositionalArgMsg_clib()
{
    argparser::BaseParserStringList parserStr;
    return "\n"+parserStr.getPositionalArgumentsMessage()+"\n";
}

parserstr getPositionalMsg_clib(parserstr name, parserstr help, size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
{
    parserstr argMsg = " " + name;
    while (argMsg.size() < argWidth)
    {
        argMsg += ' ';
    }

    parserstr helpmsg = help;
    while (helpmsg.size() < (consoleWidth - argWidth))
    {
        helpmsg += ' ';
    }

    return argMsg + helpmsg + "\n";
}

//==================================================================
// Basic test
//==================================================================

//======================================================================================
// Public Interface testing, English
//======================================================================================
TEST(cmd_line_parse_clib, defaultConstructor)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description",
                                               '-', true, false,
                                               0);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
}

TEST(cmd_line_parse_clib, test_epilog)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description",
                                               '-',true, false,
                                               0);
    setEpilog(testParser, "This is the epilog");

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getEpilogStr_clib("This is the epilog");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
}

TEST(cmd_line_parse_clib, addFlagArgHelp)
{
    bool testFlag = false;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', true, false, 0);
    argHandle           cvarghandle = createParserFlagArg(&testFlag, true);
    addFlagArgument(testParser, cvarghandle, "flag", "-f,--flag", "This is the flag argument", false);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getOptionMsg_clib("-f,--flag", "This is the flag argument");
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvarghandle);
}

TEST(cmd_line_parse_clib, addPositionalHelp)
{
    int testarg = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', true, false, 0);
    argHandle           cvarghandle = createIntParserArg(&testarg);

    addPositionalArgument(testParser, cvarghandle, "postst", "This is a positional argument", true);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getPositionalArgMsg_clib();
    expectedStr += getPositionalMsg_clib("postst", "This is a positional argument");
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvarghandle);
}

TEST(cmd_line_parse_clib, addKeyArgHelp)
{
    int testarg = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', true, false, 0);
    argHandle           cvarghandle = createIntParserArg(&testarg);

    addKeyArgument(testParser, cvarghandle, "tstint", "-i,--val", "This is the test key argument", true);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib(testArgWidth);
    expectedStr += getOptionMsg_clib("-i,--val=tstint", "This is the test key argument", testArgWidth);
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvarghandle);
}

TEST(cmd_line_parse_clib, addAllArgHelp)
{
    bool testFlagArg = false;
    int  testKeyInt = 1;
    int  testPositionInt = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', true, false, 0);
    argHandle           cvargFlagHandle = createParserFlagArg(&testFlagArg, true);
    argHandle           cvargKeyIntHandle = createIntParserArg(&testKeyInt);
    argHandle           cvargPosIntHandle = createIntParserArg(&testPositionInt);

    addPositionalArgument(testParser, cvargPosIntHandle, "postst", "This is a positional argument", true);
    addFlagArgument(testParser, cvargFlagHandle, "flag", "-f,--flag", "This is the flag argument", false);
    addFlagArgument(testParser, cvargKeyIntHandle, "tstint", "-i,--val", "This is the test key argument", true);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("myprog [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getOptionMsg_clib("-f,--flag", "This is the flag argument");
    expectedStr += getOptionMsg_clib("-i,--val", "This is the test key argument");
    expectedStr += getPositionalArgMsg_clib();
    expectedStr += getPositionalMsg_clib("postst", "This is a positional argument");
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvargFlagHandle);
    releaseParserArg(cvargKeyIntHandle);
    releaseParserArg(cvargPosIntHandle);
}

TEST(cmd_line_parse_clib, parseTestFlag)
{
    bool testFlagArg = false;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargFlagHandle = createParserFlagArg(&testFlagArg, true);

    addFlagArgument(testParser, cvargFlagHandle, "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_FALSE(testFlagArg);
    EXPECT_EQ(2, parse(testParser, 2, argv, 0, -1));   // NOLINT
    EXPECT_TRUE(testFlagArg);

    releaseParser(testParser);
    releaseParserArg(cvargFlagHandle);
}

TEST(cmd_line_parse_clib, parseTestFlagFailure)
{
    bool testFlagArg = false;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargFlagHandle = createParserFlagArg(&testFlagArg, true);

    addFlagArgument(testParser, cvargFlagHandle, "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f=false";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));   // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_FALSE(testFlagArg);
    EXPECT_STREQ("\"-f\" invalid assignment\n", output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvargFlagHandle);

}

TEST(cmd_line_parse_clib, parseTestKeyWithAssign)
{
    int testArg = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addKeyArgument(testParser, cvargIntHandle, "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=5";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 0, -1));   // NOLINT
    EXPECT_EQ(5, testArg);

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parseTestKeyAssignMissing)
{
    int testArg = 10;   // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addKeyArgument(testParser, cvargIntHandle, "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));  // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(10, testArg);
    EXPECT_STREQ("\"-i\" missing assignment value\n", output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parseTestKeyAssignFail)
{
    int testArg = 10;   // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addKeyArgument(testParser, cvargIntHandle, "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=foo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));  // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(10, testArg);
    EXPECT_STREQ("\"-i foo\" assignment failed\n", output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parseTestKeyAssignNextArg)
{
    int testArg = 10;   // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addKeyArgument(testParser, cvargIntHandle, "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_EQ(18, testArg);

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parsePositional)
{
    int testArg = 10;   // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addPositionalArgument(testParser, cvargIntHandle, "postst", "This is a positional argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "42";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
    EXPECT_EQ(42, testArg);

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parsePositionalFailed)
{
    int testArg = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargIntHandle = createIntParserArg(&testArg);

    addPositionalArgument(testParser, cvargIntHandle, "postst", "This is a positional argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "goo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 1, -1));     // NOLINT

    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(0, testArg);
    EXPECT_STREQ("\"postst goo\" assignment failed\n", output.c_str());

    releaseParser(testParser);
    releaseParserArg(cvargIntHandle);
}

TEST(cmd_line_parse_clib, parseTestMultiple)
{
    bool testFlagArg = false;
    int testKeyArg = testValue;
    int testPosArg = 0;
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           cvargFlagHandle = createParserFlagArg(&testFlagArg, true);
    argHandle           cvargKeyHandle = createIntParserArg(&testKeyArg);
    argHandle           cvargPosHandle = createIntParserArg(&testPosArg);

    addPositionalArgument(testParser, cvargPosHandle, "postst", "This is a positional argument", true);
    addFlagArgument(testParser, cvargFlagHandle, "flag", "-f,--flag", "This is the flag argument", false);
    addKeyArgument(testParser, cvargKeyHandle, "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar opt2[] = "-i";
    parserchar val2[] = "5";
    parserchar opt3[] = "42";
    parserchar* argv[] = {progname, opt1, opt2, val2, opt3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testFlagArg);
    EXPECT_EQ(42, testPosArg);
    EXPECT_EQ(5, testKeyArg);

    releaseParser(testParser);
    releaseParserArg(cvargPosHandle);
    releaseParserArg(cvargKeyHandle);
    releaseParserArg(cvargFlagHandle);
}

TEST(cmd_line_parse_clib, parseTestAddListArg)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3, true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));          // NOLINT
    EXPECT_EQ(3, getAssignmentCount(testlistvarg));
    EXPECT_EQ(18, testArg[0]);
    EXPECT_EQ(22, testArg[1]);
    EXPECT_EQ(43, testArg[2]);
}

TEST(cmd_line_parse_clib, parseTestAddListArgMultipleArgv)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "19";
    parserchar val2[] = "21";
    parserchar val3[] = "42";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_EQ(3, getAssignmentCount(testlistvarg));
    EXPECT_EQ(19, testArg[0]);
    EXPECT_EQ(21, testArg[1]);
    EXPECT_EQ(42, testArg[2]);
}

TEST(cmd_line_parse_clib, parseTestAddListArgTooFew)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_EQ(0, getAssignmentCount(testlistvarg));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestAddListArgTooMany)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,42,56";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_EQ(0, getAssignmentCount(testlistvarg));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_EQ(3, getAssignmentCount(testlistvarg));
    EXPECT_EQ(18, testArg[0]);
    EXPECT_EQ(22, testArg[1]);
    EXPECT_EQ(43, testArg[2]);
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg2Vals)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, getAssignmentCount(testlistvarg));
    EXPECT_EQ(18, testArg[0]);
    EXPECT_EQ(22, testArg[1]);
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg2ValsMultiArgv)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    // NOLINTEND

    EXPECT_EQ(4, parse(testParser, 4, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, getAssignmentCount(testlistvarg));
    EXPECT_EQ(81, testArg[0]);
    EXPECT_EQ(13, testArg[1]);
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArgMultiArgvSwitchTermination)
{
    int testArg[3] = {0,0,0}; // NOLINT
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    argHandle           testlistvarg = createIntArrayParserArg(&(testArg[0]), 3);

    addKeyArrayArgument(testParser, testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3, false);

    bool      testFlag    = false;
    argHandle testflgvarg = createParserFlagArg(&testFlag, true);
    addFlagArgument(testParser, testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument",false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, getAssignmentCount(testlistvarg));
    EXPECT_EQ(81, testArg[0]);
    EXPECT_EQ(13, testArg[1]);
    EXPECT_TRUE(testFlag);
}


TEST(cmd_line_parse_clib, parseTestDualSingleCharFlag)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    disableHelpDisplayOnError(testParser);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser, testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-if";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));    // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
}

TEST(cmd_line_parse_clib, parseTestTripleSingleCharFlag)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    bool      testFlag2    = true;
    argHandle testflgvarg2 = createParserFlagArg(&testFlag2, false);
    addFlagArgument(testParser,testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
}

TEST(cmd_line_parse_clib, parseTestDualSingleCharFlagVal)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    int       testval2 = 0;
    argHandle testvalvarg2 = createIntParserArg(&testval2);
    addKeyArgument(testParser, testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg=42";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_EQ(42, testval2);
}

TEST(cmd_line_parse_clib, parseTestDualSingleCharFlagValNextargv)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    int       testval2 = 0;
    argHandle testvalvarg2 = createIntParserArg(&testval2);
    addKeyArgument(testParser, testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_EQ(42, testval2);
}

TEST(cmd_line_parse_clib, parseTestMissingRequired)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    disableHelpDisplayOnError(testParser);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", true);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    int       testval2 = 0;
    argHandle testvalvarg2 = createIntParserArg(&testval2);
    addKeyArgument(testParser, testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-fg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_FALSE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_EQ(42, testval2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i|--val\" required argument missing\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestMultiplePositionalArgs)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    char subcommand[30];    // NOLINT
    argHandle testsubarg = createStringArrayParserArg(subcommand, 30);  // NOLINT
    addPositionalArgument(testParser, testsubarg, "subcmd", "This is the test subcmd argument", true);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    bool      testFlag2    = true;
    argHandle testflgvarg2 = createParserFlagArg(&testFlag2, false);
    addFlagArgument(testParser,testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    char outname[100];  // NOLINT
    argHandle testoutname = createStringArrayParserArg(outname, 100);    // NOLINT
    addPositionalArgument(testParser, testoutname, "outputname", "This is the test output name argument", false);

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
    EXPECT_EQ(argc, parse(testParser, argc, argv, 1, -1));  // NOLINT
    EXPECT_STREQ("foo", subcommand);                        // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
    EXPECT_STREQ("george", outname);                        // NOLINT
}

TEST(cmd_line_parse_clib, parseTestTwoPhasedParse)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    char subcommand[30];    // NOLINT
    argHandle testsubarg = createStringArrayParserArg(subcommand, 30);  // NOLINT
    addPositionalArgument(testParser, testsubarg, "subcmd", "This is the test subcmd argument", true);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    bool      testFlag2    = true;
    argHandle testflgvarg2 = createParserFlagArg(&testFlag2, false);
    addFlagArgument(testParser,testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

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
    EXPECT_EQ(2, parse(testParser, argc, argv, 1, 2));  // NOLINT
    EXPECT_STREQ("foo", subcommand);                    // NOLINT
    EXPECT_FALSE(testFlag0);
    EXPECT_FALSE(testFlag1);
    EXPECT_TRUE(testFlag2);

    // Phase 2
    EXPECT_EQ(argc, parse(testParser, argc, argv, 2, -1));  // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
}

TEST(cmd_line_parse_clib, parseTestThreePhasedParse)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    char subcommand[30];    // NOLINT
    argHandle testsubarg = createStringArrayParserArg(subcommand, 30);  // NOLINT
    addPositionalArgument(testParser, testsubarg, "subcmd", "This is the test subcmd argument", true);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    bool      testFlag2    = true;
    argHandle testflgvarg2 = createParserFlagArg(&testFlag2, false);
    addFlagArgument(testParser,testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    char outname[100] = {'\0'}; // NOLINT
    argHandle testoutname = createStringArrayParserArg(outname, 100);    // NOLINT
    addPositionalArgument(testParser, testoutname, "outputname", "This is the test output name argument", false);

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
    EXPECT_EQ(2, parse(testParser, argc, argv, 1, 2));  // NOLINT
    EXPECT_STREQ("foo", subcommand);                    // NOLINT
    EXPECT_FALSE(testFlag0);
    EXPECT_FALSE(testFlag1);
    EXPECT_TRUE(testFlag2);
    EXPECT_STREQ("", outname);                          // NOLINT

    // Phase 2
    EXPECT_EQ(argc-1, parse(testParser, argc, argv, 2, argc-1));    // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
    EXPECT_STREQ("", outname);                                      // NOLINT

    // Phase 3
    EXPECT_EQ(argc, parse(testParser, argc, argv, argc-1, -1));     // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
    EXPECT_STREQ("george", outname);                                // NOLINT
}

TEST(cmd_line_parse_clib, parseTestTwPhasedTwoParserPositionalStop)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);

    bool      testFlag0    = false;
    argHandle testflgvarg0 = createParserFlagArg(&testFlag0, true);
    addFlagArgument(testParser,testflgvarg0, "tstflg", "-i,--val", "This is the test flag0 argument", false);

    bool      testFlag1    = false;
    argHandle testflgvarg1 = createParserFlagArg(&testFlag1, true);
    addFlagArgument(testParser,testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    bool      testFlag2    = true;
    argHandle testflgvarg2 = createParserFlagArg(&testFlag2, false);
    addFlagArgument(testParser,testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    char subcommand[30];    // NOLINT
    argHandle testsubarg = createStringArrayParserArg(subcommand, 30);  // NOLINT
    addPositionalArgument(testParser, testsubarg, "subcmd", "This is the test subcmd argument", true);

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
    setPositionalNameStop(testParser, "subcmd");
    EXPECT_EQ(5, parse(testParser, argc, argv, 1, -1));       // NOLINT
    EXPECT_TRUE(testFlag0);
    EXPECT_TRUE(testFlag1);
    EXPECT_FALSE(testFlag2);
    EXPECT_STREQ("cmd1", subcommand);   // NOLINT

    // Phase 2, Parser 2
    cmdLineParserHandle cmd1Parser = getParser("testprog [global options] cmd1 [subcommand options]",
                                               "Description of the subcommand test program", '-',
                                               false, false, 0);

    bool      testCmd1Flag    = false;
    unsigned  testCmd1Value   = 0;
    char      testCmd1OutStr[30] = {'\0'};  // NOLINT

    argHandle testCmd1flgvarg = createParserFlagArg(&testCmd1Flag, true);
    argHandle testCmd1Valuevarg = createUIntParserArg(&testCmd1Value);
    argHandle testCmd1OutArg = createStringArrayParserArg(testCmd1OutStr, 30);  // NOLINT

    addFlagArgument(cmd1Parser, testCmd1flgvarg, "testCmd1flg", "-x", "This is the cmd1 test flag argument", false);
    addKeyArgument(cmd1Parser, testCmd1Valuevarg, "testCmd1Value", "-y", "This is the cmd1 test value argument", false);
    addKeyArgument(cmd1Parser, testCmd1OutArg, "testCmd1OutArg", "-o, --output", "This is the cmd1 test output argument", false);

    EXPECT_EQ(argc, parse(cmd1Parser, argc, argv, 5, -1));      // NOLINT
    EXPECT_TRUE(testCmd1Flag);
    EXPECT_EQ(13, testCmd1Value);
    EXPECT_STREQ("myfile", testCmd1OutStr);                     // NOLINT
}

TEST(cmd_line_parse_clib, parseTestMissingRequiredSubcommand)
{
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", '-', false, false, 0);
    disableHelpDisplayOnError(testParser);

    bool      testFlag          = false;
    char      subcommandStr[30] = {'n', 'o', 'n', 'e', '\0'};  // NOLINT

    argHandle testflgvarg = createParserFlagArg(&testFlag, true);
    argHandle subcommand = createStringArrayParserArg(subcommandStr, 30);   // NOLINT
    addFlagArgument(testParser, testflgvarg, "version", "-V,--version", "Example of a simple true/false flag argument", false);
    addPositionalArgument(testParser, subcommand, "subcommand", "Example of a positional argument as subcommand", true); // sub command argument

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar* argv[] = {progname};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    testing::internal::CaptureStderr();

    EXPECT_EQ(-1, parse(testParser, argc, argv, 1, 2));     // NOLINT
    EXPECT_FALSE(testFlag);
    EXPECT_STREQ("none", subcommandStr);                    // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"subcommand\" required argument missing\n", output.c_str());
}

TEST(cmd_line_parse_clib, TestSetProgramName)
{
    cmdLineParserHandle testParser = getParser(nullptr, "test description", '-', false, false, 0);
    disableHelpDisplayOnError(testParser);
    setProgramName(testParser, "progName");

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = getDefaultUsage_clib("progName [options]");
    expectedStr += getDescriptionStr_clib("test description");
    expectedStr += getOptionArgMsg_clib();
    expectedStr += getDefaultHelpMsg_clib();
    expectedStr += getEpilogStr_clib("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
}

TEST(cmd_line_parse_clib, enableUnknownArgIgnore)
{
/*
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

     flagArg;
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
    */
}

/** @} */
