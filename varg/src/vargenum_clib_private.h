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
 * @file vargenum_clib_private.h
 * @ingroup libvarg
 * @{
 */

#pragma once

#include "varg_intf.h"
#include "vargenum.h"

namespace argparser
{

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================

//=================================================================================================
//=========================== Class definitions ===================================================
//=================================================================================================
/**
 * @brief Variable argument c enum type template class
 */
class vargptrenum : public varg_intf
{
    private:
        int*              cstorage;     ///< Pointer to the cstorage location
        vargenum< int >   tempVar;      ///< Varg to use for parsing

    public:
        /**
        * @brief Construct a vargptrenum object
        *
        * @param valueAddr - Addresss of the enum integer cstorage
        * @param name - name of the enum
        */
        vargptrenum(int* valueAddr, const char* name = "enum value"): cstorage(valueAddr), tempVar(0, name) {}

        /**
        * @brief Copy constructor for vargptrenum object
        *
        * @param other - Reference to the object to copy
        */
        vargptrenum(const vargptrenum& other) = default;

        /**
        * @brief Reference copy constructor for vargptrenum object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptrenum(vargptrenum&& other) = default;

        /**
        * @brief Assignment copy constructor for vargptrenum object
        *
        * @param other - Reference to the object to copy
        */
        vargptrenum& operator=(const vargptrenum& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargptrenum object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptrenum& operator=(vargptrenum&& other) = default;

        /**
        * @brief Destroy the vargptrenum object
        */
        ~vargptrenum() override = default;

        /**
        * @brief Get the base argument type as a string
        *
        * @return const char* - Base type string
        */
        const char* getTypeString() override           {return tempVar.getTypeString();}

        /**
        * @brief Get the base argument type as a string
        *
        * @return const char* - Base type string
        */
        const char* getRangeString() override           {return tempVar.getRangeString();}

        /**
        * @brief Return if vargptrenum is a list of elements or a single element type
        *
        * @return true - List type variable, multiple arguement values are allowed
        * @return false - Only 0 or 1 argument values are allowed.val
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
        * @return valueParseStatus_e::PARSE_INVALID_INPUT_e - enum type must have an input string to map to the enum value
        */
        valueParseStatus_e setValue() override     {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        [[nodiscard]] bool isEmpty() override      {return (nullptr == cstorage);}

        /**
        * @brief Assign enum map values
        *
        * @param entryName - Name of the enum value
        * @param enumValue - Value to assign for the name
        */
        void setEnumValue(const char* entryName, int enumValue)
        {
            tempVar.setEnumValue(entryName, enumValue);
        }

}; // end of class definition

}; // end of namespace argparser

/** @} */
