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
 * @file vargptrenum_test.cpp
 * @ingroup libvarg_unittest
 * @defgroup variable argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg_intf.h"
#include "../src/vargenum_clib_private.h"
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

TEST(vargptr_enum, ConstructorTest)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_EQ(test_enum_e::defaultValue_e, enumval);
}

TEST(vargptr_enum, SetValue)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_EQ(test_enum_e::defaultValue_e, enumval);

    testvar.setEnumValue("default", test_enum_e::defaultValue_e);
    testvar.setEnumValue("first", test_enum_e::firstVal_e);
    testvar.setEnumValue("second", test_enum_e::secondVal_e);
    testvar.setEnumValue("third", test_enum_e::thirdVal_e);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("first"));
    EXPECT_EQ(test_enum_e::firstVal_e, enumval);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("second"));
    EXPECT_EQ(test_enum_e::secondVal_e, enumval);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("third"));
    EXPECT_EQ(test_enum_e::thirdVal_e, enumval);
}

TEST(vargptr_enum, SetValueFail)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_EQ(test_enum_e::defaultValue_e, enumval);

    testvar.setEnumValue("default", test_enum_e::defaultValue_e);
    testvar.setEnumValue("first", test_enum_e::firstVal_e);
    testvar.setEnumValue("second", test_enum_e::secondVal_e);
    testvar.setEnumValue("third", test_enum_e::thirdVal_e);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("notthere"));
    EXPECT_EQ(test_enum_e::defaultValue_e, enumval);
}

TEST(vargptr_enum, IsListTest)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_FALSE(testvar.isList());
}

TEST(vargptr_enum, IsEmptyTest)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(vargptr_enum, GetAssignmentCount)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TEST(vargptr_enum, GetTypeStringDefault)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval);
    EXPECT_STREQ("enum value", testvar.getTypeString());
}

TEST(vargptr_enum, GetTypeString)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_STREQ("test_enum_e", testvar.getTypeString());
}

TEST(vargptr_enum, SetValueNoParmTest)
{
    int enumval = 0; // NOLINT
    argparser::vargptrenum testvar(&enumval, "test_enum_e");
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue());
}

TEST(vargptr_enum, GetRangeString)
{
    argparser::vargptrenum testvar(nullptr, "test_enum_e");
    testvar.setEnumValue("default", test_enum_e::defaultValue_e);
    testvar.setEnumValue("first", test_enum_e::firstVal_e);
    testvar.setEnumValue("second", test_enum_e::secondVal_e);
    testvar.setEnumValue("third", test_enum_e::thirdVal_e);
    EXPECT_STREQ("<default|first|second|third>", testvar.getRangeString());
}

/** @} */
