/*
 Copyright (c) 2023-2024 Randal Eike

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
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <iostream>
#include <memory>
#include "varg_clib.h"
#include "varg_clib_private.h"

//=================================================================================================
//=================================================================================================
//======================== Parser Argument API interface methods ==================================
//=================================================================================================
//=================================================================================================
/**
 * @brief Create a short int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntParserArg(short int* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<short int>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntParserArg(int* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<int>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a long int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntParserArg(long int* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<long int>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a long long int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongLongIntParserArg(long long int* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<long long int>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a short unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntParserArg(short unsigned* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<short unsigned>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create an unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntParserArg(unsigned* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<unsigned>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a long unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntParserArg(long unsigned* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<long unsigned>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a long long unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongLongUIntParserArg(long long unsigned* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<long long unsigned>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a double parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createFloatParserArg(double* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<double>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a single character parser argument object
 *
 * @param valueAddr - Pointer to the character argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createCharParserArg(char* valueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargptr<char>(valueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a parser argument objectintegerVal
 * @param flag     - Pointer to the argument storage
 * @param setValue - Value to set if the flag is encountered in the argument parsing stream
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool* flag, bool setValue)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg  = new argparser::vargptr<bool>(flag, setValue);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a incrementing integer parser argument object
 *
 * @param incValueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIncrementingArg(int* incValueAddr)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg  = new argparser::vargptrincrement(incValueAddr);    // Create the argument NOLINT
    return argument;
}

/**
 * @brief Create a short integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntArrayParserArg(short int* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<short int>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntArrayParserArg(int* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<int>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a long integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntArrayParserArg(long int* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<long int>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a long long integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntArrayParserArg(long long int* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<long long int>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create an short unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntArrayParserArg(short unsigned* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<short unsigned>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create an unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntArrayParserArg(unsigned* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<unsigned>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create an long unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntArrayParserArg(long unsigned* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<long unsigned>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create an long long unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLUIntArrayParserArg(long long unsigned* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<long long unsigned>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a double array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createDoubleArrayParserArg(double* valueArray, size_t arraySize)
{
    argHandle argument = new cvarptr;    // NOLINT
    argument->vararg = new argparser::vargcarray<double>(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a character array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringArrayParserArg(char* valueArray, size_t arraySize)
{
    cvarptr* argument = new cvarptr;        // NOLINT
    argument->vararg = new argparser::vargcstring(valueArray, arraySize);
    return argument;
}

/**
 * @brief Create a enum parser argument object
 *
 * @param enumAddr - Pointer to the incrementing integer storage
 * @param enumName - Name of the enum displayed in the help message
 * @param enumCount - Number of enum value specification structures
 * @param enumSpecs - Array of enum specification structures
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createEnumArg(int* enumAddr, const char* enumName, size_t enumCount, enumValueSpec* enumValueList)
{
    cvarptr* argument = new cvarptr;        // NOLINT
    argparser::vargptrenum* enumArg = new argparser::vargptrenum(enumAddr, enumName);    // Create the argument, NOLINT

    enumValueSpec* currentSpec = enumValueList;
    for (size_t index = 0;  index < enumCount; index++)
    {
        enumArg->setEnumValue(currentSpec->enumValueName, currentSpec->enumValue);
        currentSpec++;      // NOLINT
    }

    argument->vararg = enumArg;
    return argument;
}

/**
 * @brief Release the parser argument handle.
 *
 * @param handle - Argument handle to release
 */
void releaseParserArg(argHandle handle)
{
    if (handle != nullptr)
    {
        delete handle->vararg;  // NOLINT
        delete handle;          // NOLINT
    }
}



/** @} */
