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
 * @file vargstring.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <cstring>
#include <regex>
#include "vargstring.h"

using namespace argparser;

/**
* @brief Virtual interface method implementation for the template variable implementation setValue with input function
*
* @param newValue - Input character string
*
* @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
* @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
* @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
*/
valueParseStatus_e vargstring::setValue(const char* newValue)
{
    size_t inputStrLen = strlen(newValue);
    // Check input string length against min/max limits
    if ((inputStrLen < minStringLength) || (inputStrLen > maxStringLength))
    {
        return valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
    }
    // Check regex if specified
    if (!includeRegex.empty())
    {
        // Must match include regex
        if (!std::regex_match(newValue, std::regex(includeRegex)))
        {
            return valueParseStatus_e::PARSE_INVALID_INPUT_e;
        }
    }

    // All checks passed, assign the value
    value = newValue;
    return valueParseStatus_e::PARSE_SUCCESS_e;
}

/**
 * @brief Set the inclusion and exclusion regex strings
 *
 * @param incStr - Regex inclusion string to be matched
 * @param excStr - Regex exclusion string to be matched
 */
void vargstring::setRegex(std::string incStr)
{
    includeRegex = std::move(incStr);
    std::string range = (includeRegex.empty() ? "<string>" : ("Valid expression: " + includeRegex));
    varg_intf::setRangeString(range);
}

/** @} */
