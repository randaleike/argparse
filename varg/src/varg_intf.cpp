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
 * @file varg_intf.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes 
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <limits>
#include "varg_intf.h"

namespace argparser
{
//============================================================================================================================
//============================================================================================================================
//  Private functions
//============================================================================================================================
//============================================================================================================================

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Set the Bool Value object
 * 
 * @param newValue - input argument string
 * @param parsedValue - parsed boolean value if parsing succeeded
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
valueParseStatus_e varg_intf::setBoolValue(const char* newValue, bool& parsedValue)
{
    if (newValue[1] != 0)
    {
        // Deal with multi-character string
        std::string testValue = newValue;
        std::transform(testValue.begin(), testValue.end(), testValue.begin(), ::toupper);
        if (testValue == "TRUE")
        {
            parsedValue = true;
            return valueParseStatus_e::PARSE_SUCCESS_e;
        }
        else if (testValue == "FALSE")
        {
            parsedValue = false;
            return valueParseStatus_e::PARSE_SUCCESS_e;
        }
        else
        {
            return valueParseStatus_e::PARSE_INVALID_INPUT_e;
        }
    }
    else
    {
        // Single character 
        switch(newValue[0])
        {
            case 't':
            case 'T':
            case '1':
                parsedValue = true;
                return valueParseStatus_e::PARSE_SUCCESS_e;
            case 'f':
            case 'F':
            case '0':
                parsedValue = false;
                return valueParseStatus_e::PARSE_SUCCESS_e;
            default:
                return valueParseStatus_e::PARSE_INVALID_INPUT_e;
        }
    }
}

/**
 * @brief Set the New character object value
 * 
 * @param newValue - input argument string
 * @param parsedValue - parsed character value if parsing succeeded
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
valueParseStatus_e varg_intf::setCharValue(const char* newValue, char& parsedValue)
{
    std::string inputValue(newValue);
    if ((inputValue.length() > 1) || (inputValue.empty()))
    {
        return valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    else
    {
        parsedValue = inputValue[0];
        return valueParseStatus_e::PARSE_SUCCESS_e;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Construct a varg_intf object
 */
varg_intf::varg_intf()
{
    maxSignedValue = LLONG_MAX;
    minSignedValue = LLONG_MIN;
    maxUnsignedValue = ULLONG_MAX;
    minUnsignedValue = 0ULL;
    maxDoubleValue = std::numeric_limits<double>::max();
    minDoubleValue = std::numeric_limits<double>::min();
}

/**
 * @brief Destroy the varg object
 */
varg_intf::~varg_intf()     {}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

}
/** @} */
