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
 * @file vargenum.h
 * @ingroup libvarg
 * @{
 */

#pragma once

// Includes
#include <cstdlib>
#include <map>
#include <string>
#include <sstream>
#include "varg_intf.h"

namespace argparser
{
/**
 * @brief Variable argument enum type template class
 */
template <typename T> class vargenum : public varg_intf
{
    private:
        std::map< std::string, T >  enumNameMap;    ///< Map of enum strings to value
        std::string                 enumName;       ///< Enum name

    public:
        // NOLINTNEXTLINE
        T                           value;          ///< Current saved value

        /**
         * @brief Construct a vargenum object
         *
         * @param defaultValue - Default value for the vargenum.value to start with
         * @param name - name of the enum
         */
        vargenum(T defaultValue, const char* name = "enum value"): value(defaultValue), enumName(name) {enumNameMap.clear();}

        /**
         * @brief Copy constructor for vargenum object
         *
         * @param other - Reference to the object to copy
         */
        vargenum(const vargenum& other) = default;

        /**
         * @brief Reference copy constructor for vargenum object
         *
         * @param other - Reference to the reference object to copy
         */
        vargenum(vargenum&& other) = default;

        /**
         * @brief Assignment copy constructor for vargenum object
         *
         * @param other - Reference to the object to copy
         */
        vargenum& operator=(const vargenum& other) = default;

        /**
         * @brief Assignment reference copy constructor for vargenum object
         *
         * @param other - Reference to the reference object to copy
         */
        vargenum& operator=(vargenum&& other) = default;

        /**
         * @brief Destroy the vargenum object
         */
        ~vargenum() override                        {enumNameMap.clear(); enumName.clear();}

        /**
         * @brief Get the base argument type as a string
         *
         * @return const char* - Base type string
         */
        const char* getTypeString() override           {return ((enumName.empty()) ? "enum value" : enumName.c_str());}

        /**
        * @brief Get the base argument range as a string
        *
        * @return const char* - Base type string
        */
        const char* getRangeString() override
        {
            if (rangeString.empty())
            {
                rangeString = "<>";
            }
            else if (rangeString.back() == '|')
            {
                rangeString.pop_back();
                rangeString = "<" + rangeString + ">";
            }
            return rangeString.c_str();
        }

        /**
         * @brief Return if vargenum is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        [[nodiscard]] bool isList() const override     {return false;}

        /**
         * @brief Virtual interface method implementation for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
         */
        valueParseStatus_e setValue(const char* newValue) override
        {
            valueParseStatus_e status = valueParseStatus_e::PARSE_INVALID_INPUT_e;
            std::string searchString = newValue;
            for (auto const & [key, enumval] : enumNameMap)
            {
                if (key == searchString)
                {
                    value = enumval;
                    status = valueParseStatus_e::PARSE_SUCCESS_e;
                    break;
                }
            }
            return status;
        }

        /**
         * Virtual interface method implementation for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - enum type must have an input string to map to the enum value
         */
        valueParseStatus_e setValue() override     {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - Base variable is never empty
         */
        [[nodiscard]] bool isEmpty() override      {return false;}

        /**
         * @brief Assign enum map values
         *
         * @param entryName - Name of the enum value
         * @param enumValue - Value to assign for the name
         */
        void setEnumValue(const char* entryName, T enumValue)
        {
            std::string entryNameStr = entryName;
            enumNameMap.emplace(entryNameStr, enumValue);
            rangeString.append(entryNameStr + "|");
        }

        /**
         * @brief Assign enum name value
         *
         * @param enumTypeName - Name of the enum type
         */
        void setEnumTypeName(const char* enumTypeName)      {enumName = enumTypeName;}

}; // end of class definition

}; // end of namespace argparser

/** @} */
