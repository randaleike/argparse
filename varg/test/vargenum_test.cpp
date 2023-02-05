/*
Copyright (c) 2023 Randal Eike

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
 * @file vargenum_test.cpp
 * @ingroup varg_unittest
 * @defgroup variable argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg.h"
#include "vargenum.h"
#include <gtest/gtest.h>

/*
* Enum varg test
*/
enum test_enum_e
{
    defaultValue_e = 0,
    firstVal_e,
    secondVal_e,
    thirdVal_e,
};

TEST(varg_enum, ConstructorTest)
{
    argparser::vargenum<test_enum_e> testvar(test_enum_e::defaultValue_e, "test_enum_e");
    EXPECT_EQ(test_enum_e::defaultValue_e, testvar.value);
}

TEST(varg_enum, SetValue)
{
    argparser::vargenum<test_enum_e> testvar(test_enum_e::defaultValue_e, "test_enum_e");
    EXPECT_EQ(test_enum_e::defaultValue_e, testvar.value);

    testvar.setEnumValue("default", test_enum_e::defaultValue_e);
    testvar.setEnumValue("first", test_enum_e::firstVal_e);
    testvar.setEnumValue("second", test_enum_e::secondVal_e);
    testvar.setEnumValue("third", test_enum_e::thirdVal_e);

    EXPECT_TRUE(testvar.setValue("first"));
    EXPECT_EQ(test_enum_e::firstVal_e, testvar.value);

    EXPECT_TRUE(testvar.setValue("second"));
    EXPECT_EQ(test_enum_e::secondVal_e, testvar.value);

    EXPECT_TRUE(testvar.setValue("third"));
    EXPECT_EQ(test_enum_e::thirdVal_e, testvar.value);
}

TEST(varg_enum, IsListTest)
{
    argparser::vargenum<test_enum_e> testvar(test_enum_e::defaultValue_e, "test_enum_e");
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_enum, GetTypeStringDefault)
{
    argparser::vargenum<test_enum_e> testvar(test_enum_e::defaultValue_e);
    EXPECT_STREQ("enum value", testvar.getTypeString());
}

TEST(varg_enum, GetTypeString)
{
    argparser::vargenum<test_enum_e> testvar(test_enum_e::defaultValue_e, "test_enum_e");
    EXPECT_STREQ("test_enum_e", testvar.getTypeString());
}

/** @} */
