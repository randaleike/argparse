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
 * @file varginc_clib_private.h
 * @ingroup libvarg
 * @{
 */

#pragma once

#include "varg_intf.h"

namespace argparser
{

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================

//=================================================================================================
//=========================== Class definitions ===================================================
//=================================================================================================
/**
 * @brief Incrementing variable argument type template class
 */
class vargptrincrement : public varg_intf
{
    private:
        int*      cstorage;              ///< Current saved value

    public:
        /**
        * @brief Construct a varg_intf object
        *
        * @param valueAddr - Pointer to the value cstorage location
        */
        vargptrincrement(int* valueAddr) : cstorage(valueAddr) {}

        /**
        * @brief Copy constructor for vargptrincrement object
        *
        * @param other - Reference to the object to copy
        */
        vargptrincrement(const vargptrincrement& other) = default;

        /**
        * @brief Reference copy constructor for vargptrincrement object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptrincrement(vargptrincrement&& other) = default;

        /**cstorage
        * @brief Assignment copy constructor for vargptrincrement object
        *
        * @param other - Reference to the object to copy
        */
        vargptrincrement& operator=(const vargptrincrement& other) = default;

        /**
        * @brief Assignment reference copy constructor for vargptrincrement object
        *
        * @param other - Reference to the reference object to copy
        */
        vargptrincrement& operator=(vargptrincrement&& other) = default;

        /**
        * @brief Destroy the vargptrincrement object
        */
        ~vargptrincrement() override = default;

        /**
        * @brief Get the base argument type as a string
        *
        * @return const char* - Base type string
        */
        const char* getTypeString() override                        {return "incrementing flag";}

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
        valueParseStatus_e setValue(const char* newValue) override  {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
        * Virtual interface method implementation for the template variable implementation setValue function
        *
        * @return valueParseStatus_e::PARSE_SUCCESS_e - if value was successsfully incremented
        */
        valueParseStatus_e setValue() override                      {(*cstorage)++; return valueParseStatus_e::PARSE_SUCCESS_e;}

        /**
        * Virtual interface method implementation for the template variable implementation isEmpty function
        *
        * @return true - Base variable is never empty
        */
        [[nodiscard]] bool isEmpty() override                       {return false;}
}; // end of class definition

}; // end of namespace argparser

/** @} */
