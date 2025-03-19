/*
 Copyright (c) 2025 Randal Eike

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
 * @file vargcstring_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "../src/vargcstring_clib_private.h"
#include "varg_intf.h"
#include <gtest/gtest.h>


/*
* String varg test
*/
TEST(cstringvarg, ValueSetPass)
{
    char testArray[20];                             // NOLINT
    argparser::vargcstring testvar(testArray, 20);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("Test String"));
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(11, testvar.getAssignmentCount());
    EXPECT_STREQ("Test String", (&testArray[0]));   // NOLINT
}

TEST(cstringvarg, GetTypeString)
{
    char testArray[20];                             // NOLINT
    argparser::vargcstring testvar(testArray, 20);  // NOLINT
    EXPECT_STREQ("<string>", testvar.getTypeString());
}

TEST(cstringvarg, IsListTest)
{
    char testArray[20];                             // NOLINT
    argparser::vargcstring testvar(testArray, 20);  // NOLINT
    EXPECT_FALSE(testvar.isList());
}

TEST(cstringvarg, SetFlagValue)
{
    char testArray[20];                             // NOLINT
    argparser::vargcstring testvar(testArray, 20);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue());
}

/** @} */
