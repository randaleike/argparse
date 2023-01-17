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
 * @file cmd_line_parse.cpp
 * @ingroup cmd_line_parse
 * @defgroup Command line parsing utility
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include "varg_intf.h"
#include "varg.h"
#include "cmd_line_parse.h"
#include "parser_base.h"

using namespace argparser;

static varg<bool> helpFlag(false, true);

//============================================================================================================================
//============================================================================================================================
//  Private functions
//============================================================================================================================
//============================================================================================================================

//=================================================================================================
//======================= Argument add helper interface methods ===================================
//=================================================================================================

/**
 * @brief Test if the next command line argument is a key switch
 *
 * @return true  - Next command line argument is a key switch
 * @return false - Next argument is a value
 */
bool cmd_line_parse::isCurrentArgKeySwitch()
{
    return (parserstr::npos != keyPrefix.find(argvArray[currentArgumentIndex][0]));
}

/**
 * @brief Add the default help arguments
 */
void cmd_line_parse::addDefaultHelpArgument()
{
    // Add the new keys
    parserstr helpKey = " h";
    parserstr helpKey1 = " ?";
    parserstr helpArg = "  help";
    parserstr keyList;
    for (int n = 0; n < keyPrefix.length(); n++)
    {
        // Put the prefix character on the help flag
        helpKey[0] = keyPrefix[n];
        keyList += helpKey;

        // Add a double prefix character to the help argument
        helpArg[0] = keyPrefix[n];
        helpArg[1] = keyPrefix[n];
        if (keyPrefix[n] == '/')
        {
            // One slash is enough
            keyList += keyListDelimeter;
            keyList += helpArg.substr(1, parserstr::npos);
        }
        else
        {
            keyList += keyListDelimeter;
            keyList += helpArg;
        }

        // Add ?
        helpKey1[0] = keyPrefix[n];
        keyList += keyListDelimeter;
        keyList += helpKey1;
    }

    // Add the argument to the key argument list
    addKeyArgument(&helpFlag, "help", keyList, parserStringList->getHelpString(), 0, false);
}

/**
 * @brief Break the input value list into it's parts
 *
 * @param valueString - delimited list of value data
 * @param valueList  - Reference to the current value string list
 *
 * @return size_t - number of elements in the list
 */
size_t cmd_line_parse::getInitialValueList(parserstr& valueString, std::list<parserstr>& valueList)
{
    if (valueString.empty() && (currentArgumentIndex < argcount))
    {
        // Get the value list from the next argument
        parserstr currentValue = argvArray[currentArgumentIndex++];
        return parser_base::getValueList(currentValue, valueList);
    }
    else
    {
        // Get the value list from the next argument
        return parser_base::getValueList(valueString, valueList);
    }
}

/**
 * @brief Find the argument object that matches the input string
 *
 * @param keystring   - Input string to match
 * @param found       - Set to true if match was found, else false
 *
 * @return ArgEntry - Reference to the ArgEntry from the ArgEntry if match was found. Or nullptr if not.
 */
ArgEntry& cmd_line_parse::findMatchingArg(const char* keystring, bool& found)
{
    ArgEntry& returnArg = parser_base::findMatchingArg(keystring, found);

    // Unknown argument key
    if ((false == found) && ((false == ignoreUnknownKey) || (debugMsgLevel > 0)))
    {
        std::cerr << parserStringList->getUnknownArgumentMessage(keystring) << std::endl;
        parsingError |= (ignoreUnknownKey ? false : true);
    }
    return returnArg;
}

/**
 * @brief Assign the flag value to the key argument
 *
 * @param currentArg - Pointer to the argument to set
 * @param keyString  - Key value that was matched
 * @param valueString - Reference to the embedded value string if present, else empty string
 *
 * @return bool - False = assignment worked, true = assignment failed
 */
bool cmd_line_parse::assignKeyFlagValue(ArgEntry& currentArg, const char* keyString, parserstr& valueString)
{
    if (!valueString.empty())
    {
        std::cerr << parserStringList->getInvalidAssignmentMessage(keyString) << std::endl;
        return true;
    }

    if (eAssignSuccess != parser_base::assignKeyFlagValue(currentArg))
    {
        std::cerr << parserStringList->getAssignmentFailedMessage(keyString, valueString) << std::endl;
        return true;
    }

    return false;
}


/**
 * @brief Assign multiple values to a list argument storeage.
 *
 * @param currentArg - Pointer to the argument to set
 * @param keyString  - Key value that was matched
 * @param valueString - Reference to the embedded value string if present, else empty string
 *
 * @return bool - False = assignment worked, true = assignment failed
 */
bool cmd_line_parse::assignKeyValue(ArgEntry& currentArg, const char* keyString, parserstr& valueString)
{
    if (debugMsgLevel > 5)
    {
        std::cout << "Initial value string: " << valueString << std::endl;
    }
    std::list<parserstr> assignmentValues;
    size_t valueCount = getInitialValueList(valueString, assignmentValues);
    size_t requiredValueCount = static_cast<size_t>(abs(currentArg.nargs));
    if (debugMsgLevel > 5)
    {
        std::cout << "Value string post get: " << valueString << std::endl;
        std::cout << "Initial value count: " << valueCount << " Required Count(abs): " << requiredValueCount << " Narg: " << currentArg.nargs << std::endl;
        std::cout << "Current arg index: " << currentArgumentIndex << " of " << argcount << std::endl;
    }

    // Argument list is the input values list
    while (((valueCount < requiredValueCount) || (currentArg.nargs == -1)) &&
           (currentArgumentIndex < argcount) &&
           !isCurrentArgKeySwitch()
          )
    {
        // Get the next argument
        parserstr currentValueString = argvArray[currentArgumentIndex++];
        if (debugMsgLevel > 5) std::cout << "Next value string: " << currentValueString << std::endl;
        size_t addCount = parser_base::getValueList(currentValueString, assignmentValues);
        if (debugMsgLevel > 5) std::cout << "Next addCount: " << addCount << std::endl;

        if (addCount == 0)
        {
            // Negative number required == max count
            break;
        }

        // Add the new values to the counter
        valueCount += addCount;
    }

    // Check we got the correct number of arguments
    if (debugMsgLevel > 5) std::cout << "Assignment string count: " << assignmentValues.size() << std::endl;
    parserstr failedValue;
    eAssignmentReturn status = parser_base::assignListKeyValue(currentArg, assignmentValues, failedValue);
    switch(status)
    {
        case eAssignSuccess:
            return false;  // return success

        case eAssignTooMany:
            // Not enough values to meet the minimum required
            std::cerr << parserStringList->getTooManyAssignmentMessage(keyString, requiredValueCount, valueCount) << std::endl;
            return true;

        case eAssignNoValue:
            // Need at least one value
            std::cerr << parserStringList->getMissingAssignmentMessage(keyString) << std::endl;
            return true;

        case eAssignTooFew:
            // More values than required
            std::cerr << parserStringList->getMissingListAssignmentMessage(keyString, requiredValueCount, valueCount) << std::endl;
            return true;

        case eAssignFailed:
        default:
            // Failed an assignment
            std::cerr << parserStringList->getAssignmentFailedMessage(keyString, failedValue) << std::endl;
            return true;
    }
}

/**
 * @brief Find the matching argument and assign the value
 *
 * @param searchString - Key argument value to find and set
 * @param valueString - Value string from the argument or empty string
 */
void cmd_line_parse::parseSingleKeyArg(const char* searchString, parserstr valueString)
{
    // Find the matching key in the argument list
    bool found = false;
    ArgEntry currentArg = findMatchingArg(searchString, found);
    if (found)
    {
        // Flag argument, set value and exit
        if (currentArg.nargs == 0)
        {
            parsingError |= assignKeyFlagValue(currentArg, searchString, valueString);
        }
        else
        {
            parsingError |= assignKeyValue(currentArg, searchString, valueString);
        }
    }
    else if (!ignoreUnknownKey)
    {
        parsingError = true;
        std::cerr << parserStringList->getUnknownArgumentMessage(searchString) << std::endl;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Parse the input key argument type
 */
void cmd_line_parse::parseKeyArg()
{
    parserstr currentArg = argvArray[currentArgumentIndex++];

    // Check for value as part of the argument
    std::size_t valuePos = currentArg.find(assignmentDelimeter);
    parserstr valueString;
    if (valuePos != parserstr::npos)
    {
        // Copy the assignment string into valueString and truncate the currentArg string
        // to just the argument protion
        valueString = currentArg.substr(valuePos + sizeof(char), parserstr::npos);
        currentArg.erase(valuePos);
    }

    // Check for switch list
    if (parserstr::npos != keyPrefix.find(currentArg[1]))
    {
        // Single argument value
        parseSingleKeyArg(currentArg.c_str(), valueString);
    }
    else
    {
        if ((singleCharArgListAllowed) && (currentArg.size() > 2))
        {
            // parse the single character key list backwards
            for (size_t n = currentArg.size() - 1; n > 0; n--)
            {
                char searchArg[2];
                searchArg[0] = currentArg[0];
                searchArg[1] = currentArg[n];

                // Parse the current single character
                parseSingleKeyArg(searchArg, valueString);

                // Clear the value string
                valueString.erase();
            }
        }
        else
        {
            // Single argument value
            parseSingleKeyArg(currentArg.c_str(), valueString);
        }
    }
}

/**
 * @brief Parse the input positional argument type
 */
void cmd_line_parse::parsePositionalArg()
{
    if (!positionalArgList.empty())
    {
        parserstr valueString = argvArray[currentArgumentIndex++];
        for (std::list<ArgEntry>::iterator currentArg = positionalArgList.begin(); currentArg != positionalArgList.end(); ++currentArg)
        {
            if ((currentArg->position == parseingPositionNumber) || (currentArg->position == 0))
            {
                if (debugMsgLevel > 5)
                {
                    std::cout << "Positional Argument Name: " << currentArg->name << ", position: " << currentArg->position << std::endl;
                    std::cout << "Current Parsing Position: " << parseingPositionNumber << std::endl;
                }

                if (currentArg->position != 0)
                {
                    parseingPositionNumber++;
                }
                currentArg->isFound = true;
                parsingError = assignKeyValue(*currentArg, currentArg->name.c_str(), valueString);
                break;
            }
        }
    }
    else
    {
        parsingError = true;
        std::cerr << parserStringList->getUnknownArgumentMessage(argvArray[currentArgumentIndex++]) << std::endl;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
cmd_line_parse::cmd_line_parse(parserstr usage, parserstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel) :
    parser_base(abortOnError, debugLevel),
    programName(""), usageText(usage), descriptionText(description), epilogText(""),
    keyPrefix("-"), displayHelpOnError(true), enableDefaultHelp(!disableDefaultHelp), ignoreUnknownKey(false),
    singleCharArgListAllowed(true), positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0), argvArray(nullptr),
    debugMsgLevel(debugLevel)
{
    positionalArgList.clear();

    if (false == disableDefaultHelp)
    {
        addDefaultHelpArgument();
    }
}

cmd_line_parse::~cmd_line_parse()
{
    positionalArgList.clear();
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

//=================================================================================================
//======================= Add argument interface methods ==========================================
//=================================================================================================

/**
 * @brief Add a new key based command line argument to the argument list
 *
 * @param arg      - Pointer to the defined var argument to fill
 * @param name     - Name of the argument
 * @param argKeys  - Delimieted list of argument key values
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
void cmd_line_parse::addKeyArgument(varg_intf* arg, parserstr name, parserstr argKeys, parserstr helpText, int nargs, bool required)
{
    // Only list type varg_intf are allowed more than 1 value
    if ((nargs != 0) && (nargs != 1) && !arg->isList())
    {
        std::cerr << parserStringList->getNotListTypeMessage(nargs) << std::endl;
    }
    else
    {
        // Add the keys to the argument
        ArgEntry newKeyArg = {};
        size_t   keyCount = parser_base::addArgKeyList(newKeyArg, argKeys);

        // Setup the key argument parameters
        newKeyArg.name = name;
        newKeyArg.help = helpText;
        newKeyArg.argData = arg;
        newKeyArg.nargs = nargs;
        newKeyArg.position = 0;
        newKeyArg.isRequired = required;
        newKeyArg.isFound = false;

        // Construct the key option help string
        parserstr optionString = " ";
        optionString += argKeys;
        if (nargs != 0)
        {
            optionString += assignmentDelimeter;

            if (nargs > 0)
            {
                optionString += newKeyArg.name;
                if (nargs > 1)
                {
                    optionString += assignmentListDelimeter;
                    optionString += "...";
                }
            }

            if (nargs < 0)
            {
                optionString += newKeyArg.name;
                optionString += assignmentListDelimeter;
                optionString += "[";
                optionString += assignmentListDelimeter;
                optionString += "...";
                optionString += "]";
            }
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
 * @brief Add a new key based flag command line argument to the argument list
 *
 * @param arg      - Pointer to the defined var argument to fill
 * @param name     - Name of the argument
 * @param argKeys  - Delimieted list of argument key values
 * @param helpText - Help text to be printed in the help message
 * @param required - True if argument is required, false if arguemnt is optional
 */
void cmd_line_parse::addFlagArgument(varg_intf* arg, parserstr name, parserstr argKeys, parserstr helpText, bool required)
{
    addKeyArgument(arg, name, argKeys, helpText, 0, required);
}

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param arg      - Pointer to the defined var argument to fill
 * @param name     - Name of the argument
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
void cmd_line_parse::addPositionalArgument(varg_intf* arg, parserstr name, parserstr helpText, int nargs, bool required)
{
    // Only list type varg_intf are allowed more than 1 value
    if ((nargs != 0) && (nargs != 1) && !arg->isList())
    {
        std::cerr << parserStringList->getNotListTypeMessage(nargs) << std::endl;
    }
    else
    {
        // Add the keys to the argument
        ArgEntry newArg = {};

        // Setup the key argument parameters
        newArg.name = name;
        newArg.help = helpText;
        newArg.argData = arg;
        newArg.nargs = nargs;
        newArg.position = positionNumber++;
        newArg.isRequired = required;
        newArg.isFound = false;
        newArg.keyList.clear();

        // Construct the key option help string
        parserstr optionString = " ";
        optionString += newArg.name;
        if (nargs > 1)
        {
            optionString += assignmentListDelimeter;
            optionString += newArg.name;
            optionString += assignmentListDelimeter;
            optionString += "...";
        }
        if (nargs < 0)
        {
            optionString += newArg.name;
            optionString += "[";
            optionString += assignmentListDelimeter;
            optionString += newArg.name;
            optionString += assignmentListDelimeter;
            optionString += "...]";
        }

        if (optionString.size() > maxOptionLength)
        {
            maxOptionLength = optionString.size();
        }

        newArg.optionString = optionString;

        // Add the new argument to the list
        positionalArgList.push_back(newArg);
    }
}

//=================================================================================================
//======================= Parse interface methods =================================================
//=================================================================================================

/**
 * @brief Parse the input arguments
 *
 * @param argc - Number of input arguments
 * @param argv - Array of char* input command line arguments
 *
 * @return true  - No error parsing
 * @return false - Error in the input parameters that caused a parsing failure
 */
bool cmd_line_parse::parse(int argc, char* argv[], int startingArgIndex, int endingArgIndex)
{
    // Check for program name default
    if(programName.empty())
    {
        // Set program name from argument 0
        programName = argv[0];
    }

    // Parse the rest of the arguments
    currentArgumentIndex = startingArgIndex;
    argcount = ((endingArgIndex > 0) ? endingArgIndex : argc);
    argvArray = argv;
    parsingError = false;

    while ((currentArgumentIndex < argcount) && ((parsingError && errorAbort) == false))
    {
        // Check for key delimiter
        if (isCurrentArgKeySwitch())
        {
            parseKeyArg();
        }
        else
        {
            parsePositionalArg();
        }
    }

    // If we haven't already failed, check if all required arguments were found
    if (false == parsingError)
    {
        // Check that all the required positional arguments were found
        for (auto const& positionalArg : positionalArgList)
        {
            if ((positionalArg.isRequired) && !(positionalArg.isFound))
            {
                std::cerr << parserStringList->getMissingArgumentMessage(positionalArg.name.c_str()) << std::endl;
                parsingError = true;
            }
        }

        // Check that all the required positional arguments were found
        for (auto const& keyArg : keyArgList)
        {
            if ((keyArg.isRequired) && !(keyArg.isFound))
            {
                parserstr optionString = "";
                for (auto const& keyString : keyArg.keyList)
                {
                    optionString += keyString;
                    optionString += "|";
                }
                optionString.resize(optionString.size() - 1);

                std::cerr << parserStringList->getMissingArgumentMessage(optionString.c_str()) << std::endl;
                parsingError = true;
            }
        }
    }

    // Display help on error
    if ((parsingError && displayHelpOnError) || (enableDefaultHelp && helpFlag.value))
    {
        displayHelp(std::cerr);
    }

    return (!parsingError);
}


//=================================================================================================
//======================= Help display interface methods ==========================================
//=================================================================================================
/**
 * @brief Print the formatted option help message to the input stream
 *
 * @param outStream - Output streem to use for text output.  Default is the standard error stream
 */
void cmd_line_parse::displayOptionHelp(std::ostream &outStream)
{
    const size_t optionKeyWidth = std::min(maxOptionLength, maxColumnWidth/2);
    const size_t helpKeyWidth = maxColumnWidth - optionKeyWidth - 1;

    if (!keyArgList.empty())
    {
        // Display the key arguments help
        outStream << parserStringList->getSwitchArgumentsMessage() << std::endl;
        for (auto const& keyArg : keyArgList)
        {
            // Display the arg block
            displayArgHelpBlock(outStream, keyArg.optionString, keyArg.help, optionKeyWidth, helpKeyWidth);
        }
        outStream << std::endl;
    }
}

/**
 * @brief Print the formatted environment help message to the input stream
 *
 * @param outStream - Output streem to use for text output.  Default is the standard error stream
 */
void cmd_line_parse::displayPositionHelp(std::ostream &outStream)
{
    const size_t optionKeyWidth = std::min(maxOptionLength, maxColumnWidth/2);
    const size_t helpKeyWidth = maxColumnWidth - optionKeyWidth - 1;

    if (!positionalArgList.empty())
    {
        // Display the position arguments list
        outStream << parserStringList->getPositionalArgumentsMessage() << std::endl;
        for (auto const& positionalArg : positionalArgList)
        {
            // Display the arg block
            displayArgHelpBlock(outStream, positionalArg.optionString, positionalArg.help, optionKeyWidth, helpKeyWidth);
        }
        outStream << std::endl;
    }
}

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param outStream - Output streem to use for text output.  Default is the standard error stream
 */
void cmd_line_parse::displayHelp(std::ostream &outStream)
{
    // Display the help header
    outStream << parserStringList->getUsageMessage() << std::endl << usageText << std::endl;
    if (!descriptionText.empty())
    {
        outStream << std::endl << descriptionText << std::endl;
        outStream << std::endl;
    }

    displayOptionHelp(outStream);
    displayPositionHelp(outStream);

    if (!epilogText.empty())
    {
        // Display the help epilog text
        outStream << std::endl << epilogText << std::endl;
    }
}

/** @} */
