/*
 Copyright (c) 2024-2025 Randal Eike

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
 * @file vargptr_basic_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include <gtest/gtest.h>
#include <array>
#include <string>
#include <sstream>
#include "../src/vargptr_clib_private.h"

/*
* Integer vargptr test
*/
template <typename T> class IntegerPtrUnitTest : public testing::Test
{
    private:

    protected:
        const std::array<T, 13> initValues {{0, 1, -1, 10, -11, 12, 55, -133, 116, -1958, 1876, 11653, -15639}};    // NOLINT
        const std::array<T, 12> flagValues {{2, -2, 20, -21, 32, 66, -233, 126, -1948, 1856, 12653, -13639}};       // NOLINT

    public:
        IntegerPtrUnitTest() = default;
        IntegerPtrUnitTest(const IntegerPtrUnitTest& other) = default;
        IntegerPtrUnitTest(IntegerPtrUnitTest&& other) = default;
        IntegerPtrUnitTest& operator=(const IntegerPtrUnitTest& other) = default;
        IntegerPtrUnitTest& operator=(IntegerPtrUnitTest&& other) = default;
        ~IntegerPtrUnitTest() override = default;

        long long int getMaxValue();
        long long int getMinValue();

        std::string getExpectedRangeString()
        {
            std::stringstream typeString;
            typeString << "<" << getMinValue() << ":" << getMaxValue() << ">";
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

        bool runMaxMinTest();
};

template <> long long int IntegerPtrUnitTest<short int>::getMaxValue()       {return SHRT_MAX;}
template <> long long int IntegerPtrUnitTest<int>::getMaxValue()             {return INT_MAX;}
template <> long long int IntegerPtrUnitTest<long int>::getMaxValue()        {return LONG_MAX;}
template <> long long int IntegerPtrUnitTest<long long int>::getMaxValue()   {return LLONG_MAX;}

template <> long long int IntegerPtrUnitTest<short int>::getMinValue()       {return SHRT_MIN;}
template <> long long int IntegerPtrUnitTest<int>::getMinValue()             {return INT_MIN;}
template <> long long int IntegerPtrUnitTest<long int>::getMinValue()        {return LONG_MIN;}
template <> long long int IntegerPtrUnitTest<long long int>::getMinValue()   {return LLONG_MIN;}

template <> bool IntegerPtrUnitTest<long long int>::runMaxMinTest()        {return false;}
template <> bool IntegerPtrUnitTest<long int>::runMaxMinTest()             {return false;}
template <typename T> bool IntegerPtrUnitTest<T>::runMaxMinTest()          {return true;}

TYPED_TEST_SUITE_P(IntegerPtrUnitTest);
TYPED_TEST_P(IntegerPtrUnitTest, ConstructorValueSignedPos)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(testValue, value);
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, ConstructorValueDefaultFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(testValue, value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
        EXPECT_EQ(0, value);
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, ConstructorValueFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        for (const TypeParam setValue : this->flagValues)
        {
            TypeParam value = testValue;
            argparser::vargptr< TypeParam > testvar(&value, setValue);
            EXPECT_EQ(testValue, value);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
            EXPECT_EQ(setValue, value);
        }
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetPassSignedPos)
{
    const std::array<int, 12> assignValues {{2, -2, 33, -21, 32, 66,
                                             -233, 126, -1948, 1856, 12653, -13639}};

    for (const TypeParam testValue : this->initValues)
    {
        for (const TypeParam assignValue : assignValues)
        {
            TypeParam value = testValue;
            argparser::vargptr< TypeParam > testvar(&value);
            EXPECT_EQ(testValue, value);

            std::string setString = std::to_string(assignValue);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
            EXPECT_EQ(assignValue, value);
        }
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetFail)
{
    const TypeParam initValue = 35;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(initValue, testValue);
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetMaxPass)
{
    const TypeParam initValue = 45;
    TypeParam testValue = initValue;
    std::string maxValue = this->getMaxString(0LL);
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(this->getMaxValue(), testValue);
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        const TypeParam initValue = 48;
        TypeParam testValue = initValue;
        std::string maxValue = this->getMaxString(1LL);

        argparser::vargptr< TypeParam > testvar(&testValue);
        EXPECT_EQ(initValue, testValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(initValue, testValue);
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetMinPass)
{
    const TypeParam initValue = -13;
    TypeParam testValue = initValue;
    std::string minValue = this->getMinString(0LL);

    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minValue.c_str()));
    EXPECT_EQ(this->getMinValue(), testValue);
}

TYPED_TEST_P(IntegerPtrUnitTest, ValueSetMinFail)
{
    if (this->runMaxMinTest())
    {
        const TypeParam initValue = -57;
        TypeParam testValue = initValue;
        std::string minValue = this->getMinString(1LL);

        argparser::vargptr< TypeParam > testvar(&testValue);
        EXPECT_EQ(initValue, testValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(minValue.c_str()));
        EXPECT_EQ(initValue, testValue);
    }
}

TYPED_TEST_P(IntegerPtrUnitTest, IsListTest)
{
    const TypeParam initValue = 11;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(IntegerPtrUnitTest, IsNotEmptyTest)
{
    TypeParam testValue = 0;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(IntegerPtrUnitTest, IsEmptyTest)
{
    argparser::vargptr< TypeParam > testvar(nullptr);
    EXPECT_TRUE(testvar.isEmpty());
}

TYPED_TEST_P(IntegerPtrUnitTest, GetRangeString)
{
    const TypeParam initValue = 12;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    std::string expectedString = this->getExpectedRangeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getRangeString());
}

TYPED_TEST_P(IntegerPtrUnitTest, GetTypeString)
{
    const TypeParam initValue = 12;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_STREQ("integer", testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(IntegerPtrUnitTest, ConstructorValueSignedPos, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                             ValueSetPassSignedPos, ValueSetFail,
                                             ValueSetMaxPass, ValueSetMaxFail, ValueSetMinPass, ValueSetMinFail,
                                             IsListTest, IsNotEmptyTest, IsEmptyTest, GetRangeString, GetTypeString);

typedef testing::Types<short int, int, long int, long long int> intTypes;   // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(vargptr_int, IntegerPtrUnitTest, intTypes);

/*
* Unsigned Integer vargptr test
*/
template <typename T> class UIntegerPtrUnitTest : public testing::Test
{
    private:

    protected:
        const std::array<T, 13> initValues {{0, 1, 5, 10, 31, 12, 55, 133, 116, 1958, 1876, 11653, 15639}}; // NOLINT
        const std::array<T, 12> flagValues {{2, 2, 20, 21, 32, 66, 233, 126, 1948, 1856, 12653, 13639}};    // NOLINT

    public:
        UIntegerPtrUnitTest() = default;
        UIntegerPtrUnitTest(const UIntegerPtrUnitTest& other) = default;
        UIntegerPtrUnitTest(UIntegerPtrUnitTest&& other) = default;
        UIntegerPtrUnitTest& operator=(const UIntegerPtrUnitTest& other) = default;
        UIntegerPtrUnitTest& operator=(UIntegerPtrUnitTest&& other) = default;
        ~UIntegerPtrUnitTest() override = default;

        long long unsigned getMaxValue();
        long long unsigned getMinValue()     {return 0ULL;}

        std::string getExpectedRangeString()
        {
            std::stringstream typeString;
            typeString << "<[+]" << getMinValue() << ":[+]" << getMaxValue() << ">";
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

        bool runMaxMinTest();
};

template <> long long unsigned UIntegerPtrUnitTest<short unsigned>::getMaxValue()      {return USHRT_MAX;}
template <> long long unsigned UIntegerPtrUnitTest<unsigned>::getMaxValue()            {return UINT_MAX;}
template <> long long unsigned UIntegerPtrUnitTest<long unsigned>::getMaxValue()       {return ULONG_MAX;}
template <> long long unsigned UIntegerPtrUnitTest<long long unsigned>::getMaxValue()  {return ULLONG_MAX;}

template <> bool UIntegerPtrUnitTest<long long unsigned>::runMaxMinTest()              {return false;}
template <> bool UIntegerPtrUnitTest<long unsigned>::runMaxMinTest()                   {return false;}
template <typename T> bool UIntegerPtrUnitTest<T>::runMaxMinTest()                     {return true;}

TYPED_TEST_SUITE_P(UIntegerPtrUnitTest);
TYPED_TEST_P(UIntegerPtrUnitTest, ConstructorValue)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(value, testValue);
    }
}

TYPED_TEST_P(UIntegerPtrUnitTest, ConstructorValueFlag)
{
    for (const TypeParam setValue : this->flagValues)
    {
        for (const TypeParam testValue : this->initValues)
        {
            TypeParam value = testValue;
            argparser::vargptr< TypeParam > testvar(&value, setValue);
            EXPECT_EQ(testValue, value);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
            EXPECT_EQ(setValue, value);
        }
    }
}

TYPED_TEST_P(UIntegerPtrUnitTest, ConstructorValueDefaultFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(testValue, value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
        EXPECT_EQ(0, value);
    }
}

TYPED_TEST_P(UIntegerPtrUnitTest, ValueSetPass)
{
    const std::array<TypeParam, 12> assignValues {{2, 5, 33, 21, 32, 66,
                                                   233, 126, 1948, 1856, 12653, 13639}};

    for (const TypeParam testValue : this->initValues)
    {
        for (const TypeParam assignValue : assignValues)
        {
            TypeParam value = testValue;
            argparser::vargptr< TypeParam > testvar(&value);
            EXPECT_EQ(testValue, value);

            std::string setString = std::to_string(assignValue);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
            EXPECT_EQ(assignValue, value);
        }
    }
}

TYPED_TEST_P(UIntegerPtrUnitTest, ValueSetFail)
{
    const TypeParam initValue = 30;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ(initValue, testValue);
}

TYPED_TEST_P(UIntegerPtrUnitTest, ValueSetMaxPass)
{
    const TypeParam initValue = 45;
    TypeParam testValue = initValue;
    std::string maxValue = this->getMaxString(0ULL);
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(this->getMaxValue(), testValue);
}

TYPED_TEST_P(UIntegerPtrUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        const TypeParam initValue = 48;
        TypeParam testValue = initValue;
        std::string maxValue = this->getMaxString(1ULL);

        argparser::vargptr< TypeParam > testvar(&testValue);
        EXPECT_EQ(initValue, testValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(initValue, testValue);
    }
}

TYPED_TEST_P(UIntegerPtrUnitTest, IsListTest)
{
    const TypeParam initValue = 31;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(UIntegerPtrUnitTest, IsNotEmptyTest)
{
    TypeParam testValue = 0;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(UIntegerPtrUnitTest, IsEmptyTest)
{
    argparser::vargptr< TypeParam > testvar(nullptr);
    EXPECT_TRUE(testvar.isEmpty());
}

TYPED_TEST_P(UIntegerPtrUnitTest, GetRangeString)
{
    const TypeParam initValue = 32;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    std::string expectedString = this->getExpectedRangeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getRangeString());
}

TYPED_TEST_P(UIntegerPtrUnitTest, GetTypeString)
{
    const TypeParam initValue = 32;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_STREQ("unsigned integer", testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(UIntegerPtrUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                              ValueSetPass, ValueSetFail, ValueSetMaxPass, ValueSetMaxFail,
                                              IsListTest, IsNotEmptyTest, IsEmptyTest, GetRangeString, GetTypeString);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;  // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(vargptr_uint, UIntegerPtrUnitTest, uintTypes);

/*
* Float vargptr test
*/
template <typename T> class FloatPtrUnitTest : public testing::Test
{
    private:

    protected:
        const std::array<T, 9> initValues{{3.14,1.345e6,30.1,1e6,2.76, 3.14156, 100, 5.0, 124578}}; // NOLINT

    public:
        FloatPtrUnitTest() = default;
        FloatPtrUnitTest(const FloatPtrUnitTest& other) = default;
        FloatPtrUnitTest(FloatPtrUnitTest&& other) = default;
        FloatPtrUnitTest& operator=(const FloatPtrUnitTest& other) = default;
        FloatPtrUnitTest& operator=(FloatPtrUnitTest&& other) = default;
        ~FloatPtrUnitTest() override = default;

        std::string getExpectedRangeString();
};

template <> std::string FloatPtrUnitTest<double>::getExpectedRangeString()
{
    std::stringstream typeString;
    typeString << "<" << std::numeric_limits<double>::min() << ":" << std::numeric_limits<double>::max() << ">";
    return typeString.str();
}

TYPED_TEST_SUITE_P(FloatPtrUnitTest);

TYPED_TEST_P(FloatPtrUnitTest, ConstructorValue)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(testValue, value);
    }
}

TYPED_TEST_P(FloatPtrUnitTest, ConstructorValueFlag)
{
    const TypeParam initValue = 3.14;
    TypeParam testValue = initValue;
    const TypeParam setValue = 2.76;
    argparser::vargptr< TypeParam > testvar(&testValue, setValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ(setValue, testValue);
}

TYPED_TEST_P(FloatPtrUnitTest, ConstructorValueDefaultFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        TypeParam value = testValue;
        argparser::vargptr< TypeParam > testvar(&value);
        EXPECT_EQ(testValue, value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
        EXPECT_EQ(0.0, value);
    }
}

TYPED_TEST_P(FloatPtrUnitTest, ValueSetPass)
{
    const std::array< TypeParam, 7> assignValues {{3.1415, 98.6, 5.0, 100.7, 1.59e4, -127, -3.79e8}};
    const TypeParam initValue = 2.76;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    for (const TypeParam assignValue : assignValues)
    {
        std::string setString = std::to_string(assignValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
        EXPECT_EQ(assignValue, testValue);
    }
}

TYPED_TEST_P(FloatPtrUnitTest, ValueSetPass_integer)
{
    const TypeParam initValue = 100.0;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("5"));
    EXPECT_EQ(5.0, testValue);
}

TYPED_TEST_P(FloatPtrUnitTest, ValueSetFail)
{
    const TypeParam initValue = 211.56;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ(initValue, testValue);
}

TYPED_TEST_P(FloatPtrUnitTest, ValueSetMaxPass)
{
    const TypeParam initValue = 56.78e2;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);

    std::stringstream maxString;
    maxString.precision(DBL_MANT_DIG);
    maxString << std::numeric_limits<double>::max();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxString.str().c_str()));
    EXPECT_EQ(std::numeric_limits<double>::max(), testValue);
}

TYPED_TEST_P(FloatPtrUnitTest, ValueSetMinPass)
{
    const TypeParam initValue = 435.33e4;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);

    std::stringstream minString;
    minString.precision(DBL_MANT_DIG);
    minString << std::numeric_limits<double>::min();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minString.str().c_str()));
    EXPECT_EQ(std::numeric_limits<double>::min(), testValue);
}

TYPED_TEST_P(FloatPtrUnitTest, IsListTest)
{
    const TypeParam initValue = 56.78e2;
    TypeParam testValue = initValue;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_EQ(initValue, testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(FloatPtrUnitTest, IsNotEmptyTest)
{
    TypeParam testValue = 0.0;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(FloatPtrUnitTest, IsEmptyTest)
{
    argparser::vargptr< TypeParam > testvar(nullptr);
    EXPECT_TRUE(testvar.isEmpty());
}

TYPED_TEST_P(FloatPtrUnitTest, GetRangeString)
{
    TypeParam testValue = 0.0;
    argparser::vargptr< TypeParam > testvar(&testValue);
    std::string expectedString = this->getExpectedRangeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getRangeString());
}

TYPED_TEST_P(FloatPtrUnitTest, GetTypeString)
{
    TypeParam testValue = 0.0;
    argparser::vargptr< TypeParam > testvar(&testValue);
    EXPECT_STREQ("real number", testvar.getTypeString());
}

REGISTER_TYPED_TEST_SUITE_P(FloatPtrUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                           ValueSetPass, ValueSetFail, ValueSetPass_integer,
                                           ValueSetMaxPass, ValueSetMinPass, IsListTest, IsNotEmptyTest,
                                           IsEmptyTest, GetTypeString, GetRangeString);

typedef testing::Types<double> floatTypes;                                      // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(vargptr_float, FloatPtrUnitTest, floatTypes);

/*
* Bool vargptr test
*/
TEST(vargptr_bool, ConstructorValue)
{
    bool testValue = true;
    const argparser::vargptr<bool> testvar(&testValue);
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ConstructorValueFlag)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue, true);
    EXPECT_FALSE(testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_true_singlechar)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_true_singlechar_cap)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("T"));
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_true_word)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("true"));
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_true_word_cap)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("TRUE"));
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_true_digit)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("1"));
    EXPECT_TRUE(testValue);
}

TEST(vargptr_bool, ValueSetPass_false_singlechar)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_FALSE(testValue);
}

TEST(vargptr_bool, ValueSetPass_false_singlechar_cap)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("F"));
    EXPECT_FALSE(testValue);
}
TEST(vargptr_bool, ValueSetPass_false_word)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("false"));
    EXPECT_FALSE(testValue);
}
TEST(vargptr_bool, ValueSetPass_false_word_cap)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("FALSE"));
    EXPECT_FALSE(testValue);
}
TEST(vargptr_bool, ValueSetPass_false_digit)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("0"));
    EXPECT_FALSE(testValue);
}

TEST(vargptr_bool, ValueSetFail)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
}

TEST(vargptr_bool, ValueSetFail_digit)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("2"));
}

TEST(vargptr_bool, IsListTest)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_FALSE(testvar.isList());
}

TEST(vargptr_bool, IsNotEmptyTest)
{
    bool testValue = true;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(vargptr_bool, IsEmptyTest)
{
    argparser::vargptr<bool> testvar(nullptr);
    EXPECT_TRUE(testvar.isEmpty());
}

TEST(vargptr_bool, GetTypeString)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_STREQ("boolean", testvar.getTypeString());
}

TEST(vargptr_bool, GetRangeString)
{
    bool testValue = false;
    argparser::vargptr<bool> testvar(&testValue);
    EXPECT_STREQ("<t|T|1|f|F|0>", testvar.getRangeString());
}

/*
* Character vargptr test
*/
TEST(vargptr_char, ConstructorValue)
{
    char testValue = 't';
    const argparser::vargptr<char> testvar(&testValue);
    EXPECT_EQ('t', testValue);
}

TEST(vargptr_char, ConstructorValueFlag)
{
    char testValue = 't';
    argparser::vargptr<char> testvar(&testValue, 'f');
    EXPECT_EQ('t', testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
    EXPECT_EQ('f', testValue);
}

TEST(vargptr_char, ValueSetPass)
{
    char testValue = 'a';
    argparser::vargptr<char> testvar(&testValue);
    EXPECT_EQ('a', testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_EQ('b', testValue);
}

TEST(vargptr_char, ValueSetFail)
{
    char testValue = 'c';
    argparser::vargptr<char> testvar(&testValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ('c', testValue);
}

TEST(vargptr_char, IsListTest)
{
    char testValue = 'd';
    const argparser::vargptr<char> testvar(&testValue);
    EXPECT_FALSE(testvar.isList());
}

TEST(vargptr_char, IsNotEmptyTest)
{
    char testValue = 'd';
    argparser::vargptr<char> testvar(&testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(vargptr_char, IsEmptyTest)
{
    argparser::vargptr<char> testvar(nullptr);
    EXPECT_TRUE(testvar.isEmpty());
}

TEST(vargptr_char, GetTypeString)
{
    char testValue = 'e';
    argparser::vargptr<char> testvar(&testValue);
    EXPECT_STREQ("character", testvar.getTypeString());
}

TEST(vargptr_char, GetRangeString)
{
    char testValue = 'e';
    argparser::vargptr<char> testvar(&testValue);
    EXPECT_STREQ("<char>", testvar.getRangeString());
}

/** @} */
