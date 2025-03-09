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
#include "parser_string_list.h"
#include "cmd_line_parse.h"

const size_t defaultArgWidth = 14;
const size_t defaultColWidth = 80;
const size_t testArgWidth    = 17;
const int    testValue       = 10;

using ::testing::StrictMock;
using ::testing::Return;

//======================================================================================
// String Helper functions
//======================================================================================
parserstr getDefaultUsage(parserstr inputuseage = "%(prog) [options]")
{
    argparser::BaseParserStringList parserStr;
    return parserStr.getUsageMessage() + "\n" + inputuseage;
}

parserstr getOptionArgMsg()
{
    argparser::BaseParserStringList parserStr;
    return "\n"+parserStr.getSwitchArgumentsMessage()+"\n";
}

parserstr getOptionMsg(parserstr keys, parserstr keyhelp, size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
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

parserstr getDefaultHelpMsg(size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
{
    return getOptionMsg("-h,--help,-?", "show this help message and exit", argWidth, consoleWidth);
}

parserstr getDefaultSlashHelpMsg(size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
{
    return getOptionMsg("/h,/help,/?", "show this help message and exit", argWidth, consoleWidth);
}

parserstr getEpilogStr(parserstr epilog)
{
    return (epilog.empty() ? "\n" : "\n\n" + epilog + "\n");
}

parserstr getDescriptionStr(parserstr descstr)
{
    return (descstr.empty() ? "" : "\n\n" + descstr + "\n");
}

parserstr getPositionalArgMsg()
{
    argparser::BaseParserStringList parserStr;
    return "\n"+parserStr.getPositionalArgumentsMessage()+"\n";
}

parserstr getPositionalMsg(parserstr name, parserstr help, size_t argWidth = defaultArgWidth , size_t consoleWidth = defaultColWidth)
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
// Constructor/Display help Interface testing, English
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
    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, normalConstructorwithStrings)
{
    parserstr usage = "testprog [options]";
    parserstr description = "Description of the test program";
    argparser::cmd_line_parse testvar(usage, description);
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage(usage) +
                            getDescriptionStr(description) + getOptionArgMsg() +
                            getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, normalConstructorwithKeyprefix)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program", "-");
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, normalConstructorwithKeyprefixStrings)
{
    parserstr usage = "testprog [options]";
    parserstr description = "Description of the test program";
    parserstr keyprefix = "-";
    argparser::cmd_line_parse testvar(usage, description, keyprefix);
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage(usage) +
                            getDescriptionStr(description) + getOptionArgMsg() +
                            getDefaultHelpMsg() + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addFlagArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

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
    StrictMock<argparser::mock_varg_intf> testvarg;

    testvar.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") + getOptionArgMsg() +
                            getDefaultHelpMsg() + getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") + getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, copyConstructor)
{
    argparser::cmd_line_parse baseParser("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

    baseParser.addFlagArgument(&testvarg, "flag", "-f,--flag", "This is the flag argument");
    baseParser.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() + getDefaultHelpMsg() +
                            getOptionMsg("-f,--flag", "This is the flag argument") +
                            getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") +
                            getEpilogStr("");

    testing::internal::CaptureStdout();
    baseParser.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    argparser::cmd_line_parse copiedParser(baseParser);

    testing::internal::CaptureStdout();
    copiedParser.displayHelp(std::cout);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, equateConstructor)
{
    argparser::cmd_line_parse baseParser("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

    baseParser.addFlagArgument(&testvarg, "flag", "-f,--flag", "This is the flag argument");
    baseParser.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() + getDefaultHelpMsg() +
                            getOptionMsg("-f,--flag", "This is the flag argument") +
                            getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") +
                            getEpilogStr("");

    testing::internal::CaptureStdout();
    baseParser.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    argparser::cmd_line_parse copiedParser;
    copiedParser = baseParser;

    testing::internal::CaptureStdout();
    copiedParser.displayHelp(std::cout);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, moveConstructor)
{
    argparser::cmd_line_parse baseParser("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

    baseParser.addFlagArgument(&testvarg, "flag", "-f,--flag", "This is the flag argument");
    baseParser.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() + getDefaultHelpMsg() +
                            getOptionMsg("-f,--flag", "This is the flag argument") +
                            getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") +
                            getEpilogStr("");

    testing::internal::CaptureStdout();
    baseParser.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    argparser::cmd_line_parse copiedParser(std::move(baseParser));

    testing::internal::CaptureStdout();
    copiedParser.displayHelp(std::cout);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, equateMoveConstructor)
{
    argparser::cmd_line_parse baseParser("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

    baseParser.addFlagArgument(&testvarg, "flag", "-f,--flag", "This is the flag argument");
    baseParser.addPositionalArgument(&testvarg, "postst", "This is a positional argument", 1);

    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() + getDefaultHelpMsg() +
                            getOptionMsg("-f,--flag", "This is the flag argument") +
                            getPositionalArgMsg() + getPositionalMsg("postst", "This is a positional argument") +
                            getEpilogStr("");

    testing::internal::CaptureStdout();
    baseParser.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    argparser::cmd_line_parse copiedParser;
    copiedParser = std::move(baseParser);

    testing::internal::CaptureStdout();
    copiedParser.displayHelp(std::cout);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addKeyArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testvarg;

    testvar.addKeyArgument(&testvarg, "tstint", "-i,--val", "This is the test key argument", 1);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") + getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() + getDefaultHelpMsg(testArgWidth) +
                            getOptionMsg("-i,--val=tstint", "This is the test key argument", testArgWidth) +
                            getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, addAllArgHelp)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    StrictMock<argparser::mock_varg_intf> testposvarg;

    testvar.addPositionalArgument(&testposvarg, "postst", "This is a positional argument", 1);
    testvar.addFlagArgument(&testflgvarg, "flag", "-f,--flag", "This is the flag argument");
    testvar.addKeyArgument(&testkeyvarg, "tstint", "-i,--val", "This is the test key argument");

    // Set the option column width
    parserstr keyVargStr = "-i,--val=tstint";
    size_t optionWidth = keyVargStr.size() + 2;

    // Test the help output
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() +
                            getDefaultHelpMsg(optionWidth) +
                            getOptionMsg("-f,--flag", "This is the flag argument", optionWidth) +
                            getOptionMsg("-i,--val=tstint", "This is the test key argument", optionWidth) +
                            getPositionalArgMsg() +
                            getPositionalMsg("postst", "This is a positional argument", optionWidth) +
                            getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, WithKeySlash)
{
    parserstr usage = "winutil [options]";
    parserstr description = "test slash keymarker";
    parserstr keyPrefix = "/";
    argparser::cmd_line_parse testvar(usage, description, keyPrefix);

    StrictMock<argparser::mock_varg_intf> flagArg;
    testvar.addFlagArgument(&flagArg, "flag", "/f,/flag", "This is the flag argument");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("winutil [options]") +
                            getDescriptionStr("test slash keymarker") + getOptionArgMsg() +
                            getDefaultSlashHelpMsg(defaultArgWidth - 1) +
                            getOptionMsg("/f,/flag", "This is the flag argument", defaultArgWidth-1) +
                            getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(cmd_line_parse, MixedListValueHelpTest)
{
    argparser::cmd_line_parse testvar("testprog [options]", "Description of the test program");

    StrictMock<argparser::mock_varg_intf> flagarg;
    StrictMock<argparser::mock_varg_intf> keyarg;
    StrictMock<argparser::mock_varg_intf> keyListarg;
    EXPECT_CALL(keyListarg, isList())
        .WillOnce(Return(true));
    StrictMock<argparser::mock_varg_intf> posarg;
    EXPECT_CALL(posarg, isList())
        .WillOnce(Return(true));

    testvar.addFlagArgument(&flagarg, "myflag", "-f,--flag", "Flag argument help text");
    testvar.addKeyArgument(&keyListarg, "mykeylist", "-l,--list", "Keyed list argument help text",3);
    testvar.addKeyArgument(&keyarg, "mykey", "-x,--extra", "Key argument help text");
    testvar.addPositionalArgument(&posarg, "myposition", "List positional argument", 4);

    // Set the option column width
    parserstr maxVargStr = "myposition,myposition,...";
    size_t optionWidth = maxVargStr.size()+2;

    // Test the help output
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getDefaultUsage("testprog [options]") +
                            getDescriptionStr("Description of the test program") +
                            getOptionArgMsg() +
                            getDefaultHelpMsg(optionWidth) +
                            getOptionMsg("-f,--flag", "Flag argument help text", optionWidth) +
                            getOptionMsg("-l,--list=mykeylist,...", "Keyed list argument help text", optionWidth) +
                            getOptionMsg("-x,--extra=mykey", "Key argument help text", optionWidth) +
                            getPositionalArgMsg() +
                            getPositionalMsg("myposition,myposition,...", "List positional argument", optionWidth) +
                            getEpilogStr("");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

/** @} */
