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
 * @return true if conversion succeeded
 * @return false if conversion failed
 */
bool varg_intf::setBoolValue(const char* newValue, bool& parsedValue)
{
    if (newValue[1] != 0)
    {
        // Deal with multi-character string
        std::string testValue = newValue;
        std::transform(testValue.begin(), testValue.end(), testValue.begin(), ::toupper);
        if (testValue == "TRUE")
        {
            parsedValue = true;
            return true;
        }
        else if (testValue == "FALSE")
        {
            parsedValue = false;
            return true;
        }
        else
        {
            return false;
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
                return true;
            case 'f':
            case 'F':
            case '0':
                parsedValue = false;
                return true;
            default:
                return false;
        }
    }
}

/**
 * @brief Set the New character object value
 * 
 * @param newValue - input argument string
 * @param parsedValue - parsed character value if parsing succeeded
 * 
 * @return true if conversion succeeded
 * @return false if conversion failed
 */
bool varg_intf::setCharValue(const char* newValue, char& parsedValue)
{
    std::string inputValue(newValue);
    if ((inputValue.length() > 1) || (inputValue.empty()))
    {
        return false;
    }
    else
    {
        parsedValue = inputValue[0];
        return true;
    }
}


//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
varg_intf::varg_intf()      {}

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
