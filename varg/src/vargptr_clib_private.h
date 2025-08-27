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
 * @file vargptr_clib_private.h
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
template <typename T> class vargptr : public varg_intf
{
    private:
        T*          cstorage;               ///< Pointer to the cstorage location
        varg< T >   tempVar;                ///< Varg to use for parsing

    public:
        /**
        * @brief Construct a varg_intf object
        *
        * @param valueAddr - Pointer to the value cstorage location
        */
        vargptr(T* valueAddr) : cstorage(valueAddr), tempVar(0) {};

        /**
        * @brief Construct a varg_intf object
        *
        * @param valueAddr - Pointer to the value cstorage location
        * @param flagValue - Flag set value of varg.value
        */
        vargptr(T* valueAddr, T flagValue) : cstorage(valueAddr), tempVar(0, flagValue) {};

        /**
        * @brief Construct a varg_intf object
        *
        * @param valueAddr - Pointer to the value cstorage location
        * @param min - Minimum allowed set value
        * @param max - Maximum allowed set value
        */
        vargptr(T* valueAddr, T min, T max) : cstorage(valueAddr), tempVar(0, min, max) {};

        /**
        * @brief Copy constructor for vargptr object
        *
        * @param other - Reference to the object to copy
        */
        vargptr(const vargptr& other) = default;

        /**
        * @brief Reference copy constructor for vargptr object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptr(vargptr&& other) = default;

        /**
        * @brief Assignment copy constructor for vargptr object
        *
        * @param other - Reference to the object to copy
        */
        vargptr& operator=(const vargptr& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargptr object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptr& operator=(vargptr&& other) = default;

        /**
        * @brief Destroy the varg object
        */
        ~vargptr() override = default;

        /**
        * @brief Get the base argument type as a string
        *
        * @return char* - Base type string
        */
        const char* getTypeString() override    {return tempVar.getTypeString();}

        /**
        * @brief Get the base argument range as a string
        *
        * @return char* - Base type string
        */
        const char* getRangeString() override    {return tempVar.getRangeString();}

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
        valueParseStatus_e setValue(const char* newValue) override
        {
            valueParseStatus_e status = valueParseStatus_e::PARSE_STORAGE_NULLPTR_e;
            if (nullptr != cstorage)
            {
                status = tempVar.setValue(newValue);
                if (valueParseStatus_e::PARSE_SUCCESS_e == status)
                {
                    *cstorage = tempVar.value;
                }
            }
            return status;
        }

        /**
        * Virtual interface method implementation for the template variable implementation setValue function
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e
        */
        valueParseStatus_e setValue() override
        {
            valueParseStatus_e status = valueParseStatus_e::PARSE_STORAGE_NULLPTR_e;
            if (nullptr != cstorage)
            {
                status = tempVar.setValue();
                if (valueParseStatus_e::PARSE_SUCCESS_e == status)
                {
                    *cstorage = tempVar.value;
                }
            }
            return status;
        }

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        [[nodiscard]] bool isEmpty() override      {return (nullptr == cstorage);}

}; // end of vargptr class


}; // end of namespace argparser

/** @} */
