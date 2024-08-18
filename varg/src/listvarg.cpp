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
 * @file listvarg.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes
#include <string>
#include "listvarg.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Set the value of a signed list element and add the new element to the value list object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
 */
template <> valueParseStatus_e listvarg<short unsigned>::setSignedElementValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<unsigned>::setSignedElementValue(const char* newValue)              {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<long unsigned>::setSignedElementValue(const char* newValue)         {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<long long unsigned>::setSignedElementValue(const char* newValue)    {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<double>::setSignedElementValue(const char* newValue)                {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<char>::setSignedElementValue(const char* newValue)                  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<bool>::setSignedElementValue(const char* newValue)                  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<std::string>::setSignedElementValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e listvarg<T>::setSignedElementValue(const char* newValue)
{
    long long int tempValue;
    valueParseStatus_e status = varg_intf::getSignedValue(newValue, tempValue);
    if (status == valueParseStatus_e::PARSE_SUCCESS_e)
    {
        T element = static_cast<T>(tempValue);
        value.push_back(element);
    }
    return status;
}

/**
 * @brief Set the value of an unsigned list element and add the new element to the value list object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
 */
template <> valueParseStatus_e listvarg<short int>::setUnsignedElementValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<int>::setUnsignedElementValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<long int>::setUnsignedElementValue(const char* newValue)            {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<long long int>::setUnsignedElementValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<double>::setUnsignedElementValue(const char* newValue)              {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<char>::setUnsignedElementValue(const char* newValue)                {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<bool>::setUnsignedElementValue(const char* newValue)                {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e listvarg<std::string>::setUnsignedElementValue(const char* newValue)         {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e listvarg<T>::setUnsignedElementValue(const char* newValue)
{
    long long unsigned tempValue;
    valueParseStatus_e status = varg_intf::getUnsignedValue(newValue, tempValue);
    if (status == valueParseStatus_e::PARSE_SUCCESS_e)
    {
        T element = static_cast<T>(tempValue);
        value.push_back(element);
    }
    return status;
}

/**
 * @brief Set the value of a double list element and add the new element to the value list object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
 * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
 */
template <> valueParseStatus_e listvarg<double>::setDoubleElementValue(const char* newValue)
{
    double element;
    valueParseStatus_e status = varg_intf::getDoubleValue(newValue, element);
    if (status == valueParseStatus_e::PARSE_SUCCESS_e)
    {
        value.push_back(element);
    }
    return status;
}

template <typename T> valueParseStatus_e listvarg<T>::setDoubleElementValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

/**
 * @brief Set the Element Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
template <> valueParseStatus_e listvarg<char>::setCharElementValue(const char* newValue)
{
    char element;
    valueParseStatus_e status = varg_intf::getCharValue(newValue, element);
    if (valueParseStatus_e::PARSE_SUCCESS_e == status)
    {
        value.push_back(element);
    }
    return status;
}

/**
 * @brief Set the Bool Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
template <> valueParseStatus_e listvarg<bool>::setBoolElementValue(const char* newValue)
{
    bool element;
    valueParseStatus_e status = varg_intf::getBoolValue(newValue, element);
    if (valueParseStatus_e::PARSE_SUCCESS_e == status)
    {
        value.push_back(element);
    }
    return status;
}
template <typename T> valueParseStatus_e listvarg<T>::setBoolElementValue(const char* newValue)     {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

/**
 * @brief Set the string Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 */
template <> valueParseStatus_e listvarg<std::string>::setStringElementValue(const char* newValue)
{
    std::string element = newValue;
    value.push_back(element);
    return valueParseStatus_e::PARSE_SUCCESS_e;
}

template <typename T> valueParseStatus_e listvarg<T>::setStringElementValue(const char* newValue)   {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

//============================================================================================================================
//============================================================================================================================
//  Constructor
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Construct a varg_intf object
 */
template <> listvarg<short int>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxSigned(SHRT_MIN, SHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> listvarg<int>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxSigned(INT_MIN, INT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> listvarg<long int>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxSigned(LONG_MIN, LONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> listvarg<long long int>::listvarg()
{
    value.clear();
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> listvarg<short unsigned>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxUnsigned(0ULL, USHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> listvarg<unsigned>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxUnsigned(0ULL, UINT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> listvarg<long unsigned>::listvarg()
{
    value.clear();
    varg_intf::setMinMaxUnsigned(0ULL, ULONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> listvarg<long long unsigned>::listvarg()
{
    value.clear();
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> listvarg<double>::listvarg()
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> listvarg<char>::listvarg()
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> listvarg<bool>::listvarg()
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

template <> listvarg<std::string>::listvarg()
{
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Set value from input string specializations
 *
 * @param newValue - Input string to parse
 *
 * @return true - if argment string was parsed
 * @return false - if argment string failed to properly parse
 */
template <> valueParseStatus_e listvarg<short int>::setValue(const char* newValue)          {return setSignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<int>::setValue(const char* newValue)                {return setSignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<long int>::setValue(const char* newValue)           {return setSignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<long long int>::setValue(const char* newValue)      {return setSignedElementValue(newValue);}

template <> valueParseStatus_e listvarg<short unsigned>::setValue(const char* newValue)     {return setUnsignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<unsigned>::setValue(const char* newValue)           {return setUnsignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<long unsigned>::setValue(const char* newValue)      {return setUnsignedElementValue(newValue);}
template <> valueParseStatus_e listvarg<long long unsigned>::setValue(const char* newValue) {return setUnsignedElementValue(newValue);}

template <> valueParseStatus_e listvarg<double>::setValue(const char* newValue)             {return setDoubleElementValue(newValue);}

template <> valueParseStatus_e listvarg<char>::setValue(const char* newValue)               {return setCharElementValue(newValue);}
template <> valueParseStatus_e listvarg<bool>::setValue(const char* newValue)               {return setBoolElementValue(newValue);}
template <> valueParseStatus_e listvarg<std::string>::setValue(const char* newValue)        {return setStringElementValue(newValue);}

/** @} */
