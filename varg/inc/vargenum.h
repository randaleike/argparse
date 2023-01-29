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
 * @file vargenum.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once

// Includes
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <string>
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
        T                           value;          ///< Current saved value

        /**
         * @brief Construct a vargenum object
         *
         * @param name - name of the enum
         */
        vargenum(T defaultValue, const char* name = ""): varg_intf(), value(defaultValue), enumName(name)    {enumNameMap.clear();}

        /**
         * @brief Destroy the vargenum object
         */
        virtual ~vargenum()                                             {enumNameMap.clear(); enumName.clear();}

        /**
         * @brief Get the base argument type as a string
         *
         * @return const char* - Base type string
         */
        virtual const char* getTypeString()
        {
            if (enumName.empty()) return "enum value";
            else return enumName.c_str();
        }

        /**
         * @brief Return if varg is a list of elements or a single element type
         *
         * @return true - List type variable, multiple arguement values are allowed
         * @return false - Only 0 or 1 argument values are allowed.
         */
        virtual const bool isList()                                     {return false;}

        /**
         * @brief Virtual interface method implementation for the template variable implementation setValue with input function
         *
         * @param newValue - Input character string
         *
         * @return true - if value was successsfully set
         * @return false - if input string could not
         */
        virtual bool setValue(const char* newValue)
        {
            std::string searchString = newValue;
            for (auto const & [key, val] : enumNameMap)
            {
                if (key == searchString)
                {
                    value = val;
                    return true;
                }
            }

            return false;
        }


        /**
         * Virtual interface method implementation for the template variable implementation setValue function
         *
         * @return false - enum type must have an input string to map to the enum value
         */
        virtual bool setValue()                                         {return false;}

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - Base variable is never empty
         */
        virtual bool isEmpty()                                          {return false;}

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
        }

}; // end of class definition

}; // end of namespace argparser

/** @} */
