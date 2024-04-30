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
 * @file varg_intf_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "varg_intf.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include <limits.h>

using namespace argparser;

const long long int signedtestMin = -100;
const long long int signedtestMax = 100;

const long long unsigned unsignedtestMin = 10;
const long long unsigned unsignedtestMax = 100;

const double doubletestMin = 1e-7;
const double doubletestMax = 1e7;

/*
* varg interface test
*/
class vargintfUnitTest : public varg_intf, public testing::Test
{
    private:

    public:
        vargintfUnitTest() {}
        virtual ~vargintfUnitTest() override {}

        /**
         * @brief Get the base argument type as a string
         *
         * @return char* - Base type string
         */
        virtual const char* getTypeString() override                       {return varg_intf::getTypeString();}

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual bool isList() const override                               {return false;}

        /**
         * @brief Virtual place holder for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        virtual valueParseStatus_e setValue(const char* newValue) override {return valueParseStatus_e::PARSE_SUCCESS_e;}

        /**
         * Virtual place holder for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         */
        virtual valueParseStatus_e setValue() override                     {return valueParseStatus_e::PARSE_SUCCESS_e;}

        /**
         * Virtual place holder for the template variable implementation isEmpty function
         *
         * @return true - if the variable is empty
         * @return false - if variable is not empty
         */
        virtual bool isEmpty() override                                    {return false;}
};

TEST_F(vargintfUnitTest, GetBool_true)
{
    bool testValue = false;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("true", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_false)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("false", testValue));
    EXPECT_FALSE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_TRUE)
{
    bool testValue = false;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("TRUE", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_FALSE)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("FALSE", testValue));
    EXPECT_FALSE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_One)
{
    bool testValue = false;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("1", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_Zero)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("0", testValue));
    EXPECT_FALSE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_UT)
{
    bool testValue = false;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("T", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_UF)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("F", testValue));
    EXPECT_FALSE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_lt)
{
    bool testValue = false;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("t", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_lf)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getBoolValue("f", testValue));
    EXPECT_FALSE(testValue);
}

TEST_F(vargintfUnitTest, GetBool_fail)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_INVALID_INPUT_e, vargintfUnitTest::getBoolValue("not", testValue));
    EXPECT_TRUE(testValue);
}

TEST_F(vargintfUnitTest, GetChar)
{
    char testValue = 0;
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getCharValue("n", testValue));
    EXPECT_EQ('n', testValue);
}

TEST_F(vargintfUnitTest, GetChar_Fail_str)
{
    char testValue = 0;
    EXPECT_EQ(valueParseStatus_e::PARSE_INVALID_INPUT_e, vargintfUnitTest::getCharValue("not", testValue));
    EXPECT_EQ(0, testValue);
}

TEST_F(vargintfUnitTest, GetChar_Fail_empty)
{
    char testValue = 0;
    EXPECT_EQ(valueParseStatus_e::PARSE_INVALID_INPUT_e, vargintfUnitTest::getCharValue("", testValue));
    EXPECT_EQ(0, testValue);
}

TEST_F(vargintfUnitTest, GetSignedValue)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("50", testValue));
    EXPECT_EQ(50, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("-50", testValue));
    EXPECT_EQ(-50, testValue);
}

TEST_F(vargintfUnitTest, GetSignedValue_max)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("99", testValue));
    EXPECT_EQ(99, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("100", testValue));
    EXPECT_EQ(signedtestMax, testValue);
}

TEST_F(vargintfUnitTest, GetSignedValue_min)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("-99", testValue));
    EXPECT_EQ(-99, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getSignedValue("-100", testValue));
    EXPECT_EQ(signedtestMin, testValue);
}

TEST_F(vargintfUnitTest, GetSignedValue_max_fail)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getSignedValue("101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getSignedValue("102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getSignedValue("200", testValue));
}

TEST_F(vargintfUnitTest, GetSignedValue_min_fail)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getSignedValue("-101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getSignedValue("-102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getSignedValue("-200", testValue));
}

TEST_F(vargintfUnitTest, GetUnsignedValue)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("50", testValue));
    EXPECT_EQ(50, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("25", testValue));
    EXPECT_EQ(25, testValue);
}

TEST_F(vargintfUnitTest, GetUnsignedValue_max)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("99", testValue));
    EXPECT_EQ(99, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("100", testValue));
    EXPECT_EQ(unsignedtestMax, testValue);
}

TEST_F(vargintfUnitTest, GetUnsignedValue_min)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("11", testValue));
    EXPECT_EQ(11, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getUnsignedValue("10", testValue));
    EXPECT_EQ(unsignedtestMin, testValue);
}

TEST_F(vargintfUnitTest, GetUnsignedValue_max_fail)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getUnsignedValue("101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getUnsignedValue("102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getUnsignedValue("200", testValue));
}

TEST_F(vargintfUnitTest, GetUnsignedValue_min_fail)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getUnsignedValue("9", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getUnsignedValue("8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getUnsignedValue("0", testValue));
}

TEST_F(vargintfUnitTest, GetDoubleValue)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("50", testValue));
    EXPECT_EQ(50, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("-25", testValue));
    EXPECT_EQ(-25, testValue);
}

TEST_F(vargintfUnitTest, GetDoubleValue_max)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("9.99999e6", testValue));
    EXPECT_EQ(9.99999e6, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("1e7", testValue));
    EXPECT_EQ(doubletestMax, testValue);
}

TEST_F(vargintfUnitTest, GetDoubleValue_min)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("1.1e-7", testValue));
    EXPECT_EQ(1.1e-7, testValue);

    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::getDoubleValue("1e-7", testValue));
    EXPECT_EQ(doubletestMin, testValue);
}

TEST_F(vargintfUnitTest, GetDoubleValue_max_fail)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getDoubleValue("1.1e7", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getDoubleValue("1e8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_HIGH_e, vargintfUnitTest::getDoubleValue("2e7", testValue));
}

TEST_F(vargintfUnitTest, GetDoubleValue_min_fail)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getDoubleValue("1e-8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_BOUNDARY_LOW_e, vargintfUnitTest::getDoubleValue("9.99999e-8", testValue));
}

/** @} */
