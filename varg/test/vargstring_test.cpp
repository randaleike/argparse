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
 * @file varg_basic_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include <array>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>
#include <climits>
#include "vargstring.h"
#include <gtest/gtest.h>

/*
* String varg test
*/
TEST(varg_string, ConstructorValue)
{
    const argparser::vargstring testvar("test string");
    const std::string testValue("test string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, ValueSetPass)
{
    argparser::vargstring testvar("test string");
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("value string"));

    const std::string testValue("value string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, ValueSetFail)
{
    argparser::vargstring testvar("test string");
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue());

    const std::string testValue("test string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, IsListTest)
{
    argparser::vargstring testvar("test string");
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_string, IsEmptyTest)
{
    argparser::vargstring testvar("test string");
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(varg_string, GetAssignmentCount)
{
    argparser::vargstring testvar("test string");
    EXPECT_EQ(0, testvar.getAssignmentCount());

    testvar.setValue("short");
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TEST(varg_string, GetTypeString)
{
    argparser::vargstring testvar("test string");
    EXPECT_STREQ("string", testvar.getTypeString());
}

TEST(varg_string, GetRangeString)
{
    argparser::vargstring testvar("test string");
    EXPECT_STREQ("<string>", testvar.getRangeString());
}

TEST(varg_string, MinMaxLengthTest)
{
    argparser::vargstring testvar("", 5, 10);   // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(""));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("v"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("va"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("val"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("valu"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("value"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("value test"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("value test1"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("value test12"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("value test really over limit"));
}

/** @} */
