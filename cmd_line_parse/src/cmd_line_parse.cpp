/*
Copyright (c) 2022-2023 Randal Eike

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
 * @ingroup libcmd_line_parser
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include <algorithm>
#include "parser_string_list.h"
#include "varg_intf.h"
#include "varg.h"
#include "cmd_line_parse.h"
#include "parser_base.h"

using namespace argparser;

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
 * @brief Make sure all arguments that are marked as required were found during the 
 *        command line parsing
 */
void cmd_line_parse::checkRequiredArgsFound()
{
    // Check that all the required positional arguments were found
    for (auto const& positionalArg : positionalArgList)
    {
        if ((positionalArg.isRequired) && !(positionalArg.isFound))
        {
            std::cerr << parser_base::getParserStringList()->getMissingArgumentMessage(positionalArg.name) << std::endl;
            parser_base::setParsingError(true);
        }
    }

    // Check that all the required key arguments were found
    for (auto const& keyArg : parser_base::getKeyArgList())
    {
        if ((keyArg.isRequired) && !(keyArg.isFound))
        {
            parserstr optionString;
            for (auto const& keyString : keyArg.keyList)
            {
                optionString += keyString;
                optionString += "|";
            }
            optionString.resize(optionString.size() - 1);

            std::cerr << parser_base::getParserStringList()->getMissingArgumentMessage(optionString) << std::endl;
            parser_base::setParsingError(true);
        }
    }
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
    for (char &  keyPrefixChar : keyPrefix)
    {
        // Put the prefix character on the help flag
        helpKey[0] = keyPrefixChar;
        keyList += helpKey;

        // Add a double prefix character to the help argument
        helpArg[0] = keyPrefixChar;
        helpArg[1] = keyPrefixChar;
        keyList += parser_base::getKeyListDelimeter();
        if (keyPrefixChar == '/')
        {
            // One slash is enough
            keyList += helpArg.substr(1, parserstr::npos);
        }
        else
        {
            keyList += helpArg;
        }

        // Add ?
        helpKey1[0] = keyPrefixChar;
        keyList += parser_base::getKeyListDelimeter();
        keyList += helpKey1;
    }

    // Add the argument to the key argument list
    helpFlag = new varg<bool>(false, true);
    addKeyArgument(helpFlag, "help", keyList, parser_base::getParserStringList()->getHelpString(), 0, false);
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
    parserstr currentValue = ((valueString.empty() && (currentArgumentIndex < argcount)) ? argvArray[currentArgumentIndex++] : valueString);
    return parser_base::getValueList(currentValue, valueList);
}

/**
 * @brief Find the argument object that matches the input string
 *
 * @param keystring   - Input string to match
 * @param found       - Set to true if match was found, else false
 *
 * @return ArgEntry - Reference to the ArgEntry from the ArgEntry if match was found. Or nullptr if not.
 */
ArgEntry& cmd_line_parse::findMatchingArg(const parserstr& keystring, bool& found)
{
    ArgEntry& returnArg = parser_base::findMatchingArg(keystring, found);

    // Unknown argument key
    if ((!found) && ((!ignoreUnknownKey) || (debugMsgLevel > noDebugMsg)))
    {
        std::cerr << parser_base::getParserStringList()->getUnknownArgumentMessage(keystring) << std::endl;
        parser_base::setParsingError(!ignoreUnknownKey);
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
    bool status = false;
    if (!valueString.empty())
    {
        std::cerr << parser_base::getParserStringList()->getInvalidAssignmentMessage(keyString) << std::endl;
        status = true;
    }
    else if (eAssignSuccess != parser_base::assignKeyFlagValue(currentArg))
    {
        std::cerr << parser_base::getParserStringList()->getAssignmentFailedMessage(keyString, valueString) << std::endl;
        status = true;
    }

    if (debugMsgLevel >= debugVerbosityLevel_e::veryVerboseDebug)
    {
        std::cout << "cmd_line_parse::assignKeyFlagValue return status: " << status << std::endl;
    }
    return status;
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
    if (debugMsgLevel > veryVerboseDebug)
    {
        std::cout << "Initial value string: " << valueString << std::endl;
    }
    std::list<parserstr> assignmentValues;
    size_t valueCount = getInitialValueList(valueString, assignmentValues);
    auto requiredValueCount = static_cast<size_t>(abs(currentArg.nargs));
    if (debugMsgLevel > veryVerboseDebug)
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
        size_t    addCount = parser_base::getValueList(currentValueString, assignmentValues);
        if (debugMsgLevel > veryVerboseDebug)
        {
            std::cout << "Next value string: " << currentValueString << std::endl;
            std::cout << "Next addCount: " << addCount << std::endl;
        }

        if (addCount == 0)
        {
            // Negative number required == max count
            break;
        }

        // Add the new values to the counter
        valueCount += addCount;
    }

    // Check we got the correct number of arguments
    if (debugMsgLevel > veryVerboseDebug) 
    {
        std::cout << "Assignment string count: " << assignmentValues.size() << std::endl;
    }

    parserstr failedValue;
    eAssignmentReturn status = parser_base::assignListKeyValue(currentArg, assignmentValues, failedValue);
    switch(status)
    {
        case eAssignSuccess:
            return false;  // return success

        case eAssignTooMany:
            // Not enough values to meet the minimum required
            std::cerr << parser_base::getParserStringList()->getTooManyAssignmentMessage(keyString, requiredValueCount, valueCount) << std::endl;
            return true;

        case eAssignNoValue:
            // Need at least one value
            std::cerr << parser_base::getParserStringList()->getMissingAssignmentMessage(keyString) << std::endl;
            return true;

        case eAssignTooFew:
            // More values than required
            std::cerr << parser_base::getParserStringList()->getMissingListAssignmentMessage(keyString, requiredValueCount, valueCount) << std::endl;
            return true;

        case eAssignFailed:
        default:
            // Failed an assignment
            std::cerr << parser_base::getParserStringList()->getAssignmentFailedMessage(keyString, failedValue) << std::endl;
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
    if (debugMsgLevel >= debugVerbosityLevel_e::verboseDebug)
    {
        std::cout << "Parsing key arg: " << searchString << std::endl;
        if (!valueString.empty())
        {
            std::cout << "Value string: " << valueString << std::endl;
        }
    }
    ArgEntry currentArg = findMatchingArg(searchString, found);
    if (found)
    {
        if (debugMsgLevel >= debugVerbosityLevel_e::veryVerboseDebug)
        {
            std::cout << "match found, name = " << currentArg.name << std::endl;
            std::cout << "match found, nargs = " << currentArg.nargs << std::endl;
        }

        // Flag argument, set value and exit
        if (currentArg.nargs == 0)
        {
            parser_base::setParsingError(assignKeyFlagValue(currentArg, searchString, valueString));
        }
        else
        {
            parser_base::setParsingError(assignKeyValue(currentArg, searchString, valueString));
        }
    }
    else if (!ignoreUnknownKey)
    {
        parser_base::setParsingError(true);
        std::cerr << parser_base::getParserStringList()->getUnknownArgumentMessage(searchString) << std::endl;
    }

    if (debugMsgLevel >= debugVerbosityLevel_e::veryVerboseDebug)
    {
        std::cout << "Parsing error status = " << parser_base::isParsingError() << std::endl;
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
    std::size_t valuePos = currentArg.find(parser_base::getAssignmentDelimeter());
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
            for (size_t index = currentArg.size() - 1; index > 0; index--)
            {
                parserstr searchArg;
                searchArg += currentArg[0];     // key delemiter
                searchArg += currentArg[index]; // key character

                // Parse the current single character
                parseSingleKeyArg(searchArg.c_str(), valueString);

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
        for (auto& currentArg : positionalArgList)
        {
            if ((currentArg.position == parseingPositionNumber) || (currentArg.position == 0))
            {
                if (debugMsgLevel > veryVerboseDebug)
                {
                    std::cout << "Positional Argument Name: " << currentArg.name << ", position: " << currentArg.position << std::endl;
                    std::cout << "Current Parsing Position: " << parseingPositionNumber << std::endl;
                }

                if (currentArg.position != 0)
                {
                    parseingPositionNumber++;
                }
                currentArg.isFound = true;
                if (currentArg.name == positionalStop)
                {
                    positionalStopArgumentFound = true;
                }

                parser_base::setParsingError(assignKeyValue(currentArg, currentArg.name.c_str(), valueString));
                break;
            }
        }
    }
    else
    {
        parser_base::setParsingError(true);
        std::cerr << parser_base::getParserStringList()->getUnknownArgumentMessage(argvArray[currentArgumentIndex++]) << std::endl;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Constructor/Destructor functions
//============================================================================================================================
//============================================================================================================================
cmd_line_parse::cmd_line_parse() : usageText("%(prog) [options]"), keyPrefix("-"), 
    displayHelpOnError(true), enableDefaultHelp(true), 
    ignoreUnknownKey(false), singleCharArgListAllowed(true), 
    positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0),
    debugMsgLevel(0), positionalStopArgumentFound(false), helpFlag(nullptr)
{
    positionalArgList.clear();
    argvArray.clear();
    addDefaultHelpArgument();
}

cmd_line_parse::cmd_line_parse(const cmd_line_parse& other) : 
    parser_base(other), programName(other.programName), 
    usageText(other.usageText), descriptionText(other.descriptionText), keyPrefix(other.keyPrefix),
    displayHelpOnError(other.displayHelpOnError), enableDefaultHelp(other.enableDefaultHelp),
    ignoreUnknownKey(other.ignoreUnknownKey), singleCharArgListAllowed(other.singleCharArgListAllowed),
    positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0),
    debugMsgLevel(other.debugMsgLevel), positionalStopArgumentFound(false), 
    positionalArgList(other.positionalArgList), helpFlag(nullptr)
{
    argvArray.clear();
    if (nullptr != other.helpFlag)
    {
        addDefaultHelpArgument();
    }
}

cmd_line_parse::cmd_line_parse(cmd_line_parse&& other) : 
    parser_base(other), programName(other.programName), 
    usageText(other.usageText), descriptionText(other.descriptionText), keyPrefix(other.keyPrefix),
    displayHelpOnError(other.displayHelpOnError), enableDefaultHelp(other.enableDefaultHelp),
    ignoreUnknownKey(other.ignoreUnknownKey), singleCharArgListAllowed(other.singleCharArgListAllowed),
    positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0),
    debugMsgLevel(other.debugMsgLevel), positionalStopArgumentFound(false), 
    positionalArgList(other.positionalArgList), helpFlag(nullptr)
{
    argvArray.clear();
    if (nullptr != other.helpFlag)
    {
        addDefaultHelpArgument();
    }
}

cmd_line_parse::cmd_line_parse(parserstr& usage, parserstr& description, bool abortOnError, bool disableDefaultHelp, int debugLevel) :
    parser_base(abortOnError, debugLevel), keyPrefix("-"), 
    displayHelpOnError(true), enableDefaultHelp(!disableDefaultHelp), 
    ignoreUnknownKey(false), singleCharArgListAllowed(true), 
    positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0),  
    debugMsgLevel(debugLevel), positionalStopArgumentFound(false), helpFlag(nullptr)
{
    positionalArgList.clear();
    argvArray.clear();

    if (!usage.empty())
    {
        usageText = usage;
    }
    
    if (!description.empty())
    {
        descriptionText = description;
    }

    if (!disableDefaultHelp)
    {
        addDefaultHelpArgument();
    }
}

cmd_line_parse::cmd_line_parse(const char* usage, const char* description, bool abortOnError, bool disableDefaultHelp, int debugLevel) :
    parser_base(abortOnError, debugLevel), keyPrefix("-"), 
    displayHelpOnError(true), enableDefaultHelp(!disableDefaultHelp), 
    ignoreUnknownKey(false), singleCharArgListAllowed(true), 
    positionNumber(1), parseingPositionNumber(1), currentArgumentIndex(0), argcount(0),  
    debugMsgLevel(debugLevel), positionalStopArgumentFound(false), helpFlag(nullptr)
{
    positionalArgList.clear();
    argvArray.clear();

    if (nullptr != usage)
    {
        usageText = usage;
    }
    
    if (nullptr != description)
    {
        descriptionText = description;
    }

    if (!disableDefaultHelp)
    {
        addDefaultHelpArgument();
    }
}

cmd_line_parse& cmd_line_parse::operator=(const cmd_line_parse& other) 
{
    if (this != &other)
    {
        programName                 = other.programName; 
        usageText                   = other.usageText; 
        descriptionText             = other.descriptionText;
        keyPrefix                   = other.keyPrefix; 
        displayHelpOnError          = other.displayHelpOnError;
        enableDefaultHelp           = other.enableDefaultHelp; 
        ignoreUnknownKey            = other.ignoreUnknownKey;
        singleCharArgListAllowed    = other.singleCharArgListAllowed; 
        debugMsgLevel               = other.debugMsgLevel;

        positionNumber              = 1; 
        parseingPositionNumber      = 1; 
        currentArgumentIndex        = 0;
        argcount                    = 0;
        positionalStopArgumentFound = false;
        helpFlag                    = nullptr;

        argvArray.clear(); 
        positionalArgList.clear();
        positionalArgList           = other.positionalArgList;
        if (enableDefaultHelp)
        {
            addDefaultHelpArgument();
        }
    }
    return *this;
}

cmd_line_parse& cmd_line_parse::operator=(cmd_line_parse&& other) 
{
    if (this != &other)
    {
        programName                 = other.programName; 
        usageText                   = other.usageText; 
        descriptionText             = other.descriptionText;
        keyPrefix                   = other.keyPrefix; 
        displayHelpOnError          = other.displayHelpOnError;
        enableDefaultHelp           = other.enableDefaultHelp; 
        ignoreUnknownKey            = other.ignoreUnknownKey;
        singleCharArgListAllowed    = other.singleCharArgListAllowed; 
        debugMsgLevel               = other.debugMsgLevel;

        positionNumber              = 1; 
        parseingPositionNumber      = 1; 
        currentArgumentIndex        = 0;
        argcount                    = 0;
        positionalStopArgumentFound = false;

        positionalArgList.clear();
        argvArray.clear(); 
        positionalArgList           = other.positionalArgList;
        other.positionalArgList.clear();
    }
    return *this;
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
        std::cerr << parser_base::getParserStringList()->getNotListTypeMessage(nargs) << std::endl;
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
            optionString += parser_base::getAssignmentDelimeter();

            if (nargs > 0)
            {
                optionString += newKeyArg.name;
                if (nargs > 1)
                {
                    optionString += parser_base::getAssignmentListDelimeter();
                    optionString += "...";
                }
            }

            if (nargs < 0)
            {
                optionString += newKeyArg.name;
                optionString += parser_base::getAssignmentListDelimeter();
                optionString += "[";
                optionString += parser_base::getAssignmentListDelimeter();
                optionString += "...";
                optionString += "]";
            }
        }

        parser_base::resizeMaxOptionLength(optionString.size());
        newKeyArg.optionString = optionString;

        // Add the new argument to the list
        parser_base::addKeyArgListEntry(newKeyArg);
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
        std::cerr << parser_base::getParserStringList()->getNotListTypeMessage(nargs) << std::endl;
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
            optionString += parser_base::getAssignmentListDelimeter();
            optionString += newArg.name;
            optionString += parser_base::getAssignmentListDelimeter();
            optionString += "...";
        }
        if (nargs < 0)
        {
            optionString += newArg.name;
            optionString += "[";
            optionString += parser_base::getAssignmentListDelimeter();
            optionString += newArg.name;
            optionString += parser_base::getAssignmentListDelimeter();
            optionString += "...]";
        }

        parser_base::resizeMaxOptionLength(optionString.size());
        newArg.optionString = optionString;

        // Add the new argument to the list
        positionalArgList.push_back(newArg);
    }
}

//=================================================================================================
//======================= Parse interface methods =================================================
//=================================================================================================
/*
 * @brief Set the name of the positional argument to stop parsing
 *
 * @param positionalArgumentName - Positional name to find
 */
void cmd_line_parse::setPositionalNameStop(const char* positionalArgumentName)
{
    positionalStop = positionalArgumentName;
}

/**
 * @brief Parse the input arguments
 *
 * @param argc - Number of input arguments
 * @param argv - Array of char* input command line arguments
 *
 * @return int  - Number of arguments parsed or -1 on error
 */
int cmd_line_parse::parse(int argc, char* argv[], int startingArgIndex, int endingArgIndex)
{
    // Load the argument vector
    argvArray.clear();
    for (int argIdx = 0; argIdx < argc; argIdx++)
    {
        argvArray.emplace_back(argv[argIdx]);
    }

    // Check for program name default
    if(programName.empty())
    {
        // Set program name from argument 0
        programName = argvArray[0];
    }

    // Parse the rest of the arguments
    currentArgumentIndex = startingArgIndex;
    argcount = ((endingArgIndex > 0) ? endingArgIndex : argc);
    argcount = std::min(argcount, argc);
    parser_base::clearParsingError();

    while ((currentArgumentIndex < argcount) && parser_base::isParserAbort() && (!positionalStopArgumentFound))
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

    int returnValue = currentArgumentIndex; //number of arguments parsed

    // If we haven't already failed, check if all required arguments were found
    if (!parser_base::isParsingError())
    {
        checkRequiredArgsFound();
    }
    
    // Display help on error
    if (parser_base::isParsingError()) 
    {
        if (displayHelpOnError || (enableDefaultHelp && dynamic_cast< varg<bool>* >(helpFlag)->value))
        {
            displayHelp(std::cerr);
        }
        returnValue = -1; //-1 for error
    }

    // Return the number of arguments parsed or -1 for error
    return returnValue;
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
    const size_t optionKeyWidth = parser_base::getOptionKeyWidth();
    const size_t helpKeyWidth = parser_base::getHelpKeyWidth(optionKeyWidth);

    if (!parser_base::isKeyArgListEmpty())
    {
        // Display the key arguments help
        outStream << parser_base::getParserStringList()->getSwitchArgumentsMessage() << std::endl;
        for (auto const& keyArg : parser_base::getKeyArgList())
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
    const size_t optionKeyWidth = parser_base::getOptionKeyWidth();
    const size_t helpKeyWidth = parser_base::getHelpKeyWidth(optionKeyWidth);

    if (!positionalArgList.empty())
    {
        // Display the position arguments list
        outStream << parser_base::getParserStringList()->getPositionalArgumentsMessage() << std::endl;
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
    outStream << parser_base::getParserStringList()->getUsageMessage() << std::endl << usageText << std::endl;
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
