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
 * @file envparse_test.cpp
 * @defgroup libenvparse_unittest Environment input argument parsing utilities unit test
 * @{
 */

// Includes
#include <cstdlib>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "envparse.h"
#include "varg_intf_mock.h"
#include "mock_ParserStringListInterface.h"

#if ((_WIN32) || (_WIN64))
    #include <Windows.h>
    #define SETENV(name, value, overwrite)  SetEnvironmentVariable(name, value)
    #define UNSETENV(name)                  SetEnvironmentVariable(name, "")
#elif defined(__linux__) || defined(__unix__)
    int SETENV(const char* name, const char* value, int overwrite) {return setenv(name, value, overwrite);}
    int UNSETENV(const char* name)                                 {return unsetenv(name);}
#else
    #error "Define setenv/unsetenv for this OS!"
#endif

using ::testing::StrictMock;
using ::testing::Mock;
using ::testing::Return;
using stringMockptr = StrictMock<argparser::mock_ParserStringListInterface>*;

const int    testValue = 10;

class envparser_test : public argparser::envparser
{
    public:
        envparser_test(bool abortOnError = false, int debugLevel = 0) : argparser::envparser(abortOnError, debugLevel) {}
        envparser_test(const envparser_test& other) noexcept = default;
        envparser_test(envparser_test&& other) noexcept = default;
        envparser_test& operator=(const envparser_test& other) noexcept = default;
        envparser_test& operator=(envparser_test&& other) noexcept = default;
        ~envparser_test() = default;

        stringMockptr getStringsMock()
        {
            argparser::ParserStringListInterface* mock = msgGeneration.get();
            return reinterpret_cast<stringMockptr> (mock);   // NOLINT
        }
};

//======================================================================================
// Test functions constructor and help
//======================================================================================
TEST(envTestParser, ConstructorBasic)
{
    ::testing::internal::CaptureStdout();
    envparser_test testvar;
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("", output.c_str());
}

TEST(envTestParser, addArgument)
{
    StrictMock<argparser::mock_varg_intf> testvalvarg;
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getEnvArgumentsMessage()).WillOnce(Return("Mock EnvHelp Args:"));

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");

    ::testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expected = "Mock EnvHelp Args:\n";
    expected += " MYENVTEST=MYENVTEST My environment test var                                    \n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
}

TEST(envTestParser, addArgument2)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getEnvArgumentsMessage()).WillOnce(Return("Mock EnvHelp Args2:"));

    StrictMock<argparser::mock_varg_intf> testvalvarg;
    StrictMock<argparser::mock_varg_intf> testvalvarg1;

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = "Mock EnvHelp Args2:\n";
    expectedStr += " MYENVTEST=MYENVTEST         My environment test var                            \n";
    expectedStr += " SECONDENVTEST=SECONDENVTEST My second environment test var                     \n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(envTestParser, addArgumentNargZero)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getEnvironmentNoFlags("MYENVTEST")).WillOnce(Return("Mock environment value MYENVTEST narg must be > 0"));

    StrictMock<argparser::mock_varg_intf> testvalvarg;

    testing::internal::CaptureStderr();
    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var",0);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock environment value MYENVTEST narg must be > 0\n", output.c_str());
}

TEST(envTestParser, addArgumentInvalidNargs)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getNotListTypeMessage(2)).WillOnce(Return("Mock only list type arguments can have an argument count of 2"));

    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    testvar.addArgument(&testvarg, "tstint", "This is the test key argument", 2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock only list type arguments can have an argument count of 2\n", output.c_str());
}

TEST(envTestParser, parsetest)
{
    envparser_test testvar;
    StrictMock<argparser::mock_varg_intf> testvalvarg;
    StrictMock<argparser::mock_varg_intf> testvalvarg1;
    EXPECT_CALL(testvalvarg, setValue(::testing::StrEq("10")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    SETENV("MYENVTEST","10", 1);
    UNSETENV("SECONDENVTEST");
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
    UNSETENV("SECONDENVTEST");
}

TEST(envTestParser, parsetestDual)
{
    envparser_test testvar;
    StrictMock<argparser::mock_varg_intf> testvalvarg;
    StrictMock<argparser::mock_varg_intf> testvalvarg1;
    EXPECT_CALL(testvalvarg, setValue(::testing::StrEq("10")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvalvarg1, setValue(::testing::StrEq("true")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    SETENV("MYENVTEST","10", 1);
    SETENV("SECONDENVTEST","true", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
    UNSETENV("SECONDENVTEST");
}

TEST(envTestParser, parsetestlist)
{
    envparser_test testvar;
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("10")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 2);

    SETENV("MYENVTEST","10,21", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parsetestlistTooFew)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getMissingListAssignmentMessage("MYENVTEST", 3, 2))
        .WillOnce(Return("Mock \"MYENVTEST\" missing assignment. Expected: 3 found: 2 arguments"));

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 3);

    SETENV("MYENVTEST","10,21", 1);
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock \"MYENVTEST\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parsetestlistTooMany)
{
    envparser_test testvar(false);
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getTooManyAssignmentMessage("MYENVTEST", 3, 4))
        .WillOnce(Return("Mock \"MYENVTEST\" too many assignment values. Expected: 3 found: 4 arguments"));

    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 3);

    SETENV("MYENVTEST","10,21,32,45", 1);
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock \"MYENVTEST\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parseTestAddDynamicListArg)
{
    envparser_test testvar(false);
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("43")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -3);
    SETENV("MYENVTEST","18,22,43", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parseTestAddDynamicList2Arg)
{
    envparser_test testvar(false);
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -3);
    SETENV("MYENVTEST","18,22", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parseTestAddDynamicListIndefinite2Arg)
{
    envparser_test testvar(false);
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -1);
    SETENV("MYENVTEST","18,22", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parseTestAddDynamicListIndefinite6Arg)
{
    envparser_test testvar(false);
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("12")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("11")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("55")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -1);
    SETENV("MYENVTEST","18,22,13,12,11,55", 1);
    EXPECT_TRUE(testvar.parse());
    UNSETENV("MYENVTEST");
}

TEST(envTestParser, parseTestDebugCompleteness)
{
    envparser_test testvar(false, veryVerboseDebug);
    StrictMock<argparser::mock_varg_intf> testlistvarg;
    EXPECT_CALL(testlistvarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("18")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("22")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("12")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("11")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testlistvarg, setValue(::testing::StrEq("55")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -1);
    SETENV("MYENVTEST","18,22,13,12,11,55", 1);
    testing::internal::CaptureStdout();
    EXPECT_TRUE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStdout();
    UNSETENV("MYENVTEST");

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Environment value: MYENVTEST=18,22,13,12,11,55\n";
    expected += "Environment var name: MYENVTEST\n";
    expected += "Value count: 6\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif // if(ENABLE_DEBUG_STRING_CHECK)
}

#if defined(__linux__)
TEST(envTestParser, NoValueTest)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getMissingAssignmentMessage("MYENVTEST"))
        .WillOnce(Return("Mock \"MYENVTEST\" missing assignment value"));

    StrictMock<argparser::mock_varg_intf> testvalvarg;

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");

    SETENV("MYENVTEST","", 1);

    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock \"MYENVTEST\" missing assignment value\n", output.c_str());

    UNSETENV("MYENVTEST");
}
#endif

TEST(envTestParser, AssignmentFailedTest)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getAssignmentFailedMessage("MYENVTEST", "moo"))
        .WillOnce(Return("Mock \"MYENVTEST\", \"moo\" assignment failed"));     // NOLINT

    StrictMock<argparser::mock_varg_intf> testvalvarg;
    EXPECT_CALL(testvalvarg, setValue(::testing::StrEq("moo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    SETENV("MYENVTEST","moo", 1);

    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock \"MYENVTEST\", \"moo\" assignment failed\n", output.c_str());

    UNSETENV("MYENVTEST");
}

TEST(envTestParser, RequiredArgumentTest)
{
    envparser_test testvar;
    stringMockptr stringMock = testvar.getStringsMock();
    EXPECT_CALL(*stringMock, getRequiredEnvironmentArgMissing("MYENVTEST"))
        .WillOnce(Return("Mock environment value \"MYENVTEST\" must be defined"));

    StrictMock<argparser::mock_varg_intf> testvalvarg;

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var", 1, true);
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();

    EXPECT_STREQ("Mock environment value \"MYENVTEST\" must be defined\n", output.c_str());
}

//======================================================================================
// Test functions parse
//======================================================================================

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
/** @} */
