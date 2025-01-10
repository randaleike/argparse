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
#include "parser_string_list.h"
#include "varg_clib.h"
#include "cmd_line_parse_api.h"
#include "../src/cmd_line_parse_api_lib_private.h"
#include "parser_base.h"
#include <gtest/gtest.h>

//==================================================================
// Helper functions
//==================================================================
const size_t defaultArgWidth = 14;
const size_t defaultColWidth = 80;
const size_t testArgWidth    = 17;
const int    testValue       = 10;

parserstr getDefaultUsage_clib(parserstr inputuseage)
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return parserStr->getUsageMessage() + "\n" + inputuseage;
}

parserstr getOptionArgMsg_clib()
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return "\n"+parserStr->getSwitchArgumentsMessage()+"\n";
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
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return "\n"+parserStr->getPositionalArgumentsMessage()+"\n";
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);

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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", true, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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
    cmdLineParserHandle testParser = getParser("myprog [options]", "test description", false, false, 0);
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

#if(0)
TEST(cmd_line_parse_clib, parseTestAddListArgMultipleArgv)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "19";
    parserchar val2[] = "21";
    parserchar val3[] = "42";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(19, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(21, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(42, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddListArgTooFew)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

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
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestAddListArgTooMany)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

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
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(43, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg2Vals)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArg2ValsMultiArgv)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    // NOLINTEND

    EXPECT_EQ(4, parse(testParser, 4, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListArgMultiArgvSwitchTermination)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListNargNeg1ArgcTermination)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.disableHelpDisplayOnError();

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    // NOLINTEND

    EXPECT_EQ(4, parse(testParser, 4, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListNargNeg1SwitchTermination)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListNargNeg1LongSwitchTermination)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
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
    // NOLINTEND

    EXPECT_EQ(8, parse(testParser, 8, argv, 1, -1));      // NOLINT
    EXPECT_EQ(5, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(14, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(15, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(16, testlistvarg.value.front());
}

TEST(cmd_line_parse_clib, parseTestAddDynamicListNargNeg1FailAssignment)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
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
    EXPECT_EQ(-1, parse(testParser, 5, argv, 1, -1));     // NOLINT
    EXPECT_EQ(1, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i foo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestDualSingleCharFlag)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");
    testvar.disableHelpDisplayOnError();

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-if";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));    // NOLINT
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
}

TEST(cmd_line_parse_clib, parseTestTripleSingleCharFlag)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
}

TEST(cmd_line_parse_clib, parseTestDualSingleCharFlagVal)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg=42";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
}

TEST(cmd_line_parse_clib, parseTestDualSingleCharFlagValNextargv)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
}

TEST(cmd_line_parse_clib, parseTestMissingRequired)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument", true);

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-fg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i|--val\" required argument missing\n", output.c_str());
}

TEST(cmd_line_parse_clib, parseTestMultiplePositionalArgs)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::vargstring testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::vargstring testoutname("outname");
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
    EXPECT_EQ(argc, parse(testParser, argc, argv, 1, -1));        // NOLINT
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("george", testoutname.value.c_str());
}

TEST(cmd_line_parse_clib, parseTestTwoPhasedParse)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::vargstring testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
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
    EXPECT_EQ(2, parse(testParser, argc, argv, 1, 2));     // NOLINT
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_FALSE(testflgvarg1.value);
    EXPECT_TRUE(testflgvarg2.value);

    // Phase 2
    testsubarg.value = "phase2";
    EXPECT_EQ(argc, parse(testParser, argc, argv, 2));     // NOLINT
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
}

TEST(cmd_line_parse_clib, parseTestThreePhasedParse)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::vargstring testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::vargstring testoutname("outname");
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
    EXPECT_EQ(2, parse(testParser, argc, argv, 1, 2));     // NOLINT
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_FALSE(testflgvarg1.value);
    EXPECT_TRUE(testflgvarg2.value);
    EXPECT_STREQ("outname", testoutname.value.c_str());

    // Phase 2
    testsubarg.value = "phase2";
    EXPECT_EQ(argc-1, parse(testParser, argc, argv, 2, argc-1));       // NOLINT
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("outname", testoutname.value.c_str());

    // Phase 3
    EXPECT_EQ(argc, parse(testParser, argc, argv, argc-1));            // NOLINT
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("george", testoutname.value.c_str());
}

TEST(cmd_line_parse_clib, parseTestTwPhasedTwoParserPositionalStop)
{
    argparser::cmd_line_parse_clib testvar("testprog [global options] subcommand [subcommand options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::vargstring testsubarg("");
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
    EXPECT_EQ(5, parse(testParser, argc, argv, 1, -1));       // NOLINT
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("cmd1", testsubarg.value.c_str());

    // Phase 2, Parser 2
    argparser::cmd_line_parse_clib cmd1Parser("testprog [global options] cmd1 [subcommand options]", "Description of the subcommand test program");
    argparser::varg<bool> testCmd1flgvarg(false, true);
    cmd1Parser.addFlagArgument(&testCmd1flgvarg, "testCmd1flg", "-x", "This is the cmd1 test flag argument");

    argparser::varg<int> testCmd1Valuevarg(0);
    cmd1Parser.addKeyArgument(&testCmd1Valuevarg, "testCmd1Value", "-y", "This is the cmd1 test value argument");

    argparser::vargstring testCmd1OutArg("");
    cmd1Parser.addKeyArgument(&testCmd1OutArg, "testCmd1OutArg", "-o, --output", "This is the cmd1 test output argument");

    EXPECT_EQ(argc, cmd1Parser.parse(argc, argv, 5));      // NOLINT
    EXPECT_TRUE(testCmd1flgvarg.value);
    EXPECT_EQ(13, testCmd1Valuevarg.value);
    //EXPECT_STREQ("myfile", testCmd1OutArg.value.c_str());
}

TEST(cmd_line_parse_clib, parseTestMissingRequiredSubcommand)
{
    argparser::cmd_line_parse_clib testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool>        flagArg(false, true);          // Default = false, set to true if command line option found
    testvar.addFlagArgument(&flagArg, "version", "-V,--version", "Example of a simple true/false flag argument");

    argparser::vargstring subcommand("none");            // Default = none
    testvar.addPositionalArgument(&subcommand, "subcommand", "Example of a positional argument as subcommand", 1, true); // sub command argument

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar* argv[] = {progname};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();

    EXPECT_EQ(-1, parse(testParser, argc, argv, 1, 2));    // NOLINT
    EXPECT_FALSE(flagArg.value);
    EXPECT_STREQ("none", subcommand.value.c_str());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"subcommand\" required argument missing\n", output.c_str());
}
#endif

/** @} */
