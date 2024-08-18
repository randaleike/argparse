/*
 Copyright (c) 2023-2024 Randal Eike

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
 * @file vargincrement_test.cpp
 * @ingroup libvarg_unittest
 * @defgroup variable argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg.h"
#include "vargincrement.h"
#include <gtest/gtest.h>

/*
* Incrementing varg test
*/
TEST(varg_increment, ConstructorTest)
{
    const argparser::vargincrement testvar;
    EXPECT_EQ(0, testvar.value);
}

TEST(varg_increment, ValueSetPass_singleInc)
{
    argparser::vargincrement testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(1, testvar.value);
}

TEST(varg_increment, ValueSetPass_doubleInc)
{
    argparser::vargincrement testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(2, testvar.value);
}

TEST(varg_increment, ValueSetPass_tripleInc)
{
    argparser::vargincrement testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(3, testvar.value);
}

TEST(varg_increment, ValueSetFail)
{
    argparser::vargincrement testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("10"));
    EXPECT_EQ(0, testvar.value);
}

TEST(varg_increment, ValueSetFail_AfterInc)
{
    argparser::vargincrement testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("10"));
    EXPECT_EQ(1, testvar.value);
}

TEST(varg_increment, IsListTest)
{
    argparser::vargincrement testvar;
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_increment, GetTypeString)
{
    argparser::vargincrement testvar;
    EXPECT_STREQ("incrementing flag", testvar.getTypeString());
}

/** @} */
