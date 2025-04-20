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
#include "varg_intf_mock.h"
#include "varg_clib_shared.h"
#include "parser_string_list.h"
#include "parser_base.h"
#include "cmd_line_parse_api.h"
#include "../src/cmd_line_parse_api_lib_private.h"

//==================================================================
// mock definitions
//==================================================================
using ::testing::StrictMock;
using ::testing::Return;

#if(0)
//==================================================================
// Helper Class
//==================================================================
/**
 * @brief Help message generation helper functions
 */
class helpMsg
{
    private:
        size_t defaultArgWidth{};
        size_t defaultColWidth{};
        size_t testArgWidth{};
        argparser::BaseParserStringList parserBaseStrings;
        parserstr defaultUsage;
        parserstr defaultDesc;

        parserstr getKeyMsg(parserstr keys, size_t argWidth = 0)
        {
            size_t workingArgWidth = ((argWidth == 0) ? defaultArgWidth : argWidth);

            parserstr argMsg = " " + keys;
            while (argMsg.size() < workingArgWidth)
            {
                argMsg += ' ';
            }
            return argMsg;
        }

        parserstr getHelpMsg(parserstr helpText, size_t argWidth = 0, size_t consoleWidth = 0)
        {
            size_t workingArgWidth = ((argWidth == 0) ? defaultArgWidth : argWidth);
            size_t workingConsoleWidth = ((consoleWidth == 0) ? defaultColWidth : consoleWidth);

            parserstr helpmsg = helpText;
            while (helpmsg.size() < (workingConsoleWidth - workingArgWidth))
            {
                helpmsg += ' ';
            }

            return helpmsg + "\n";
        }

    public:
        helpMsg(parserstr usage = "", parserstr desc = "") :
            defaultArgWidth(14), defaultColWidth(80), testArgWidth(17),     // NOLINT
            defaultUsage(std::move(usage)), defaultDesc(std::move(desc))
        {}

        helpMsg(const helpMsg& other) = default;
        helpMsg(helpMsg&& other) noexcept = default;
        helpMsg& operator=(const helpMsg& other) = default;
        helpMsg& operator=(helpMsg&& other) noexcept = default;
        ~helpMsg() = default;

        void setDefaultUsage(parserstr usage)       {defaultUsage = std::move(usage);}
        void setDefaultDesc(parserstr desc)         {defaultDesc = std::move(desc);}

        parserstr getUsage(parserstr inputuseage = "")
        {
            return parserBaseStrings.getUsageMessage() + "\n" + (inputuseage.empty() ? defaultUsage : inputuseage);
        }

        static parserstr getDescriptionStr(parserstr descstr = "")
        {
            return (descstr.empty() ? "" : "\n\n" + descstr + "\n");
        }

        parserstr getOptionArgMsg()
        {
            return "\n"+parserBaseStrings.getSwitchArgumentsMessage()+"\n";
        }

        parserstr getOptionMsg(parserstr keys, parserstr keyhelp, size_t argWidth = 0, size_t consoleWidth = 0)
        {
            return getKeyMsg(keys, argWidth) + getHelpMsg(keyhelp, argWidth, consoleWidth);
        }

        parserstr getDefaultHelpMsg(size_t argWidth = 0 , size_t consoleWidth = 0)
        {
            return getOptionMsg("-h,--help,-?", "show this help message and exit",
                                argWidth, consoleWidth);
        }

        static parserstr getEpilogStr(parserstr epilog = "")
        {
            return (epilog.empty() ? "\n" : "\n\n" + epilog + "\n");
        }

        parserstr getPositionalArgMsg()
        {
            return "\n"+parserBaseStrings.getPositionalArgumentsMessage()+"\n";
        }

        parserstr getPositionalMsg(parserstr name, parserstr help, size_t argWidth = 0 , size_t consoleWidth = 0)
        {
            return getKeyMsg(name, argWidth) + getHelpMsg(help, argWidth, consoleWidth);
        }

        parserstr expectedBaseHelp(parserstr epilog = "")
        {
            parserstr expectedStr = getUsage(defaultUsage);
            expectedStr += getDescriptionStr(defaultDesc);
            expectedStr += getOptionArgMsg();
            expectedStr += getDefaultHelpMsg();
            expectedStr += getEpilogStr(epilog);
            return expectedStr;
        }

        parserstr expectedBaseStart()
        {
            parserstr expectedStr = getUsage(defaultUsage);
            expectedStr += getDescriptionStr(defaultDesc);
            expectedStr += getOptionArgMsg();
            return expectedStr;
        }

};

class cmd_line_parse_clib_test : public ::testing::Test
{
    protected:
        // NOLINTBEGIN
        cmdLineParserHandle testParser;
        helpMsg             helpMsgGen;
        // NOLINTEND

        void SetUp() override
        {
            testParser = getParser("myprog [options]", "test description",
                                   '-', true,
                                   false, 0);

            helpMsgGen.setDefaultUsage("myprog [options]");
            helpMsgGen.setDefaultDesc("test description");
        }

        void TearDown() override
        {
            releaseParser(testParser);
        }


    public:
        cmd_line_parse_clib_test() : testParser() {};
        cmd_line_parse_clib_test(const cmd_line_parse_clib_test& other) = delete;
        cmd_line_parse_clib_test(cmd_line_parse_clib_test&& other) noexcept = delete;
        cmd_line_parse_clib_test& operator=(const cmd_line_parse_clib_test& other) = delete;
        cmd_line_parse_clib_test& operator=(cmd_line_parse_clib_test&& other) noexcept = delete;
        ~cmd_line_parse_clib_test() override = default;

};

//==================================================================
// Basic test
//==================================================================

//======================================================================================
// Public Interface testing, English
//======================================================================================
TEST_F(cmd_line_parse_clib_test, defaultConstructor)
{
    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = helpMsgGen.expectedBaseHelp();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, test_epilog)
{
    setEpilog(testParser, "This is the epilog");

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = helpMsgGen.expectedBaseHelp("This is the epilog");
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, addFlagArgHelp)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addFlagArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = helpMsgGen.expectedBaseStart();
    expectedStr += helpMsgGen.getDefaultHelpMsg();
    expectedStr += helpMsgGen.getOptionMsg("-f,--flag", "This is the flag argument");
    expectedStr += helpMsg::getEpilogStr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, addPositionalHelp)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addPositionalArgument(testParser, &cvarghandle, "postst", "This is a positional argument", true);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = helpMsgGen.expectedBaseStart();
    expectedStr += helpMsgGen.getDefaultHelpMsg();
    expectedStr += helpMsgGen.getPositionalArgMsg();
    expectedStr += helpMsgGen.getPositionalMsg("postst", "This is a positional argument");
    expectedStr += helpMsg::getEpilogStr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, addKeyArgHelp)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArgument(testParser, &cvarghandle, "tstint", "-i,--val", "This is the test key argument", true);

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr keyVargStr = "-i,--val=tstint";
    size_t optionWidth = keyVargStr.size() + 2;
    parserstr expectedStr = helpMsgGen.expectedBaseStart();
    expectedStr += helpMsgGen.getDefaultHelpMsg(optionWidth);
    expectedStr += helpMsgGen.getOptionMsg(keyVargStr, "This is the test key argument", optionWidth);
    expectedStr += helpMsg::getEpilogStr();

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, addAllArgHelp)
{
    struct cvarptr cvarghandle[3];  // NOLINT

    StrictMock<argparser::mock_varg_intf> posvarg;
    EXPECT_CALL(posvarg, isList()).WillOnce(Return(false));
    StrictMock<argparser::mock_varg_intf> flagvarg;
    EXPECT_CALL(flagvarg, isList()).WillOnce(Return(false));
    StrictMock<argparser::mock_varg_intf> keyvarg;
    EXPECT_CALL(keyvarg, isList()).WillOnce(Return(false));
    cvarghandle[0].vararg = &posvarg;
    cvarghandle[1].vararg = &flagvarg;
    cvarghandle[2].vararg = &keyvarg;

    addPositionalArgument(testParser, &(cvarghandle[0]),
                          "postst", "This is a positional argument", true);
    addFlagArgument(testParser, &(cvarghandle[1]),
                    "flag", "-f,--flag", "This is the flag argument", false);
    addKeyArgument(testParser, &(cvarghandle[2]),
                   "tstint", "-i,--val", "This is the test key argument", true);

    parserstr keyargstr = "-i,--val=tstint";
    size_t optionWidth = keyargstr.size() + 2;

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    parserstr expectedStr = helpMsgGen.expectedBaseStart();
    expectedStr += helpMsgGen.getDefaultHelpMsg(optionWidth);
    expectedStr += helpMsgGen.getOptionMsg("-f,--flag", "This is the flag argument", optionWidth);
    expectedStr += helpMsgGen.getOptionMsg(keyargstr, "This is the test key argument", optionWidth);
    expectedStr += helpMsgGen.getPositionalArgMsg();
    expectedStr += helpMsgGen.getPositionalMsg("postst", "This is a positional argument", optionWidth);
    expectedStr += helpMsg::getEpilogStr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestFlag)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addFlagArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 0, -1));   // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestFlagFailure)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addFlagArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-f=false";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));   // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-f\" invalid assignment\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestKeyWithAssign)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("5")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArgument(testParser, &cvarghandle,
                   "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=5";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 0, -1));   // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestKeyAssignMissing)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArgument(testParser, &cvarghandle,
                   "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));  // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" missing assignment value\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestKeyAssignFail)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("foo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArgument(testParser, &cvarghandle,
                   "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i=foo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));  // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i foo\" assignment failed\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestKeyAssignNextArg)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArgument(testParser, &cvarghandle,
                   "tstint", "-i,--val", "This is the test key argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseIncrementing)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addIncrementingArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ff";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 0, -1));   // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseIncrementingFail)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addIncrementingArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ff";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 0, -1));   // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    parserstr expected = "\"-f \" assignment failed\n";
    expected += "\"-f \" assignment failed\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parsePositional)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addPositionalArgument(testParser, &cvarghandle,
                          "postst", "This is a positional argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "42";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parsePositionalFailed)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("goo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addPositionalArgument(testParser, &cvarghandle,
                          "postst", "This is a positional argument", true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "goo";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 2, argv, 1, -1));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"postst goo\" assignment failed\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestMultiple)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    StrictMock<argparser::mock_varg_intf> testvarg1;
    EXPECT_CALL(testvarg1, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg1, setValue(::testing::StrEq("5")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle1;
    cvarghandle1.vararg = &testvarg1;

    StrictMock<argparser::mock_varg_intf> testvarg2;
    EXPECT_CALL(testvarg2, isList()).WillOnce(Return(false));
    EXPECT_CALL(testvarg2, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle2;
    cvarghandle2.vararg = &testvarg2;

    addPositionalArgument(testParser, &cvarghandle2,
                          "postst", "This is a positional argument", true);
    addFlagArgument(testParser, &cvarghandle,
                    "flag", "-f,--flag", "This is the flag argument", false);
    addKeyArgument(testParser, &cvarghandle1,
                   "tstint", "-i,--val", "This is the test key argument", true);
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
}


TEST_F(cmd_line_parse_clib_test, parseTestAddListArg)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", 3, true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));          // NOLINT
}

TEST_F(cmd_line_parse_clib_test, TestAddPositionalArray)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addPositionalArrayArgument(testParser, &cvarghandle,
        "tstint", "This is the test key argument", 3, true);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, val1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));          // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestAddListArgMultipleArgv)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("19")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", 3, false);
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
}

TEST_F(cmd_line_parse_clib_test, parseTestAddListArgTooFew)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", 3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());

}

TEST_F(cmd_line_parse_clib_test, parseTestAddListArgTooMany)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", 3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,42,56";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestAddDynamicListArg)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22,43";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestAddDynamicListArg2Vals)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "18,22";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestAddDynamicListArg2ValsMultiArgv)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", -3, false);
    disableHelpDisplayOnError(testParser);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar* argv[] = {progname, opt1, val1, val2};
    // NOLINTEND

    EXPECT_EQ(4, parse(testParser, 4, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestAddDynamicListArgMultiArgvSwitchTermination)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList())
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("81")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvarghandle;
    cvarghandle.vararg = &testvarg;

    addKeyArrayArgument(testParser, &cvarghandle,
                        "tstint", "-i,--val", "This is the test key argument", -3, false);

    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-f,--flag", "This is the test flag argument",false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-i";
    parserchar val1[] = "81";
    parserchar val2[] = "13";
    parserchar val3[] = "-f";
    parserchar* argv[] = {progname, opt1, val1, val2, val3};
    // NOLINTEND

    EXPECT_EQ(5, parse(testParser, 5, argv, 1, -1));      // NOLINT
}


TEST_F(cmd_line_parse_clib_test, parseTestDualSingleCharFlag)
{
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    disableHelpDisplayOnError(testParser);

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-if";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));    // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestTripleSingleCharFlag)
{
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testflg2varg;
    EXPECT_CALL(testflg2varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg2varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg2handle;
    cvargflg2handle.vararg = &testflg2varg;

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addFlagArgument(testParser, &cvargflg2handle,
                    "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestDualSingleCharFlagVal)
{
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargkeyhandle;
    cvargkeyhandle.vararg = &testkeyvarg;

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addKeyArgument(testParser, &cvargkeyhandle,
                   "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg=42";
    parserchar* argv[] = {progname, opt1};
    // NOLINTEND

    EXPECT_EQ(2, parse(testParser, 2, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestDualSingleCharFlagValNextargv)
{
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargkeyhandle;
    cvargkeyhandle.vararg = &testkeyvarg;

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addKeyArgument(testParser, &cvargkeyhandle,
                   "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-ifg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    EXPECT_EQ(3, parse(testParser, 3, argv, 1, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestMissingRequired)
{
    StrictMock<argparser::mock_varg_intf> testflgvarg;
    EXPECT_CALL(testflgvarg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflghandle;
    cvargflghandle.vararg = &testflgvarg;

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testkeyvarg;
    EXPECT_CALL(testkeyvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testkeyvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargkeyhandle;
    cvargkeyhandle.vararg = &testkeyvarg;

    disableHelpDisplayOnError(testParser);

    addFlagArgument(testParser, &cvargflghandle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", true);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addKeyArgument(testParser, &cvargkeyhandle,
                   "tstval", "-g,--goo", "This is the test value argument", true);

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar opt1[] = "-fg";
    parserchar val1[] = "42";
    parserchar* argv[] = {progname, opt1, val1};
    // NOLINTEND

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, 3, argv, 1, -1));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"-i|--val\" required argument missing\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, parseTestMultiplePositionalArgs)
{
    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg1varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testflg2varg;
    EXPECT_CALL(testflg2varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg2varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg2handle;
    cvargflg2handle.vararg = &testflg2varg;

    StrictMock<argparser::mock_varg_intf> testflg3varg;
    EXPECT_CALL(testflg3varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(testflg3varg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr cvargflg3handle;
    cvargflg3handle.vararg = &testflg3varg;

    StrictMock<argparser::mock_varg_intf> teststr1varg;
    EXPECT_CALL(teststr1varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(teststr1varg, setValue(::testing::StrEq("foo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr testsubarg;
    testsubarg.vararg = &teststr1varg;

    StrictMock<argparser::mock_varg_intf> teststr2varg;
    EXPECT_CALL(teststr2varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(teststr2varg, setValue(::testing::StrEq("george")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    struct cvarptr testoutname;
    testoutname.vararg = &teststr2varg;

    addPositionalArgument(testParser, &testsubarg,
                          "subcmd", "This is the test subcmd argument", true);
    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg2handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addFlagArgument(testParser, &cvargflg3handle,
                    "tstflg2", "-g,--goo", "This is the test flag2 argument", false);
    addPositionalArgument(testParser, &testoutname,
                          "outputname", "This is the test output name argument", false);

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
}

TEST_F(cmd_line_parse_clib_test, parseTestTwoPhasedParse)
{
    StrictMock<argparser::mock_varg_intf> teststr1varg;
    EXPECT_CALL(teststr1varg, isList()).WillOnce(Return(false));
    struct cvarptr testsubarg;
    testsubarg.vararg = &teststr1varg;
    ::testing::ExpectationSet subcmd = EXPECT_CALL(teststr1varg, setValue(::testing::StrEq("foo")))
                                        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testflg2varg;
    EXPECT_CALL(testflg2varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg2handle;
    cvargflg2handle.vararg = &testflg2varg;

    StrictMock<argparser::mock_varg_intf> testflg3varg;
    EXPECT_CALL(testflg3varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg3handle;
    cvargflg3handle.vararg = &testflg3varg;

    EXPECT_CALL(testflg1varg, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testflg2varg, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testflg3varg, setValue())
        .After(subcmd)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addPositionalArgument(testParser, &testsubarg,
                          "subcmd", "This is the test subcmd argument", true);

    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg2handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addFlagArgument(testParser, &cvargflg3handle,
                    "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

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

    // Phase 2
    EXPECT_EQ(argc, parse(testParser, argc, argv, 2, -1));  // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestThreePhasedParse)
{
    StrictMock<argparser::mock_varg_intf> teststr1varg;
    EXPECT_CALL(teststr1varg, isList()).WillOnce(Return(false));
    struct cvarptr testsubarg;
    testsubarg.vararg = &teststr1varg;
    ::testing::ExpectationSet subcmd = EXPECT_CALL(teststr1varg, setValue(::testing::StrEq("foo")))
                                        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addPositionalArgument(testParser, &testsubarg,
                          "subcmd", "This is the test subcmd argument", true);

    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testflg2varg;
    EXPECT_CALL(testflg2varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg2handle;
    cvargflg2handle.vararg = &testflg2varg;

    StrictMock<argparser::mock_varg_intf> testflg3varg;
    EXPECT_CALL(testflg3varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg3handle;
    cvargflg3handle.vararg = &testflg3varg;

    ::testing::ExpectationSet flags;
    flags += EXPECT_CALL(testflg1varg, setValue())
            .After(subcmd)
            .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflg2varg, setValue())
            .After(subcmd)
            .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    flags += EXPECT_CALL(testflg3varg, setValue())
            .After(subcmd)
            .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg2handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addFlagArgument(testParser, &cvargflg3handle,
                    "tstflg2", "-g,--goo", "This is the test flag2 argument", false);


    StrictMock<argparser::mock_varg_intf> teststr2varg;
    struct cvarptr testoutname;
    testoutname.vararg = &teststr2varg;
    EXPECT_CALL(teststr2varg, isList()).WillOnce(Return(false));
    EXPECT_CALL(teststr2varg, setValue(::testing::StrEq("george")))
        .After(subcmd, flags)
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addPositionalArgument(testParser, &testoutname,
                          "outputname", "This is the test output name argument", false);

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

    // Phase 2
    EXPECT_EQ(argc-1, parse(testParser, argc, argv, 2, argc-1));    // NOLINT

    // Phase 3
    EXPECT_EQ(argc, parse(testParser, argc, argv, argc-1, -1));     // NOLINT
}

TEST(cmd_line_parse_clib, parseTestTwoPhasedTwoParserPositionalStop)
{
    StrictMock<argparser::mock_varg_intf> testflg1varg;
    EXPECT_CALL(testflg1varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg1handle;
    cvargflg1handle.vararg = &testflg1varg;

    StrictMock<argparser::mock_varg_intf> testflg2varg;
    EXPECT_CALL(testflg2varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg2handle;
    cvargflg2handle.vararg = &testflg2varg;

    StrictMock<argparser::mock_varg_intf> testflg3varg;
    EXPECT_CALL(testflg3varg, isList()).WillOnce(Return(false));
    struct cvarptr cvargflg3handle;
    cvargflg3handle.vararg = &testflg3varg;

    cmdLineParserHandle testParser = getParser("myprog [options]", "test description",
                                               '-', true,
                                               false, 0);

    addFlagArgument(testParser, &cvargflg1handle,
                    "tstflg", "-i,--val", "This is the test flag0 argument", false);
    addFlagArgument(testParser, &cvargflg2handle,
                    "tstflg1", "-f,--flag", "This is the test flag1 argument", false);
    addFlagArgument(testParser, &cvargflg3handle,
                    "tstflg2", "-g,--goo", "This is the test flag2 argument", false);

    StrictMock<argparser::mock_varg_intf> teststrvarg;
    EXPECT_CALL(teststrvarg, isList()).WillOnce(Return(false));
    struct cvarptr testsubarg;
    testsubarg.vararg = &teststrvarg;
    addPositionalArgument(testParser, &testsubarg,
                          "subcmd", "This is the test subcmd argument", true);

    ::testing::ExpectationSet phase1;
    phase1 += EXPECT_CALL(testflg1varg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    phase1 += EXPECT_CALL(testflg2varg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    phase1 += EXPECT_CALL(testflg3varg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    phase1 += EXPECT_CALL(teststrvarg, setValue(::testing::StrEq("cmd1")))
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

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

    // Phase 2, Parser 2
    cmdLineParserHandle cmd1Parser = getParser("testprog [global options] cmd1 [subcommand options]",
                                               "Description of the subcommand test program", '-',
                                               false, false, 0);


    StrictMock<argparser::mock_varg_intf> phase2flgvarg;
    struct cvarptr testCmd1flgvarg;
    testCmd1flgvarg.vararg = &phase2flgvarg;
    EXPECT_CALL(phase2flgvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(phase2flgvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> phase2intvarg;
    struct cvarptr testCmd1Valuevarg;
    testCmd1Valuevarg.vararg = &phase2intvarg;
    EXPECT_CALL(phase2intvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(phase2intvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> cmdOutvarg;
    struct cvarptr testCmd1OutArg;
    testCmd1OutArg.vararg = &cmdOutvarg;
    EXPECT_CALL(cmdOutvarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(cmdOutvarg, setValue(::testing::StrEq("myfile")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addFlagArgument(cmd1Parser, &testCmd1flgvarg,
                    "testCmd1flg", "-x", "This is the cmd1 test flag argument", false);
    addKeyArgument(cmd1Parser, &testCmd1Valuevarg,
                   "testCmd1Value", "-y", "This is the cmd1 test value argument", false);
    addKeyArgument(cmd1Parser, &testCmd1OutArg,
                   "testCmd1OutArg", "-o, --output", "This is the cmd1 test output argument", false);


    EXPECT_EQ(argc, parse(cmd1Parser, argc, argv, 5, -1));      // NOLINT
}

TEST_F(cmd_line_parse_clib_test, parseTestMissingRequiredSubcommand)
{
    StrictMock<argparser::mock_varg_intf> flgvarg;
    struct cvarptr testflgvarg;
    testflgvarg.vararg = &flgvarg;
    EXPECT_CALL(flgvarg, isList()).WillOnce(Return(false));

    StrictMock<argparser::mock_varg_intf> stringvarg;
    struct cvarptr subcommand;
    subcommand.vararg = &stringvarg;
    EXPECT_CALL(stringvarg, isList()).WillOnce(Return(false));

    disableHelpDisplayOnError(testParser);
    addFlagArgument(testParser, &testflgvarg,
                    "version", "-V,--version", "Example of a simple true/false flag argument", false);
    addPositionalArgument(testParser, &subcommand,
                          "subcommand", "Example of a positional argument as subcommand", true); // sub command argument

    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar* argv[] = {progname};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    testing::internal::CaptureStderr();

    EXPECT_EQ(-1, parse(testParser, argc, argv, 1, 2));     // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"subcommand\" required argument missing\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, enableUnknownArgIgnore)
{

    StrictMock<argparser::mock_varg_intf> flagArg;
    struct cvarptr testflgvarg;
    testflgvarg.vararg = &flagArg;
    EXPECT_CALL(flagArg, isList()).WillOnce(Return(false));
    EXPECT_CALL(flagArg, setValue())
        .Times(2)
        .WillRepeatedly(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    StrictMock<argparser::mock_varg_intf> keyarg;
    struct cvarptr testkeyvarg;
    testkeyvarg.vararg = &keyarg;
    EXPECT_CALL(keyarg, isList()).WillOnce(Return(false));
    EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    addFlagArgument(testParser, &testflgvarg, "flag", "-f,--flag",
                    "Example of a simple true/false flag argument", false);
    addKeyArgument(testParser, &testkeyvarg,
                   "key", "-v,--value", "Example of a key argument", true);
    // NOLINTBEGIN
    parserchar progname[] = "runprog";
    parserchar vargflg[] = "-f";
    parserchar posArg[] = "-v=15";
    parserchar unknownArg[] = "--test";
    parserchar* argv[] = {progname, vargflg, unknownArg, posArg};
    // NOLINTEND

    int argc = sizeof(argv) / sizeof(argv[0]);

    disableHelpDisplayOnError(testParser);

    testing::internal::CaptureStderr();
    EXPECT_EQ(-1, parse(testParser, argc, argv, 1, -1));    // NOLINT
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Unknown argument --test\n", output.c_str());

    enableUnknowArgumentIgnore(testParser);
    EXPECT_EQ(4, parse(testParser, argc, argv, 1, -1));    // NOLINT
}

TEST_F(cmd_line_parse_clib_test, TestInvalidKeyVargCheck)
{
    testing::internal::CaptureStderr();
    addKeyArgument(testParser, nullptr,
                   "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestInvalidKeyArrayVargCheck)
{
    testing::internal::CaptureStderr();
    addKeyArrayArgument(testParser, nullptr,
                        "testvarg", "-t,--test", "Simple help text", 3, false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());

}

TEST_F(cmd_line_parse_clib_test, TestInvalidFlagVargCheck)
{
    testing::internal::CaptureStderr();
    addFlagArgument(testParser, nullptr,
                        "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());

}

TEST_F(cmd_line_parse_clib_test, TestInvalidIncVargCheck)
{
    testing::internal::CaptureStderr();
    addIncrementingArgument(testParser, nullptr,
                        "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestInvalidPositionalVargCheck)
{
    testing::internal::CaptureStderr();
    addPositionalArgument(testParser, nullptr,
                        "testvarg", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestInvalidPosArrayVargCheck)
{
    testing::internal::CaptureStderr();
    addPositionalArrayArgument(testParser, nullptr,
                        "testvarg", "Simple help text", 4, false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Invalid valueAddr input, nullptr\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestWrongKeyVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    struct cvarptr testclibvarg;
    testclibvarg.vararg = &testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(true));

    testing::internal::CaptureStderr();
    addKeyArgument(testParser, &testclibvarg,
                   "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Use addKeyArrayArgument() to add array key arguments to the parser\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestWrongFlagVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    struct cvarptr testclibvarg;
    testclibvarg.vararg = &testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(true));

    testing::internal::CaptureStderr();
    addFlagArgument(testParser, &testclibvarg,
                    "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Flag arguments cannot be lists\n", output.c_str());

}

TEST_F(cmd_line_parse_clib_test, TestWrongIncVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    struct cvarptr testclibvarg;
    testclibvarg.vararg = &testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(true));

    testing::internal::CaptureStderr();
    addIncrementingArgument(testParser, &testclibvarg,
                            "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Incrementing flag arguments cannot be lists\n", output.c_str());

}

TEST_F(cmd_line_parse_clib_test, TestWrongPositionalVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    struct cvarptr testclibvarg;
    testclibvarg.vararg = &testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(true));

    testing::internal::CaptureStderr();
    addPositionalArgument(testParser, &testclibvarg,
                          "testvarg", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Use addPositionalArrayArgument() to add positional array arguments to the parser\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestWrongKeyArrayVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    struct cvarptr testlistclibvarg;
    testlistclibvarg.vararg = &testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    addKeyArrayArgument(testParser, &testlistclibvarg,
                        "testvarg", "-t,--test", "Simple help text", 3, false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Use addKeyArgument() to add single value key arguments to the parser\n", output.c_str());
}

TEST_F(cmd_line_parse_clib_test, TestWrongPosArrayVargTypeCheck)
{
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    struct cvarptr testlistclibvarg;
    testlistclibvarg.vararg = &testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    addPositionalArrayArgument(testParser, &testlistclibvarg,
                        "testvarg", "Simple help text", 4, false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Use addPositionalArgument() to add single value positional arguments to the parser\n", output.c_str());
}

TEST(cmd_line_parse_clib, TestSetProgramName)
{
    cmdLineParserHandle testParser = getParser(nullptr, "test description",
                                               '-', false,
                                               false, 0);
    disableHelpDisplayOnError(testParser);
    setProgramName(testParser, "progName");

    testing::internal::CaptureStderr();
    displayHelp(testParser);
    parserstr output = testing::internal::GetCapturedStderr();

    helpMsg helpGen("progName [options]", "test description");
    parserstr expectedStr = helpGen.expectedBaseHelp();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    releaseParser(testParser);
}

TEST(cmd_line_parse_clib, TestInvalidParserCheck)
{
    StrictMock<argparser::mock_varg_intf> testvarg;
    struct cvarptr testclibvarg;
    testclibvarg.vararg = &testvarg;
    parserstr expectedStr = "Invalid parser input, nullptr\n";

    testing::internal::CaptureStderr();
    addKeyArgument(nullptr, &testclibvarg,
                   "testvarg", "-t,--test", "Simple help text", false);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    testing::internal::CaptureStderr();
    addKeyArrayArgument(nullptr, &testclibvarg,
                        "testvarg", "-t,--test", "Simple help text", 3, false);
    output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    testing::internal::CaptureStderr();
    addFlagArgument(nullptr, &testclibvarg,
                        "testvarg", "-t,--test", "Simple help text", false);
    output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    testing::internal::CaptureStderr();
    addIncrementingArgument(nullptr, &testclibvarg,
                        "testvarg", "-t,--test", "Simple help text", false);
    output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    testing::internal::CaptureStderr();
    addPositionalArgument(nullptr, &testclibvarg,
                        "testvarg", "Simple help text", false);
    output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());

    testing::internal::CaptureStderr();
    addPositionalArrayArgument(nullptr, &testclibvarg,
                        "testvarg", "Simple help text", 4, false);
    output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}
#endif
/** @} */
