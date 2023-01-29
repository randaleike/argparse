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
 * @brief Set the Element Value object
 *
 * @param newValue - input argument string
 * @param typeStr - scanf type string
 *
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <typename T> bool listvarg<T>::setElementValue(const char* newValue, const char* typeStr)
{
    T element;
    int ret = sscanf(newValue, typeStr, &(element));
    if (ret == 1)
    {
        value.push_back(element);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Set the Element Value object
 *
 * @param newValue - input argument string
 *
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <> bool listvarg<char>::setCharElementValue(const char* newValue)
{
    char element;
    if (varg_intf::setCharValue(newValue, element))
    {
        value.push_back(element);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Set the Bool Value object
 *
 * @param newValue - input argument string
 *
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <> bool listvarg<bool>::setBoolValue(const char* newValue)
{
    bool element;
    if (varg_intf::setBoolValue(newValue, element))
    {
        value.push_back(element);
        return true;
    }
    else
    {
        return false;
    }
}
template <typename T> bool listvarg<T>::setBoolValue(const char* newValue)          {return false;}

/**
 * @brief Set the string Value object
 *
 * @param newValue - input argument string
 *
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <> bool listvarg<std::string>::setStringValue(const char* newValue)
{
    std::string element = newValue;
    value.push_back(element);
    return true;
}

template <typename T> bool listvarg<T>::setStringValue(const char* newValue)        {return false;}

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
template <> bool listvarg<short int>::setValue(const char* newValue)          {return setElementValue(newValue, "%hd");}
template <> bool listvarg<int>::setValue(const char* newValue)                {return setElementValue(newValue, "%d");}
template <> bool listvarg<long int>::setValue(const char* newValue)           {return setElementValue(newValue, "%ld");}
template <> bool listvarg<long long int>::setValue(const char* newValue)      {return setElementValue(newValue, "%lld");}

template <> bool listvarg<short unsigned>::setValue(const char* newValue)     {return setElementValue(newValue, "%hu");}
template <> bool listvarg<unsigned>::setValue(const char* newValue)           {return setElementValue(newValue, "%u");}
template <> bool listvarg<long unsigned>::setValue(const char* newValue)      {return setElementValue(newValue, "%lu");}
template <> bool listvarg<long long unsigned>::setValue(const char* newValue) {return setElementValue(newValue, "%llu");}

template <> bool listvarg<float>::setValue(const char* newValue)              {return setElementValue(newValue, "%f");}
template <> bool listvarg<double>::setValue(const char* newValue)             {return setElementValue(newValue, "%lf");}
template <> bool listvarg<long double>::setValue(const char* newValue)        {return setElementValue(newValue, "%llf");}

template <> bool listvarg<char>::setValue(const char* newValue)               {return setCharElementValue(newValue);}
template <> bool listvarg<std::string>::setValue(const char* newValue)        {return setStringValue(newValue);}
template <> bool listvarg<bool>::setValue(const char* newValue)               {return setBoolValue(newValue);}

/**
 * @brief Get the base argument type as a string
 *
 * @return char* - Base type string
 */
template <> const char* listvarg<short int>::getTypeString()            {return "<[+|-]int>";}
template <> const char* listvarg<int>::getTypeString()                  {return "<[+|-]int>";}
template <> const char* listvarg<long int>::getTypeString()             {return "<[+|-]int>";}
template <> const char* listvarg<long long int>::getTypeString()        {return "<[+|-]int>";}

template <> const char* listvarg<short unsigned>::getTypeString()       {return "<[+]int>";}
template <> const char* listvarg<unsigned>::getTypeString()             {return "<[+]int>";}
template <> const char* listvarg<long unsigned>::getTypeString()        {return "<[+]int>";}
template <> const char* listvarg<long long unsigned>::getTypeString()   {return "<[+]int>";}

template <> const char* listvarg<float>::getTypeString()                {return "<float>";}
template <> const char* listvarg<double>::getTypeString()               {return "<float>";}
template <> const char* listvarg<long double>::getTypeString()          {return "<float>";}

template <> const char* listvarg<char>::getTypeString()                 {return "<char>";}
template <> const char* listvarg<std::string>::getTypeString()          {return "<string>";}
template <> const char* listvarg<bool>::getTypeString()                 {return "<t|T|f|F>";}

/** @} */
