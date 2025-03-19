/*
 Copyright (c) 2025 Randal Eike

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
 * @file vargcstring_clib_private.h
 * @ingroup libvarg
 * @{
 */

#pragma once

#include "varg_intf.h"
#include "vargstring.h"

namespace argparser
{

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================

//=================================================================================================
//=========================== Class definitions ===================================================
//=================================================================================================
/**
 * @brief C-style string variable argument type template class
 */
class vargcstring : public varg_intf
{
    private:
        char*           cstorage;               ///< Pointer to the array cstorage location
        size_t          elementCount;           ///< Number of elements the array can store
        size_t          assignmentCount;        ///< Number of characters assigned
        vargstring      argValue;               ///< Temporary argument cstorage
    public:
        /**
        * @brief Construct a vargcarray object
        *
        * @param valueArray - Pointer to the value cstorage array
        * @param arraySize - Number of elements in the cstorage array
        */
        vargcstring(char* valueArray, size_t arraySize) :
            cstorage(valueArray), elementCount(arraySize), assignmentCount(0), argValue("")
        {
            varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
        }

        /**
        * @brief Construct a vargcstring object
        *
        * @param valueArray - Pointer to the value cstorage array
        * @param arraySize - Number of elements in the cstorage array
        * @param min - Minimum allowed srtring
        * @param max - Maximum allowed set value
        */
        vargcstring(char* valueArray, size_t arraySize, size_t min, size_t max) :
            cstorage(valueArray), elementCount(arraySize), assignmentCount(0), argValue("", min, max)
        {
            varg_intf::setTypeString(typeStringFormat_e::TYPE_FMT_STRING);
        }


        /**
        * @brief Copy constructor for vargcstring object
        *
        * @param other - Reference to the object to copy
        */
        vargcstring(const vargcstring& other) = default;

        /**
        * @brief Reference copy constructor for vargcstring object
        *
        * @param other - Reference to the reference object to copy
        */
        vargcstring(vargcstring&& other) = default;

        /**
        * @brief Assignment copy constructor for vargcstring object
        *
        * @param other - Reference to the object to copy
        */
        vargcstring& operator=(const vargcstring& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargcstring object
        *
        * @param other - Reference to the reference object to copy
        */
        vargcstring& operator=(vargcstring&& other) = default;

        /**
        * @brief Destroy the varg object
        */
        ~vargcstring() override = default;

        /**
        * @brief Return if vargptrincrement is a list of elements or a single element type
        *
        * @return true - List type variable, multiple arguement values are allowed
        * @return false - Only 0 or 1 argument values are allowed.
        */
        [[nodiscard]] bool isList() const override                  {return false;}

        /**
        * @brief Virtual interface method implementation for the template variable implementation setValue with input function
        *
        * @param newValue - Input character string
        *
        * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - increment variables don't take in input
        */
        valueParseStatus_e setValue(const char* newValue) override
        {
            // Get the string value
            valueParseStatus_e status = argValue.setValue(newValue);

            if (valueParseStatus_e::PARSE_SUCCESS_e == status)
            {
                // Copy the string to the c storage
                char* currentChar = cstorage;
                size_t index = 0;
                for (char& stringChar : argValue.value)
                {
                    if (index < (elementCount-1))
                    {
                        *currentChar++ = stringChar;        // NOLINT
                        *currentChar = '\0';
                        assignmentCount++;
                    }
                    index++;
                }
            }
            return status;
        }

        /**
        * Virtual interface method implementation for the template variable implementation setValue function
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully incremented
        */
        valueParseStatus_e setValue() override                      {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        [[nodiscard]] bool isEmpty() override                       {return ((nullptr == cstorage) && (argValue.isEmpty()));}

        /**
        * Virtual place holder for the template variable implementation getAssignmentCount function
        *
        * @return size_t - number of elements assigned to list object
        */
        size_t getAssignmentCount() override                        {return assignmentCount;}
}; // end of class definition

}; // end of namespace argparser

/** @} */
