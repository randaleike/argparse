/*
 Copyright (c) 2022-2024 Randal Eike

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
 * @file listvarg.h
 * @ingroup libvarg
 * @{
 */

#pragma once

// Includes
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "varg_intf.h"

namespace argparser
{

/**
 * @brief Variable list argument type template class
 */
template <typename T> class listvarg : public varg_intf
{
    protected:
        /**
         * @brief Set the value of a signed element and add the new element to the value list object
         *
         * @param newValue - Pointer to the text value string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        valueParseStatus_e setSignedElementValue(const char* newValue);

        /**
         * @brief Set the value of an unsigned list element and add the new element to the value list object
         *
         * @param newValue - Pointer to the text value string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        valueParseStatus_e setUnsignedElementValue(const char* newValue);

        /**
         * @brief Set the value of a double list element and add the new element to the value list object
         *
         * @param newValue - Pointer to the text value string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        valueParseStatus_e setDoubleElementValue(const char* newValue);

        /**
         * @brief Set the value of a boolean element and add the new element to the value list object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        valueParseStatus_e setBoolElementValue(const char* newValue);

        /**
         * @brief Set the Element Value object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        valueParseStatus_e setCharElementValue(const char* newValue);

        /**
         * @brief Set the value of a string element and add the new element to the value list object
         *
         * @param newValue - input argument string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        valueParseStatus_e setStringElementValue(const char* newValue);

    public:
        std::list< T >  value;          ///< Current saved list values

        /**
         * @brief Construct a varg_intf object
         */
        listvarg();

        /**
         * @brief Destroy the varg object
         */
        virtual ~listvarg()                                             {value.clear();}

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual bool isList() const                                     {return true;}

        /**
         * @brief Virtual interface method implementation for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         * @return valueParseStatus_e::PARSE_BOUNDARY_LOW_e  - if value exceeds lower value limit
         * @return valueParseStatus_e::PARSE_BOUNDARY_HIGH_e - if value exceeds upper value limit
         */
        virtual valueParseStatus_e setValue(const char* newValue);

        /**
         * Virtual interface method implementation for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - Lists do not have a default set value
         */
        virtual valueParseStatus_e setValue()                           {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - if the list is empty
         * @return false - if list is not empty
         */
        virtual bool isEmpty()                                          {return value.empty();}
}; // end of class definition

}; // end of namespace argparser

/** @} */
