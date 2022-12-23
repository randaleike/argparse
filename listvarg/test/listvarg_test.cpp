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

#define VARLIST_INT_TEST_SUITE(type) \
    TEST(listvarg_##type, ConstructorDefault) { \
        const argparser::listvarg<type> testvar; \
        EXPECT_EQ(0, testvar.defaultValue); \
        EXPECT_TRUE(testvar.value.empty()); } \
    TEST(listvarg_##type, ConstructorValuePos) { \
        argparser::listvarg<type> testvar(10); \
        EXPECT_EQ(10, testvar.defaultValue); \
        EXPECT_TRUE(testvar.value.empty()); } \
    TEST(listvarg_##type, ConstructorValueNeg) { \
        argparser::listvarg<type> testvar(-10); \
        EXPECT_EQ(-10, testvar.defaultValue); \
        EXPECT_TRUE(testvar.value.empty()); } \
    TEST(listvarg_##type, ValueSetPassSignedPos) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("33")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(1, testvar.value.size()); \
        EXPECT_EQ(33, testvar.value.front()); } \
    TEST(listvarg_##type, ValueSetPassSignedNeg) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("-55")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(1, testvar.value.size()); \
        EXPECT_EQ(-55, testvar.value.front()); } \
    TEST(listvarg_##type, ValueSetFail) { \
        argparser::listvarg<type> testvar; \
        EXPECT_FALSE(testvar.setValue("foo")); } \
    TEST(listvarg_##type, ValueSetNullFail) { \
        argparser::listvarg<type> testvar; \
        EXPECT_FALSE(testvar.setValue()); } \
    TEST(listvarg_##type, IsListTest) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.isList()); } \
    TEST(listvarg_##type, getTypeString) { \
        argparser::listvarg<type> testvar; \
        EXPECT_STREQ("<[+|-]int>", testvar.getTypeString()); } \
    TEST(listvarg_##type, ValueSetPassDoubleEntry) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("-55")); \
        EXPECT_TRUE(testvar.setValue("33")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(2, testvar.value.size()); \
        EXPECT_EQ(-55, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(33, testvar.value.front()); } \
    TEST(listvarg_##type, ValueSetPassTripleEntry) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("-11")); \
        EXPECT_TRUE(testvar.setValue("28")); \
        EXPECT_TRUE(testvar.setValue("17")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(3, testvar.value.size()); \
        EXPECT_EQ(-11, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(28, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(17, testvar.value.front()); } 

#define VARLIST_UINT_TEST_SUITE(type) \
    TEST(listvarg_##type, ConstructorDefault) { \
        const argparser::listvarg<type> testvar; \
        EXPECT_EQ(0, testvar.defaultValue); \
        EXPECT_TRUE(testvar.value.empty()); } \
    TEST(listvarg_##type, ConstructorValuePos) { \
        argparser::listvarg<type> testvar(10); \
        EXPECT_EQ(10, testvar.defaultValue); \
        EXPECT_TRUE(testvar.value.empty()); } \
    TEST(listvarg_##type, ValueSetPassSignedPos) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("33")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(1, testvar.value.size()); \
        EXPECT_EQ(33, testvar.value.front()); } \
    TEST(listvarg_##type, ValueSetFail) { \
        argparser::listvarg<type> testvar; \
        EXPECT_FALSE(testvar.setValue("foo")); } \
    TEST(listvarg_##type, ValueSetNullFail) { \
        argparser::listvarg<type> testvar; \
        EXPECT_FALSE(testvar.setValue()); } \
    TEST(listvarg_##type, IsListTest) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.isList()); } \
    TEST(listvarg_##type, getTypeString) { \
        argparser::listvarg<type> testvar; \
        EXPECT_STREQ("<[+]int>", testvar.getTypeString()); } \
    TEST(listvarg_##type, ValueSetPassDoubleEntry) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("55")); \
        EXPECT_TRUE(testvar.setValue("33")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(2, testvar.value.size()); \
        EXPECT_EQ(55, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(33, testvar.value.front()); } \
    TEST(listvarg_##type, ValueSetPassTripleEntry) { \
        argparser::listvarg<type> testvar; \
        EXPECT_TRUE(testvar.setValue("13")); \
        EXPECT_TRUE(testvar.setValue("45")); \
        EXPECT_TRUE(testvar.setValue("12")); \
        EXPECT_FALSE(testvar.value.empty()); \
        EXPECT_EQ(3, testvar.value.size()); \
        EXPECT_EQ(13, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(45, testvar.value.front()); \
        testvar.value.pop_front(); \
        EXPECT_EQ(12, testvar.value.front()); } 

typedef short int shortint;
typedef signed int signedint;
typedef long int longint;
typedef long long int longlongint;

typedef short unsigned shortuint;
typedef unsigned uint;
typedef long unsigned longuint;
typedef long long unsigned longlonguint;

/*
* Integer listvarg test
*/
VARLIST_INT_TEST_SUITE(shortint)
VARLIST_INT_TEST_SUITE(signedint)
VARLIST_INT_TEST_SUITE(longint)
VARLIST_INT_TEST_SUITE(longlongint)

/*
* Unsigned Integer listvarg test
*/
VARLIST_UINT_TEST_SUITE(shortuint)
VARLIST_UINT_TEST_SUITE(uint)
VARLIST_UINT_TEST_SUITE(longuint)
VARLIST_UINT_TEST_SUITE(longlonguint)

/*
* Double listvarg test
*/
TEST(listvarg_double, ConstructorDefault) 
{ 
    const argparser::listvarg<double> testvar; 
    EXPECT_EQ(0.0, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
} 

TEST(listvarg_double, ConstructorValuePos)
{
    argparser::listvarg<double> testvar(3.1415); 
    EXPECT_EQ(3.1415, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
} 

TEST(listvarg_double, ConstructorValueNeg)
{
    argparser::listvarg<double> testvar(-3.1415); 
    EXPECT_EQ(-3.1415, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
}

TEST(listvarg_double, ValueSetNullFail) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_FALSE(testvar.setValue()); 
}

TEST(listvarg_double, IsListTest) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.isList()); 
}

TEST(listvarg_double, getTypeString) 
{ 
    argparser::listvarg<double> testvar; 
    EXPECT_STREQ("<float>", testvar.getTypeString()); 
}

TEST(listvarg_double, ValueSetPassSignedPos) 
{
    argparser::listvarg<double> testvar;
    EXPECT_TRUE(testvar.setValue("3.1415"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(3.1415, testvar.value.front());
}

TEST(listvarg_double, ValueSetPassSignedNeg) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.setValue("-3.1415"));
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(1, testvar.value.size()); 
    EXPECT_EQ(-3.1415, testvar.value.front()); 
}

TEST(listvarg_double, ValueSetPassExponent) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.setValue("3.1415e7"));
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(1, testvar.value.size()); 
    EXPECT_EQ(3.1415e7, testvar.value.front()); 
}

TEST(listvarg_double, ValueSetPassInteger) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.setValue("3"));
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(1, testvar.value.size()); 
    EXPECT_EQ(3.0, testvar.value.front()); 
}

TEST(listvarg_double, ValueSetFail) 
{ 
    argparser::listvarg<double> testvar; 
    EXPECT_FALSE(testvar.setValue("foo")); 
}

TEST(listvarg_double, ValueSetPassDoubleEntry) 
{
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.setValue("-2.72355"));
    EXPECT_TRUE(testvar.setValue("3.1415")); 
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(2, testvar.value.size()); 
    EXPECT_EQ(-2.72355, testvar.value.front()); 
    testvar.value.pop_front(); 
    EXPECT_EQ(3.1415, testvar.value.front()); 
} 

TEST(listvarg_double, ValueSetPassTripleEntry) 
{ 
    argparser::listvarg<double> testvar; 
    EXPECT_TRUE(testvar.setValue("-11.5"));
    EXPECT_TRUE(testvar.setValue("28.4")); 
    EXPECT_TRUE(testvar.setValue("17.9")); 
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(3, testvar.value.size()); 
    EXPECT_EQ(-11.5, testvar.value.front()); 
    testvar.value.pop_front(); 
    EXPECT_EQ(28.4, testvar.value.front()); 
    testvar.value.pop_front(); 
    EXPECT_EQ(17.9, testvar.value.front()); 
} 

/*
* Bool listvarg test
*/
TEST(listvarg_bool, ConstructorDefault) 
{ 
    const argparser::listvarg<bool> testvar; 
    EXPECT_EQ(false, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
} 

TEST(listvarg_bool, ConstructorValueTrue)
{
    argparser::listvarg<bool> testvar(true); 
    EXPECT_EQ(true, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
} 

TEST(listvarg_bool, ConstructorValueFalse)
{
    argparser::listvarg<bool> testvar(false); 
    EXPECT_EQ(false, testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty()); 
} 

TEST(listvarg_bool, ValueSetPass_true_singlechar)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("t"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_true_singlechar_cap)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("T"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_true_word)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("true"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_true_word_cap)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("TRUE"));
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_true_digit)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("1")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(true, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_false_singlechar)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("f")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(false, testvar.value.front());
}

TEST(listvarg_bool, ValueSetPass_false_singlechar_cap)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("F")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(false, testvar.value.front());
}
TEST(listvarg_bool, ValueSetPass_false_word)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("false")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(false, testvar.value.front());
}
TEST(listvarg_bool, ValueSetPass_false_word_cap)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("FALSE")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(false, testvar.value.front());
}
TEST(listvarg_bool, ValueSetPass_false_digit)
{
    argparser::listvarg<bool> testvar; 
    EXPECT_TRUE(testvar.setValue("0")); 
    EXPECT_FALSE(testvar.value.empty());
    EXPECT_EQ(1, testvar.value.size());
    EXPECT_EQ(false, testvar.value.front());
}

TEST(listvarg_bool, ValueSetFail)
{
    argparser::listvarg<bool> testvar(true); 
    EXPECT_FALSE(testvar.setValue("goo")); 
}

TEST(listvarg_bool, ValueSetFail_digit)
{
    argparser::listvarg<bool> testvar(true); 
    EXPECT_FALSE(testvar.setValue("2")); 
    EXPECT_TRUE(testvar.value.empty());
}

TEST(listvarg_bool, IsListTest) 
{ 
    argparser::listvarg<bool> testvar(false);
    EXPECT_TRUE(testvar.isList()); 
} 

TEST(listvarg_bool, GetTypeString) 
{ 
    argparser::listvarg<bool> testvar(false); 
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
TEST(listvarg_char, ConstructorTest) 
{ 
    const argparser::listvarg<char> testvar;
    EXPECT_EQ('\0', testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty());
} 

TEST(listvarg_char, ConstructorDefaultSet) 
{ 
    const argparser::listvarg<char> testvar('a');
    EXPECT_EQ('a', testvar.defaultValue); 
    EXPECT_TRUE(testvar.value.empty());
} 

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

TEST(listvarg_char, IsListTest) 
{ 
    argparser::listvarg<char> testvar;
    EXPECT_TRUE(testvar.isList()); 
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
TEST(listvarg_string, ConstructorTest) 
{ 
    const argparser::listvarg<std::string> testvar;
    EXPECT_TRUE(testvar.defaultValue.empty()); 
    EXPECT_TRUE(testvar.value.empty());
} 

TEST(listvarg_string, ConstructorDefaultSet) 
{ 
    const argparser::listvarg<std::string> testvar("default String");
    EXPECT_STREQ("default String", testvar.defaultValue.c_str()); 
    EXPECT_TRUE(testvar.value.empty());
} 

TEST(listvarg_string, ValueSetPass)
{
    argparser::listvarg<std::string> testvar; 
    EXPECT_TRUE(testvar.setValue("Test String")); 
    EXPECT_FALSE(testvar.value.empty()); 
    EXPECT_EQ(1, testvar.value.size()); 
    EXPECT_STREQ("Test String", testvar.value.front().c_str()); 
}

TEST(listvarg_string, IsListTest) 
{ 
    argparser::listvarg<std::string> testvar;
    EXPECT_TRUE(testvar.isList()); 
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


int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
/** @} */
