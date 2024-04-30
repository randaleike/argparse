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

const double testValues[] = {3.14,1.345e6,30.1,1e6,2.76};

/*
* Integer varg test
*/
template <typename T> class IntegerUnitTest : public testing::Test
{
    private:

    public:
        IntegerUnitTest() {}
        ~IntegerUnitTest() override {}

        long long int getMaxValue(void);
        long long int getMinValue(void);

        std::string getExpectedTypeString(void)
        {
            std::stringstream typeString;
            typeString << "<" << getMinValue() << ":[+]" << getMaxValue() << ">";
            return typeString.str();
        }

        std::string getMaxString(long long int overflow)
        {
            std::stringstream maxString;
            maxString << (long long int)getMaxValue() + overflow;
            return maxString.str();
        }

        std::string getMinString(long long int overflow)
        {
            std::stringstream minString;
            minString << (long long int)getMinValue() - overflow;
            return minString.str();
        }

        bool runMaxMinTest(void);
};

template <> long long int IntegerUnitTest<short int>::getMaxValue(void)       {return SHRT_MAX;}
template <> long long int IntegerUnitTest<int>::getMaxValue(void)             {return INT_MAX;}
template <> long long int IntegerUnitTest<long int>::getMaxValue(void)        {return LONG_MAX;}
template <> long long int IntegerUnitTest<long long int>::getMaxValue(void)   {return LLONG_MAX;}

template <> long long int IntegerUnitTest<short int>::getMinValue(void)       {return SHRT_MIN;}
template <> long long int IntegerUnitTest<int>::getMinValue(void)             {return INT_MIN;}
template <> long long int IntegerUnitTest<long int>::getMinValue(void)        {return LONG_MIN;}
template <> long long int IntegerUnitTest<long long int>::getMinValue(void)   {return LLONG_MIN;}

template <> bool IntegerUnitTest<long long int>::runMaxMinTest(void)        {return false;}
template <> bool IntegerUnitTest<long int>::runMaxMinTest(void)             {return false;}
template <typename T> bool IntegerUnitTest<T>::runMaxMinTest(void)          {return true;}

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
    const TypeParam testValue = 10;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueFlag)
{
    const TypeParam initValue = 10;
    const TypeParam setValue = -100;
    argparser::varg< TypeParam > testvar(initValue,setValue);
    EXPECT_EQ(initValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(setValue, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedPos)
{
    const TypeParam testValue = -21;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(33, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedNeg)
{
    const TypeParam testValue = 25;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-44"));
    EXPECT_EQ(-44, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetFail)
{
    const TypeParam testValue = 35;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(testValue, testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetMaxPass)
{
    std::string maxValue = this->getMaxString(0LL);
    const TypeParam testValue = 45;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(this->getMaxValue(), testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        std::string maxValue = this->getMaxString(1LL);
        const TypeParam testValue = 48;
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(IntegerUnitTest, ValueSetMinPass)
{
    std::string minValue = this->getMinString(0LL);
    const TypeParam testValue = -13;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minValue.c_str()));
    EXPECT_EQ(this->getMinValue(), testvar.value);
}

TYPED_TEST_P(IntegerUnitTest, ValueSetMinFail)
{
    if (this->runMaxMinTest())
    {
        std::string minValue = this->getMinString(1LL);
        const TypeParam testValue = -57;
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_LOW_e, testvar.setValue(minValue.c_str()));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(IntegerUnitTest, IsListTest)
{
    const TypeParam testValue = 11;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(IntegerUnitTest, GetTypeString)
{
    const TypeParam testValue = 12;
    argparser::varg< TypeParam > testvar(testValue);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(IntegerUnitTest, ConstructorValueSignedPos, ConstructorValueSignedNeg, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                             ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetFail, 
                                             ValueSetMaxPass, ValueSetMaxFail, ValueSetMinPass, ValueSetMinFail,
                                             IsListTest, GetTypeString);

typedef testing::Types<short int, int, long int, long long int> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(varg_int, IntegerUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerUnitTest : public testing::Test
{
    private:

    public:
        UIntegerUnitTest() {}
        ~UIntegerUnitTest() override {}

        long long unsigned getMaxValue(void);
        long long unsigned getMinValue(void)     {return 0ULL;}

        std::string getExpectedTypeString(void)
        {
            std::stringstream typeString;
            typeString << "<" << getMinValue() << ":[+]" << getMaxValue() << ">";
            return typeString.str();
        }

        std::string getMaxString(long long unsigned overflow)
        {
            std::stringstream maxString;
            maxString << (long long unsigned)getMaxValue() + overflow;
            return maxString.str();
        }

        std::string getMinString(long long unsigned overflow)
        {
            std::stringstream minString;
            minString << (long long unsigned)getMinValue() - overflow;
            return minString.str();
        }

        bool runMaxMinTest(void);
};

template <> long long unsigned UIntegerUnitTest<short unsigned>::getMaxValue(void)      {return USHRT_MAX;}
template <> long long unsigned UIntegerUnitTest<unsigned>::getMaxValue(void)            {return UINT_MAX;}
template <> long long unsigned UIntegerUnitTest<long unsigned>::getMaxValue(void)       {return ULONG_MAX;}
template <> long long unsigned UIntegerUnitTest<long long unsigned>::getMaxValue(void)  {return ULLONG_MAX;}

template <> bool UIntegerUnitTest<long long unsigned>::runMaxMinTest(void)              {return false;}
template <> bool UIntegerUnitTest<long unsigned>::runMaxMinTest(void)                   {return false;}
template <typename T> bool UIntegerUnitTest<T>::runMaxMinTest(void)                     {return true;}

TYPED_TEST_SUITE_P(UIntegerUnitTest);
TYPED_TEST_P(UIntegerUnitTest, ConstructorValue)
{
    const TypeParam testValue = 10;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueFlag)
{
    const TypeParam initValue = 10;
    const TypeParam setValue = 33;
    argparser::varg< TypeParam > testvar(initValue, setValue);
    EXPECT_EQ(initValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(setValue, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueDefaultFlag)
{
    const TypeParam testValue = 12;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetPass)
{
    const TypeParam testValue = 20;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(33, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetFail)
{
    const TypeParam testValue = 30;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ(testValue, testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetMaxPass)
{
    std::string maxValue = this->getMaxString(0ULL);
    const TypeParam testValue = 45;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(this->getMaxValue(), testvar.value);
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        std::string maxValue = this->getMaxString(1ULL);
        const TypeParam testValue = 48;
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(UIntegerUnitTest, IsListTest)
{
    const TypeParam testValue = 31;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(UIntegerUnitTest, GetTypeString)
{
    const TypeParam testValue = 32;
    argparser::varg< TypeParam > testvar(testValue);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(UIntegerUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                              ValueSetPass, ValueSetFail, ValueSetMaxPass, ValueSetMaxFail, 
                                              IsListTest, GetTypeString);

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
    const TypeParam testValue = 3.14156;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ConstructorValueFlag)
{
    const TypeParam initValue = 3.14;
    const TypeParam setValue = 2.76;
    argparser::varg< TypeParam > testvar(initValue, setValue);
    EXPECT_EQ(initValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(setValue, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ConstructorValueDefaultFlag)
{
    const TypeParam testValue = 3.14;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(testValue, testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(0.0, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_simple)
{
    const TypeParam testValue = 2.76;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_EQ(3.1415, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_exponent)
{
    const TypeParam testValue = 1e6;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("1.59e4"));
    EXPECT_EQ(1.59e4, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetPass_integer)
{
    const TypeParam testValue = 100;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("5"));
    EXPECT_EQ(5.0, testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetFail)
{
    argparser::varg< TypeParam > testvar(testValues[0]);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
}

TYPED_TEST_P(FloatUnitTest, ValueSetMaxPass)
{
    argparser::varg< TypeParam > testvar(testValues[1]);
    EXPECT_EQ(1.345e6, testvar.value);

    std::stringstream maxString;
    maxString.precision(DBL_MANT_DIG);
    maxString << std::numeric_limits<double>::max();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxString.str().c_str()));
    EXPECT_EQ(std::numeric_limits<double>::max(), testvar.value);
}

TYPED_TEST_P(FloatUnitTest, ValueSetMinPass)
{
    argparser::varg< TypeParam > testvar(testValues[1]);
    EXPECT_EQ(1.345e6, testvar.value);

    std::stringstream minString;
    minString.precision(DBL_MANT_DIG);
    minString << std::numeric_limits<double>::min();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minString.str().c_str()));
    EXPECT_EQ(std::numeric_limits<double>::min(), testvar.value);
}

TYPED_TEST_P(FloatUnitTest, IsListTest)
{
    argparser::varg< TypeParam > testvar(testValues[2]);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(FloatUnitTest, GetTypeString)
{
    argparser::varg< TypeParam > testvar(testValues[2]);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(FloatUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                           ValueSetPass_simple, ValueSetPass_exponent, ValueSetFail, ValueSetPass_integer,
                                           ValueSetMaxPass, ValueSetMinPass, IsListTest, GetTypeString);

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
