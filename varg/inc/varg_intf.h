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
 * @file varg_intf.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once

// Includes
#include <cstdlib>
#include <unistd.h>
#include <climits>
#include <limits>
#include <string>

namespace argparser
{

/**
 * @brief SetValue return code values 
 */
enum valueParseStatus_e
{
    PARSE_SUCCESS_e = 0,                ///< Input string successfully parsed
    PARSE_INVALID_INPUT_e,              ///< Input could not be parsed into an appropriate value
    PARSE_BOUNDARY_LOW_e,               ///< Parsed value exceeds lower value limit
    PARSE_BOUNDARY_HIGH_e,              ///< Parsed value exceeds upper value limit
};

/**
 * @brief Type string format type selection
 * 
 */
enum typeStringFormat_e
{
    TYPE_FMT_SIGNED,                    ///< Use the signed min/max
    TYPE_FMT_UNSIGNED,                  ///< Use the unsigned min/max
    TYPE_FMT_DOUBLE,                    ///< Use the double min/max
    TYPE_FMT_CHAR,                      ///< Set the default char type string
    TYPE_FMT_BOOL,                      ///< Set the default boolean type string
    TYPE_FMT_STRING,                    ///< Set the default string type string
};

/**
 * @brief Base variable argument varg interface and methods 
 */
class varg_intf
{
    private:
        long long int       maxSignedValue;         ///< Maximum allowed signed integer value
        long long int       minSignedValue;         ///< Minimum allowed signed integer value
        long long unsigned  maxUnsignedValue;       ///< Maximum allowed unsigned integer value
        long long unsigned  minUnsignedValue;       ///< Minimum allowed unsigned integer value
        double              minDoubleValue;         ///< Minimum allowed floating point value
        double              maxDoubleValue;         ///< Minimum allowed floating point value

        std::string         typeString;             ///< Type description string with min/max values
    
    protected:
        /**
         * @brief Set the Min Max Signed object
         * 
         * @param min - Minimum signed value allowed to be assigned
         * @param max - Minimum signed value allowed to be assigned
         */
        void setMinMaxSigned(long long int min, long long int max)              {minSignedValue = min; maxSignedValue = max;}

        /**
         * @brief Set the Min Max Signed object
         * 
         * @param min - Minimum unsigned value allowed to be assigned
         * @param max - Minimum unsigned value allowed to be assigned
         */
        void setMinMaxUnsigned(long long unsigned min, long long unsigned max)  {minUnsignedValue = min; maxUnsignedValue = max;}

        /**
         * @brief Set the Min Max Signed object
         * 
         * @param min - Minimum floating point value allowed to be assigned
         * @param max - Minimum floating point value allowed to be assigned
         */
        void setMinMaxDouble(double min, double max)                            {minDoubleValue = min; maxDoubleValue = max;}

        /**
         * @brief Construct a type string
         */
        void setTypeString(typeStringFormat_e fmtType);

        /**
         * @brief Gt the Bool Value object
         *
         * @param newValue - input argument string
         * @param parsedValue - parsed boolean value if parsing succeeded
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        static valueParseStatus_e getBoolValue(const char* newValue, bool& parsedValue);

        /**
         * @brief Get the New character object value
         * 
         * @param newValue - input argument string
         * @param parsedValue - parsed character value if parsing succeeded
         * 
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        static valueParseStatus_e getCharValue(const char* newValue, char& parsedValue);

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
        valueParseStatus_e getSignedValue(const char* newValue, long long int &parsedValue) const;

        /**
         * @brief Get a unsigned value from the input string
         * 
         * @param newValue - Input string to parse
         * @param parsedValue - return long long unsigned value
         * 
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
         */
        valueParseStatus_e getUnsignedValue(const char* newValue, long long unsigned &parsedValue) const;

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
        valueParseStatus_e getDoubleValue(const char* newValue, double& parsedValue) const;

    public:
        varg_intf();                                                ///< @brief Default constructor for varg_intf object
        varg_intf(const varg_intf& other) = default;                ///< @brief Copy constructor for varg_intf object
        varg_intf(varg_intf&& other) = default;                     ///< @brief Reference copy constructor for varg_intf object
        varg_intf& operator=(const varg_intf& other) = default;     ///< @brief Copy assignment constructor for varg_intf object
        varg_intf& operator=(varg_intf&& other) = default;          ///< @brief Reference copy assignment constructor for varg_intf object

        /**
         * @brief Destroy the varg object
         */
        virtual ~varg_intf() = default;

        /**
         * @brief Get the base argument type as a string
         *
         * @return char* - Base type string
         */
        virtual const char* getTypeString()                     {return typeString.c_str();}

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual bool isList() const = 0;

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
        virtual valueParseStatus_e setValue(const char* newValue) = 0;

        /**
         * Virtual place holder for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         */
        virtual valueParseStatus_e setValue() = 0;

        /**
         * Virtual place holder for the template variable implementation isEmpty function
         *
         * @return true - if the variable is empty
         * @return false - if variable is not empty
         */
        virtual bool isEmpty() = 0;
};

}; // end of namespace argparser

/** @} */
