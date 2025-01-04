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
 * @file cmd_line_parse_lib_api.cpp
 * @ingroup libcmd_line_parser
 * @{
 */

#pragma once

#include "varg_intf.h"
#include "varg.h"
#include "vargincrement.h"
#include "vargenum.h"
#include "listvarg.h"

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================

namespace argparser
{
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
                    cstorage[currentElementIndex++] = argValue.value;       // NOLINT
                    assignmentCount++;
                }
            }
            return status;
        }

        /**
         * Virtual interface method implementation for the template variable implementation setValue function
         *
         * @return valueParseStatus_e::PARSE_SUCCESS_e
         */
        valueParseStatus_e setValue() override {return valueParseStatus_e::PARSE_INVALID_INPUT_e;}

        /**
         * Virtual interface method implementation for the template variable implementation isEmpty function
         *
         * @return true - Base variable is never empty
         */
        bool isEmpty() override                {return (nullptr == cstorage);}
}; // end of class vargcarray definition

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

/**
 * @brief C-style string variable argument type template class
 */
class vargcstring : public varg_intf
{
    private:
        char*           cstorage;               ///< Pointer to the array cstorage location
        size_t          elementCount;           ///< Number of elements the array can store
        vargstring      argValue;               ///< Temporary argument cstorage

    public:
        /**
         * @brief Construct a vargcarray object
         *
         * @param valueArray - Pointer to the value cstorage array
         * @param arraySize - Number of elements in the cstorage array
         */
        vargcstring(char* valueArray, size_t arraySize) : cstorage(valueArray), elementCount(arraySize), argValue("") {}

        /**
         * @brief Construct a vargcstring object
         *
         * @param valueArray - Pointer to the value cstorage array
         * @param arraySize - Number of elements in the cstorage array
         * @param min - Minimum allowed srtring
         * @param max - Maximum allowed set value
         */
        vargcstring(char* valueArray, size_t arraySize, size_t min, size_t max) :
        cstorage(valueArray), elementCount(arraySize), argValue("", min, max) {}

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
         * @brief Get the base argument type as a string
         *
         * @return const char* - Base type string
         */
        const char* getTypeString() override                        {return argValue.getTypeString();}

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
                    *currentChar = '\0';
                    if (index < (elementCount-1))
                    {
                        *currentChar++ = stringChar;        // NOLINT
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
        [[nodiscard]] bool isEmpty() override                       {return (nullptr == cstorage);}
}; // end of class definition

}; // end of namespace argparser

//=================================================================================================
//========================= Structure definitions =================================================
//=================================================================================================
/**
 * @brief Generic varg C abstraction
 */
struct cvarptr
{
    argparser::varg_intf* vararg{nullptr};
};

/** @} */
