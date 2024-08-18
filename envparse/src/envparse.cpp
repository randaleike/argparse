/*
 Copyright (c) 2022-2024 Randal Eike

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
 * @file envparse.cpp
 * @ingroup libenvparse
 * @{
 */

// Includes
#include <stdlib.h>
#include <cstring>
#include <string>
#include "varg_intf.h"
#include "varg.h"
#include "envparse.h"
#include "parser_base.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Private functions
//============================================================================================================================
//============================================================================================================================

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================


//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
envparser::envparser(bool abortOnError, int debugLevel): parser_base(abortOnError,debugLevel), debugMsgLevel(debugLevel)  {}
envparser::~envparser()         {}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Add a new key based environment argument
 *
 * @param arg      - Pointer to the defined var argument to fill
 * @param argKey   - Argument key value
 * @param helpText - Help text to be printed in the help message
 * @param nargs    - Number of argument values that follow
 *                   0 : Argument is a flag with no following values
 *                   1 : Simple argument with a single value
 *                   N : List argument with exactly N arguments, if less than N arguments
 *                       are found it is flagged as an error
 *                  -N : List argument with up to N arguments, if more than N arguments
 *                       are found it is flagged as an error
 * @param required - True if argument is required, false if arguemnt is optional
 */
void envparser::addArgument(varg_intf* arg, const char* argKey, const char* helpText, int nargs, bool required)
{
    // Only list type varg_intf are allowed more than 1 value
    if (nargs == 0)
    {
        std::cerr << parser_base::getParserStringList()->getEnvironmentNoFlags(argKey) << std::endl;
    }
    else if ((nargs != 1) && !arg->isList())
    {
        std::cerr << parser_base::getParserStringList()->getNotListTypeMessage(nargs) << std::endl;
    }
    else
    {
        // Add the keys to the argument
        ArgEntry newKeyArg = {};
        std::string keyString = argKey;
        newKeyArg.keyList.push_back(keyString);

        // Setup the key argument parameters
        newKeyArg.name = argKey;
        newKeyArg.help = helpText;
        newKeyArg.argData = arg;
        newKeyArg.nargs = nargs;
        newKeyArg.position = 0;
        newKeyArg.isRequired = required;
        newKeyArg.isFound = false;

        // Construct the key option help string
        std::string optionString = " ";
        optionString += keyString;
        optionString += parser_base::getAssignmentDelimeter();
        optionString += newKeyArg.name;
        if (nargs > 1)
        {
            optionString += parser_base::getAssignmentDelimeter();
            optionString += "...";
        }

        if (optionString.size() > parser_base::getOptionKeyWidth())
        {
            parser_base::resizeMaxOptionLength(optionString.size());
        }

        newKeyArg.optionString = optionString;

        // Add the new argument to the list
        parser_base::addKeyArgListEntry(newKeyArg);
    }
}

/**
 * @brief Parse the input arguments
 *
 * @return true  - No error parsing
 * @return false - Error in the input parameters that caused a parsing failure
 */
bool envparser::parse()
{
    // Scan the environment
    for (auto & currentArg : parser_base::getKeyArgList())
    {
        const char* envValue = getenv(currentArg.name.c_str());
        if (NULL != envValue)
        {
            // Process the return value string
            std::list<std::string> assignmentValues;
            std::string valueString = envValue;
            size_t valueCount = parser_base::getValueList(valueString, assignmentValues);
            size_t requiredValueCount = static_cast<size_t>(abs(currentArg.nargs));
            if (debugMsgLevel > debugVerbosityLevel_e::veryVerboseDebug)
            {
                std::cout << "Environment value: " << envValue << std::endl;
                std::cout << "Environment var name: " << currentArg.name << std::endl;
                std::cout << "Value count: " << valueCount << std::endl;
            }

            // Assign the values
            currentArg.isFound = true;
            parserstr failedAssignment = valueString;
            eAssignmentReturn status = ((valueCount > 1) ?
                                            parser_base::assignListKeyValue(currentArg, assignmentValues, failedAssignment) :
                                            parser_base::assignKeyValue(currentArg, valueString));

            // Check the assignment error
            switch(status)
            {
                case eAssignSuccess:
                    break;

                case eAssignTooMany:
                    // Not enough values to meet the minimum required
                    std::cerr << parser_base::getParserStringList()->getTooManyAssignmentMessage(currentArg.name, requiredValueCount, valueCount) << std::endl;
                    parser_base::setParsingError(true);
                    break;

                case eAssignNoValue:
                    // Need at least one value
                    std::cerr << parser_base::getParserStringList()->getMissingAssignmentMessage(currentArg.name) << std::endl;
                    parser_base::setParsingError(true);
                    break;

                case eAssignTooFew:
                    // More values than required
                    std::cerr << parser_base::getParserStringList()->getMissingListAssignmentMessage(currentArg.name, requiredValueCount, valueCount) << std::endl;
                    parser_base::setParsingError(true);
                    break;

                case eAssignFailed:
                default:
                    // Failed an assignment
                    std::cerr << parser_base::getParserStringList()->getAssignmentFailedMessage(currentArg.name, failedAssignment) << std::endl;
                    parser_base::setParsingError(true);
                    break;
            } // end of switch status
        } // end of if found
    } // end of for keyArgList

    // If we haven't already failed, check if all required arguments were found
    if (!parser_base::isParsingError())
    {
        // Check that all the required positional arguments were found
        for (auto const& keyArg : parser_base::getKeyArgList())
        {
            if ((keyArg.isRequired) && !(keyArg.isFound))
            {
                std::cerr << parser_base::getParserStringList()->getMissingArgumentMessage(keyArg.name) << std::endl;
                parser_base::setParsingError(true);
            }
        }
    }

    return (!parser_base::isParsingError());
}


/**
 * @brief Print the formatted help message to the input stream
 *
 * @param outStream - Output streem to use for text output.  Default is the standard error stream
 */
void envparser::displayHelp(std::ostream &outStream)
{
    const size_t optionKeyWidth = parser_base::getOptionKeyWidth();
    const size_t helpKeyWidth = parser_base::getHelpKeyWidth(optionKeyWidth);

    if (!parser_base::isKeyArgListEmpty())
    {
        // Display the key arguments help
        outStream << parser_base::getParserStringList()->getEnvArgumentsMessage() << std::endl;
        for (auto const& keyArg : parser_base::getKeyArgList())
        {
            // Display the arg block
            displayArgHelpBlock(outStream, keyArg.optionString, keyArg.help, optionKeyWidth, helpKeyWidth);
        }
    }
}

/** @} */
