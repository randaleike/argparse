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
 * @file vargincrement.cpp
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes 
#include "vargincrement.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

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
vargincrement::vargincrement(): varg_intf(), value(0)   {}
vargincrement::~vargincrement()                         {}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Return if varg is a list of elements or a single element type
 * 
 * @return false - Only 0 or 1 argument values are allowed.
 */
const bool vargincrement::isList()                      {return false;}

/**
 * @brief Set the Flag Value object
 * 
 * @return true - Flag value set
 */
bool vargincrement::setValue()                          {value++; return true;}

/**
 * @brief Set value from input string specializations
 * 
 * @param newValue - Input string to parse
 * 
 * @return true - if argment string was parsed
 * @return false - if argment string failed to properly parse
 */
bool vargincrement::setValue(const char* newValue)      {return false;}

/**
 * @brief Get the base argument type as a string
 * 
 * @return char* - Base type string
 */
const char* vargincrement::getTypeString()              {return "incrementing flag";}

/** @} */
