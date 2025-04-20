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
 * @file parser_string_list_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mock_ParserStringListInterface.h"
#include "parser_base.h"
#include "parser_string_list.h"

class langsetup
{
    private:
        parserstr envValue;         //!< Linux environment LANG value
        parserstr isoCode;          //!< ISO-639 language code

    public:
        langsetup(parserstr setupVal, parserstr code): envValue(std::move(setupVal)), isoCode(std::move(code)) {}
        langsetup(const langsetup& other) = default;
        langsetup(langsetup&& other) noexcept = default;
        langsetup& operator=(const langsetup& other) = default;
        langsetup& operator=(langsetup&& other) noexcept = default;
        ~langsetup() = default;

        [[nodiscard]] parserstr getLangEnv() const          {return envValue;}
        [[nodiscard]] const char* getIsoCode() const        {return isoCode.c_str();}
        #if defined(__linux__) || defined(__unix__)
        [[nodiscard]] int setLang(int override) const       {return setenv("LANG", envValue.c_str(), override);}
        static int restoreOriginalLang(parserstr original)  {return setenv("LANG", original.c_str(), 1);}
        static parserstr getOriginalLang()
            {
                const char* current = getenv("LANG");
                return ((current != nullptr) ? current : "en_US.UTF-8");
            }
        #endif
};

//======================================================================================
// Public Interface testing, english
//======================================================================================
TEST(BaseParserStringList, formatToLength)
{
    constexpr size_t testMaxLength = 32;
    argparser::BaseParserStringList testvar;
    parserstr baseString = "This is a test string that will be broken into two strings";

    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {' '}, testMaxLength);
    EXPECT_EQ(2, strList.size());
    EXPECT_STREQ("This is a test string that will ", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("be broken into two strings      ", strList.front().c_str());
}

TEST(BaseParserStringList, formatToLengthWithDebug)
{
    // Enable debug messages fo completeness
    constexpr size_t testMaxLength = 32;
    argparser::BaseParserStringList testvar;
    testvar.setDebugMsgLevel(debugVerbosityLevel_e::veryVerboseDebug);

    parserstr baseString = "This is a test string that will be broken into two strings";

    testing::internal::CaptureStderr();
    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {' '}, testMaxLength);
    parserstr output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(2, strList.size());
    EXPECT_STREQ("This is a test string that will ", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("be broken into two strings      ", strList.front().c_str());

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "Start string: \"This is a test string that will be broken into two strings\"\n";
    expected += "Start break: 50\n";
    expected += "Current break: 50\n";
    expected += "Current break: 46\n";
    expected += "Current break: 41\n";
    expected += "Current break: 34\n";
    expected += "Current break: 31\n";
    expected += "Current string: \"be broken into two strings\"\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif // if(ENABLE_DEBUG_STRING_CHECK)
}

TEST(BaseParserStringList, formatToLengthPad)
{
    constexpr size_t testMaxLength = 32;
    argparser::BaseParserStringList testvar;
    parserstr baseString = "Padded test string";

    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {' '}, testMaxLength);
    EXPECT_EQ(1, strList.size());
    EXPECT_STREQ("Padded test string              ", strList.front().c_str());
}

TEST(BaseParserStringList, formatToLengthExactlyRight)
{
    argparser::BaseParserStringList testvar;
    parserstr baseString = "Perfect test string";

    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {' '}, baseString.size());
    EXPECT_EQ(1, strList.size());
    EXPECT_STREQ("Perfect test string", strList.front().c_str());
}

TEST(BaseParserStringList, formatToLengthAwkward)
{
    constexpr size_t testMaxLength = 30;
    argparser::BaseParserStringList testvar;
    parserstr baseString = "This is a test string that will be broken into two strings";

    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {';'}, testMaxLength);
    EXPECT_STREQ("This is a test string that wil", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("l be broken into two strings  ", strList.front().c_str());
}

TEST(BaseParserStringList, formatToLengthDoubleBreak)
{
    constexpr size_t testMaxLength = 31;
    argparser::BaseParserStringList testvar;
    parserstr baseString = "This is a test string that; will be broken into two strings";

    std::list<parserstr> strList = testvar.formatStringToLength(baseString, {';', ' '}, testMaxLength);
    EXPECT_STREQ("This is a test string that;    ", strList.front().c_str());
    strList.pop_front();
    EXPECT_STREQ("will be broken into two strings", strList.front().c_str());
}

/** @} */
