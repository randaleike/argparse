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
 * @file varg.cpp
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
#include "varg.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  varg Protected functions
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
template <> valueParseStatus_e varg<short int>::setBoolValue(const char* newValue)      {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<int>::setBoolValue(const char* newValue)            {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long int>::setBoolValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long int>::setBoolValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned short>::setBoolValue(const char* newValue) {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned>::setBoolValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long>::setBoolValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long long>::setBoolValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setBoolValue(const char* newValue)         {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setBoolValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

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
template <> valueParseStatus_e varg<short int>::setCharValue(const char* newValue)      {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<int>::setCharValue(const char* newValue)            {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long int>::setCharValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long int>::setCharValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned short>::setCharValue(const char* newValue) {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned>::setCharValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long>::setCharValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long long>::setCharValue(const char* newValue)  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setCharValue(const char* newValue)         {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setCharValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

/**
 * @brief Set the Double Value object
 *
 * @param newValue - input argument string
 *
 * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
 * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
template <> valueParseStatus_e varg<short unsigned>::setSignedValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned>::setSignedValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long unsigned>::setSignedValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long unsigned>::setSignedValue(const char* newValue)   {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setSignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setSignedValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setSignedValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e varg<T>::setSignedValue(const char* newValue)
{
    long long int tempValue = 0LL;
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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
template <> valueParseStatus_e varg<short int>::setUnsignedValue(const char* newValue)          {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<int>::setUnsignedValue(const char* newValue)                {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long int>::setUnsignedValue(const char* newValue)           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long int>::setUnsignedValue(const char* newValue)      {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<double>::setUnsignedValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setUnsignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setUnsignedValue(const char* newValue)               {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

template <typename T> valueParseStatus_e varg<T>::setUnsignedValue(const char* newValue)
{
    long long unsigned tempValue = 0ULL;
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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
 */
template <> valueParseStatus_e varg<double>::setDoubleValue(const char* newValue)               {return varg_intf::getDoubleValue(newValue, value);}
template <> valueParseStatus_e varg<short int>::setDoubleValue(const char* newValue)            {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<int>::setDoubleValue(const char* newValue)                  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long int>::setDoubleValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<long long int>::setDoubleValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned short>::setDoubleValue(const char* newValue)       {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned>::setDoubleValue(const char* newValue)             {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long>::setDoubleValue(const char* newValue)        {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<unsigned long long>::setDoubleValue(const char* newValue)   {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<bool>::setDoubleValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}
template <> valueParseStatus_e varg<char>::setDoubleValue(const char* newValue)                 {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

//============================================================================================================================
//============================================================================================================================
//  varg Constructor with no input set functions
//============================================================================================================================
//============================================================================================================================
template <> varg<short int>::varg() :
    value(0), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(SHRT_MIN, SHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg() :
    value(0), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(INT_MIN, INT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg() :
    value(0L), flagSetValue(0L)
{
    varg_intf::setMinMaxSigned(LONG_MIN, LONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg() :
    value(0LL), flagSetValue(0LL)
{
    varg_intf::setMinMaxSigned(LLONG_MIN, LLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg() :
    value(0), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(0ULL, USHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg() :
    value(0), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(0ULL, UINT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg() :
    value(0UL), flagSetValue(0UL)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg() :
    value(0ULL), flagSetValue(0ULL)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg() :
    value(0.0), flagSetValue(0.0)
{
    varg_intf::setMinMaxDouble(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg() :
    value('\0'), flagSetValue('\0')
{
    varg_intf::setMinMaxSigned(0LL, SCHAR_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> varg<bool>::varg() :
    value(false), flagSetValue(true)
{
    varg_intf::setMinMaxUnsigned(0ULL, 1ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

//============================================================================================================================
//============================================================================================================================
//  varg Constructor with default value set functions
//============================================================================================================================
//============================================================================================================================
template <> varg<short int>::varg(short int defaultValue) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(SHRT_MIN, SHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg(int defaultValue) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(INT_MIN, INT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg(long int defaultValue) :
    value(defaultValue), flagSetValue(0L)
{
    varg_intf::setMinMaxSigned(LONG_MIN, LONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg(long long int defaultValue) :
    value(defaultValue), flagSetValue(0LL)
{
    varg_intf::setMinMaxSigned(LLONG_MIN, LLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg(short unsigned defaultValue) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(0ULL, USHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg(unsigned defaultValue) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(0ULL, UINT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg(long unsigned defaultValue) :
    value(defaultValue), flagSetValue(0UL)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg(long long unsigned defaultValue) :
    value(defaultValue), flagSetValue(0ULL)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg(double defaultValue) :
    value(defaultValue), flagSetValue(0.0)
{
    varg_intf::setMinMaxDouble(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg(char defaultValue) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(0LL, SCHAR_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> varg<bool>::varg(bool defaultValue) :
    value(defaultValue), flagSetValue(!defaultValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, 1ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

//============================================================================================================================
//============================================================================================================================
//  varg Constructor with default and flag value set functions
//============================================================================================================================
//============================================================================================================================
template <> varg<short int>::varg(short int defaultValue, short int flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxSigned(SHRT_MIN, SHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg(int defaultValue, int flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxSigned(INT_MIN, INT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg(long int defaultValue, long int flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxSigned(LONG_MIN, LONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg(long long int defaultValue, long long int flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxSigned(LLONG_MIN, LLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg(short unsigned defaultValue, short unsigned flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, USHRT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg(unsigned defaultValue, unsigned flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, UINT_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg(long unsigned defaultValue, long unsigned flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg(long long unsigned defaultValue, long long unsigned flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, ULLONG_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg(double defaultValue, double flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxDouble(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg(char defaultValue, char flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxSigned(0LL, SCHAR_MAX);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> varg<bool>::varg(bool defaultValue, bool flagValue) :
    value(defaultValue), flagSetValue(flagValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, 1ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

//============================================================================================================================
//============================================================================================================================
//  varg Constructor with default and min/max set functions
//============================================================================================================================
//============================================================================================================================
template <> varg<short int>::varg(short int defaultValue, short int min, short int max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<int>::varg(int defaultValue, int min, int max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long int>::varg(long int defaultValue, long int min, long int max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<long long int>::varg(long long int defaultValue, long long int min, long long int max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_SIGNED);
}

template <> varg<short unsigned>::varg(short unsigned defaultValue, short unsigned min, short unsigned max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<unsigned>::varg(unsigned defaultValue, unsigned min, unsigned max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long unsigned>::varg(long unsigned defaultValue, long unsigned min, long unsigned max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<long long unsigned>::varg(long long unsigned defaultValue, long long unsigned min, long long unsigned max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxUnsigned(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_UNSIGNED);
}

template <> varg<double>::varg(double defaultValue, double min, double max) :
    value(defaultValue), flagSetValue(0.0)
{
    varg_intf::setMinMaxDouble(min, max);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_DOUBLE);
}

template <> varg<char>::varg(char defaultValue, char min, char max) :
    value(defaultValue), flagSetValue(0)
{
    varg_intf::setMinMaxSigned(static_cast<long long int>(min), static_cast<long long int>(max));
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_CHAR);
}

template <> varg<bool>::varg(bool defaultValue, bool min, bool max) :
    value(defaultValue), flagSetValue(!defaultValue)
{
    varg_intf::setMinMaxUnsigned(0ULL, 1ULL);
    varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_BOOL);
}

//============================================================================================================================
//============================================================================================================================
//  varg Public functions
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
 * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
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
template <> valueParseStatus_e varg<bool>::setValue(const char* newValue)               {return varg_intf::getBoolValue(newValue, value);}

/** @} */
