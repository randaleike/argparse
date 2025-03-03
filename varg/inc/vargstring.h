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
 * @file vargstring.h
 * @ingroup libvarg
 * @{
 */

#pragma once

// Includes
#include <cstdlib>
#include <string>
#include <utility>
#include "varg_intf.h"

namespace argparser
{
/**
 * @brief String variable argument class
 */
class vargstring : public varg_intf
{
    private:
        size_t          maxStringLength;    ///< Maximum number of characters to assign
        size_t          minStringLength;    ///< Minimum number of characters to assign
    public:
        // NOLINTNEXTLINE
        std::string     value;              ///< Current assigned value

        /**
        * @brief Construct a varg_intf object
        *
        * @param defaultValue - Initial value of vargstring.value, default = ""
        * @param min - Minimum allowed string length, default = 0
        * @param max - Maximum allowed string length, default = max string length
        */
        vargstring(std::string defaultValue = "", size_t min = 0, size_t max = 0) :
            maxStringLength(max), minStringLength(min), value(std::move(defaultValue))
        {
            if (max == 0)
            {
                maxStringLength = value.max_size();
            }
            varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
        }

        /**
        * @brief Copy constructor for vargstring object
        *
        * @param other - Reference to the object to copy
        */
        vargstring(const vargstring& other) = default;

        /**
        * @brief Reference copy constructor for vargstring object
        *
        * @param other - Reference to the reference object to copy
        */
        vargstring(vargstring&& other) = default;

        /**
        * @brief Assignment copy constructor for vargstring object
        *
        * @param other - Reference to the object to copy
        */
        vargstring& operator=(const vargstring& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargstring object
        *
        * @param other - Reference to the reference object to copy
        */
        vargstring& operator=(vargstring&& other) = default;

        /**
        * @brief Destroy the vargstring object
        */
        ~vargstring() override = default;

        /**
        * @brief Return if vargstring is a list of elements or a single element type
        *
        * @return true - List type variable, multiple arguement values are allowed
        * @return false - Only 0 or 1 argument values are allowed.
        */
        [[nodiscard]] bool isList() const override {return false;}

        /**
        * @brief Virtual interface method implementation for the template variable implementation setValue with input function
        *
        * @param newValue - Input character string
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
        * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
        * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
        */
        valueParseStatus_e setValue(const char* newValue) override;

        /**
        * Virtual interface method implementation for the template variable implementation setValue function
        *
        * @return valueParseStatus_e::PARSE_INVALID_INPUT_e
        */
        valueParseStatus_e setValue() override  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        bool isEmpty() override                 {return false;}

}; // end of class vargstring definition

}; // end of namespace argparser

/** @} */
