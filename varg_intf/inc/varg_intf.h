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
 * @file varg_intf.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once 

// Includes 
#include <stdlib.h>
#include <unistd.h>

namespace argparser
{

class varg_intf
{
    private:

    protected:
        /**
         * @brief Set the Bool Value object
         * 
         * @param newValue - input argument string
         * @param parsedValue - parsed boolean value if parsing succeeded
         * 
         * @return true if conversion succeeded
         * @return false if conversion failed
         */
        bool setBoolValue(const char* newValue, bool& parsedValue);

        /**
         * @brief Set the New character object value
         * 
         * @param newValue - input argument string
         * @param parsedValue - parsed character value if parsing succeeded
         * 
         * @return true if conversion succeeded
         * @return false if conversion failed
         */
        bool setCharValue(const char* newValue, char& parsedValue);

    public:
        /**
         * @brief Construct a varg_intf object
         */
        varg_intf();

        /**
         * @brief Destroy the varg object
         */
        virtual ~varg_intf();

        /**
         * @brief Get the base argument type as a string
         * 
         * @return char* - Base type string
         */
        virtual const char* getTypeString() = 0;

        /**
         * @brief Return if varg is a list of elements or a single element type
         * 
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual const bool isList() = 0;

        /**
         * @brief Virtual place holder for the template variable implementation setValue with input function
         * 
         * @param newValue - Input character string
         * 
         * @return true - if value was successsfully set
         * @return false - if input string could not
         */
        virtual bool setValue(const char* newValue) = 0;

        /**
         * Virtual place holder for the template variable implementation setValue function
         * 
         * @return true - if value was successsfully set
         * @return false - if input string could not
         */
        virtual bool setValue() = 0;
};

}; // end of namespace argparser

/** @} */
