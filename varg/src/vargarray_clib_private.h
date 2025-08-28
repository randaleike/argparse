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
 * @file vargarray_clib_private.h
 * @ingroup libvarg
 * @{
 */

#pragma once

#include "varg_intf.h"
#include "varg.h"

namespace argparser
{

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================

//=================================================================================================
//=========================== Class definitions ===================================================
//=================================================================================================
/**
 * @brief Variable argument pouinter type template class
 */
template <typename T> class vargcarray : public varg_intf
{
    private:
        // NOLINTNEXTLINE
        T*          cstorage;               ///< Pointer to the array cstorage location
        size_t      elementCount;           ///< Number of elements the array can store
        size_t      currentElementIndex;    ///< Number of elements the array can store
        size_t      assignmentCount;        ///< Number of elements that were stored including wrapping values
        varg<T>     argValue;               ///< Temporary argument cstorage

    public:
        /**
        * @brief Construct a vargcarray object
        *
        * @param valueArray - Pointer to the value cstorage array
        * @param arraySize - Number of elements in the cstorage array
        */
        vargcarray(T* valueArray, size_t arraySize) :
        cstorage(valueArray), elementCount(arraySize),
        currentElementIndex(0), assignmentCount(0), argValue() {}

        /**
        * @brief Construct a vargcarray object
        *
        * @param valueArray - Pointer to the value cstorage array
        * @param arraySize - Number of elements in the cstorage array
        * @param min - Minimum allowed set value
        * @param max - Maximum allowed set value
        */
        vargcarray(T* valueArray, size_t arraySize, T min, T max) :
        cstorage(valueArray), elementCount(arraySize),
        currentElementIndex(0), assignmentCount(0), argValue(0, 0, min, max) {}

        /**
        * @brief Copy constructor for vargcarray object
        *
        * @param other - Reference to the object to copy
        */
        vargcarray(const vargcarray& other) = default;

        /**
        * @brief Reference copy constructor for vargcarray object
        *
        * @param other - Reference to the reference object to copy
        */
        vargcarray(vargcarray&& other) = default;

        /**
        * @brief Assignment copy constructor for vargcarray object
        *
        * @param other - Reference to the object to copy
        */
        vargcarray& operator=(const vargcarray& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargcarray object
        *
        * @param other - Reference to the reference object to copy
        */
        vargcarray& operator=(vargcarray&& other) = default;

        /**
        * @brief Destroy the varg object
        */
        ~vargcarray() override = default;

        /**
        * @brief Return if varg is a list of elements or a single element type
        *
        * @return true - List type variable, mcstorageultiple arguement values are allowed
        * @return false - Only 0 or 1 argument values are allowed.
        */
        [[nodiscard]] bool isList() const override {return true;}

        /**
        * @brief Virtual interface method implementation for the template variable implementation setValue with input function
        *
        * @param newValue - Input character string
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e       - if value was successsfully set
        * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - if input string could not be translated
        * @return valueParseStatus_e::PARSE_OUT_OF_RANGE_e  - if value exceeds upper or lower value limit
        */
        valueParseStatus_e setValue(const char* newValue) override
        {
            valueParseStatus_e status = argValue.setValue(newValue);
            if (status == valueParseStatus_e::PARSE_SUCCESS_e)
            {
                if (currentElementIndex >= elementCount)
                {
                    currentElementIndex = 0;
                }
                cstorage[currentElementIndex++] = argValue.value;       // NOLINT
                assignmentCount++;
            }
            return status;
        }

        /**
        * @brief Get the base argument type as a string
        *
        * @return const char* - Base type string
        */
        const char* getTypeString() override                        {return argValue.getTypeString();}

        /**
        * @brief Get the base argument range as a string
        *
        * @return const char* - Base type string
        */
        const char* getRangeString() override                        {return argValue.getRangeString();}

        /**
        * Virtual interface method implementation for the template variable implementation setValue function
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e
        */
        valueParseStatus_e setValue() override  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        bool isEmpty() override                 {return (nullptr == cstorage);}

        /**
        * Virtual place holder for the template variable implementation getAssignmentCount function
        *
        * @return size_t - number of elements assigned to list object
        */
        size_t getAssignmentCount() override    {return assignmentCount;}

}; // end of class vargcarray definition

}; // end of namespace argparser

/** @} */
