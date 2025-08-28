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
#include <climits>

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
        vargintfUnitTest() = default;
        vargintfUnitTest(const vargintfUnitTest& other) = delete;
        vargintfUnitTest(vargintfUnitTest&& other) = delete;
        vargintfUnitTest& operator=(const vargintfUnitTest& other) = delete;
        vargintfUnitTest& operator=(vargintfUnitTest&& other) = delete;
        ~vargintfUnitTest() override = default;

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        [[nodiscard]] bool isList() const override        {return false;}

        /**
         * @brief Virtual place holder for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e - if value exceeds upper value limit
         */
        valueParseStatus_e setValue(const char* newValue) override {return valueParseStatus_e::PARSE_SUCCESS_e;}

        /**
         * Virtual place holder for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         */
        valueParseStatus_e setValue() override                     {return valueParseStatus_e::PARSE_SUCCESS_e;}

        /**
         * Virtual place holder for the template variable implementation isEmpty function
         *
         * @return true - if the variable is empty
         * @return false - if variable is not empty
         */
        bool isEmpty() override                                    {return false;}
};

TEST_F(vargintfUnitTest, VirtualOverrideTest)
{
    // Just to get 100% coverage
    EXPECT_FALSE(vargintfUnitTest::isList());
    EXPECT_FALSE(vargintfUnitTest::isEmpty());
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::setValue("dummy"));
    EXPECT_EQ(valueParseStatus_e::PARSE_SUCCESS_e, vargintfUnitTest::setValue());
}

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

TEST_F(vargintfUnitTest, GetBool_Empty)
{
    bool testValue = true;
    EXPECT_EQ(valueParseStatus_e::PARSE_INVALID_INPUT_e, vargintfUnitTest::getBoolValue("", testValue));
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
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("200", testValue));
}

TEST_F(vargintfUnitTest, GetSignedValue_min_fail)
{
    long long int testValue = 0;
    vargintfUnitTest::setMinMaxSigned(signedtestMin, signedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("-101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("-102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue("-200", testValue));
}

TEST_F(vargintfUnitTest, Get_stoll_range_error_max)
{
    long long int testValue = 0;
    const char* overflowString = "9223372036854775808";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue(overflowString, testValue));
    overflowString = "9323372036854775807";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue(overflowString, testValue));
}

TEST_F(vargintfUnitTest, Get_stoll_range_error_min)
{
    long long int testValue = 0;
    const char* overflowString = "-9223372036854775809";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue(overflowString, testValue));
    overflowString = "-9323372036854775807";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getSignedValue(overflowString, testValue));
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
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("101", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("102", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("200", testValue));
}

TEST_F(vargintfUnitTest, GetUnsignedValue_min_fail)
{
    long long unsigned testValue = 0;
    vargintfUnitTest::setMinMaxUnsigned(unsignedtestMin, unsignedtestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("9", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue("0", testValue));
}

TEST_F(vargintfUnitTest, Get_stoull_range_error)
{
    unsigned long long testValue = 0;
    const char* overflowString = "18446744073709551616";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue(overflowString, testValue));
    overflowString = "28446744073709551615";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getUnsignedValue(overflowString, testValue));
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
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue("1.1e7", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue("1e8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue("2e7", testValue));
}

TEST_F(vargintfUnitTest, GetDoubleValue_min_fail)
{
    double testValue = 0;
    vargintfUnitTest::setMinMaxDouble(doubletestMin, doubletestMax);
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue("1e-8", testValue));
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue("9.99999e-8", testValue));
}

TEST_F(vargintfUnitTest, Get_stod_range_error_max)
{
    double testValue = 0.0;
    const char* overflowString = "5.3e310";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue(overflowString, testValue));
}

TEST_F(vargintfUnitTest, Get_stod_range_error_min)
{
    double testValue = 0.0;
    const char* overflowString = "5.3e-310";
    EXPECT_EQ(valueParseStatus_e::PARSE_OUT_OF_RANGE_e, vargintfUnitTest::getDoubleValue(overflowString, testValue));
}

TEST_F(vargintfUnitTest, getAssignmentCount)
{
    EXPECT_EQ(0, vargintfUnitTest::getAssignmentCount());
}

TEST_F(vargintfUnitTest, setTypeString_String)
{
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
    EXPECT_STREQ("string", vargintfUnitTest::getTypeString());
    EXPECT_STREQ("<string>", vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Char)
{
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
    EXPECT_STREQ("character", vargintfUnitTest::getTypeString());
    EXPECT_STREQ("<char>", vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Bool)
{
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
    EXPECT_STREQ("boolean", vargintfUnitTest::getTypeString());
    EXPECT_STREQ("<t|T|1|f|F|0>", vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Double)
{
    const double minvalue = 5e6;
    const double maxvalue = 10e7;
    std::stringstream typeStr;
    typeStr << "<" << minvalue << ":" << maxvalue << ">";

    vargintfUnitTest::setMinMaxDouble(minvalue, maxvalue);
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
    EXPECT_STREQ("real number", vargintfUnitTest::getTypeString());
    EXPECT_STREQ(typeStr.str().c_str(), vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Unsigned)
{
    const unsigned long long minvalue = 15;
    const unsigned long long maxvalue = 100;
    std::stringstream typeStr;
    typeStr << "<[+]" << minvalue << ":[+]" << maxvalue << ">";

    vargintfUnitTest::setMinMaxUnsigned (minvalue, maxvalue);
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
    EXPECT_STREQ("unsigned integer", vargintfUnitTest::getTypeString());
    EXPECT_STREQ(typeStr.str().c_str(), vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Signed)
{
    const long long int minvalue = -15;
    const long long int maxvalue = 100;
    std::stringstream typeStr;
    typeStr << "<" << minvalue << ":" << maxvalue << ">";

    vargintfUnitTest::setMinMaxSigned (minvalue, maxvalue);
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
    EXPECT_STREQ("integer", vargintfUnitTest::getTypeString());
    EXPECT_STREQ(typeStr.str().c_str(), vargintfUnitTest::getRangeString());
}

TEST_F(vargintfUnitTest, setTypeString_Unknown)
{
    vargintfUnitTest::setTypeString(typeStringFormat_e::TYPE_FMT_MAX);
    EXPECT_STREQ("unknown", vargintfUnitTest::getTypeString());
    EXPECT_STREQ("<unknown>", vargintfUnitTest::getRangeString());
}

/** @} */
