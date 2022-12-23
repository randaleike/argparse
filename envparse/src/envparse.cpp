/* 
Copyright (c) 2022 Randal Eike
 
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
 * @ingroup argparser
 * @defgroup Environment parsing utility
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
        parserStringList->printEnvironmentNoFlags(std::cerr, argKey);
    }
    else if ((nargs != 1) && !arg->isList())
    {
        parserStringList->printNotListTypeMessage(std::cerr, nargs);
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
        optionString += assignmentDelimeter;
        optionString += newKeyArg.name;
        if (nargs > 1)
        {
            optionString += assignmentListDelimeter;
            optionString += "...";
        }

        if (optionString.size() > maxOptionLength)
        {
            maxOptionLength = optionString.size();
        }

        newKeyArg.optionString = optionString;

        // Add the new argument to the list
        keyArgList.push_back(newKeyArg);
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
    for (std::list<ArgEntry>::iterator currentArg = keyArgList.begin(); currentArg != keyArgList.end(); ++currentArg)
    {
        const char* envValue = getenv(currentArg->name.c_str());
        if (NULL != envValue)
        {
            // Process the return value string
            std::list<std::string> assignmentValues;
            std::string valueString = envValue;
            size_t valueCount = parser_base::getValueList(valueString, assignmentValues);
            size_t requiredValueCount = static_cast<size_t>(abs(currentArg->nargs));
            if (debugMsgLevel > 5)
            {
                std::cout << "Environment value: " << envValue << std::endl;
                std::cout << "Environment var name: " << currentArg->name << std::endl;
                std::cout << "Value count: " << valueCount << std::endl;
            }

            // Assign the values
            currentArg->isFound = true;
            parserstr failedAssignment = valueString;
            eAssignmentReturn status = ((valueCount > 1) ? 
                                            parser_base::assignListKeyValue(*currentArg, assignmentValues, failedAssignment) :
                                            parser_base::assignKeyValue(*currentArg, valueString));

            // Check the assignment error
            switch(status)
            {
                case eAssignSuccess:
                    break;

                case eAssignTooMany:
                    // Not enough values to meet the minimum required
                    parserStringList->printTooManyAssignmentMessage(std::cerr, currentArg->name.c_str(), requiredValueCount, valueCount);
                    parsingError = true;
                    break;

                case eAssignNoValue:
                    // Need at least one value
                    parserStringList->printMissingAssignmentMessage(std::cerr, currentArg->name.c_str());
                    parsingError = true;
                    break;

                case eAssignTooFew:
                    // More values than required
                    parserStringList->printMissingListAssignmentMessage(std::cerr, currentArg->name.c_str(), requiredValueCount, valueCount);
                    parsingError = true;
                    break;

                case eAssignFailed:
                default:
                    // Failed an assignment
                    parserStringList->printAssignmentFailedMessage(std::cerr, currentArg->name.c_str(), failedAssignment);
                    parsingError = true;
                    break;
            } // end of switch status
        } // end of if found
    } // end of for keyArgList

    // If we haven't already failed, check if all required arguments were found
    if (false == parsingError)
    {
        // Check that all the required positional arguments were found
        for (auto const& keyArg : keyArgList)
        {
            if ((keyArg.isRequired) && !(keyArg.isFound))
            {
                parserStringList->printMissingArgumentMessage(std::cerr, keyArg.name.c_str());
                parsingError = true;
            }
        }
    }

    return (!parsingError);
}


/**
 * @brief Print the formatted help message to the input stream
 * 
 * @param outStream - Output streem to use for text output.  Default is the standard error stream
 */
void envparser::displayHelp(std::ostream &outStream)
{
    const size_t optionKeyWidth = std::min(maxOptionLength, maxColumnWidth/2);
    const size_t helpKeyWidth = maxColumnWidth - optionKeyWidth - 1;

    if (!keyArgList.empty())
    {
        // Display the key arguments help
        outStream << parserStringList->getEnvArgumentsMessage() << std::endl;
        for (auto const& keyArg : keyArgList)
        {
            // Display the arg block
            displayArgHelpBlock(outStream, keyArg.optionString, keyArg.help, optionKeyWidth, helpKeyWidth);
        }
    }
}

/** @} */
