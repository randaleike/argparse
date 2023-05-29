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
 * @file listvarg_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "listvarg.h"
#include <gtest/gtest.h>

/*
* listvarg base function test
*/
template <typename T> class ListBaseUnitTest : public testing::Test
{
    public:
        ListBaseUnitTest() {}
        ~ListBaseUnitTest() override {}
};

TYPED_TEST_SUITE_P(ListBaseUnitTest);
TYPED_TEST_P(ListBaseUnitTest, ConstructorTest)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
}

TYPED_TEST_P(ListBaseUnitTest, ValueSetNullFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue());
}

TYPED_TEST_P(ListBaseUnitTest, IsListTest)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.isList());
}

REGISTER_TYPED_TEST_SUITE_P(ListBaseUnitTest, ConstructorTest, ValueSetNullFail, IsListTest);

typedef testing::Types<short int, int, long int, long long int,
                       unsigned short, unsigned, unsigned long, unsigned long long,
                       double, bool, char, std::string> allTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_base, ListBaseUnitTest, allTypes);

/*
* Integer listvarg test
*/
template <typename T> class IntegerListUnitTest : public testing::Test
{
    private:

    public:
        IntegerListUnitTest() {}
        ~IntegerListUnitTest() override {}

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

template <> long long int IntegerListUnitTest<short int>::getMaxValue(void)     {return SHRT_MAX;}
template <> long long int IntegerListUnitTest<int>::getMaxValue(void)           {return INT_MAX;}
template <> long long int IntegerListUnitTest<long int>::getMaxValue(void)      {return LONG_MAX;}
template <> long long int IntegerListUnitTest<long long int>::getMaxValue(void) {return LLONG_MAX;}

template <> long long int IntegerListUnitTest<short int>::getMinValue(void)     {return SHRT_MIN;}
template <> long long int IntegerListUnitTest<int>::getMinValue(void)           {return INT_MIN;}
template <> long long int IntegerListUnitTest<long int>::getMinValue(void)      {return LONG_MIN;}
template <> long long int IntegerListUnitTest<long long int>::getMinValue(void) {return LLONG_MIN;}

template <> bool IntegerListUnitTest<long long int>::runMaxMinTest(void)        {return false;}
template <> bool IntegerListUnitTest<long int>::runMaxMinTest(void)             {return false;}
template <typename T> bool IntegerListUnitTest<T>::runMaxMinTest(void)          {return true;}

TYPED_TEST_SUITE_P(IntegerListUnitTest);

TYPED_TEST_P(IntegerListUnitTest, ValueSetPassSignedPos)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetPassSignedNeg)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-55"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(-55, testvar.value.front());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetMaxPass)
{
    std::string maxValue = this->getMaxString(0LL);
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(this->getMaxValue(), testvar.value.front());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        std::string maxValue = this->getMaxString(1LL);
        argparser::listvarg< TypeParam > testvar;
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, testvar.setValue(maxValue.c_str()));
        EXPECT_TRUE(testvar.value.empty());
        EXPECT_TRUE(testvar.isEmpty());
    }
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetMinPass)
{
    std::string minValue = this->getMinString(0LL);
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minValue.c_str()));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(this->getMinValue(), testvar.value.front());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetMinFail)
{
    if (this->runMaxMinTest())
    {
        std::string minValue = this->getMinString(1LL);
        argparser::listvarg< TypeParam > testvar;
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_LOW_e, testvar.setValue(minValue.c_str()));
        EXPECT_TRUE(testvar.value.empty());
        EXPECT_TRUE(testvar.isEmpty());
    }
}

TYPED_TEST_P(IntegerListUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-55"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(-55, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(IntegerListUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-11"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(-11, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17, testvar.value.front());
}

REGISTER_TYPED_TEST_SUITE_P(IntegerListUnitTest, ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetFail,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry, 
                            ValueSetMaxPass, ValueSetMaxFail, ValueSetMinPass, ValueSetMinFail);

typedef testing::Types<short int, int, long int, long long int> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_int, IntegerListUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerListUnitTest : public testing::Test
{
    private:

    public:
        UIntegerListUnitTest() {}
        ~UIntegerListUnitTest() override {}

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

template <> long long unsigned UIntegerListUnitTest<short unsigned>::getMaxValue(void)      {return USHRT_MAX;}
template <> long long unsigned UIntegerListUnitTest<unsigned>::getMaxValue(void)            {return UINT_MAX;}
template <> long long unsigned UIntegerListUnitTest<long unsigned>::getMaxValue(void)       {return ULONG_MAX;}
template <> long long unsigned UIntegerListUnitTest<long long unsigned>::getMaxValue(void)  {return ULLONG_MAX;}

template <> bool UIntegerListUnitTest<long long unsigned>::runMaxMinTest(void)              {return false;}
template <> bool UIntegerListUnitTest<long unsigned>::runMaxMinTest(void)                   {return false;}
template <typename T> bool UIntegerListUnitTest<T>::runMaxMinTest(void)                     {return true;}

TYPED_TEST_SUITE_P(UIntegerListUnitTest);
TYPED_TEST_P(UIntegerListUnitTest, ValueSetPass)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetFailNeg)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("-1"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetMaxPass)
{
    std::string maxValue = this->getMaxString(0ULL);
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxValue.c_str()));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(this->getMaxValue(), testvar.value.front());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetMaxFail)
{
    if (this->runMaxMinTest())
    {
        std::string maxValue = this->getMaxString(1ULL);
        argparser::listvarg< TypeParam > testvar;
        EXPECT_EQ(argparser::valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, testvar.setValue(maxValue.c_str()));
        EXPECT_TRUE(testvar.value.empty());
        EXPECT_TRUE(testvar.isEmpty());
        EXPECT_EQ(0, testvar.value.size());
    }
}

TYPED_TEST_P(UIntegerListUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("55"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(55, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(UIntegerListUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("11"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(11, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17, testvar.value.front());
}


REGISTER_TYPED_TEST_SUITE_P(UIntegerListUnitTest, ValueSetPass, ValueSetFail, ValueSetFailNeg,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry,
                            ValueSetMaxPass, ValueSetMaxFail);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_uint, UIntegerListUnitTest, uintTypes);

/*
* Double listvarg test
*/
template <typename T> class DoubleListUnitTest : public testing::Test
{
    private:
        std::stringstream typeString;

    public:
        DoubleListUnitTest() {}

        ~DoubleListUnitTest() override {}
        std::string getExpectedTypeString(void);
};

template <> std::string DoubleListUnitTest<double>::getExpectedTypeString(void) 
{
    typeString << "<" << std::numeric_limits<double>::min() << ":" << std::numeric_limits<double>::max() << ">";
    return typeString.str();
}

TYPED_TEST_SUITE_P(DoubleListUnitTest);
TYPED_TEST_P(DoubleListUnitTest, ValueSetPassSignedPos)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetPassSignedNeg)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(-3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetPassExponent)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415e7"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.1415e7, testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetPassInteger)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.0, testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetMaxPass)
{
    argparser::listvarg< TypeParam > testvar;
    std::stringstream maxString;
    maxString.precision(DBL_MANT_DIG);
    maxString << std::numeric_limits<double>::max();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(maxString.str().c_str()));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(std::numeric_limits<double>::max(), testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetMinPass)
{
    argparser::listvarg< TypeParam > testvar;
    std::stringstream minString;
    minString.precision(DBL_MANT_DIG);
    minString << std::numeric_limits<double>::min();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(minString.str().c_str()));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(std::numeric_limits<double>::min(), testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ(this->getExpectedTypeString().c_str(), testvar.getTypeString());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-2.72355"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(-2.72355, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleListUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("-11.5"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("28.4"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("17.9"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(-11.5, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28.4, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17.9, testvar.value.front());
}

REGISTER_TYPED_TEST_SUITE_P(DoubleListUnitTest, ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetPassExponent,
                            ValueSetPassInteger, ValueSetFail, GetTypeString, ValueSetPassDoubleEntry,
                            ValueSetPassTripleEntry, ValueSetMaxPass, ValueSetMinPass);

typedef testing::Types<double> doubleTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_double, DoubleListUnitTest, doubleTypes);

/*
* Bool listvarg test
*/
class ListBoolUnitTestWithParams : public ::testing::TestWithParam< std::tuple<const char*, bool> >
{
    protected:
        argparser::listvarg<bool> testvar;
};

TEST_P(ListBoolUnitTestWithParams, ValueSet)
{
    bool        expected   = std::get<1>(GetParam());
    const char* inpVal     = std::get<0>(GetParam());

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue(inpVal));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(expected, testvar.value.front());
}

INSTANTIATE_TEST_SUITE_P(listvarg_bool_param, ListBoolUnitTestWithParams,
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


class BoolUnitTestWithBadParams : public ::testing::TestWithParam< const char* >
{
    protected:
        argparser::listvarg<bool> testvar;
};

TEST_P(BoolUnitTestWithBadParams, ValueSet)
{
    const char* inpVal     = GetParam();

    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue(inpVal));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

INSTANTIATE_TEST_SUITE_P(listvarg_bool, BoolUnitTestWithBadParams, ::testing::Values("goo", "2", "-1"));

TEST(listvarg_bool, GetTypeString)
{
    argparser::listvarg<bool> testvar;
    EXPECT_STREQ("<t|T|f|F>", testvar.getTypeString());
}

TEST(listvarg_bool, ValueSetPassDoubleEntry)
{
    argparser::listvarg<bool> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(false, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPassTripleEntry)
{
    argparser::listvarg<bool> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("0"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(false, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(false, testvar.value.front());
}

/*
* Character listvarg test
*/

TEST(listvarg_char, ValueSetPass)
{
    argparser::listvarg<char> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ('b', testvar.value.front());
}

TEST(listvarg_char, ValueSetFail)
{
    argparser::listvarg<char> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e, testvar.setValue("goo"));
    EXPECT_TRUE(testvar.value.empty());
}

TEST(listvarg_char, GetTypeString)
{
    argparser::listvarg<char> testvar;
    EXPECT_STREQ("<char>", testvar.getTypeString());
}

TEST(listvarg_char, ValueSetPassDoubleEntry)
{
    argparser::listvarg<char> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("t"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("f"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ('t', testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ('f', testvar.value.front());
}

TEST(listvarg_char, ValueSetPassTripleEntry)
{
    argparser::listvarg<char> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("a"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("b"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("c"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ('a', testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ('b', testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ('c', testvar.value.front());
}

/*
* String varg test
*/
TEST(listvarg_string, ValueSetPass)
{
    argparser::listvarg<std::string> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("Test String"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_STREQ("Test String", testvar.value.front().c_str());
}

TEST(listvarg_string, GetTypeString)
{
    argparser::listvarg<std::string> testvar;
    EXPECT_STREQ("<string>", testvar.getTypeString());
}

TEST(listvarg_string, ValueSetPassDoubleEntry)
{
    argparser::listvarg<std::string> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("test1 string"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("test2 string"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_STREQ("test1 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test2 string", testvar.value.front().c_str());
}

TEST(listvarg_string, ValueSetPassTripleEntry)
{
    argparser::listvarg<std::string> testvar;
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("test1 string"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("test2 string"));
    EXPECT_EQ(argparser::valueParseStatus_e::PARSE_SUCCESS_e, testvar.setValue("test3 string"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_STREQ("test1 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test2 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test3 string", testvar.value.front().c_str());
}

/** @} */
