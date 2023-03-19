/*
Copyright (c) 2023 Randal Eike

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
 * @file cmd_line_parse_api.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//=================================================================================================
//========================= Parser constant definitions ===========================================
//=================================================================================================
/**
 * @brief Available argument types
 *
 */
typedef enum
{
    ARG_SHORT_INT,                  ///< Short signed integer
    ARG_INT,                        ///< Signed integer
    ARG_LONG_INT,                   ///< Long signed integer
    ARG_LONG_LONG_INT,              ///< Long long signed integer
    ARG_SHORT_UINT,                 ///< Short unsigned integer
    ARG_UINT,                       ///< Unsigned integer
    ARG_LONG_UINT,                  ///< Long unsigned integer
    ARG_LONG_LONG_UINT,             ///< Long long unsigned integer
    ARG_DOUBLE,                     ///< Double precision floating point value
    ARG_CHAR,                       ///< Single character value
    ARG_BOOL,                       ///< Boolean value
    ARG_CHAR_STRING,                ///< Character string
    ARG_INCREMENT,                  ///< Incrementing integer value
    ARG_ARRAY_SHORT_INT,            ///< Short signed integer array
    ARG_ARRAY_INT,                  ///< Signed integer array
    ARG_ARRAY_LONG_INT,             ///< Long signed integer array
    ARG_ARRAY_LONG_LONG_INT,        ///< Long long signed integer array
    ARG_ARRAY_SHORT_UINT,           ///< Short unsigned integer array
    ARG_ARRAY_UINT,                 ///< Unsigned integer array
    ARG_ARRAY_LONG_UINT,            ///< Long unsigned integer array
    ARG_ARRAY_LONG_LONG_UINT,       ///< Long long unsigned integer array
    ARG_ARRAY_DOUBLE,               ///< Double precision floating point value array
    ARG_ARRAY_CHAR,                 ///< Character array
    ARG_ARRAY_BOOL,                 ///< Boolean value array
    ARG_ARRAY_CHAR_STRING,          ///< Character pointer array
} argtype_e;

//=================================================================================================
//========================= Parser Typedef definitions ============================================
//=================================================================================================
struct argData;
typedef struct argData* argHandle;

struct argParser;
typedef struct argParser* argParserHandle;
typedef char* parsercstr;

//=================================================================================================
//=================================================================================================
//======================== Parser Argument API interface methods ==================================
//=================================================================================================
//=================================================================================================
/**
 * @brief Create a Parser argument object
 *
 * @param type       - Type of argument
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserArg(argtype_e type);

/**
 * @brief Create a Parser argument object
 *
 * @param defaultValue - Initial value
 * @param setValue     - Value if flag variable was found
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool defaultValue, bool setValue)

/**
 * @brief Release the parser argument handle.
 *
 * @param arg - Argument handle to release
 */
void releaseParserArg(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return short int - Value from parsing
 */
short int getShortIntValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return int - Value from parsing
 */
int getIntValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return long int - Value from parsing
 */
long int getLongIntValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return long long int - Value from parsing
 */
long long int getLLIntValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return short unsigned - Value from parsing
 */
short unsigned getShortUintValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return unsigned - Value from parsing
 */
unsigned getValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return long unsigned - Value from parsing
 */
long unsigned getLongUintValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return long long unsigned - Value from parsing
 */
long long unsigned getLLUintValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return double - Value from parsing
 */
double getDoubleValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return char - Value from parsing
 */
char getCharValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return bool - Value from parsing
 */
bool getBoolValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 */
char* getStringValue(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 *
 * @return size_t - Number of elements in the array
 */
size_t gettArrayDataSize(argHandle arg);

/**
 * @brief Get the argument handle value.
 *
 * @param arg - Argument handle to release
 * @param index - Index of array element to fetch
 */
short int getShortIntArrayData(argHandle arg, const int index);



//=================================================================================================
//=================================================================================================
//========================= Parser API interface methods ==========================================
//=================================================================================================
//=================================================================================================
/**
 * @brief Get the Parser object
 *
 * @param usage - Usage sting for help display
 * @param description - Description of tool for the help display
 * @param abortOnError - True = abort parsing on the first error, False = continue parsing to the end
 * @param disableDefaultHelp - True = display the help mesage if a parsing error occurs,
 *                             False = do not display the help message until parserDisplayHelp() is called
 * @param debugLevel - Debug output verbosity level, 0 = none, 5 = full
 *
 * @return argParserHandle - Handle to the created argument parser
 */
argParserHandle getParser(parsercstr usage, parsercstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel);

/**
 * @brief Release the parser instance
 *
 * @param parser - Handle value returned by getParser()
 * @param releaseArgHandles - True, release the argument handles that were assigned to the parer add argument calls
 *                            False, Do not release the argument handles assigned to the parser
 */
void releaseParser(argParserHandle parser, bool releaseArgHandles);

//=================================================================================================
//======================= Parser setup interface methods ==========================================
//=================================================================================================

/**
 * @brief Set the Epilog text
 *
 * @param parser - Handle value returned by getParser()
 * @param epilog - Text to display at the end of the help block
 */
void setEpilog(argParserHandle parser, parsercstr epilog);

/**
 * @brief Set the Program Name for the usage string
 *
 * @param parser - Handle value returned by getParser()
 * @param progName - Program name to use in the usage string
 */
void setProgramName(argParserHandle parser, parsercstr progName);

/**
 * @brief Set the argument key prefix value.
 *
 * The argument key prefix is the character or string
 * the identifies an input argument key string.  Any input
 * argument that does not begin with this character is
 * assumed to be a positional argument value.
 *
 * @param parser - Handle value returned by getParser()
 * @param prefix - argument prefix value
 */
void setKeyPrefix(argParserHandle parser, parsercstr prefix);

/**
 * @brief Disable the default help argument setup
 */
void disableDefaultHelpArgument(argParserHandle parser);

/**
 * @brief Disable the help display on parsing error
 *
 * @param parser - Handle value returned by getParser()
 */
void disableHelpDisplayOnError(argParserHandle parser);

/**
 * @brief Disable the single character list input argument parsing
 *
 * @param parser - Handle value returned by getParser()
 */
void disableSingleCharListArgument(argParserHandle parser);

/**
 * @brief Enable flag to ignore unknown arguments, default
 *        is to generate an error if an unknown argument
 *        is found
 *
 * @param parser - Handle value returned by getParser()
 */
void enableUnknowArgumentIgnore(argParserHandle parser);

//=================================================================================================
//======================= Argument add interface methods ==========================================
//=================================================================================================
/**
 * @brief Add a new key based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param nargs      - Number of argument values that follow
 *                     0 : Argument is a flag with no following values
 *                     1 : Simple argument with a single value
 *                     N : ARRAY argument with exactly N arguments, if less than N arguments
 *                         are found it is flagged as an error
 *                    -N : ARRAY argument with up to N arguments, if more than N arguments
 *                         are found it is flagged as an error
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addKeyArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, int nargs, bool required);

/**
 * @brief Add a new key based flag command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addFlagArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, bool required);

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param nargs      - Number of argument values that follow
 *                     0 : Argument is a flag with no following values
 *                     1 : Simple argument with a single value
 *                     N : ARRAY argument with exactly N arguments, if less than N arguments
 *                         are found it is flagged as an error
 *                    -N : ARRAY argument with up to N arguments, if more than N arguments
 *                         are found it is flagged as an error
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addPositionalArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr helpText, int nargs, bool required);

//=================================================================================================
//======================= Commandline parser interface methods ====================================
//=================================================================================================
/**
 * @brief Set the name of the positional argument to stop parsing
 *
 * @param parser - Handle value returned by getParser()
 * @param positionalArgumentName - Positional name to find
 */
void setPositionalNameStop(argParserHandle parser, const parsercstr positionalArgumentName);

/**
 * @brief Parse the input command line arguments
 *
 * @param parser - Handle value returned by getParser()
 * @param argc - Number of char pointers in the argv[] array
 * @param argv - Array of command line text entries
 * @param startingArgIndex - argv index to start parsing arguments
 * @param endingArgIndex - argv index to stop parsing arguments, -1 == argc
 *
 * @return int - Index of the last argument parsed or -1 if an error occured
 */
int parse(argParserHandle parser, int argc, char* argv[], int startingArgIndex, int endingArgIndex);

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param parser - Handle value returned by getParser()
 */
void displayHelp(argParserHandle parser);

#ifdef __cplusplus
}
#endif

/** @} */
