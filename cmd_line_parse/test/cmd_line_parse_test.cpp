/*
Copyright (c) 2022 Randal Eike

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
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg.h"
#include "listvarg.h"
#include "parser_string_list.h"
#include "cmd_line_parse.h"
#include <gtest/gtest.h>

//======================================================================================
// String Helper functions
//======================================================================================
parserstr getDefaultUsage(parserstr inputuseage = "%(prog) [options]")
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return parserStr->getUsageMessage() + "\n" + inputuseage;
}

parserstr getOptionArgMsg()
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return "\n"+parserStr->getSwitchArgumentsMessage()+"\n";
}

parserstr getOptionMsg(parserstr keys, parserstr keyhelp, size_t argWidth = 14, size_t consoleWidth = 80)
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

parserstr getDefaultHelpMsg(size_t argWidth = 14, size_t consoleWidth = 80)
{
    return getOptionMsg("-h,--help,-?", "show this help message and exit", argWidth, consoleWidth);
}

parserstr getEpilogStr(parserstr epilog)
{
    if (epilog.empty()) return "\n";
    else return "\n\n" + epilog + "\n";
}

parserstr getDescriptionStr(parserstr descstr)
{
    if (descstr.empty()) return "";
    else return "\n\n" + descstr + "\n";
}


parserstr getPositionalArgMsg()
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return "\n"+parserStr->getPositionalArgumentsMessage()+"\n";
}

parserstr getPositionalMsg(parserstr name, parserstr help, size_t argWidth = 14, size_t consoleWidth = 80)
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

//======================================================================================
// Public Interface testing, English
//======================================================================================
TEST(cmd_line_parse, defaultConstructor)
{
    argparser::cmd_line_parse testvar;
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage() + getOptionArgMsg() + getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, test_epilog)
{
    argparser::cmd_line_parse testvar;
    testvar.setEpilog("This is the epilog");
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage() + getOptionArgMsg() + getDefaultHelpMsg() + getEpilogStr("This is the epilog");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, normalConstructor)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addFlagArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<bool> testvarg(false, true);
    testvar.addFlagArgument(&testvarg, "flag", "-f,--flag", "This is the flag argument");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getOptionMsg("-f,--flag", "This is the flag argument") + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addPositionalHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testvarg(0);
    testvar.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addKeyArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testvarg(10);
    testvar.addKeyArgument(&testvarg, "tstint", "-i,--val", "This is the test key argument", 1);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg(17) + getOptionMsg("-i,--val=tstint", "This is the test key argument", 17) + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addAllArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<bool> testflgvarg(false, true);
    argparser::varg<int> testkeyvarg(10);
    argparser::varg<int> testposvarg(0);

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.addFlagArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getOptionMsg("-f,--flag", "This is the flag argument") +
                            getOptionMsg("-i,--val", "This is the test key argument") +
                            getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") +
                            getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, parseTestFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar* argv[] = {progname, opt1};

    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_TRUE(testflgvarg.value);
}

TEST(cmd_line_parse, parseTestFlagFailure)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f=false";
    parserchar* argv[] = {progname, opt1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_STREQ("\"-f\" invalid assignment\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyWithAssign)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testkeyvarg(10);

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=5";
    parserchar* argv[] = {progname, opt1};

    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_EQ(5, testkeyvarg.value);
}

TEST(cmd_line_parse, parseTestKeyAssignMissing)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testkeyvarg(10);

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar* argv[] = {progname, opt1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(10, testkeyvarg.value);
    EXPECT_STREQ("\"-i\" missing assignment value\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyAssignFail)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testkeyvarg(10);

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=foo";
    parserchar* argv[] = {progname, opt1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(10, testkeyvarg.value);
    EXPECT_STREQ("\"-i foo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestKeyAssignNextArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testkeyvarg(10);

    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18";
    parserchar* argv[] = {progname, opt1, val1};

    EXPECT_EQ(3, testvar.parse(3, argv));
    EXPECT_EQ(18, testkeyvarg.value);
}

TEST(cmd_line_parse, parsePositional)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testposvarg(0);

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "42";
    parserchar* argv[] = {progname, opt1};

    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_EQ(42, testposvarg.value);
}

TEST(cmd_line_parse, parsePositionalFailed)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testposvarg(0);

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "goo";
    parserchar* argv[] = {progname, opt1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(2, argv));
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(0, testposvarg.value);
    EXPECT_STREQ("\"postst goo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestMultiple)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<bool> testflgvarg(false, true);
    argparser::varg<int> testkeyvarg(10);
    argparser::varg<int> testposvarg(0);

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument", 1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar opt2[] = "-i";
    parserchar val2[] = "5";
    parserchar opt3[] = "42";
    parserchar* argv[] = {progname, opt1, opt2, val2, opt3};

    EXPECT_EQ(5, testvar.parse(5, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_EQ(42, testposvarg.value);
    EXPECT_EQ(5, testkeyvarg.value);
}

TEST(cmd_line_parse, addKeyArgInvalidNargs)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::varg<int> testvarg(10);

    testing::internal::CaptureStderr();
    testvar.addKeyArgument(&testvarg, "tstint", "-i,--val", "This is the test key argument", 2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Only list type arguments can have an argument count of 2\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddListArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};

    EXPECT_EQ(3, testvar.parse(3, argv));
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(43, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddListArgMultipleArgv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "19";
    parserchar val2[] = "21";
    parserchar val3[] = "42";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};

    EXPECT_EQ(5, testvar.parse(5, argv));
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(19, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(21, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(42, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddListArgTooFew)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddListArgTooMany)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", 3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,42,56";
    parserchar* argv[] = {progname, opt1, val1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
}

TEST(cmd_line_parse, parseTestAddDynamicListArg)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};

    EXPECT_EQ(3, testvar.parse(3, argv));
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(43, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListArg2Vals)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};

    EXPECT_EQ(3, testvar.parse(3, argv));
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListArg2ValsMultiArgv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};

    EXPECT_EQ(4, testvar.parse(4, argv));
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListArgMultiArgvSwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -3);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};

    EXPECT_EQ(5, testvar.parse(5, argv));
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1ArgcTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    argparser::listvarg<int> testlistvarg;

    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);
    testvar.disableHelpDisplayOnError();

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};

    EXPECT_EQ(4, testvar.parse(4, argv));
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1SwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");


    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};

    EXPECT_EQ(5, testvar.parse(5, argv));
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
}

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1LongSwitchTermination)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");


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

TEST(cmd_line_parse, parseTestAddDynamicListNargNeg1FailAssignment)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::listvarg<int> testlistvarg;
    testvar.addKeyArgument(&testlistvarg, "tstint", "-i,--val", "This is the test key argument", -1);

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-f,--flag", "This is the test flag argument");


    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "foo";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(5, argv));
    EXPECT_EQ(1, testlistvarg.value.size());
    EXPECT_EQ(81, testlistvarg.value.front());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i foo\" assignment failed\n", output.c_str());
}

TEST(cmd_line_parse, parseTestDualSingleCharFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-if";
    parserchar* argv[] = {progname, opt1};

    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
}

TEST(cmd_line_parse, parseTestTripleSingleCharFlag)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar* argv[] = {progname, opt1};
    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
}

TEST(cmd_line_parse, parseTestDualSingleCharFlagVal)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg=42";
    parserchar* argv[] = {progname, opt1};

    EXPECT_EQ(2, testvar.parse(2, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
}

TEST(cmd_line_parse, parseTestDualSingleCharFlagValNextargv)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};

    EXPECT_EQ(3, testvar.parse(3, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
}

TEST(cmd_line_parse, parseTestMissingRequired)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument", true);

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<int> testvalvarg2(0);
    testvar.addKeyArgument(&testvalvarg2, "tstval", "-g,--goo", "This is the test value argument", 1);

    parserchar progname[] = "runprog";
    parserchar opt1[] = "-fg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};

    testing::internal::CaptureStderr();
    testvar.disableHelpDisplayOnError();
    EXPECT_EQ(-1, testvar.parse(3, argv));
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_EQ(42, testvalvarg2.value);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i|--val\" required argument missing\n", output.c_str());

}

TEST(cmd_line_parse, parseTestMultiplePositionalArgs)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<std::string> testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::varg<std::string> testoutname("outname");
    testvar.addPositionalArgument(&testoutname, "outputname", "This is the test output name argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar opt5[] = "george";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4, opt5};
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Parse
    EXPECT_EQ(argc, testvar.parse(argc, argv));
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("george", testoutname.value.c_str());
}

TEST(cmd_line_parse, parseTestTwoPhasedParse)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<std::string> testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4};
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    EXPECT_EQ(2, testvar.parse(argc, argv, 1, 2));
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_FALSE(testflgvarg1.value);
    EXPECT_TRUE(testflgvarg2.value);

    // Phase 2
    testsubarg.value = "phase2";
    EXPECT_EQ(argc, testvar.parse(argc, argv, 2));
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
}

TEST(cmd_line_parse, parseTestThreePhasedParse)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    argparser::varg<std::string> testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::varg<std::string> testoutname("outname");
    testvar.addPositionalArgument(&testoutname, "outputname", "This is the test output name argument");

    parserchar progname[] = "runprog";
    parserchar opt1[] = "foo";
    parserchar opt2[] = "-i";
    parserchar opt3[] = "-f";
    parserchar opt4[] = "-g";
    parserchar opt5[] = "george";
    parserchar* argv[] = {progname, opt1, opt2, opt3, opt4, opt5};
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    EXPECT_EQ(2, testvar.parse(argc, argv, 1, 2));
    EXPECT_STREQ("foo", testsubarg.value.c_str());
    EXPECT_FALSE(testflgvarg.value);
    EXPECT_FALSE(testflgvarg1.value);
    EXPECT_TRUE(testflgvarg2.value);
    EXPECT_STREQ("outname", testoutname.value.c_str());

    // Phase 2
    testsubarg.value = "phase2";
    EXPECT_EQ(argc-1, testvar.parse(argc, argv, 2, argc-1));
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("outname", testoutname.value.c_str());

    // Phase 3
    EXPECT_EQ(argc, testvar.parse(argc, argv, argc-1));
    EXPECT_STREQ("phase2", testsubarg.value.c_str());
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("george", testoutname.value.c_str());
}

TEST(cmd_line_parse, parseTestTwPhasedTwoParserPositionalStop)
{
    argparser::cmd_line_parse testvar("testprog [global options] subcommand [subcommand options]", "Description of the test program");

    argparser::varg<bool> testflgvarg(false, true);
    testvar.addFlagArgument(&testflgvarg, "tstflg", "-i,--val", "This is the test flag0 argument");

    argparser::varg<bool> testflgvarg1(false, true);
    testvar.addFlagArgument(&testflgvarg1, "tstflg1", "-f,--flag", "This is the test flag1 argument");

    argparser::varg<bool> testflgvarg2(true, false);
    testvar.addFlagArgument(&testflgvarg2, "tstflg2", "-g,--goo", "This is the test flag2 argument");

    argparser::varg<std::string> testsubarg("");
    testvar.addPositionalArgument(&testsubarg, "subcmd", "This is the test subcmd argument");


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
    int argc = sizeof(argv) / sizeof(argv[0]);

    // Phase 1
    testvar.setPositionalNameStop("subcmd");
    EXPECT_EQ(5, testvar.parse(argc, argv));
    EXPECT_TRUE(testflgvarg.value);
    EXPECT_TRUE(testflgvarg1.value);
    EXPECT_FALSE(testflgvarg2.value);
    EXPECT_STREQ("cmd1", testsubarg.value.c_str());

    // Phase 2, Parser 2
    argparser::cmd_line_parse cmd1Parser("testprog [global options] cmd1 [subcommand options]", "Description of the subcommand test program");
    argparser::varg<bool> testCmd1flgvarg(false, true);
    cmd1Parser.addFlagArgument(&testCmd1flgvarg, "testCmd1flg", "-x", "This is the cmd1 test flag argument");

    argparser::varg<int> testCmd1Valuevarg(0);
    cmd1Parser.addKeyArgument(&testCmd1Valuevarg, "testCmd1Value", "-y", "This is the cmd1 test value argument");

    argparser::varg<std::string> testCmd1OutArg("");
    cmd1Parser.addKeyArgument(&testCmd1OutArg, "testCmd1OutArg", "-o, --output", "This is the cmd1 test output argument");

    EXPECT_EQ(argc, cmd1Parser.parse(argc, argv, 5));
    EXPECT_TRUE(testCmd1flgvarg.value);
    EXPECT_EQ(13, testCmd1Valuevarg.value);
    //EXPECT_STREQ("myfile", testCmd1OutArg.value.c_str());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
/** @} */
