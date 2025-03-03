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
 * @file varg.h
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
 * @brief Variable argument type template class
 */
template <typename T> class varg : public varg_intf
{
    private:
        T           flagSetValue;       ///< Value to set when flag key value is found

    protected:
        /**
         * @brief Set the New character object value
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        valueParseStatus_e setCharValue(const char* newValue);

        /**
         * @brief Set the Bool Value object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        valueParseStatus_e setBoolValue(const char* newValue);

        /**
         * @brief Set the Signed Value object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
         */
        valueParseStatus_e setSignedValue(const char* newValue);

        /**
         * @brief Set the Unsigned Value object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
         */
        valueParseStatus_e setUnsignedValue(const char* newValue);

        /**
         * @brief Set the Double Value object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
         */
        valueParseStatus_e setDoubleValue(const char* newValue);

    public:
        // NOLINTNEXTLINE
        T       value;              ///< Current saved value

        /**
         * @brief Construct a varg_intf object
         */
        varg();

        /**
         * @brief Construct a varg_intf object
         *
         * @param defaultValue - Initial value of varg.value
         */
        varg(T defaultValue);

        /**
         * @brief Construct a varg_intf object
         *
         * @param defaultValue - Initial value of varg.value
         * @param flagValue    - Flag set value of varg.value
         */
        varg(T defaultValue, T flagValue);

        /**
         * @brief Construct a varg_intf object
         *
         * @param defaultValue - Initial value of varg.value
         * @param min - Minimum allowed set value
         * @param max - Maximum allowed set value
         */
        varg(T defaultValue, T min, T max);

        /**
         * @brief Copy constructor for varg object
         *
         * @param other - Reference to the object to copy
         */
        varg(const varg& other) = default;

        /**
         * @brief Reference copy constructor for varg object
         *
         * @param other - Reference to the reference object to copy
         */
        varg(varg&& other) = default;

        /**
         * @brief Assignment copy constructor for varg object
         *
         * @param other - Reference to the object to copy
         */
        varg& operator=(const varg& other) = default;

        /**
         * @brief Assignment reference copy constructor for varg object
         *
         * @param other - Reference to the reference object to copy
         */
        varg& operator=(varg&& other) = default;

        /**
         * @brief Destroy the varg object
         */
        ~varg() override = default;

        /**
         * @brief Return if varg is a list of elements or a single element type
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
         * @return valueParseStatus_e::PARSE_SUCCESS_e
         */
        valueParseStatus_e setValue() override;

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - Base variable is never empty
         */
        bool isEmpty() override {return false;}
}; // end of class varg definition

}; // end of namespace argparser

/** @} */
