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
 * @file vargincrement.h
 * @ingroup libvarg
 * @{
 */

#pragma once

// Includes
#include <stdlib.h>
#include <unistd.h>
#include "varg_intf.h"

namespace argparser
{
/**
 * @brief Variable argument type template class
 */
class vargincrement : public varg_intf
{
    private:
    public:
        int       value;              ///< Current saved value

        /**
         * @brief Construct a varg_intf object
         */
        vargincrement();

        /**
         * @brief Destroy the varg object
         */
        virtual ~vargincrement();

        /**
         * @brief Get the base argument type as a string
         *
         * @return const char* - Base type string
         */
        virtual const char* getTypeString()             {return "incrementing flag";}

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual bool isList() const                     {return false;}

        /**
         * @brief Virtual interface method implementation for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - increment variables don't take in input
         */
        virtual valueParseStatus_e setValue(const char* newValue);

        /**
         * Virtual interface method implementation for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully incremented
         */
        virtual valueParseStatus_e setValue();

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - Base variable is never empty
         */
        virtual bool isEmpty()                              {return false;}
}; // end of class definition

}; // end of namespace argparser

/** @} */
