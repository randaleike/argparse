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
 * @file parser_string_list.cpp
 * @ingroup argparser
 * @defgroup Internationalized string list for all parsers
 * @{
 */

// Includes
#include <cstddef>
#include <cstring>
#include <string>
#include <sstream>
#include <memory>
#include "parser_string_list.h"
#include "ParserStringListInterface.h"

using namespace argparser;

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Find the best position to break the sting given the input list of break characters to choose from
 *
 * @param workingString - Current working string
 * @param breakCharList - List of characters to position the break on
 * @param maxLength     - Maximum character length
 *
 * @return size_t  - best breack position found
 */
size_t BaseParserStringList::findBestBreakPos(parserstr workingString, std::list<parserchar> breakCharList, size_t maxLength) const
{
    std::list<size_t> breakPos;
    size_t            retVal = maxLength;

    if (!breakCharList.empty())
    {
        // Start with the first char in the list
        for (auto breakCharacter : breakCharList)
        {
            // Find the best break location
            size_t workingBreakPos = workingString.rfind(breakCharacter);
            if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug)
            {
                std::cerr << "Start break: " << workingBreakPos << std::endl;
                std::cerr << "Current break: " << workingBreakPos << std::endl;
            }

            while((workingBreakPos > maxLength) && (workingBreakPos != parserstr::npos))
            {
                // Search for the next break character
                workingBreakPos = workingString.rfind(breakCharacter, workingBreakPos-1);
                if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug)
                {
                    std::cerr << "Current break: " << workingBreakPos << std::endl;
                }
            }

            // Record the break position for that character
            if (workingBreakPos <= maxLength)
            {
                breakPos.push_back(workingBreakPos);
            }
        }

        // Sort the list and return the best break position
        if (!breakPos.empty())
        {
            // Return the break position character closest to the max length
            breakPos.sort();
            retVal = breakPos.back();
        }
    }

    return retVal;
}

//============================================================================================================================
//============================================================================================================================
//  Public Constructor/Destructor
//============================================================================================================================
//============================================================================================================================
BaseParserStringList::BaseParserStringList() :
    defaultBreakList({' '}), debugMsgLevel(0)
{}

BaseParserStringList::BaseParserStringList(const BaseParserStringList& other) :
    defaultBreakList(std::move(other.defaultBreakList)),
    debugMsgLevel(other.debugMsgLevel)
{}

BaseParserStringList::BaseParserStringList(BaseParserStringList&& other) noexcept :
    defaultBreakList(std::move(other.defaultBreakList)),
    debugMsgLevel(other.debugMsgLevel)
{}


BaseParserStringList& BaseParserStringList::operator=(const BaseParserStringList& other)
{
    if (this != &other)
    {
        defaultBreakList = other.defaultBreakList;
        debugMsgLevel = other.debugMsgLevel;
    }
    return *this;
}

BaseParserStringList& BaseParserStringList::operator=(BaseParserStringList&& other) noexcept
{
    if (this != &other)
    {
        defaultBreakList = other.defaultBreakList;
        debugMsgLevel = other.debugMsgLevel;
    }
    return *this;
}


//============================================================================================================================
//============================================================================================================================
//  Public Functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Format the input string to the required width.  Break the string
 *        if longer than maxWidth at the nearest break charater.  Pad any
 *        output strings to the minwidth with spaces.
 *
 * @param baseString     - Input string
 * @param breakCharList  - Character list to break the string at if the input string is to be broken
 * @param maxLength      - maximum length of a string in the output list
 *
 * @return std::list<parserstr>
 */
std::list<parserstr> BaseParserStringList::formatStringToLength(parserstr baseString, std::list<parserchar> breakCharList, size_t maxLength)
{
    std::list<parserstr> returnList;
    parserstr workingString = baseString;
    size_t breakPos = parserstr::npos;

    if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug)
    {
        std::cerr << "Start string: \"" << workingString << "\"" << std::endl;
    }

    while (workingString.length() > maxLength)
    {
        size_t breakPos = findBestBreakPos(workingString, breakCharList, maxLength);
        //Found a good breaking point
        if (workingString[breakPos] <= ' ')
        {
            returnList.push_back(workingString.substr(0, breakPos));
            workingString.erase(0, breakPos+1);
        }
        else
        {
            returnList.push_back(workingString.substr(0, breakPos));
            workingString.erase(0, breakPos);
        }

        if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug)
        {
            std::cerr << "Current string: \"" << workingString << "\"" << std::endl;
        }
    }

    // Add the last tail
    returnList.push_back(workingString);

    // Pad the string to the proper length
    for (auto & unpaddedString : returnList)
    {
        while (unpaddedString.length() < maxLength)
        {
            unpaddedString += ' ';
        }
    }

    return returnList;
}

/** @} */
