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
 * @file cmd_line_parse_test.h
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <cstddef>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "cmd_line_parse.h"
#include "parser_string_list.h"
#include "mock_ParserStringListInterface.h"

const size_t defaultArgWidth = 14;
const size_t defaultColWidth = 80;
const size_t testArgWidth    = 17;
const int    testValue       = 10;

using ::testing::StrictMock;
using ::testing::Mock;
using ::testing::Return;
using stringMockptr = StrictMock<argparser::mock_ParserStringListInterface>*;

class parser_test : public argparser::cmd_line_parse
{
    public:
        parser_test() = default;

        parser_test(parserstr& usage, parserstr& description, bool abortOnError = false,
                    bool disableDefaultHelp = false, int debugLevel = debugVerbosityLevel_e::noDebugMsg);

        parser_test(const char* usage, const char* description, bool abortOnError = false, bool disableDefaultHelp = false,
                    int debugLevel = debugVerbosityLevel_e::noDebugMsg);

        parser_test(parserstr& usage, parserstr& description, parserstr& keyPrefix, bool abortOnError = false,
                    bool disableDefaultHelp = false, int debugLevel = debugVerbosityLevel_e::noDebugMsg);

        parser_test(const char* usage, const char* description, const char* keyPrefix, bool abortOnError = false,
                    bool disableDefaultHelp = false, int debugLevel = debugVerbosityLevel_e::noDebugMsg);

        parser_test(const parser_test& other) noexcept = default;
        parser_test(parser_test&& other) noexcept = default;
        parser_test& operator=(const parser_test& other) noexcept = default;
        parser_test& operator=(parser_test&& other) noexcept = default;
        ~parser_test() = default;

        stringMockptr getStringsMock();
};
