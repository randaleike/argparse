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
#include <stdlib.h>
#include "varg.h"
#include "listvarg.h"
#include "envparse.h"
#include <gtest/gtest.h>

const size_t defaultArgWidth = 14;
const size_t testArgWidth = 21;
const size_t testArgWidth2 = 29;
const size_t defaultColWidth = 80;
const int    testValue = 10;

//======================================================================================
// String Helper functions
//======================================================================================
parserstr getEnvHelpHeader()
{
    argparser::BaseParserStringList* parserStr = argparser::BaseParserStringList::getInternationalizedClass();
    return parserStr->getEnvArgumentsMessage()+"\n";
}

parserstr getEnvKeyHelpMsg(parserstr keys, parserstr keyhelp, size_t argWidth = defaultArgWidth, size_t consoleWidth = defaultColWidth)
{
    parserstr argMsg = " " + keys + '=' + keys;
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

//======================================================================================
// Test functions constructor and help
//======================================================================================
TEST(envparse, ConstructorBasic) 
{ 
    argparser::envparser testvar;
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr;
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
} 

TEST(envparse, addArgument) 
{ 
    argparser::envparser testvar;
    argparser::varg<int> testvalvarg(0);

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getEnvHelpHeader() + getEnvKeyHelpMsg("MYENVTEST", "My environment test var", testArgWidth);
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
} 

TEST(envparse, addArgument2) 
{ 
    argparser::envparser testvar;
    argparser::varg<int> testvalvarg(0);
    argparser::varg<int> testvalvarg1(0);

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expectedStr = getEnvHelpHeader() + 
                            getEnvKeyHelpMsg("MYENVTEST", "My environment test var", testArgWidth2) + 
                            getEnvKeyHelpMsg("SECONDENVTEST", "My second environment test var", testArgWidth2);;
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
} 

TEST(envparse, addArgumentNargZero) 
{ 
    argparser::envparser testvar;
    argparser::varg<int> testvalvarg(0);

    testing::internal::CaptureStderr();
    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var",0);
    parserstr output = testing::internal::GetCapturedStderr();
    parserstr expectedStr = "Environment value MYENVTEST narg must be > 0\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
} 

TEST(envparse, addArgumentInvalidNargs) 
{ 
    argparser::envparser testvar; 
    argparser::varg<int> testvarg(testValue);

    testing::internal::CaptureStderr();
    testvar.addArgument(&testvarg, "tstint", "This is the test key argument", 2); 
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Only list type arguments can have an argument count of 2\n", output.c_str());
} 

TEST(envparse, parsetest) 
{ 
    argparser::envparser testvar;
    argparser::varg<int> testvalvarg(0);
    argparser::varg<int> testvalvarg1(0);

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    setenv("MYENVTEST","10", 1);
    unsetenv("SECONDENVTEST");
    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(10, testvalvarg.value);
    EXPECT_EQ(0, testvalvarg1.value);
    unsetenv("MYENVTEST");
    unsetenv("SECONDENVTEST");
} 

TEST(envparse, parsetestDual) 
{ 
    argparser::envparser testvar;
    argparser::varg<int> testvalvarg(0);
    argparser::varg<bool> testvalvarg1(false);

    testvar.addArgument(&testvalvarg, "MYENVTEST", "My environment test var");
    testvar.addArgument(&testvalvarg1, "SECONDENVTEST", "My second environment test var");

    setenv("MYENVTEST","10", 1);
    setenv("SECONDENVTEST","true", 1);
    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(10, testvalvarg.value);
    EXPECT_EQ(true, testvalvarg1.value);
    unsetenv("MYENVTEST");
    unsetenv("SECONDENVTEST");
} 

TEST(envparse, parsetestlist) 
{ 
    argparser::envparser testvar;
    argparser::listvarg<int> testlistvarg;
    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 2);

    setenv("MYENVTEST","10,21", 1);
    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(10, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(21, testlistvarg.value.front());
    unsetenv("MYENVTEST");
} 

TEST(envparse, parsetestlistTooFew) 
{ 
    argparser::envparser testvar;
    argparser::listvarg<int> testlistvarg;
    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 3);

    setenv("MYENVTEST","10,21", 1);
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"MYENVTEST\" missing assignment. Expected: 3 found: 2 arguments\n", output.c_str());
    unsetenv("MYENVTEST");
} 

TEST(envparse, parsetestlistTooMany) 
{ 
    argparser::envparser testvar(false);
    argparser::listvarg<int> testlistvarg;
    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", 3);

    setenv("MYENVTEST","10,21,32,45", 1);
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    EXPECT_EQ(0, testlistvarg.value.size());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("\"MYENVTEST\" too many assignment values. Expected: 3 found: 4 arguments\n", output.c_str());
    testing::internal::CaptureStderr();
    unsetenv("MYENVTEST");
} 

TEST(envparse, parseTestAddDynamicListArg) 
{ 
    argparser::envparser testvar(false);
    argparser::listvarg<int> testlistvarg;

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -3);
    setenv("MYENVTEST","18,22,43", 1);

    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(3, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(43, testlistvarg.value.front());
} 

TEST(envparse, parseTestAddDynamicList2Arg) 
{ 
    argparser::envparser testvar(false);
    argparser::listvarg<int> testlistvarg;

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -3);
    setenv("MYENVTEST","18,22", 1);

    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
} 

TEST(envparse, parseTestAddDynamicListIndefinite2Arg) 
{ 
    argparser::envparser testvar(false);
    argparser::listvarg<int> testlistvarg;

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -1);
    setenv("MYENVTEST","18,22", 1);

    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(2, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
} 

TEST(envparse, parseTestAddDynamicListIndefinite6Arg) 
{ 
    argparser::envparser testvar(false);
    argparser::listvarg<int> testlistvarg;

    testvar.addArgument(&testlistvarg, "MYENVTEST", "My environment test var", -1);
    setenv("MYENVTEST","18,22,13,12,11,55", 1);

    EXPECT_TRUE(testvar.parse());
    EXPECT_EQ(6, testlistvarg.value.size());
    EXPECT_EQ(18, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(22, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(13, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(12, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(11, testlistvarg.value.front());
    testlistvarg.value.pop_front();
    EXPECT_EQ(55, testlistvarg.value.front());
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
