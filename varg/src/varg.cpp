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
 * @file varg.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes 
#include <string>
#include <iostream>
#include <sstream>
#include "varg.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Set the Bool Value object
 * 
 * @param newValue - input argument string
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
template <> valueParseStatus_e varg<bool>::setBoolValue(const char* newValue)           {return (varg_intf::getBoolValue(newValue, value));}
template <typename T> valueParseStatus_e varg<T>::setBoolValue(const char* newValue)    {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

/**
 * @brief Set the New character object value
 * 
 * @param newValue - input argument string
 * @param parsedValue - parsed character value if parsing succeeded
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
template <> valueParseStatus_e varg<char>::setCharValue(const char* newValue)           {return varg_intf::getCharValue(newValue, value);}
template <typename T> valueParseStatus_e varg<T>::setCharValue(const char* newValue)    {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

/**
 * @brief Set the Double Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
template <> valueParseStatus_e varg<short unsigned>::setSignedValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned>::setSignedValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long unsigned>::setSignedValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long unsigned>::setSignedValue(const char* newValue)   {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setSignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setSignedValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setSignedValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<std::string>::setSignedValue(const char* newValue)          {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e varg<T>::setSignedValue(const char* newValue)
{
    long long int tempValue;
    valueParseStatus_e status = varg_intf::getSignedValue(newValue, tempValue);
    if (status == valueParseStatus_e::PARSE_SUCCESS_e)
    {
        value = static_cast<T>(tempValue);
    }
    return status;
}

/**
 * @brief Set the Unsigned Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
template <> valueParseStatus_e varg<short int>::setUnsignedValue(const char* newValue)          {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<int>::setUnsignedValue(const char* newValue)                {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long int>::setUnsignedValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long int>::setUnsignedValue(const char* newValue)      {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setUnsignedValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setUnsignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setUnsignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<std::string>::setUnsignedValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e varg<T>::setUnsignedValue(const char* newValue)
{
    long long unsigned tempValue;
    valueParseStatus_e status = varg_intf::getUnsignedValue(newValue, tempValue);
    if (status == valueParseStatus_e::PARSE_SUCCESS_e)
    {
        value = static_cast<T>(tempValue);
    }
    return status;
}

/**
 * @brief Set the Double Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value was below the lower set limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value was above the upper set limit
 */
template <> valueParseStatus_e varg<double>::setDoubleValue(const char* newValue)               {return varg_intf::getDoubleValue(newValue, value);}
template <typename T> valueParseStatus_e varg<T>::setDoubleValue(const char* newValue)          {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

//============================================================================================================================
//============================================================================================================================
//  Constructor functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Construct an argument with single input
 * 
 * @param defaultValue - Default value
 */
template <> varg<short int>::varg(short int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxSignedValue = static_cast<long long int>(SHRT_MAX);
    minSignedValue = static_cast<long long int>(SHRT_MIN);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg(int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxSignedValue = static_cast<long long int>(INT_MAX);
    minSignedValue = static_cast<long long int>(INT_MIN);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg(long int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxSignedValue = static_cast<long long int>(LONG_MAX);
    minSignedValue = static_cast<long long int>(LONG_MIN);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg(long long int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)               
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg(short unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxUnsignedValue = static_cast<long long unsigned>(USHRT_MAX);
    minUnsignedValue = static_cast<long long unsigned>(0ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg(unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxUnsignedValue = static_cast<long long unsigned>(UINT_MAX);
    minUnsignedValue = static_cast<long long unsigned>(0ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg(long unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxUnsignedValue = static_cast<long long unsigned>(ULONG_MAX);
    minUnsignedValue = static_cast<long long unsigned>(0ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg(long long unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg(double defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0.0)
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg(char defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> varg<bool>::varg(bool defaultValue) : varg_intf(), value(defaultValue), flagSetValue(!defaultValue)
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

template <> varg<std::string>::varg(std::string defaultValue) : varg_intf(), value(defaultValue), flagSetValue("")
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
}

template <> varg<short int>::varg(short int defaultValue, short int min, short int max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{ 
    maxSignedValue = static_cast<long long int>(max);
    minSignedValue = static_cast<long long int>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg(int defaultValue, int min, int max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxSignedValue = static_cast<long long int>(max);
    minSignedValue = static_cast<long long int>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg(long int defaultValue, long int min, long int max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxSignedValue = static_cast<long long int>(max);
    minSignedValue = static_cast<long long int>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg(long long int defaultValue, long long int min, long long int max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxSignedValue = max;
    minSignedValue = min;
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg(short unsigned defaultValue, short unsigned min, short unsigned max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxUnsignedValue = static_cast<long long unsigned>(max);
    minUnsignedValue = static_cast<long long unsigned>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg(unsigned defaultValue, unsigned min, unsigned max) :
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxUnsignedValue = static_cast<long long unsigned>(max);
    minUnsignedValue = static_cast<long long unsigned>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg(long unsigned defaultValue, long unsigned min, long unsigned max) :
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxUnsignedValue = static_cast<long long unsigned>(max);
    minUnsignedValue = static_cast<long long unsigned>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg(long long unsigned defaultValue, long long unsigned min, long long unsigned max) :
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxUnsignedValue = max;
    minUnsignedValue = min;
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg(double defaultValue, double min, double max) :
    varg_intf(), value(defaultValue), flagSetValue(0.0)
{
    maxDoubleValue = max;
    minDoubleValue = min;
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg(char defaultValue, char min, char max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxSignedValue = static_cast<long long int>(max);
    minSignedValue = static_cast<long long int>(min);

    std::stringstream myTypeStr;
    myTypeStr << "<" << min << ":" << max << ">";
    typeString = myTypeStr.str();
}

template <> varg<bool>::varg(bool defaultValue, bool min, bool max) : 
    varg_intf(), value(defaultValue), flagSetValue(0)
{
    maxUnsignedValue = static_cast<long long unsigned>(max);
    minUnsignedValue = static_cast<long long unsigned>(min);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

template <> varg<std::string>::varg(std::string defaultValue, std::string min, std::string max) : 
    varg_intf(), value(defaultValue), flagSetValue("")
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Set the Flag Value object
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 */
template <typename T> valueParseStatus_e varg<T>::setValue()                            {value = flagSetValue; return valueParseStatus_e::PARSE_SUCCESS_e;}

/**
 * @brief Set value from input string specializations
 * 
 * @param newValue - Input string to parse
 * 
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
 */
template <> valueParseStatus_e varg<short int>::setValue(const char* newValue)          {return setSignedValue(newValue);}
template <> valueParseStatus_e varg<int>::setValue(const char* newValue)                {return setSignedValue(newValue);}
template <> valueParseStatus_e varg<long int>::setValue(const char* newValue)           {return setSignedValue(newValue);}
template <> valueParseStatus_e varg<long long int>::setValue(const char* newValue)      {return setSignedValue(newValue);}

template <> valueParseStatus_e varg<short unsigned>::setValue(const char* newValue)     {return setUnsignedValue(newValue);}
template <> valueParseStatus_e varg<unsigned>::setValue(const char* newValue)           {return setUnsignedValue(newValue);}
template <> valueParseStatus_e varg<long unsigned>::setValue(const char* newValue)      {return setUnsignedValue(newValue);}
template <> valueParseStatus_e varg<long long unsigned>::setValue(const char* newValue) {return setUnsignedValue(newValue);}

template <> valueParseStatus_e varg<double>::setValue(const char* newValue)             {return setDoubleValue(newValue);}

template <> valueParseStatus_e varg<char>::setValue(const char* newValue)               {return setCharValue(newValue);}
template <> valueParseStatus_e varg<std::string>::setValue(const char* newValue)        {value = newValue; return valueParseStatus_e::PARSE_SUCCESS_e;}
template <> valueParseStatus_e varg<bool>::setValue(const char* newValue)               {return varg_intf::getBoolValue(newValue, value);}

/** @} */
