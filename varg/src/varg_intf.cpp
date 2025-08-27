/*
 Copyright (c) 2022-2024 Randal Eike

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
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>
#include <cctype>
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
    std::string inputValue(newValue);

    // Strip any whitespace
    inputValue.erase(std::remove_if(inputValue.begin(), inputValue.end(), ::isspace), inputValue.end());

    // Determine if it's a single or multi-charater string
    switch (inputValue.size())
    {
        case 0:
            // Empty string
            returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
            break;

        case 1:
            // Single character
            switch(inputValue[0])
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
            break;
        default:
            // Deal with multi-character string
            {
                std::transform(inputValue.begin(), inputValue.end(), inputValue.begin(), ::toupper);
                if (inputValue == "TRUE")
                {
                    parsedValue = true;
                }
                else if (inputValue == "FALSE")
                {
                    parsedValue = false;
                }
                else
                {
                    returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
                }
            }
            break;
    } // end of switch size

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

    // Strip any whitespace
    inputValue.erase(std::remove_if(inputValue.begin(), inputValue.end(), ::isspace), inputValue.end());

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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
valueParseStatus_e varg_intf::getSignedValue(const char* newValue, long long int &parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;

    // Find the first non-whitespace character
    std::string argString(newValue);
    std::size_t parseCount(0);
    try
    {
        const long long int numericValue {std::stoll(argString, &parseCount)};
        if (parseCount >= 1)
        {
            if ((numericValue > maxSignedValue) || (numericValue < minSignedValue))
            {
                returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
            }
            else
            {
                returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
                parsedValue = numericValue;
            }
        }
    }
    catch (std::invalid_argument)
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    catch (std::out_of_range)
    {
        returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
valueParseStatus_e varg_intf::getUnsignedValue(const char* newValue, long long unsigned& parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    std::string argString(newValue);
    std::size_t parseCount(0);
    std::size_t negativeFound = argString.find('-');

    try
    {
        const long long unsigned numericValue {std::stoull(argString, &parseCount, 0)};
        if (parseCount >= 1)
        {
            if ((numericValue > maxUnsignedValue) ||
                (numericValue < minUnsignedValue) ||
                (negativeFound != std::string::npos))
            {
                returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
            }
            else
            {
                returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
                parsedValue = numericValue;
            }
        }
    }
    catch (std::invalid_argument)
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    catch (std::out_of_range)
    {
        returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
valueParseStatus_e varg_intf::getDoubleValue(const char* newValue, double &parsedValue) const
{
    valueParseStatus_e returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
    std::string argString(newValue);
    std::size_t parseCount(0);
    try
    {
        const double numericValue {std::stod(argString, &parseCount)};
        if (parseCount >= 1)
        {
            if ((std::abs(numericValue) > maxDoubleValue) || (std::abs(numericValue) < minDoubleValue))
            {
                returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
            }
            else
            {
                returnStatus = valueParseStatus_e::PARSE_SUCCESS_e;
                parsedValue = numericValue;
            }
        }
    }
    catch (std::invalid_argument)
    {
        returnStatus = valueParseStatus_e::PARSE_INVALID_INPUT_e;
    }
    catch (std::out_of_range)
    {
        returnStatus = valueParseStatus_e::PARSE_OUT_OF_RANGE_e;
    }

    return returnStatus;
}

/**
 * @brief Construct a type string
 */
void varg_intf::setTypeString(typeStringFormat_e fmtType)
{
    std::stringstream myRange;

    switch(fmtType)
    {
        case typeStringFormat_e::TYPE_FMT_SIGNED:
            typeString = "integer";
            myRange << "<" << minSignedValue << ":" << maxSignedValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_UNSIGNED:
            typeString = "unsigned integer";
            myRange << "<[+]" << minUnsignedValue << ":[+]" << maxUnsignedValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_DOUBLE:
            typeString = "real number";
            myRange << "<" << minDoubleValue << ":" << maxDoubleValue << ">";
            break;
        case typeStringFormat_e::TYPE_FMT_CHAR:
            typeString = "character";
            myRange << "<char>";
            break;
        case typeStringFormat_e::TYPE_FMT_BOOL:
            typeString = "boolean";
            myRange << "<t|T|1|f|F|0>";
            break;
        case typeStringFormat_e::TYPE_FMT_STRING:
            typeString = "string";
            myRange << "<string>";
            break;
        default:
            typeString = "unknown";
            myRange << "<unknown>";
            break;
    }
    rangeString = myRange.str();
}

//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Construct a varg_intf object
 */
varg_intf::varg_intf() : maxSignedValue(LLONG_MAX), minSignedValue(LLONG_MIN),
                         maxUnsignedValue(ULLONG_MAX), minUnsignedValue(0ULL),
                         maxDoubleValue(std::numeric_limits<double>::max()), minDoubleValue(std::numeric_limits<double>::min())

{
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

}
/** @} */
