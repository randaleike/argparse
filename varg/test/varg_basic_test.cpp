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
#include "varg.h"
#include <gtest/gtest.h>

/*
* varg test class to access protected functions
*/
template <typename T> class ProtectedVarg : public argparser::varg<T> // NOLINT
{
    public:
        ProtectedVarg() = default;
        ProtectedVarg(const ProtectedVarg& other) = default;
        ProtectedVarg(ProtectedVarg&& other) = default;
        ProtectedVarg& operator=(const ProtectedVarg& other) = default;
        ProtectedVarg& operator=(ProtectedVarg&& other) = default;
        ~ProtectedVarg() override = default; // NOLINT

        /* Call the protected functions */
        argparser::valueParseStatus_e testSetSignedElementValue(const char* newValue)   {return argparser::varg<T>::setSignedValue(newValue);}
        argparser::valueParseStatus_e testSetUnsignedElementValue(const char* newValue) {return argparser::varg<T>::setUnsignedValue(newValue);}
        argparser::valueParseStatus_e testSetDoubleElementValue(const char* newValue)   {return argparser::varg<T>::setDoubleValue(newValue);}
        argparser::valueParseStatus_e testSetBoolElementValue(const char* newValue)     {return argparser::varg<T>::setBoolValue(newValue);}
        argparser::valueParseStatus_e testSetCharElementValue(const char* newValue)     {return argparser::varg<T>::setCharValue(newValue);}
};

/*
* Integer varg test
*/
template <typename T> class IntegerUnitTest : public testing::Test
{
    private:

    protected:
        std::array<T, 13> initValues {{0, 1, -1, 10, -11, 12, 55, -133, 116, -1958, 1876, 11653, -15639}};  // NOLINT
        std::array<T, 12> flagValues {{2, -2, 20, -21, 32, 66, -233, 126, -1948, 1856, 12653, -13639}};     // NOLINT

    public:
        IntegerUnitTest() = default;
        IntegerUnitTest(const IntegerUnitTest& other) = default;
        IntegerUnitTest(IntegerUnitTest&& other) = default;
        IntegerUnitTest& operator=(const IntegerUnitTest& other) = default;
        IntegerUnitTest& operator=(IntegerUnitTest&& other) = default;

        ~IntegerUnitTest() override = default;

        long long int getMaxValue();
        long long int getMinValue();

        std::string getExpectedTypeString()
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

template <> long long int IntegerUnitTest<short int>::getMaxValue()       {return SHRT_MAX;}
template <> long long int IntegerUnitTest<int>::getMaxValue()             {return INT_MAX;}
template <> long long int IntegerUnitTest<long int>::getMaxValue()        {return LONG_MAX;}
template <> long long int IntegerUnitTest<long long int>::getMaxValue()   {return LLONG_MAX;}

template <> long long int IntegerUnitTest<short int>::getMinValue()       {return SHRT_MIN;}
template <> long long int IntegerUnitTest<int>::getMinValue()             {return INT_MIN;}
template <> long long int IntegerUnitTest<long int>::getMinValue()        {return LONG_MIN;}
template <> long long int IntegerUnitTest<long long int>::getMinValue()   {return LLONG_MIN;}

template <> bool IntegerUnitTest<long long int>::runMaxMinTest()          {return false;}
template <> bool IntegerUnitTest<long int>::runMaxMinTest()               {return false;}
template <typename T> bool IntegerUnitTest<T>::runMaxMinTest()            {return true;}

TYPED_TEST_SUITE_P(IntegerUnitTest);
TYPED_TEST_P(IntegerUnitTest, ConstructorValueSigned)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueDefaultFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
        EXPECT_EQ(0, testvar.value);
    }
}

TYPED_TEST_P(IntegerUnitTest, ConstructorValueFlag)
{
    for (const TypeParam flagValue : this->flagValues)
    {
        for (const TypeParam testValue : this->initValues)
        {
            argparser::varg< TypeParam > testvar(testValue, flagValue);
            EXPECT_EQ(testValue, testvar.value);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
            EXPECT_EQ(flagValue, testvar.value);
        }
    }
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSigned)
{
    const std::array<int, 12> assignValues {{2, -2, 33, -21, 32, 66,
                                             -233, 126, -1948, 1856, 12653, -13639}};

    for (const TypeParam testValue : this->initValues)
    {
        for (const TypeParam assignValue : assignValues)
        {
            argparser::varg< TypeParam > testvar(testValue);
            std::string setString = std::to_string(assignValue);
            EXPECT_EQ(testValue, testvar.value);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
            EXPECT_EQ(assignValue, testvar.value);
        }
    }
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
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
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
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(minValue.c_str()));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(IntegerUnitTest, IsListTest)
{
    const TypeParam testValue = 11;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(IntegerUnitTest, IsEmptyTest)
{
    const TypeParam testValue = 37;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(IntegerUnitTest, GetAssignmentCount)
{
    const TypeParam testValue = -28;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TYPED_TEST_P(IntegerUnitTest, GetTypeString)
{
    const TypeParam testValue = 12;
    argparser::varg< TypeParam > testvar(testValue);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

TYPED_TEST_P(IntegerUnitTest, SetMinMax)
{
    const TypeParam testValue = 12;
    const TypeParam maxValue = 48;
    const TypeParam minValue = -5;
    argparser::varg< TypeParam > testvar(testValue, minValue, maxValue);

    std::string setString = std::to_string(minValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue - 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue + 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    setString = std::to_string(maxValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue + 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue - 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    std::stringstream expectedTypeString;
    expectedTypeString << "<" << minValue << ":" << maxValue << ">";

    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedTypeString.str().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(IntegerUnitTest, SetTestProtectedFail)
{
    ProtectedVarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetUnsignedElementValue("2"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetDoubleElementValue("5.9"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetBoolElementValue("T"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetCharElementValue("a"));
}

REGISTER_TYPED_TEST_SUITE_P(IntegerUnitTest, ConstructorValueSigned, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                             ValueSetPassSigned, ValueSetFail, ValueSetMaxPass, ValueSetMaxFail,
                                             ValueSetMinPass, ValueSetMinFail, IsListTest, IsEmptyTest,
                                             GetAssignmentCount, GetTypeString, SetMinMax, SetTestProtectedFail);

typedef testing::Types<short int, int, long int, long long int> intTypes;   // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(varg_int, IntegerUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerUnitTest : public testing::Test
{
    private:

    protected:
        const std::array<T, 13> initValues {{0, 1, 5, 10, 31, 12, 55, 133, 116, 1958, 1876, 11653, 15639}}; // NOLINT
        const std::array<T, 12> flagValues {{2, 2, 20, 21, 32, 66, 233, 126, 1948, 1856, 12653, 13639}};    // NOLINT

    public:
        UIntegerUnitTest() = default;
        UIntegerUnitTest(const UIntegerUnitTest& other) = default;
        UIntegerUnitTest(UIntegerUnitTest&& other) = default;
        UIntegerUnitTest& operator=(const UIntegerUnitTest& other) = default;
        UIntegerUnitTest& operator=(UIntegerUnitTest&& other) = default;

        ~UIntegerUnitTest() override = default;

        long long unsigned getMaxValue();
        long long unsigned getMinValue()     {return 0ULL;}

        std::string getExpectedTypeString()
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

template <> long long unsigned UIntegerUnitTest<short unsigned>::getMaxValue()      {return USHRT_MAX;}
template <> long long unsigned UIntegerUnitTest<unsigned>::getMaxValue()            {return UINT_MAX;}
template <> long long unsigned UIntegerUnitTest<long unsigned>::getMaxValue()       {return ULONG_MAX;}
template <> long long unsigned UIntegerUnitTest<long long unsigned>::getMaxValue()  {return ULLONG_MAX;}

template <> bool UIntegerUnitTest<long long unsigned>::runMaxMinTest()              {return false;}
template <> bool UIntegerUnitTest<long unsigned>::runMaxMinTest()                   {return false;}
template <typename T> bool UIntegerUnitTest<T>::runMaxMinTest()                     {return true;}

TYPED_TEST_SUITE_P(UIntegerUnitTest);
TYPED_TEST_P(UIntegerUnitTest, ConstructorValue)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueFlag)
{
    for (const TypeParam setValue : this->flagValues)
    {
        for (const TypeParam testValue : this->initValues)
        {
            argparser::varg< TypeParam > testvar(testValue, setValue);
            EXPECT_EQ(testValue, testvar.value);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
            EXPECT_EQ(setValue, testvar.value);
        }
    }
}

TYPED_TEST_P(UIntegerUnitTest, ConstructorValueDefaultFlag)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue());
        EXPECT_EQ(0, testvar.value);
    }
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetPass)
{
    const std::array<TypeParam, 12> assignValues {{2, 5, 33, 21, 32, 66,
                                                   233, 126, 1948, 1856, 12653, 13639}};

    for (const TypeParam testValue : this->initValues)
    {
        for (const TypeParam assignValue : assignValues)
        {
            argparser::varg< TypeParam > testvar(testValue);
            EXPECT_EQ(testValue, testvar.value);

            std::string setString = std::to_string(assignValue);
            EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
            EXPECT_EQ(assignValue, testvar.value);
        }
    }
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
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(UIntegerUnitTest, IsListTest)
{
    const TypeParam testValue = 31;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isList());
}

TYPED_TEST_P(UIntegerUnitTest, IsEmptyTest)
{
    const TypeParam testValue = 37;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(UIntegerUnitTest, GetAssignmentCount)
{
    const TypeParam testValue = 28;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TYPED_TEST_P(UIntegerUnitTest, GetTypeString)
{
    const TypeParam testValue = 32;
    argparser::varg< TypeParam > testvar(testValue);
    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
}

TYPED_TEST_P(UIntegerUnitTest, SetMinMax)
{
    const TypeParam testValue = 8;
    const TypeParam maxValue = 57;
    const TypeParam minValue = 5;
    argparser::varg< TypeParam > testvar(testValue, minValue, maxValue);

    std::string setString = std::to_string(minValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue - 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue + 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    setString = std::to_string(maxValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue + 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue - 1));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    std::stringstream expectedTypeString;
    expectedTypeString << "<[+]" << minValue << ":[+]" << maxValue << ">";

    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedTypeString.str().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(UIntegerUnitTest, SetTestProtectedFail)
{
    ProtectedVarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetSignedElementValue("2"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetDoubleElementValue("5.9"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetBoolElementValue("T"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetCharElementValue("a"));
}

REGISTER_TYPED_TEST_SUITE_P(UIntegerUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                              ValueSetPass, ValueSetFail, ValueSetMaxPass, ValueSetMaxFail,
                                              IsListTest, IsEmptyTest, GetAssignmentCount, GetTypeString,
                                              SetMinMax, SetTestProtectedFail);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;   // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(varg_uint, UIntegerUnitTest, uintTypes);

/*
* Float varg test
*/
template <typename T> class FloatUnitTest : public testing::Test
{
    private:

    protected:
        const std::array<T, 9> initValues{{3.14,1.345e6,30.1,1e6,2.76, 3.14156, 100, 5.0, 124578}}; // NOLINT

    public:
        FloatUnitTest() = default;
        FloatUnitTest(const FloatUnitTest& other) = default;
        FloatUnitTest(FloatUnitTest&& other) = default;
        FloatUnitTest& operator=(const FloatUnitTest& other) = default;
        FloatUnitTest& operator=(FloatUnitTest&& other) = default;

        ~FloatUnitTest() override = default;

        std::string getExpectedTypeString();
};

template <> std::string FloatUnitTest<double>::getExpectedTypeString()
{
    std::stringstream typeString;
    typeString << "<" << std::numeric_limits<double>::min() << ":" << std::numeric_limits<double>::max() << ">";
    return typeString.str();
}

TYPED_TEST_SUITE_P(FloatUnitTest);

TYPED_TEST_P(FloatUnitTest, ConstructorValue)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);
    }
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

TYPED_TEST_P(FloatUnitTest, ValueSetPass)
{
    const std::array< TypeParam, 7> assignValues {{3.1415, 98.6, 5.0, 100.7, 1.59e4, -127, -3.79e8}};
    const TypeParam testValue = 2.76;
    argparser::varg< TypeParam > testvar(testValue);
    for (const TypeParam assignValue : assignValues)
    {
        std::string setString = std::to_string(assignValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
        EXPECT_EQ(assignValue, testvar.value);
    }
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
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
        EXPECT_EQ(testValue, testvar.value);
    }
}

TYPED_TEST_P(FloatUnitTest, ValueSetMaxPass)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);

        std::stringstream maxString;
        maxString.precision(DBL_MANT_DIG);
        maxString << std::numeric_limits<double>::max();

        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxString.str().c_str()));
        EXPECT_EQ(std::numeric_limits<double>::max(), testvar.value);
    }
}

TYPED_TEST_P(FloatUnitTest, ValueSetMinPass)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_EQ(testValue, testvar.value);

        std::stringstream minString;
        minString.precision(DBL_MANT_DIG);
        minString << std::numeric_limits<double>::min();

        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minString.str().c_str()));
        EXPECT_EQ(std::numeric_limits<double>::min(), testvar.value);
    }
}

TYPED_TEST_P(FloatUnitTest, IsListTest)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        EXPECT_FALSE(testvar.isList());
    }
}

TYPED_TEST_P(FloatUnitTest, IsEmptyTest)
{
    const TypeParam testValue = 37.73;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(FloatUnitTest, GetAssignmentCount)
{
    const TypeParam testValue = 28.67;
    argparser::varg< TypeParam > testvar(testValue);
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TYPED_TEST_P(FloatUnitTest, GetTypeString)
{
    for (const TypeParam testValue : this->initValues)
    {
        argparser::varg< TypeParam > testvar(testValue);
        std::string expectedString = this->getExpectedTypeString();
        EXPECT_STREQ(expectedString.c_str(), testvar.getTypeString());
    }
}

TYPED_TEST_P(FloatUnitTest, SetMinMax)
{
    const TypeParam testValue = 2.7;
    const TypeParam maxValue = 8.3;
    const TypeParam minValue = 1.3;
    const TypeParam adjust = 0.0001;
    argparser::varg< TypeParam > testvar(testValue, minValue, maxValue);

    std::string setString = std::to_string(minValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue - adjust));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((minValue + adjust));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    setString = std::to_string(maxValue);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue + adjust));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(setString.c_str()));
    setString = std::to_string((maxValue - adjust));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(setString.c_str()));

    std::stringstream expectedTypeString;
    expectedTypeString << "<" << minValue << ":" << maxValue << ">";

    std::string expectedString = this->getExpectedTypeString();
    EXPECT_STREQ(expectedTypeString.str().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(FloatUnitTest, SetTestProtectedFail)
{
    ProtectedVarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetSignedElementValue("2"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetUnsignedElementValue("5"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetBoolElementValue("T"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetCharElementValue("a"));
}

REGISTER_TYPED_TEST_SUITE_P(FloatUnitTest, ConstructorValue, ConstructorValueFlag, ConstructorValueDefaultFlag,
                                           ValueSetPass, ValueSetFail, ValueSetPass_integer,
                                           ValueSetMaxPass, ValueSetMinPass, IsListTest, IsEmptyTest,
                                           GetAssignmentCount, GetTypeString, SetMinMax, SetTestProtectedFail);

//typedef testing::Types<double> floatTypes;
using floatTypes = testing::Types<double>;
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

TEST(varg_bool, ConstructorMinMax)
{
    argparser::varg<bool> testvar(false, false, true);
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

TEST(varg_bool, IsEmptyTest)
{
    argparser::varg<bool> testvar(true);
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(varg_bool, GetAssignmentCount)
{
    argparser::varg<bool> testvar(true);
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TEST(varg_bool, GetTypeString)
{
    argparser::varg<bool> testvar(false);
    EXPECT_STREQ("<t|T|1|f|F|0>", testvar.getTypeString());
}

TEST(varg_bool, SetTestProtectedFail)
{
    ProtectedVarg<bool> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetSignedElementValue("2"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetUnsignedElementValue("5"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetDoubleElementValue("7.32"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetCharElementValue("a"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.testSetBoolElementValue("f"));
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

TEST(varg_char, ConstructorMinMax)
{
    argparser::varg<char> testvar('t', 'a', 'z');
    EXPECT_EQ('t', testvar.value);
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("c"));
    EXPECT_EQ('c', testvar.value);
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

TEST(varg_char, IsEmptyTest)
{
    argparser::varg<char> testvar('z');
    EXPECT_FALSE(testvar.isEmpty());
}

TEST(varg_char, GetAssignmentCount)
{
    argparser::varg<char> testvar('y');
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TEST(varg_char, GetTypeString)
{
    argparser::varg<char> testvar('e');
    EXPECT_STREQ("<char>", testvar.getTypeString());
}

TEST(varg_char, SetTestProtectedFail)
{
    ProtectedVarg<char> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetSignedElementValue("2"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetUnsignedElementValue("5"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetDoubleElementValue("7.32"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.testSetBoolElementValue("t"));
}

/** @} */
