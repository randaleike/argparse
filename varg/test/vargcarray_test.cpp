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
 * @file vargcarray_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "../src/vargarray_clib_private.h"
#include <gtest/gtest.h>

/*
* vargcarray base function test
*/
template <typename T> class CarrayBaseUnitTest : public testing::Test
{
    public:
        CarrayBaseUnitTest() = default;
        CarrayBaseUnitTest(const CarrayBaseUnitTest& other) = default;
        CarrayBaseUnitTest(CarrayBaseUnitTest&& other) = default;
        CarrayBaseUnitTest& operator=(const CarrayBaseUnitTest& other) = default;
        CarrayBaseUnitTest& operator=(CarrayBaseUnitTest&& other) = default;
        ~CarrayBaseUnitTest() override = default;
};

TYPED_TEST_SUITE_P(CarrayBaseUnitTest);
TYPED_TEST_P(CarrayBaseUnitTest, ConstructorTest)
{
    TypeParam carray[3];    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT  // NOLINT
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TYPED_TEST_P(CarrayBaseUnitTest, ValueSetNullFail)
{
    TypeParam carray[3];    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue());
}

TYPED_TEST_P(CarrayBaseUnitTest, IsListTest)
{
    TypeParam carray[3];    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_TRUE(testvar.isList());
}

TYPED_TEST_P(CarrayBaseUnitTest, IsNotEmptyTest)
{
    TypeParam carray[3];    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_FALSE(testvar.isEmpty());
}

TYPED_TEST_P(CarrayBaseUnitTest, IsEmptyTest)
{
    argparser::vargcarray< TypeParam > testvar(nullptr, 0);    // NOLINT
    EXPECT_TRUE(testvar.isEmpty());
}

REGISTER_TYPED_TEST_SUITE_P(CarrayBaseUnitTest, ConstructorTest, ValueSetNullFail, IsListTest,
                            IsNotEmptyTest, IsEmptyTest);

// NOLINTBEGIN
typedef testing::Types<short int, int, long int, long long int,
                       unsigned short, unsigned, unsigned long, unsigned long long,
                       double, bool, char> allTypes;
// NOLINTEND
INSTANTIATE_TYPED_TEST_SUITE_P(carrayvarg_base, CarrayBaseUnitTest, allTypes);

/*
* Integer vargcarray test
*/
template <typename T> class IntegerCArrayUnitTest : public testing::Test
{
    private:

    public:
        IntegerCArrayUnitTest() = default;
        IntegerCArrayUnitTest(const IntegerCArrayUnitTest& other) = default;
        IntegerCArrayUnitTest(IntegerCArrayUnitTest&& other) = default;
        IntegerCArrayUnitTest& operator=(const IntegerCArrayUnitTest& other) = default;
        IntegerCArrayUnitTest& operator=(IntegerCArrayUnitTest&& other) = default;
        ~IntegerCArrayUnitTest() override = default;

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

template <> long long int IntegerCArrayUnitTest<short int>::getMaxValue()     {return SHRT_MAX;}
template <> long long int IntegerCArrayUnitTest<int>::getMaxValue()           {return INT_MAX;}
template <> long long int IntegerCArrayUnitTest<long int>::getMaxValue()      {return LONG_MAX;}
template <> long long int IntegerCArrayUnitTest<long long int>::getMaxValue() {return LLONG_MAX;}

template <> long long int IntegerCArrayUnitTest<short int>::getMinValue()     {return SHRT_MIN;}
template <> long long int IntegerCArrayUnitTest<int>::getMinValue()           {return INT_MIN;}
template <> long long int IntegerCArrayUnitTest<long int>::getMinValue()      {return LONG_MIN;}
template <> long long int IntegerCArrayUnitTest<long long int>::getMinValue() {return LLONG_MIN;}

template <> bool IntegerCArrayUnitTest<long long int>::runMaxMinTest()        {return false;}
template <> bool IntegerCArrayUnitTest<long int>::runMaxMinTest()             {return false;}
template <typename T> bool IntegerCArrayUnitTest<T>::runMaxMinTest()          {return true;}

TYPED_TEST_SUITE_P(IntegerCArrayUnitTest);

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetPassSigned)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT   // NOLINT
    EXPECT_EQ(0, testvar.getAssignmentCount());

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(33, carray[0]);
    EXPECT_EQ(13, carray[1]);
    EXPECT_EQ(-17, carray[2]);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-33"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_EQ(33, carray[0]);
    EXPECT_EQ(-33, carray[1]);
    EXPECT_EQ(-17, carray[2]);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("333"));
    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_EQ(33, carray[0]);
    EXPECT_EQ(-33, carray[1]);
    EXPECT_EQ(333, carray[2]);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("111"));
    EXPECT_EQ(4, testvar.getAssignmentCount());
    EXPECT_EQ(111, carray[0]);
    EXPECT_EQ(-33, carray[1]);
    EXPECT_EQ(333, carray[2]);
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetFail)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetMaxPass)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    std::string maxValue = this->getMaxString(0LL);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(this->getMaxValue(), carray[0]);
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        TypeParam carray[3] = {-11,13,-17};    // NOLINT
        argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
        std::string maxValue = this->getMaxString(1LL);
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(0, testvar.getAssignmentCount());
    }
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetMinPass)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    std::string minValue = this->getMinString(0LL);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minValue.c_str()));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(this->getMinValue(), carray[0]);
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetMinFail)
{
    if (this->runMaxMinTest())
    {
        TypeParam carray[3] = {-11,13,-17};    // NOLINT
        argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
        std::string minValue = this->getMinString(1LL);

        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(minValue.c_str()));
        EXPECT_EQ(0, testvar.getAssignmentCount());
    }
}

TYPED_TEST_P(IntegerCArrayUnitTest, GetTypeString)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetPassDoubleEntry)
{
    TypeParam carray[3] = {-11,13,-17};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-55"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_EQ(-55, carray[0]);
    EXPECT_EQ(33, carray[1]);
}

TYPED_TEST_P(IntegerCArrayUnitTest, ValueSetPassTripleEntry)
{
    TypeParam carray[3] = {23,13,-117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-11"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17"));

    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_EQ(-11, carray[0]);
    EXPECT_EQ(28, carray[1]);
    EXPECT_EQ(17, carray[2]);
}

REGISTER_TYPED_TEST_SUITE_P(IntegerCArrayUnitTest, ValueSetPassSigned, ValueSetFail,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry,
                            ValueSetMaxPass, ValueSetMaxFail, ValueSetMinPass, ValueSetMinFail);

typedef testing::Types<short int, int, long int, long long int> intTypes;   // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(carrayvarg_int, IntegerCArrayUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerCarrayUnitTest : public testing::Test
{
    private:

    public:
        UIntegerCarrayUnitTest() = default;
        UIntegerCarrayUnitTest(const UIntegerCarrayUnitTest& other) = default;
        UIntegerCarrayUnitTest(UIntegerCarrayUnitTest&& other) = default;
        UIntegerCarrayUnitTest& operator=(const UIntegerCarrayUnitTest& other) = default;
        UIntegerCarrayUnitTest& operator=(UIntegerCarrayUnitTest&& other) = default;
        ~UIntegerCarrayUnitTest() override = default;

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

template <> long long unsigned UIntegerCarrayUnitTest<short unsigned>::getMaxValue()      {return USHRT_MAX;}
template <> long long unsigned UIntegerCarrayUnitTest<unsigned>::getMaxValue()            {return UINT_MAX;}
template <> long long unsigned UIntegerCarrayUnitTest<long unsigned>::getMaxValue()       {return ULONG_MAX;}
template <> long long unsigned UIntegerCarrayUnitTest<long long unsigned>::getMaxValue()  {return ULLONG_MAX;}

template <> bool UIntegerCarrayUnitTest<long long unsigned>::runMaxMinTest()              {return false;}
template <> bool UIntegerCarrayUnitTest<long unsigned>::runMaxMinTest()                   {return false;}
template <typename T> bool UIntegerCarrayUnitTest<T>::runMaxMinTest()                     {return true;}

TYPED_TEST_SUITE_P(UIntegerCarrayUnitTest);
TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetPass)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(33, carray[0]);
    EXPECT_EQ(13, carray[1]);
    EXPECT_EQ(117, carray[2]);
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetFail)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(0, testvar.getAssignmentCount());
    EXPECT_EQ(23, carray[0]);
    EXPECT_EQ(13, carray[1]);
    EXPECT_EQ(117, carray[2]);
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetFailNeg)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue("-1"));
    EXPECT_EQ(0, testvar.getAssignmentCount());
    EXPECT_EQ(23, carray[0]);
    EXPECT_EQ(13, carray[1]);
    EXPECT_EQ(117, carray[2]);
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetMaxPass)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    std::string maxValue = this->getMaxString(0ULL);

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(this->getMaxValue(), carray[0]);
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        TypeParam carray[3] = {23,13,117};    // NOLINT
        argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
        std::string maxValue = this->getMaxString(1ULL);

        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e, testvar.setValue(maxValue.c_str()));
        EXPECT_EQ(0, testvar.getAssignmentCount());
        EXPECT_EQ(23, carray[0]);
    }
}

TYPED_TEST_P(UIntegerCarrayUnitTest, GetTypeString)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT
    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetPassDoubleEntry)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("55"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_EQ(55, carray[0]);
    EXPECT_EQ(33, carray[1]);
    EXPECT_EQ(117, carray[2]);
}

TYPED_TEST_P(UIntegerCarrayUnitTest, ValueSetPassTripleEntry)
{
    TypeParam carray[3] = {23,13,117};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("11"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17"));
    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_EQ(11, carray[0]);
    EXPECT_EQ(28, carray[1]);
    EXPECT_EQ(17, carray[2]);
}


REGISTER_TYPED_TEST_SUITE_P(UIntegerCarrayUnitTest, ValueSetPass, ValueSetFail, ValueSetFailNeg,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry,
                            ValueSetMaxPass, ValueSetMaxFail);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;      // NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(carrayvarg_uint, UIntegerCarrayUnitTest, uintTypes);

/*
* Double vargcarray test
*/
template <typename T> class DoubleCarrayUnitTest : public testing::Test
{
    private:
        std::stringstream typeString;

    public:
        DoubleCarrayUnitTest() = default;
        DoubleCarrayUnitTest(const DoubleCarrayUnitTest& other) = default;
        DoubleCarrayUnitTest(DoubleCarrayUnitTest&& other) noexcept = default;
        DoubleCarrayUnitTest& operator=(const DoubleCarrayUnitTest& other) = default;
        DoubleCarrayUnitTest& operator=(DoubleCarrayUnitTest&& other) noexcept = default;
        ~DoubleCarrayUnitTest() override = default;

        std::string getExpectedTypeString();
};

template <> std::string DoubleCarrayUnitTest<double>::getExpectedTypeString()
{
    typeString << "<" << std::numeric_limits<double>::min() << ":" << std::numeric_limits<double>::max() << ">";
    return typeString.str();
}

TYPED_TEST_SUITE_P(DoubleCarrayUnitTest);
TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassSignedPos)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(3.1415, carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassSignedNeg)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-3.1415"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(-3.1415, carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassExponent)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415e7"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(3.1415e7, carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassInteger)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(3.0, carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetFail)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_EQ(0, testvar.getAssignmentCount());
    EXPECT_EQ(23.7, carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetMaxPass)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    std::stringstream maxString;
    maxString.precision(DBL_MANT_DIG);
    maxString << std::numeric_limits<double>::max();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxString.str().c_str()));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(std::numeric_limits<double>::max(), carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetMinPass)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    std::stringstream minString;
    minString.precision(DBL_MANT_DIG);
    minString << std::numeric_limits<double>::min();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minString.str().c_str()));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ(std::numeric_limits<double>::min(), carray[0]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, GetTypeString)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassDoubleEntry)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-2.72355"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_EQ(-2.72355, carray[0]);
    EXPECT_EQ(3.1415, carray[1]);
}

TYPED_TEST_P(DoubleCarrayUnitTest, ValueSetPassTripleEntry)
{
    TypeParam carray[3] = {23.7,1.13,11.7e6};    // NOLINT
    argparser::vargcarray< TypeParam > testvar(carray, 3);    // NOLINT

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-11.5"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28.4"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17.9"));
    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_EQ(-11.5, carray[0]);
    EXPECT_EQ(28.4, carray[1]);
    EXPECT_EQ(17.9, carray[2]);
}

REGISTER_TYPED_TEST_SUITE_P(DoubleCarrayUnitTest, ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetPassExponent,
                            ValueSetPassInteger, ValueSetFail, GetTypeString, ValueSetPassDoubleEntry,
                            ValueSetPassTripleEntry, ValueSetMaxPass, ValueSetMinPass);

typedef testing::Types<double> doubleTypes;     //NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(carrayvarg_double, DoubleCarrayUnitTest, doubleTypes);

/*
* Bool vargcarray test
*/
bool boolTestArray[3] = {false, false, false}; // NOLINT
const size_t boolTestArraySize = sizeof(boolTestArray)/sizeof(boolTestArray[0]);

class CarrayBoolUnitTestWithParams : public ::testing::TestWithParam< std::tuple<const char*, bool> >
{
    protected:
        argparser::vargcarray<bool> *testvar;   // NOLINT

    public:
        CarrayBoolUnitTestWithParams() : testvar(new argparser::vargcarray<bool>(boolTestArray, boolTestArraySize)) {}  // NOLINT
};

TEST_P(CarrayBoolUnitTestWithParams, ValueSet)
{
    bool        expected   = std::get<1>(GetParam());
    const char* inpVal     = std::get<0>(GetParam());
    bool        setValue   = !expected;

    boolTestArray[0] = setValue;
    boolTestArray[1] = setValue;
    boolTestArray[2] = setValue;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar->setValue(inpVal));
    EXPECT_EQ(1, testvar->getAssignmentCount());
    EXPECT_EQ(expected, boolTestArray[0]);
    EXPECT_EQ(setValue, boolTestArray[1]);
    EXPECT_EQ(setValue, boolTestArray[2]);
}


INSTANTIATE_TEST_SUITE_P(carrayvarg_bool_param, CarrayBoolUnitTestWithParams,
        ::testing::Values(
            std::make_tuple("t", true),
            std::make_tuple("T", true),
            std::make_tuple("true", true),
            std::make_tuple("T", true),
            std::make_tuple("TRUE", true),
            std::make_tuple("1", true),
            std::make_tuple("f", false),
            std::make_tuple("F", false),
            std::make_tuple("false", false),
            std::make_tuple("F", false),
            std::make_tuple("FALSE", false),
            std::make_tuple("0", false)
            ));


class CarrayBoolUnitTestWithBadParams : public ::testing::TestWithParam< const char* >
{
    protected:
        argparser::vargcarray<bool> *testvar;   // NOLINT

    public:
        CarrayBoolUnitTestWithBadParams() : testvar(new argparser::vargcarray<bool>(boolTestArray, boolTestArraySize)) {}  // NOLINT
};

TEST_P(CarrayBoolUnitTestWithBadParams, ValueSet)
{
    const char* inpVal     = GetParam();
    boolTestArray[0] = false;
    boolTestArray[1] = false;
    boolTestArray[2] = false;

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar->setValue(inpVal));
    EXPECT_EQ(0, testvar->getAssignmentCount());
    EXPECT_FALSE(boolTestArray[0]);
    EXPECT_FALSE(boolTestArray[1]);
    EXPECT_FALSE(boolTestArray[2]);
}

INSTANTIATE_TEST_SUITE_P(carrayvarg_bool, CarrayBoolUnitTestWithBadParams, ::testing::Values("goo", "2", "-1"));

TEST(carrayvarg_bool, GetTypeString)
{
    bool testArray[3] = {false, false, false};          // NOLINT
    argparser::vargcarray<bool> testvar(testArray, 3);  // NOLINT
    EXPECT_STREQ("<t|T|1|f|F|0>", testvar.getTypeString());
}

TEST(carrayvarg_bool, ValueSetPassDoubleEntry)
{
    bool testArray[3] = {false, true, false};           // NOLINT
    argparser::vargcarray<bool> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_TRUE(testArray[0]);
    EXPECT_FALSE(testArray[1]);
    EXPECT_FALSE(testArray[2]);
}

TEST(carrayvarg_bool, ValueSetPassTripleEntry)
{
    bool testArray[3] = {false, true, true};            // NOLINT
    argparser::vargcarray<bool> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("0"));

    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_TRUE(testArray[0]);
    EXPECT_FALSE(testArray[1]);
    EXPECT_FALSE(testArray[2]);
}

/*
* Character vargcarray test
*/
TEST(carrayvarg_char, ValueSetPass)
{
    char testArray[3] = {'0', '1', '2'};                // NOLINT
    argparser::vargcarray<char> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_EQ(1, testvar.getAssignmentCount());
    EXPECT_EQ('b', testArray[0]);
}

TEST(carrayvarg_char, ValueSetFail)
{
    char testArray[3] = {'0', '1', '2'};                // NOLINT
    argparser::vargcarray<char> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_EQ(0, testvar.getAssignmentCount());
}

TEST(carrayvarg_char, GetTypeString)
{
    char testArray[3] = {'0', '1', '2'};                // NOLINT
    argparser::vargcarray<char> testvar(testArray, 3);  // NOLINT
    EXPECT_STREQ("<char>", testvar.getTypeString());
}

TEST(carrayvarg_char, ValueSetPassDoubleEntry)
{
    char testArray[3] = {'0', '1', '2'};                // NOLINT
    argparser::vargcarray<char> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_EQ(2, testvar.getAssignmentCount());
    EXPECT_EQ('t', testArray[0]);
    EXPECT_EQ('f', testArray[1]);
}

TEST(carrayvarg_char, ValueSetPassTripleEntry)
{
    char testArray[3] = {'0', '1', '2'};                // NOLINT
    argparser::vargcarray<char> testvar(testArray, 3);  // NOLINT
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("a"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("c"));

    EXPECT_EQ(3, testvar.getAssignmentCount());
    EXPECT_EQ('a', testArray[0]);
    EXPECT_EQ('b', testArray[1]);
    EXPECT_EQ('c', testArray[2]);
}


/** @} */
