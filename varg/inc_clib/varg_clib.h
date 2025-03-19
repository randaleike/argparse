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
 * @file varg_clib.h
 * @ingroup libvarg
 * @{
 */

#pragma once

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#include <stddef.h>
#include <stdbool.h>
#endif

//=================================================================================================
//================== Parser argument typedef and structure definitions ============================
//=================================================================================================
struct cvarptr;

// NOLINTBEGIN
typedef struct cvarptr* argHandle;                         ///< Argument pointer class abstraction
// NOLINTEND

/**
 * @brief enum value structure for enum parser argument creation
 */
struct enumValueSpec
{
    const char* enumValueName;      ///< Name used on the command line to identify the enumValue
    int         enumValue;          ///< Value to set the enum to when the enumValueName is seen
};

//=================================================================================================
//=================================================================================================
//==================== Parser Argument argument API interface methods =============================
//=================================================================================================
//=================================================================================================
/**
 * @brief Create a short int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntParserArg(short int* valueAddr);

/**
 * @brief Create a int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntParserArg(int* valueAddr);

/**
 * @brief Create a long int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntParserArg(long int* valueAddr);

/**
 * @brief Create a long long int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntParserArg(long long int* valueAddr);

/**
 * @brief Create a short unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntParserArg(short unsigned* valueAddr);

/**
 * @brief Create an unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntParserArg(unsigned* valueAddr);

/**
 * @brief Create a long unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntParserArg(long unsigned* valueAddr);

/**
 * @brief Create a long long unsigned int parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongUIntParserArg(long long unsigned* valueAddr);

/**
 * @brief Create a double parser argument object
 *
 * @param valueAddr - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createFloatParserArg(double* valueAddr);

/**
 * @brief Create a single character parser argument object
 *
 * @param valueAddr - Pointer to the character argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createCharParserArg(char* valueAddr);

/**
 * @brief Create a parser argument object
 *
 * @param flag         - Pointer to the argument storage
 * @param setValue     - Value if flag variable was found
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool* flag, bool setValue);

/**
 * @brief Create a incrementing parser argument object
 *
 * @param incValueAddr - Pointer to the incrementing integer storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIncrementingArg(int* incValueAddr);

/**
 * @brief Create a short integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntArrayParserArg(short int* valueArray, size_t arraySize);

/**
 * @brief Create a integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntArrayParserArg(int* valueArray, size_t arraySize);

/**
 * @brief Create a long integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntArrayParserArg(long int* valueArray, size_t arraySize);

/**
 * @brief Create a long long integer array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntArrayParserArg(long long int* valueArray, size_t arraySize);

/**
 * @brief Create an short unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntArrayParserArg(short unsigned* valueArray, size_t arraySize);

/**
 * @brief Create an unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntArrayParserArg(unsigned* valueArray, size_t arraySize);

/**
 * @brief Create an long unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntArrayParserArg(long unsigned* valueArray, size_t arraySize);

/**
 * @brief Create an long long unsigned array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongUIntArrayParserArg(long long unsigned* valueArray, size_t arraySize);

/**
 * @brief Create a double array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createDoubleArrayParserArg(double* valueArray, size_t arraySize);

/**
 * @brief Create a character array parser argument object
 *
 * @param valueArray - Pointer to the value storage array
 * @param arraySize - Number of elements in the storage array
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringArrayParserArg(char* valueArray, size_t arraySize);

/**
 * @brief Create a enum parser argument object
 *
 * @param enumAddr - Pointer to the incrementing integer storage
 * @param enumName - Name of the enum displayed in the help message
 * @param enumCount - Number of enum value specification structures
 * @param enumValueList - Array of enum specification structures
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createEnumArg(int* enumAddr, const char* enumName, size_t enumCount, struct enumValueSpec* enumValueList);

/**
 * @brief Get the Assignment Count of the array argument handle
 *
 * @param handle - Argument handle to release
 *
 * @return size_t - Number of array elements that were assigned
 */
size_t getAssignmentCount(argHandle handle);

/**
 * @brief Release the parser argument handle.
 *
 * @param handle - Argument handle to release
 */
void releaseParserArg(argHandle handle);

#ifdef __cplusplus
}
#endif

/** @} */
