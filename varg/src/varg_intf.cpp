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
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>
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
valueParseStatus_e varg_intf::getBoolValue(const char* newValue, bool& parsedValue)
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    if (newValue[1] != 0)
    {
        // Deal with multi-character string
        std::string testValue    = newValue;
        std::transform(testValue.begin(), testValue.end(), testValue.begin(), ::toupper);
        if (testValue == "TRUE")
        {
            parsedValue = true;
        }
        else if (testValue == "FALSE")
        {
            parsedValue = false;
        }
        else
        {
            returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
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
                break;
            case 'f':
            case 'F':
            case '0':
                parsedValue = false;
                break;
            default:
                returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
        }
    }

    return returnStatus;
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
valueParseStatus_e varg_intf::getCharValue(const char* newValue, char& parsedValue)
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    std::string inputValue(newValue);

    if ((inputValue.length() > 1) || (inputValue.empty()))
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    else
    {
        parsedValue = inputValue[0];
    }
    return returnStatus;
}

/**
 * @brief Get a signed value from the input string
 * 
 * @param newValue - Input string to parse
 * @param parsedValue - return long long integer value
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
valueParseStatus_e varg_intf::getSignedValue(const char* newValue, long long int &parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    int                parseCount   = sscanf(newValue, "%lld", &parsedValue);
    if (1 == parseCount)
    {
        if (parsedValue > maxSignedValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_HIGH_e;
        }
        else if (parsedValue < minSignedValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_LOW_e;
        }
    }
    else
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    return returnStatus;
}

/**
 * @brief Get the Unsigned Value object
 *
 * @param newValue - input argument string
 * @param parsedValue - return long long integer value
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
valueParseStatus_e varg_intf::getUnsignedValue(const char* newValue, long long unsigned& parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    const char*        testChar     = newValue;
    
    // Find the first non-whitespace character
    while (*testChar <= ' ') 
    {
        testChar++;
    }

    int parseCount = sscanf(newValue, "%llu", &parsedValue);
    if ((1 == parseCount) && (*testChar != '-'))
    {
        if (parsedValue > maxUnsignedValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_HIGH_e;
        }
        if (parsedValue < minUnsignedValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_LOW_e;
        }
    }
    else
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }

    return returnStatus;
}

/**
 * @brief Get the Double Value object
 *
 * @param newValue - input argument string
 * @param parsedValue - return double value
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
valueParseStatus_e varg_intf::getDoubleValue(const char* newValue, double &parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    int                parseCount   = sscanf(newValue, "%lf", &parsedValue);
    double             absValue     = std::fabs(parsedValue);

    if (1 == parseCount)
    {
        if (absValue > maxDoubleValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_HIGH_e;
        }
        else if (absValue < minDoubleValue)
        {
            returnStatus = valueParseStatus_e::PARSE_BOUNDARY_LOW_e;
        }
    }
    else
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }

    return returnStatus;
}

/**
 * @brief Construct a type string
 */
void varg_intf::setTypeString(typeStringFormat_e fmtType)
{
    std::stringstream myTypeStr;

    switch(fmtType)
    {
        case typeStringFormat_e::TYPE_FMT_SIGNED:
            myTypeStr << "<" << minSignedValue << ":[+]" << maxSignedValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_UNSIGNED:
            myTypeStr << "<" << minUnsignedValue << ":[+]" << maxUnsignedValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_DOUBLE:
            myTypeStr << "<" << minDoubleValue << ":" << maxDoubleValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_CHAR:
            myTypeStr << "<char>";
            break;
        case typeStringFormat_e::TYPE_FMT_BOOL:
            myTypeStr << "<t|T|f|F>";
            break;
        case typeStringFormat_e::TYPE_FMT_STRING:
            myTypeStr << "<string>";
            break;
        default:
            myTypeStr << "<unknown>";
            break;
    }
    typeString = myTypeStr.str();
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

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

}
/** @} */
