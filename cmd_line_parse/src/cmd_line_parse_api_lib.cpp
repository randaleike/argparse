/*
 Copyright (c) 2023-2025 Randal Eike

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
#include "cmd_line_parse.h"
#include "varg_clib_shared.h"
#include "cmd_line_parse_api_lib_private.h"

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
cmdLineParserHandle getParser(parsercstr usage, parsercstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel)
{
    parserstr newParserUsage = ((nullptr != usage) ? usage : "");
    parserstr newParserDesc = ((nullptr != description) ? description : "");
    struct cmdLineParser* wrapper = new (struct cmdLineParser);     // NOLINT
    wrapper->object = new argparser::cmd_line_parse(newParserUsage, newParserDesc, abortOnError, disableDefaultHelp, debugLevel);   // NOLINT
    return wrapper;
}

/**
 * @brief Release the parser instance
 *
 * @param parser - Handle value returned by getParser()
 */
void releaseParser(cmdLineParserHandle parser)
{
    delete parser->object;      // NOLINT
    delete parser;              // NOLINT
}

//=================================================================================================
//======================= Parser setup interface methods ==========================================
//=================================================================================================

/**
 * @brief Set the Epilog text
 *
 * @param parser - Handle value returned by getParser()
 * @param epilog - Text to display at the end of the help blockparser
 */
void setEpilog(cmdLineParserHandle parser, parsercstr epilog)
{
    if (parser != nullptr)
    {
        parser->object->setEpilog(epilog);
    }
}

/**
 * @brief Set the Program Name for the usage string
 *
 * @param parser - Handle value returned by getParser()
 * @param progName - Program name to use in the usage string
 */
void setProgramName(cmdLineParserHandle parser, parsercstr progName)
{
    if (parser != nullptr)

    {
        parser->object->setProgramName(progName);
    }
}

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
void setKeyPrefix(cmdLineParserHandle parser, parsercstr prefix)
{
    if (parser != nullptr)
    {
        parser->object->setKeyPrefix(prefix);
    }
}

/**
 * @brief Disable the default help argument setup
 */
void disableDefaultHelpArgument(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object->disableDefaultHelpArgument();
    }
}


/**
 * @brief Disable the help display on parsing error
 *
 * @param parser - Handle value returned by getParser()
 */
void disableHelpDisplayOnError(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object->disableHelpDisplayOnError();
    }
}


/**
 * @brief Disable the single character list input argument parsing
 *
 * @param parser - Handle value returned by getParser()
 */
void disableSingleCharListArgument(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableSingleCharListArgument();
    }
}

/**
 * @brief Enable flag to ignore unknown arguments, default
 *        is to generate an error if an unknown argument
 *        is found
 *
 * @param parser - Handle value returned by getParser()
 */
void enableUnknowArgumentIgnore(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->enableUnknowArgumentIgnore();
    }
}

//=================================================================================================
//======================= Argument add interface methods ==========================================
//=================================================================================================

/**
 * @brief Add a new key based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addKeyArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr argKeys,
                    parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    if (valueAddr->vararg->isList())
    {
        std::cerr << "Use addKeyArrayArgument() to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addKeyArgument(valueAddr->vararg, name, argKeys, helpText, 1, required);
}

/**
 * @brief Add a new key based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
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
void addKeyArrayArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr argKeys,
                         parsercstr helpText, int nargs, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    if (!valueAddr->vararg->isList())
    {
        std::cerr << "Use addKeyArgument() to add single value arguments to the parser" << std::endl;
        return;
    }

    parser->object->addKeyArgument(valueAddr->vararg, name, argKeys, helpText, nargs, required);
}

/**
 * @brief Add a new key based flag command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addFlagArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr argKeys,
                     parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    if (valueAddr->vararg->isList())
    {
        std::cerr << "Use addKeyArrayArgument() or addPositionalArrayArgument to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addFlagArgument(valueAddr->vararg, name, argKeys, helpText, required);
}

/**
 * @brief Add a new key based incrementing command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addIncrementingArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr argKeys,
                             parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    parser->object->addFlagArgument(valueAddr->vararg, name, argKeys, helpText, required);
}

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addPositionalArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    if (valueAddr->vararg->isList())
    {
        std::cerr << "Use addPositionalArrayArgument() to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addPositionalArgument(valueAddr->vararg, name, helpText, 1, required);
}

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param valueAddr        - Handle of the argument to add
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
void addPositionalArrayArgument(cmdLineParserHandle parser, argHandle valueAddr, parsercstr name, parsercstr helpText,
                                int nargs, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (valueAddr == nullptr)
    {
        std::cerr << "Invalid valueAddr input, nullptr" << std::endl;
        return;
    }

    if (!valueAddr->vararg->isList())
    {
        std::cerr << "Use addPositionalArgument() to add single value arguments to the parser" << std::endl;
        return;
    }

    parser->object->addPositionalArgument(valueAddr->vararg, name, helpText, nargs, required);
}

//=================================================================================================
//======================= Commandline parser interface methods ====================================
//=================================================================================================
/**
 * @brief Set the name of the positional argument to stop parsing
 *
 * @param parser - Handle value returned by getParser()
 * @param positionalArgumentName - Positional name to find
 */
void setPositionalNameStop(cmdLineParserHandle parser, parsercstr positionalArgumentName)
{
    if (parser != nullptr)
    {
        parser->object->setPositionalNameStop(positionalArgumentName);
    }
}

/**
 * @brief Parse the input command line arguments
 *
 * @param parser - Handle value returned by getParser()
 * @param argc - Number of char pointers in the argv[] array
 * @param argv - Array of command line text entries
 * @param startingArgIndex - argv index to start parsing arguments, -1 or 0 = 1
 * @param endingArgIndex - argv index to stop parsing arguments, -1 == argc
 *
 * @return int - Index of the last argument parsed or -1 if an error occured
 */
int parse(cmdLineParserHandle parser, int argc, char* argv[], int startingArgIndex, int endingArgIndex)
{
    int parserStatus = -1;
    if (parser != nullptr)
    {
        startingArgIndex = ((startingArgIndex < 1) ? 1 : startingArgIndex);
        parserStatus = parser->object->parse(argc, argv, startingArgIndex, endingArgIndex);
    }
    return parserStatus;
}

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param parser - Handle value returned by getParser()
 */
void displayHelp(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object->displayHelp();
    }
}

/** @} */
