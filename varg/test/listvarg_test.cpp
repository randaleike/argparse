/*
Copyright (c) 2022 Randal Eike

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
    EXPECT_FALSE(testvar.setValue());
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
template <typename T> class IntegerUnitTest : public testing::Test
{
    public:
        IntegerUnitTest() {}
        ~IntegerUnitTest() override {}
};

TYPED_TEST_SUITE_P(IntegerUnitTest);

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedPos)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassSignedNeg)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-55"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(-55, testvar.value.front());
}

TYPED_TEST_P(IntegerUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_FALSE(testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(IntegerUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ("<[+|-]int>", testvar.getTypeString());
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-55"));
    EXPECT_TRUE(testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(-55, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(IntegerUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-11"));
    EXPECT_TRUE(testvar.setValue("28"));
    EXPECT_TRUE(testvar.setValue("17"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(-11, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17, testvar.value.front());
}

REGISTER_TYPED_TEST_SUITE_P(IntegerUnitTest, ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetFail,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry);

typedef testing::Types<short int, int, long int, long long int> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_int, IntegerUnitTest, intTypes);

/*
* Unsigned Integer varg test
*/
template <typename T> class UIntegerUnitTest : public testing::Test
{
    public:
        UIntegerUnitTest() {}
        ~UIntegerUnitTest() override {}
};

TYPED_TEST_SUITE_P(UIntegerUnitTest);
TYPED_TEST_P(UIntegerUnitTest, ValueSetPass)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_FALSE(testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(UIntegerUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ("<[+]int>", testvar.getTypeString());
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("55"));
    EXPECT_TRUE(testvar.setValue("33"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(55, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(33, testvar.value.front());
}

TYPED_TEST_P(UIntegerUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("11"));
    EXPECT_TRUE(testvar.setValue("28"));
    EXPECT_TRUE(testvar.setValue("17"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(11, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17, testvar.value.front());
}


REGISTER_TYPED_TEST_SUITE_P(UIntegerUnitTest, ValueSetPass, ValueSetFail,
                            GetTypeString, ValueSetPassDoubleEntry, ValueSetPassTripleEntry);

typedef testing::Types<short unsigned, unsigned, long unsigned, long long unsigned> uintTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_uint, UIntegerUnitTest, uintTypes);

/*
* Double listvarg test
*/
template <typename T> class DoubleUnitTest : public testing::Test
{
    public:
        DoubleUnitTest() {}
        ~DoubleUnitTest() override {}
};

TYPED_TEST_SUITE_P(DoubleUnitTest);
TYPED_TEST_P(DoubleUnitTest, ValueSetPassSignedPos)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetPassSignedNeg)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(-3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetPassExponent)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("3.1415e7"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.1415e7, testvar.value.front());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetPassInteger)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("3"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.0, testvar.value.front());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetFail)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_FALSE(testvar.setValue("foo"));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

TYPED_TEST_P(DoubleUnitTest, GetTypeString)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_STREQ("<float>", testvar.getTypeString());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetPassDoubleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-2.72355"));
    EXPECT_TRUE(testvar.setValue("3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(-2.72355, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(3.1415, testvar.value.front());
}

TYPED_TEST_P(DoubleUnitTest, ValueSetPassTripleEntry)
{
    argparser::listvarg< TypeParam > testvar;
    EXPECT_TRUE(testvar.setValue("-11.5"));
    EXPECT_TRUE(testvar.setValue("28.4"));
    EXPECT_TRUE(testvar.setValue("17.9"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_EQ(-11.5, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(28.4, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(17.9, testvar.value.front());
}

REGISTER_TYPED_TEST_SUITE_P(DoubleUnitTest, ValueSetPassSignedPos, ValueSetPassSignedNeg, ValueSetPassExponent,
                            ValueSetPassInteger, ValueSetFail, GetTypeString, ValueSetPassDoubleEntry,
                            ValueSetPassTripleEntry);

typedef testing::Types<double> doubleTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(listvarg_double, DoubleUnitTest, doubleTypes);

/*
* Bool listvarg test
*/
class BoolUnitTestWithParams : public ::testing::TestWithParam< std::tuple<const char*, bool> >
{
    protected:
        argparser::listvarg<bool> testvar;
};

TEST_P(BoolUnitTestWithParams, ValueSet)
{
    bool        expected   = std::get<1>(GetParam());
    const char* inpVal     = std::get<0>(GetParam());

    EXPECT_TRUE(testvar.setValue(inpVal));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_FALSE(testvar.isEmpty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(expected, testvar.value.front());
}

INSTANTIATE_TEST_CASE_P(listvarg_bool, BoolUnitTestWithParams,
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

    EXPECT_FALSE(testvar.setValue(inpVal));
    EXPECT_TRUE(testvar.value.empty());
    EXPECT_TRUE(testvar.isEmpty());
    EXPECT_EQ(0, testvar.value.size());
}

INSTANTIATE_TEST_CASE_P(listvarg_bool, BoolUnitTestWithBadParams, ::testing::Values("goo", "2", "-1"));

TEST(listvarg_bool, GetTypeString)
{
    argparser::listvarg<bool> testvar;
    EXPECT_STREQ("<t|T|f|F>", testvar.getTypeString());
}

TEST(listvarg_bool, ValueSetPassDoubleEntry)
{
    argparser::listvarg<bool> testvar;
    EXPECT_TRUE(testvar.setValue("t"));
    EXPECT_TRUE(testvar.setValue("f"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ(false, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPassTripleEntry)
{
    argparser::listvarg<bool> testvar;
    EXPECT_TRUE(testvar.setValue("t"));
    EXPECT_TRUE(testvar.setValue("f"));
    EXPECT_TRUE(testvar.setValue("0"));
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
    EXPECT_TRUE(testvar.setValue("b"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ('b', testvar.value.front());
}

TEST(listvarg_char, ValueSetFail)
{
    argparser::listvarg<char> testvar;
    EXPECT_FALSE(testvar.setValue("goo"));
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
    EXPECT_TRUE(testvar.setValue("t"));
    EXPECT_TRUE(testvar.setValue("f"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_EQ('t', testvar.value.front());
    testvar.value.pop_front();
    EXPECT_EQ('f', testvar.value.front());
}

TEST(listvarg_char, ValueSetPassTripleEntry)
{
    argparser::listvarg<char> testvar;
    EXPECT_TRUE(testvar.setValue("a"));
    EXPECT_TRUE(testvar.setValue("b"));
    EXPECT_TRUE(testvar.setValue("c"));
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
    EXPECT_TRUE(testvar.setValue("Test String"));
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
    EXPECT_TRUE(testvar.setValue("test1 string"));
    EXPECT_TRUE(testvar.setValue("test2 string"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(2, testvar.value.size());
    EXPECT_STREQ("test1 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test2 string", testvar.value.front().c_str());
}

TEST(listvarg_string, ValueSetPassTripleEntry)
{
    argparser::listvarg<std::string> testvar;
    EXPECT_TRUE(testvar.setValue("test1 string"));
    EXPECT_TRUE(testvar.setValue("test2 string"));
    EXPECT_TRUE(testvar.setValue("test3 string"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(3, testvar.value.size());
    EXPECT_STREQ("test1 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test2 string", testvar.value.front().c_str());
    testvar.value.pop_front();
    EXPECT_STREQ("test3 string", testvar.value.front().c_str());
}

/** @} */
