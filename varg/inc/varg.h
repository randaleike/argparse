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
 * @file argparse_api.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once 

// Includes 
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "varg_intf.h"

namespace argparser
{
/**
 * @brief Variable argument type template class
 */
template <typename T> class varg : public varg_intf
{
    private:
        T       flagSetValue;       ///< Value to set when flag key value is found

        /**
         * @brief Set the value from the input string
         * 
         * @param newValue - Input value as a C string
         * @param typeStr  - scanf type string
         * 
         * @return true - Scanf string to value conversion succeeded
         * @return false - Scanf string to value conversion failed
         */
        bool setNewValue(const char* newValue, const char* typeStr);

        /**
         * @brief Set the New character object value
         * 
         * @param newValue - input argument string
         * 
         * @return true if conversion succeeded
         * @return false if conversion failed
         */
        bool setNewCharValue(const char* newValue);

        /**
         * @brief Set the Bool Value object
         * 
         * @param newValue - input argument string
         * 
         * @return true if conversion succeeded
         * @return false if conversion failed
         */
        bool setBoolValue(const char* newValue);

    public:
        T       value;              ///< Current saved value

        /**
         * @brief Construct a varg_intf object
         */
        varg(T defaultValue, T flagValue) : varg_intf(), value(defaultValue), flagSetValue(flagValue)   {}

        varg(T defaultValue);

        /**
         * @brief Destroy the varg object
         */
        virtual ~varg()                                                                                 {}

        /**
         * @brief Get the base argument type as a string
         * 
         * @return const char* - Base type string
         */
        virtual const char* getTypeString();

        /**
         * @brief Return if varg is a list of elements or a single element type
         * 
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual const bool isList()                                                                     {return false;}

        /**
         * @brief Virtual place holder for the template variable implementation setValue with input function
         * 
         * @param newValue - Input character string
         * 
         * @return true - if value was successsfully set
         * @return false - if input string could not
         */
        virtual bool setValue(const char* newValue);

        /**
         * Virtual place holder for the template variable implementation setValue function
         * 
         * @return true - if value was successsfully set
         * @return false - if input string could not
         */
        virtual bool setValue();
}; // end of class definition

}; // end of namespace argparser

/** @} */
