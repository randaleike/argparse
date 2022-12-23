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
 * @file listvarg_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes 
#include "parser_string_list.h"
#include <gtest/gtest.h>

//======================================================================================
// Public Interface testing, English list
//======================================================================================

TEST(BaseParserStringList, printNotListTypeMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printNotListTypeMessage(std::cout, 3);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("Only list type arguments can have an argument count of 3\n", output.c_str());
}

TEST(BaseParserStringList, printUnknownArgumentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printUnknownArgumentMessage(std::cout, "--foo");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("Unkown argument --foo\n", output.c_str());
}

TEST(BaseParserStringList, printInvalidAssignmentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printInvalidAssignmentMessage(std::cout, "--foo");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"--foo\" invalid assignment\n", output.c_str());
}

TEST(BaseParserStringList, printAssignmentFailedMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printAssignmentFailedMessage(std::cout, "--var", "13");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"--var 13\" assignment failed\n", output.c_str());
}

TEST(BaseParserStringList, printMissingAssignmentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printMissingAssignmentMessage(std::cout, "--mytest");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"--mytest\" missing assignment value\n", output.c_str());
}

TEST(BaseParserStringList, printMissingListAssignmentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printMissingListAssignmentMessage(std::cout, "/goo", 4, 3);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"/goo\" missing assignment. Expected: 4 found: 3 arguments\n", output.c_str());
}

TEST(BaseParserStringList, printTooManyAssignmentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printTooManyAssignmentMessage(std::cout, "--test", 2, 3);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"--test\" too many assignment values. Expected: 2 found: 3 arguments\n", output.c_str());
}

TEST(BaseParserStringList, printMissingArgumentMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printMissingArgumentMessage(std::cout, "-t");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("\"-t\" required argument missing\n", output.c_str());
}

TEST(BaseParserStringList, printArgumentCreationError)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printArgumentCreationError(std::cout, "--test,-t");
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("Argument add failed: --test,-t\n", output.c_str());
}

TEST(BaseParserStringList, printEnvironmentNoFlags)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    

    testing::internal::CaptureStdout();
    testvar->printEnvironmentNoFlags(std::cout, "testenvvar");        
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("Environment value testenvvar narg must be > 0\n", output.c_str());
}

TEST(BaseParserStringList, getUsageMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr testString = testvar->getUsageMessage();
    EXPECT_STREQ("Usage:", testString.c_str());
}

TEST(BaseParserStringList, getPositionalArgumentsMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr testString = testvar->getPositionalArgumentsMessage();
    EXPECT_STREQ("Positional Arguments:", testString.c_str());
}

TEST(BaseParserStringList, getSwitchArgumentsMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr testString = testvar->getSwitchArgumentsMessage();
    EXPECT_STREQ("Optional Arguments:", testString.c_str());
}

TEST(BaseParserStringList, getHelpString)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr testString = testvar->getHelpString();
    EXPECT_STREQ("show this help message and exit", testString.c_str());
}

TEST(BaseParserStringList, getEnvArgumentsMessage)
{
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr testString = testvar->getEnvArgumentsMessage();
    EXPECT_STREQ("Environment values:", testString.c_str());
}

/// @todo add non-english tests

//======================================================================================
// Public Interface testing, english
//======================================================================================
TEST(BaseParserStringList, formatToLength) 
{ 
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr baseString = "This is a test string that will be broken into two strings";

    std::list<parserstr> strList = testvar->formatStringToLength(baseString, {' '}, 32);
    EXPECT_EQ(2, strList.size());
    EXPECT_STREQ("This is a test string that will ", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("be broken into two strings      ", strList.front().c_str());
} 

TEST(BaseParserStringList, formatToLengthPad) 
{ 
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr baseString = "Padded test string";

    std::list<parserstr> strList = testvar->formatStringToLength(baseString, {' '}, 32);
    EXPECT_EQ(1, strList.size());
    EXPECT_STREQ("Padded test string              ", strList.front().c_str());
} 

TEST(BaseParserStringList, formatToLengthExactlyRight) 
{ 
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr baseString = "Perfect test string";

    std::list<parserstr> strList = testvar->formatStringToLength(baseString, {' '}, baseString.size());
    EXPECT_EQ(1, strList.size());
    EXPECT_STREQ("Perfect test string", strList.front().c_str());
} 

TEST(BaseParserStringList, formatToLengthAwkward) 
{ 
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr baseString = "This is a test string that will be broken into two strings";

    std::list<parserstr> strList = testvar->formatStringToLength(baseString, {';'}, 30);
    EXPECT_STREQ("This is a test string that wil", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("l be broken into two strings  ", strList.front().c_str());
}

TEST(BaseParserStringList, formatToLengthDoubleBreak) 
{ 
    argparser::BaseParserStringList* testvar = argparser::BaseParserStringList::getInternationalizedClass();    
    parserstr baseString = "This is a test string that; will be broken into two strings";

    std::list<parserstr> strList = testvar->formatStringToLength(baseString, {';', ' '}, 31);
    EXPECT_STREQ("This is a test string that;    ", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("will be broken into two strings", strList.front().c_str());
}

int parser_string_list_main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
/** @} */
