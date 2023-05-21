/*
Copyright (c) 2022-2023 Randal Eike

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
 * @file varg_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg.h"
#include "vargincrement.h"
#include "vargenum.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include <limits.h>

/*
* Integer varg test
*/
template <typename T> class IntegerUnitTest : public testing::Test
{
    private:
        std::stringstream typeString;
        std::string generateExpectedString(long long int min, long long int max)
        {
            typeString << "<" << min << ":[+]" << max << ">";
            return typeString.str();
        }

    public:
        IntegerUnitTest() {}
        ~IntegerUnitTest() override {}

        std::string getExpectedTypeString(void);
};

template <> std::string IntegerUnitTest<short int>::getExpectedTypeString(void)     {return generateExpectedString(SHRT_MIN, SHRT_MAX);}
template <> std::string IntegerUnitTest<int>::getExpectedTypeString(void)           {return generateExpectedString(INT_MIN, INT_MAX);}
template <> std::string IntegerUnitTest<long int>::getExpectedTypeString(void)      {return generateExpectedString(LONG_MIN, LONG_MAX);}
template <> std::string IntegerUnitTest<long long int>::getExpectedTypeString(void) {return generateExpectedString(LLONG_MIN, LLONG_MAX);}

TYPED_TEST_SUITE_P(IntegerUnitTest);
TYPED_TEST_P(IntegerUnitTest, ConstructorValueSignedPos)
{
    argparser::varg< TypeParam > testvar(1);
    EXPECT_EQ(1, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueSignedNeg)
{
    argparser::varg< TypeParam > testvar(-1);
    EXPECT_EQ(-1, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueDefaultFlag)
{
    argparser::varg< TypeParam > testvar(10);
    EXPECT_EQ(10, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueFlag)
{
    argparser::varg< TypeParam > testvar(10,-100);
    EXPECT_EQ(10, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(-100, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedPos)
{
    argparser::varg< TypeParam > testvar(-20);
    EXPECT_EQ(-20, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(33, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedNeg)
{
    argparser::varg< TypeParam > testvar(20);
    EXPECT_EQ(20, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-44"));
    EXPECT_EQ(-44, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetFail)
{
    argparser::varg< TypeParam > testvar(30);
    EXPECT_EQ(30, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(30, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, IsListTest)
{
    argparser::varg< TypeParam > testvar(30);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(IntegerUnitTest, GetTypeString)
{
    argparser::varg< TypeParam > testvar(30);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(IntegerUnitTest, ConstructorValueSignedPos, ConstructorValueSignedNeg, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                             ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetFail, IsListTest, GetTypeString);

typedef testing::Types<short int, int, long int, long long int> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(varg_int, IntegerUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerUnitTest : public testing::Test
{
    private:
        std::stringstream typeString;
        std::string generateExpectedString(long long unsigned max)
        {
            typeString << "<0:[+]" << max << ">";
            return typeString.str();
        }

    public:
        UIntegerUnitTest() {}
        ~UIntegerUnitTest() override {}

        std::string getExpectedTypeString(void);
};

template <> std::string UIntegerUnitTest<short unsigned>::getExpectedTypeString(void)       {return generateExpectedString(USHRT_MAX);}
template <> std::string UIntegerUnitTest<unsigned>::getExpectedTypeString(void)             {return generateExpectedString(UINT_MAX);}
template <> std::string UIntegerUnitTest<long unsigned>::getExpectedTypeString(void)        {return generateExpectedString(ULONG_MAX);}
template <> std::string UIntegerUnitTest<long long unsigned>::getExpectedTypeString(void)   {return generateExpectedString(ULLONG_MAX);}

TYPED_TEST_SUITE_P(UIntegerUnitTest);
TYPED_TEST_P(UIntegerUnitTest, ConstructorValue)
{
    argparser::varg< TypeParam > testvar(10);
    EXPECT_EQ(10, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueFlag)
{
    argparser::varg< TypeParam > testvar(10, 100);
    EXPECT_EQ(10, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(100, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueDefaultFlag)
{
    argparser::varg< TypeParam > testvar(10);
    EXPECT_EQ(10, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetPass)
{
    argparser::varg< TypeParam > testvar(20);
    EXPECT_EQ(20, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(33, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetFail)
{
    argparser::varg< TypeParam > testvar(30);
    EXPECT_EQ(30, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ(30, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, IsListTest)
{
    argparser::varg< TypeParam > testvar(30);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(UIntegerUnitTest, GetTypeString)
{
    argparser::varg< TypeParam > testvar(30);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(UIntegerUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                              ValueSetPass, ValueSetFail, IsListTest, GetTypeString);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(varg_uint, UIntegerUnitTest, uintTypes);

/*
* Float varg test
*/
template <typename T> class FloatUnitTest : public testing::Test
{
    private:
        std::stringstream typeString;

    public:
        FloatUnitTest() {}
        ~FloatUnitTest() override {}

        std::string getExpectedTypeString(void);
};

template <> std::string FloatUnitTest<double>::getExpectedTypeString(void) 
{
    typeString << "<" << std::numeric_limits<double>::min() << ":" << std::numeric_limits<double>::max() << ">";
    return typeString.str();
}

TYPED_TEST_SUITE_P(FloatUnitTest);

TYPED_TEST_P(FloatUnitTest, ConstructorValue)
{
    const argparser::varg< TypeParam > testvar(3.14);
    EXPECT_EQ(3.14, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ConstructorValueFlag)
{
    argparser::varg< TypeParam > testvar(3.14, 2.76);
    EXPECT_EQ(3.14, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(2.76, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ConstructorValueDefaultFlag)
{
    argparser::varg< TypeParam > testvar(3.14);
    EXPECT_EQ(3.14, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0.0, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_simple)
{
    argparser::varg< TypeParam > testvar(2.76);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_EQ(3.1415, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_exponent)
{
    argparser::varg< TypeParam > testvar(1e6);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("1.59e4"));
    EXPECT_EQ(1.59e4, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_integer)
{
    argparser::varg< TypeParam > testvar(100);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("5"));
    EXPECT_EQ(5.0, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetFail)
{
    argparser::varg< TypeParam > testvar(3.14);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
}

TYPED_TEST_P(FloatUnitTest, IsListTest)
{
    argparser::varg< TypeParam > testvar(30.1);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(FloatUnitTest, GetTypeString)
{
    argparser::varg< TypeParam > testvar(30.1);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(FloatUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                           ValueSetPass_simple, ValueSetPass_exponent, ValueSetFail, ValueSetPass_integer,
                                           IsListTest, GetTypeString);

typedef testing::Types<double> floatTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(varg_float, FloatUnitTest, floatTypes);

/*
* Bool varg test
*/
TEST(varg_bool, ConstructorValue)
{
    const argparser::varg<bool> testvar(true);
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ConstructorValueFlag)
{
    argparser::varg<bool> testvar(false, true);
    EXPECT_FALSE(testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_true_singlechar)
{
    argparser::varg<bool> testvar(false);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_true_singlechar_cap)
{
    argparser::varg<bool> testvar(false);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("T"));
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_true_word)
{
    argparser::varg<bool> testvar(false);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("true"));
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_true_word_cap)
{
    argparser::varg<bool> testvar(false);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("TRUE"));
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_true_digit)
{
    argparser::varg<bool> testvar(false);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("1"));
    EXPECT_TRUE(testvar.value);
}

TEST(varg_bool, ValueSetPass_false_singlechar)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_FALSE(testvar.value);
}

TEST(varg_bool, ValueSetPass_false_singlechar_cap)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("F"));
    EXPECT_FALSE(testvar.value);
}
TEST(varg_bool, ValueSetPass_false_word)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("false"));
    EXPECT_FALSE(testvar.value);
}
TEST(varg_bool, ValueSetPass_false_word_cap)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("FALSE"));
    EXPECT_FALSE(testvar.value);
}
TEST(varg_bool, ValueSetPass_false_digit)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("0"));
    EXPECT_FALSE(testvar.value);
}

TEST(varg_bool, ValueSetFail)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
}

TEST(varg_bool, ValueSetFail_digit)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("2"));
}

TEST(varg_bool, IsListTest)
{
    argparser::varg<bool> testvar(false);
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_bool, GetTypeString)
{
    argparser::varg<bool> testvar(false);
    EXPECT_STREQ("<t|T|f|F>", testvar.getTypeString());
}

/*
* Character varg test
*/
TEST(varg_char, ConstructorValue)
{
    const argparser::varg<char> testvar('t');
    EXPECT_EQ('t', testvar.value);
}

TEST(varg_char, ConstructorValueFlag)
{
    argparser::varg<char> testvar('t', 'f');
    EXPECT_EQ('t', testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ('f', testvar.value);
}

TEST(varg_char, ValueSetPass)
{
    argparser::varg<char> testvar('a');
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_EQ('b', testvar.value);
}

TEST(varg_char, ValueSetFail)
{
    argparser::varg<char> testvar('c');
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
}

TEST(varg_char, IsListTest)
{
    argparser::varg<char> testvar('d');
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_char, GetTypeString)
{
    argparser::varg<char> testvar('e');
    EXPECT_STREQ("<char>", testvar.getTypeString());
}

/*
* String varg test
*/
TEST(varg_string, ConstructorValue)
{
    const argparser::varg<std::string> testvar("test string");
    const std::string testValue("test string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, ConstructorValueFlag)
{
    argparser::varg<std::string> testvar("test string", "flag string");
    const std::string testValue("test string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());

    const std::string flagValue("flag string");
    EXPECT_STREQ(flagValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, ValueSetPass)
{
    argparser::varg<std::string> testvar("test string");
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("value string"));

    const std::string testValue("value string");
    EXPECT_STREQ(testValue.c_str(), testvar.value.c_str());
}

TEST(varg_string, IsListTest)
{
    argparser::varg<std::string> testvar("test string");
    EXPECT_FALSE(testvar.isList());
}

TEST(varg_string, GetTypeString)
{
    argparser::varg<std::string> testvar("test string");
    EXPECT_STREQ("<string>", testvar.getTypeString());
}

/** @} */
