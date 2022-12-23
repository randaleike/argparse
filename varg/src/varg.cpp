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
 * @file varg.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes 
#include "varg.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Set the New Value object
 * 
 * @param newValue - input argument string
 * @param typeStr - scanf type string
 * 
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <typename T> bool varg<T>::setNewValue(const char* newValue, const char* typeStr)
{
    int ret = sscanf(newValue, typeStr, &(value)); 
    return (ret == 1);
}

/**
 * @brief Set the Bool Value object
 * 
 * @param newValue - input argument string
 * 
 * @return true if scanf conversion succeeded
 * @return false if scanf conversion failed
 */
template <> bool varg<bool>::setBoolValue(const char* newValue)           {return (varg_intf::setBoolValue(newValue, value));}
template <typename T> bool varg<T>::setBoolValue(const char* newValue)    {return false;}

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
template <> varg<short int>::varg(short int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)                   { }
template <> varg<int>::varg(int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)                               { }
template <> varg<long int>::varg(long int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)                     { }
template <> varg<long long int>::varg(long long int defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)           { }

template <> varg<short unsigned>::varg(short unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)         { }
template <> varg<unsigned>::varg(unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)                     { }
template <> varg<long unsigned>::varg(long unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)           { }
template <> varg<long long unsigned>::varg(long long unsigned defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0) { }

template <> varg<double>::varg(double defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0.0)                       { }
template <> varg<char>::varg(char defaultValue) : varg_intf(), value(defaultValue), flagSetValue(0)                             { }
template <> varg<bool>::varg(bool defaultValue) : varg_intf(), value(defaultValue), flagSetValue(!defaultValue)                 { }
template <> varg<std::string>::varg(std::string defaultValue) : varg_intf(), value(defaultValue), flagSetValue("")              { }

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Set the Flag Value object
 * 
 * @return true - Flag value set
 */
template <> bool varg<short int>::setValue()                        {value = flagSetValue; return true;}
template <> bool varg<int>::setValue()                              {value = flagSetValue; return true;}
template <> bool varg<long int>::setValue()                         {value = flagSetValue; return true;}
template <> bool varg<long long int>::setValue()                    {value = flagSetValue; return true;}

template <> bool varg<short unsigned>::setValue()                   {value = flagSetValue; return true;}
template <> bool varg<unsigned>::setValue()                         {value = flagSetValue; return true;}
template <> bool varg<long unsigned>::setValue()                    {value = flagSetValue; return true;}
template <> bool varg<long long unsigned>::setValue()               {value = flagSetValue; return true;}

template <> bool varg<double>::setValue()                           {value = flagSetValue; return true;}
template <> bool varg<char>::setValue()                             {value = flagSetValue; return true;}
template <> bool varg<std::string>::setValue()                      {value = flagSetValue; return true;}
template <> bool varg<bool>::setValue()                             {value = flagSetValue; return true;}

/**
 * @brief Set value from input string specializations
 * 
 * @param newValue - Input string to parse
 * 
 * @return true - if argment string was parsed
 * @return false - if argment string failed to properly parse
 */
template <> bool varg<short int>::setValue(const char* newValue)          {return setNewValue(newValue, "%hd");}
template <> bool varg<int>::setValue(const char* newValue)                {return setNewValue(newValue, "%d");}
template <> bool varg<long int>::setValue(const char* newValue)           {return setNewValue(newValue, "%ld");}
template <> bool varg<long long int>::setValue(const char* newValue)      {return setNewValue(newValue, "%lld");}

template <> bool varg<short unsigned>::setValue(const char* newValue)     {return setNewValue(newValue, "%hu");}
template <> bool varg<unsigned>::setValue(const char* newValue)           {return setNewValue(newValue, "%u");}
template <> bool varg<long unsigned>::setValue(const char* newValue)      {return setNewValue(newValue, "%lu");}
template <> bool varg<long long unsigned>::setValue(const char* newValue) {return setNewValue(newValue, "%llu");}

template <> bool varg<double>::setValue(const char* newValue)             {return setNewValue(newValue, "%lf");}

template <> bool varg<char>::setValue(const char* newValue)               {return varg_intf::setCharValue(newValue, value);}
template <> bool varg<std::string>::setValue(const char* newValue)        {value = newValue; return true;}
template <> bool varg<bool>::setValue(const char* newValue)               {return varg_intf::setBoolValue(newValue, value);}


/**
 * @brief Get the base argument type as a string
 * 
 * @return char* - Base type string
 */
template <> const char* varg<short int>::getTypeString()             {return "<[+|-]int>";}
template <> const char* varg<int>::getTypeString()                   {return "<[+|-]int>";}
template <> const char* varg<long int>::getTypeString()              {return "<[+|-]int>";}
template <> const char* varg<long long int>::getTypeString()         {return "<[+|-]int>";}

template <> const char* varg<short unsigned>::getTypeString()        {return "<[+]int>";}
template <> const char* varg<unsigned>::getTypeString()              {return "<[+]int>";}
template <> const char* varg<long unsigned>::getTypeString()         {return "<[+]int>";}
template <> const char* varg<long long unsigned>::getTypeString()    {return "<[+]int>";}

template <> const char* varg<double>::getTypeString()                {return "<float>";}

template <> const char* varg<char>::getTypeString()                  {return "<char>";}
template <> const char* varg<std::string>::getTypeString()           {return "<string>";}
template <> const char* varg<bool>::getTypeString()                  {return "<t|T|f|F>";}

/** @} */
