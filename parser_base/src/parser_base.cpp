/* 
 Copyright (c) 2022-2024 Randal Eike
 
 Permission is hereby granted, free of parsercharge, to any person obtaining a 
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
 * @file parser_base.cpp
 * @ingroup argparser
 * @defgroup Base functionality for all varg_intf parsers
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include "varg_intf.h"
#include "varg.h"
#include "parser_base.h"
#include "parser_string_list.h"

using namespace argparser;

constexpr size_t DefaultColumnWidth = 80;
constexpr size_t DefaultOptionWidth = 0;

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
parser_base::parser_base(bool abortOnError, int debugLevel) : 
    maxColumnWidth(DefaultColumnWidth), maxOptionLength(DefaultOptionWidth),
    keyListDelimeter(','), assignmentDelimeter('='), assignmentListDelimeter(','),
    errorAbort(abortOnError), debugMsgLevel(debugLevel), parsingError(false),
    parserStringList(BaseParserStringList::getInternationalizedClass())
{
    keyArgList.clear();
    nullEntry = {};
}

parser_base::parser_base(const parser_base& other) :
    maxColumnWidth(other.assignmentDelimeter), maxOptionLength(other.maxOptionLength),
    keyListDelimeter(other.keyListDelimeter), assignmentDelimeter(other.assignmentDelimeter), assignmentListDelimeter(other.assignmentListDelimeter),
    errorAbort(other.errorAbort), debugMsgLevel(other.debugMsgLevel), parsingError(false),
    keyArgList(other.keyArgList), nullEntry(other.nullEntry), parserStringList(other.parserStringList)
{
}

parser_base::parser_base(parser_base&& other) :
    maxColumnWidth(other.assignmentDelimeter), maxOptionLength(other.maxOptionLength),
    keyListDelimeter(other.keyListDelimeter), assignmentDelimeter(other.assignmentDelimeter), assignmentListDelimeter(other.assignmentListDelimeter),
    errorAbort(other.errorAbort), debugMsgLevel(other.debugMsgLevel), parsingError(false),
    keyArgList(other.keyArgList), nullEntry(other.nullEntry), parserStringList(other.parserStringList)
{
    other.keyArgList.clear();
}

parser_base& parser_base::operator=(const parser_base& other)
{
    if (this != &other)
    {
        maxColumnWidth          = other.assignmentDelimeter;
        maxOptionLength         = other.maxOptionLength;
        keyListDelimeter        = other.keyListDelimeter;
        assignmentDelimeter     = other.assignmentDelimeter;
        assignmentListDelimeter = other.assignmentListDelimeter;
        errorAbort              = other.errorAbort;
        debugMsgLevel           = other.debugMsgLevel;
        parsingError            = false;
        keyArgList              = other.keyArgList;
        nullEntry               = {};
        parserStringList        = other.parserStringList;
    }
    return *this;
}

parser_base& parser_base::operator=(parser_base&& other)
{
    if (this != &other)
    {
        maxColumnWidth          = other.assignmentDelimeter;
        maxOptionLength         = other.maxOptionLength;
        keyListDelimeter        = other.keyListDelimeter;
        assignmentDelimeter     = other.assignmentDelimeter;
        assignmentListDelimeter = other.assignmentListDelimeter;
        errorAbort              = other.errorAbort;
        debugMsgLevel           = other.debugMsgLevel;
        parsingError            = false;
        keyArgList              = other.keyArgList;
        nullEntry               = {};
        parserStringList        = other.parserStringList;

        other.keyArgList.clear();
    }
    return *this;
}

parser_base::~parser_base()
{
    keyArgList.clear();
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================

//=================================================================================================
//======================= Argument add helper interface methods ===================================
//=================================================================================================
/**
 * @brief Break the input argument key list into it's parts
 * 
 * @param arg          - Argument to add the key values to
 * @param inputKeyList - delimited list of argument keys
 * 
 * @return size_t - number of elements in the list
 */
size_t parser_base::addArgKeyList(ArgEntry& arg, parserstr inputKeyList) const
{
    size_t pos = 0;
    parserstr inputString = inputKeyList;
    parserstr token;

    // Erase any leading spaces
    while (' ' == inputString.front())
    {
        inputString.erase(0, 1);
    }

    // Tokenize based on the delimeter
    while ((pos = inputString.find(keyListDelimeter)) != parserstr::npos) 
    {
        token = inputString.substr(0, pos);

        // Erase any trailing spaces preceeding the delimeter
        while (' ' == token.back())
        {
            token.pop_back();
        }

        arg.keyList.push_back(token);
        inputString.erase(0, pos + 1);

        // Erase any spaces following the delimeter
        while (' ' == inputString.front())
        {
            inputString.erase(0, 1);
        }
    }   

    // Add the last one
    if (inputString.length() != 0)
    {
        // Erase any trailing spaces preceeding the delimeter
        while (' ' == inputString.back())
        {
            inputString.pop_back();
        }
        arg.keyList.push_back(inputString);
    }
    return arg.keyList.size();
}

//=================================================================================================
//======================= Argument parsing helper interface methods ===============================
//=================================================================================================
/**
 * @brief Break the input value list into it's parts
 * 
 * @param valueString - delimited list of value data
 * @param valueList   - Reference to the current value string list
 * 
 * @return size_t - number of elements in the list
 */
size_t parser_base::getValueList(parserstr& valueString, std::list<parserstr>& valueList) const
{
    size_t pos = 0;
    parserstr token;
    size_t addedCount = 0;

    // Check for empty
    if (!valueString.empty())
    {
        // Strip beginning assignment delimeter if it's present
        if (valueString[0] == assignmentDelimeter)
        {
            valueString.erase(0,1);
        }

        // Parse the input value list
        while ((pos = valueString.find(assignmentListDelimeter)) != parserstr::npos) 
        {
            token = valueString.substr(0, pos);
            valueList.push_back(token);
            addedCount++;
            valueString.erase(0, pos + 1);
        }   

        // Add the last one
        if (valueString.length() != 0)
        {
            valueList.push_back(valueString);
            addedCount++;
        }
    }
    return addedCount;
}

/**
 * @brief Find the argument object that matches the input string
 * 
 * @param checkString - Input string to match
 * @param found       - Set to true if match was found, else false
 * 
 * @return ArgEntry - Reference to the ArgEntry from the ArgEntry if match was found. Or nullptr if not.
 */
ArgEntry& parser_base::findMatchingArg(const parserstr& checkString, bool& found)
{
    found = false;

    if(debugMsgLevel > 4)
    {
        std::cerr << "keyArgList size: " << keyArgList.size() << std::endl;
    }

    // Iterate through the key argument list for a match
    for (auto & argument : keyArgList)
    {
        // Check input key string for a match in the argument key string list
        parserstr testKey = checkString;
        for (auto const& argumentKey : argument.keyList)
        {
            // Check if the input key string matches this argument key list entry
            if(debugMsgLevel > 4)
            {
                std::cerr << "Testing var: " << argument.name << " test key: " << argumentKey << " input key: " << testKey << std::endl;
                std::cerr << "Test key size: " << argumentKey.size() << " input key size: " << testKey.size() << std::endl;
            }
            if (argumentKey == testKey)
            {
                // Found a match
                if(debugMsgLevel > 3) 
                {
                    std::cerr << "Found match var: " << argument.name << " key: " << argumentKey << std::endl;
                }
                argument.isFound = true;
                found = true;
                return (argument);
            }
        }
    }

    return nullEntry;
}

/**
 * @brief Assign the flag value to the key argument
 * 
 * @param currentArg - Pointer to the argument to set
 * 
 * @return eAssignmentReturn - Assignment return status
 */
eAssignmentReturn parser_base::assignKeyFlagValue(ArgEntry& currentArg)
{
    return ((valueParseStatus_e::PARSE_SUCCESS_e == currentArg.argData->setValue()) ? eAssignSuccess : eAssignFailed);
}

/**
 * @brief Assign single value to an argument storage object.  
 * 
 * @param currentArg - Pointer to the argument to set
 * @param assignmentValue - Reference to the value string
 * 
 * @return eAssignmentReturn - Assignment return status
 */
eAssignmentReturn parser_base::assignKeyValue(ArgEntry& currentArg, parserstr& assignmentValue)
{
    return ((assignmentValue.empty()) ? eAssignNoValue : 
                ((valueParseStatus_e::PARSE_SUCCESS_e == currentArg.argData->setValue(assignmentValue.c_str())) ? eAssignSuccess : eAssignFailed));
}

/**
 * @brief Assign multiple values to a list argument storage object.  
 * 
 * @param currentArg - Pointer to the argument to set
 * @param assignmentValues - List of assignment value strings
 * @param failedValue - Value string that failed assignment in the list
 * 
 * @return eAssignmentReturn - Assignment return status
 */
eAssignmentReturn parser_base::assignListKeyValue(ArgEntry& currentArg, std::list<parserstr>& assignmentValues, parserstr& failedValue)
{
    auto requiredValueCount = abs(currentArg.nargs);

    if (assignmentValues.empty())
    {
        // Need at least one value
        return eAssignNoValue;
    }

    // Check we got too many arguments
    if ((assignmentValues.size() > requiredValueCount) && (currentArg.nargs != -1))
    {
        return eAssignTooMany;
    }

    // Check we got too few arguments
    if (static_cast<int>(assignmentValues.size()) < currentArg.nargs)
    {
        return eAssignTooFew;
    }

    // Assign the values
    for (auto const& valueStr : assignmentValues)
    {
        if(valueParseStatus_e::PARSE_SUCCESS_e != currentArg.argData->setValue(valueStr.c_str()))
        {
            failedValue = valueStr;
            return eAssignFailed;
        }
    }

    return eAssignSuccess;
}

//=================================================================================================
//======================= Help display helper interface methods ===================================
//=================================================================================================

/**
 * @brief Ouput the next argument help block with the option
 *        string and help string properly wrapped to the input
 *        column widths
 * 
 * @param outStream       - Output stream for the text
 * @param baseOptionText  - Raw option string 
 * @param baseHelpText    - Raw help text string
 * @param optionWidth     - Maximum width of an option text display before it wraps
 * @param helpWidth       - Maximum width of an help text display before it wraps
 */
void parser_base::displayArgHelpBlock(std::ostream &outStream, parserstr baseOptionText, parserstr baseHelpText, const size_t optionWidth, const size_t helpWidth)
{
    // Format the columns
    std::list<parserstr> helpTextList = parserStringList->formatStringToLength(baseHelpText, parserStringList->getDefaultBreakCharList(), helpWidth);
    std::list<parserstr> optionTextList = parserStringList->formatStringToLength(baseOptionText, {'<',' '}, optionWidth);

    // Output the column text
    while ((!helpTextList.empty()) || (!optionTextList.empty()))
    {
        // Get the next option string
        if (!optionTextList.empty())
        {
            outStream << optionTextList.front();
            optionTextList.pop_front();
        }
        else
        {
            for (size_t padCount = 0; padCount < optionWidth; padCount++)
            {
                outStream << ' ';
            }
        }

        // Get the next help string
        if (!helpTextList.empty())
        {
            outStream << " " << helpTextList.front() << std::endl;
            helpTextList.pop_front();
        }
        else
        {
            outStream << std::endl;
        }
    }
}

/** @} */
