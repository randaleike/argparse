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
 * @file cmd_line_parse_api.h
 * @defgroup libcmd_line_parser Command line argument parsing utilities
 * @{
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//=================================================================================================
//========================= Parser Typedef definitions ============================================
//=================================================================================================
struct argData;
typedef struct argData* argHandle;                      ///< Argument data structure pointer abstraction

struct cmdLineParser;
typedef struct cmdLineParser* cmdLineParserHandle;      ///< Command line parser structure pointer abstraction
typedef char const* parsercstr;                         ///< Parser 'C' string abstraction

//=================================================================================================
//=================================================================================================
//======================== Parser Argument API interface methods ==================================
//=================================================================================================
//=================================================================================================
/**
 * @brief Create a short int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntParserArg(short int* arg);

/**
 * @brief Create a int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntParserArg(int* arg);

/**
 * @brief Create a long int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntParserArg(long int* arg);

/**
 * @brief Create a long long int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntParserArg(long long int* arg);

/**
 * @brief Create a short unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntParserArg(short unsigned* arg);

/**
 * @brief Create an unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntParserArg(unsigned* arg);

/**
 * @brief Create a long unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntParserArg(long unsigned* arg);

/**
 * @brief Create a long long unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongUIntParserArg(long long unsigned* arg);

/**
 * @brief Create a double parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createFloatParserArg(double* arg);

/**
 * @brief Create a single character parser argument object
 *
 * @param arg - Pointer to the character argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createCharParserArg(char* arg);

/**
 * @brief Create a string parser argument object
 *
 * @param arg - Pointer to the string argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringParserArg(char* arg, int elementCount);

/**
 * @brief Create a parser argument object
 *
 * @param defaultValue - Initial value
 * @param setValue     - Value if flag variable was found
 * @param flag         - Pointer to the argument storage 
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool defaultValue, bool setValue, bool* flag);

/**
 * @brief Create a incrementing parser argument object
 *
 * @param flag - Pointer to the argument storage 
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIncrementingFlagArg(int* flag);

/**
 * @brief Create a short integer array parser argument object
 *
 * @param arg - Pointer to the short integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntArrayParserArg(short int arg[], int elementCount);

/**
 * @brief Create a integer array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntArrayParserArg(int arg[], int elementCount);

/**
 * @brief Create a long integer array parser argument object
 *
 * @param arg - Pointer to the long integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntArrayParserArg(long int arg[], int elementCount);

/**
 * @brief Create a long long integer array parser argument object
 *
 * @param arg - Pointer to the long long integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntArrayParserArg(long long int arg[], int elementCount);

/**
 * @brief Create an short unsigned array parser argument object
 *
 * @param arg - Pointer to the short unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntArrayParserArg(short unsigned arg[], int elementCount);

/**
 * @brief Create an unsigned array parser argument object
 *
 * @param arg - Pointer to the unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntArrayParserArg(unsigned arg[], int elementCount);

/**
 * @brief Create an long unsigned array parser argument object
 *
 * @param arg - Pointer to the long unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntArrayParserArg(long unsigned arg[], int elementCount);

/**
 * @brief Create an long long unsigned array parser argument object
 *
 * @param arg - Pointer to the long long unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongUIntArrayParserArg(long long unsigned arg[], int elementCount);

/**
 * @brief Create a double array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createDoubleArrayParserArg(double arg[], int elementCount);

/**
 * @brief Create a double array parser argument object
 *
 * @param arg - Pointer to the constant character pointer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringArrayParserArg(char** arg, int elementCount);

/**
 * @brief Release the parser argument handle.
 *
 * @param arg - Argument handle to release
 */
void releaseParserArg(argHandle arg);

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
 * @return cmdLineParserHandle - Handle to the created argument parser
 */
cmdLineParserHandle getParser(const parsercstr usage, const parsercstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel);

/**
 * @brief Release the parser instance
 *
 * @param parser - Handle value returned by getParser()
 * @param releaseArgHandles - True, release the argument handles that were assigned to the parer add argument calls
 *                            False, Do not release the argument handles assigned to the parser
 */
void releaseParser(cmdLineParserHandle parser, bool releaseArgHandles);

//=================================================================================================
//======================= Parser setup interface methods ==========================================
//=================================================================================================

/**
 * @brief Set the Epilog text
 *
 * @param parser - Handle value returned by getParser()
 * @param epilog - Text to display at the end of the help block
 */
void setEpilog(cmdLineParserHandle parser, const parsercstr epilog);

/**
 * @brief Set the Program Name for the usage string
 *
 * @param parser - Handle value returned by getParser()
 * @param progName - Program name to use in the usage string
 */
void setProgramName(cmdLineParserHandle parser, const parsercstr progName);

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
void setKeyPrefix(cmdLineParserHandle parser, parsercstr prefix);

/**
 * @brief Disable the default help argument setup
 */
void disableDefaultHelpArgument(cmdLineParserHandle parser);

/**
 * @brief Disable the help display on parsing error
 *
 * @param parser - Handle value returned by getParser()
 */
void disableHelpDisplayOnError(cmdLineParserHandle parser);

/**
 * @brief Disable the single character list input argument parsing
 *
 * @param parser - Handle value returned by getParser()
 */
void disableSingleCharListArgument(cmdLineParserHandle parser);

/**
 * @brief Enable flag to ignore unknown arguments, default
 *        is to generate an error if an unknown argument
 *        is found
 *
 * @param parser - Handle value returned by getParser()
 */
void enableUnknowArgumentIgnore(cmdLineParserHandle parser);

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
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addKeyArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr argKeys, 
                    const parsercstr helpText, const bool required);

/**
 * @brief Add a new key based command line array argument to the argument list
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
void addKeyArrayArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr argKeys, 
                         const parsercstr helpText, const int nargs, const bool required);

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
void addFlagArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr argKeys, 
                     const parsercstr helpText, const bool required);

/**
 * @brief Add a new key based incrementing command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addIncrementingArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr argKeys, 
                             const parsercstr helpText, const bool required);

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addPositionalArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr helpText, const bool required);

/**
 * @brief Add a new positinal based command line array argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
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
void addPositionalArrayArgument(cmdLineParserHandle parser, const argHandle arg, const parsercstr name, const parsercstr helpText, 
                                const int nargs, const bool required);

//=================================================================================================
//======================= Commandline parser interface methods ====================================
//=================================================================================================
/**
 * @brief Set the name of the positional argument to stop parsing
 *
 * @param parser - Handle value returned by getParser()
 * @param positionalArgumentName - Positional name to find
 */
void setPositionalNameStop(cmdLineParserHandle parser, const parsercstr positionalArgumentName);

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
int parse(cmdLineParserHandle parser, int argc, char* argv[], int startingArgIndex, int endingArgIndex);

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param parser - Handle value returned by getParser()
 */
void displayHelp(cmdLineParserHandle parser);

#ifdef __cplusplus
}
#endif

/** @} */
