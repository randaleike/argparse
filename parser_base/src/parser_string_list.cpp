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
#if defined(DYNAMIC_INTERNATIONALIZATION)
  #if defined(__linux__) || defined(__unix__)
    #include <cstdlib>
  #elif defined(_WIN64) || defined(_WIN32)
    #include <Windows.h>
  #else
    #error "Unknown OS type"
  #endif
#endif

using namespace argparser;

#if defined(ENGLISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
    #include "lang/english_strings.h"
#endif
#if defined(SPANISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
    #include "lang/spanish_strings.h"
#endif
#if defined(FRENCH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
    #include "lang/french_strings.h"
#endif
#if defined(CHINESE_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
    #include "lang/simplified_chinese_strings.h"
#endif

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Determine the message generator language and
 *        initialize msgGeneration.
 */
void BaseParserStringList::intializeMessageGenerator()
{

#if defined(DYNAMIC_INTERNATIONALIZATION)
 #if defined(__linux__) || defined(__unix__)
    const parserchar* langSetting = getenv("LANG");
    if (nullptr != langSetting)
    {
        parserstr langString = langSetting;
        if (langString == "en_US.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
        }
        else if (langString == "en_UK.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
        }
        else if (langString == "es_ES.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceSpanish>();
        }
        else if (langString == "fr_FR.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceFrench>();
        }
        else if (langString == "zh_cn_utf8.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceChineseSimplified>();
        }
        else if (langString == "zh_tw_utf8.UTF-8")
        {
            msgGeneration = std::make_shared<ParserStringListInterfaceChineseSimplified>();
        }
        /// @todo add additional else if language support above here
        else
        {
            // default to US english
            msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
        }
    }
    else
    {
        // default to US english
        msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
    }
  #elif defined(_WIN64) || defined(_WIN32)
    LANGID langId = GetUserDefaultUILanguage();
    switch(langId & 0x0FF)
    {
        case 0x04:
            msgGeneration = std::make_shared<ParserStringListInterfaceChineseSimplified>();
            break;
        case 0x09:
            msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
            break;
        case 0x0A:
            msgGeneration = std::make_shared<ParserStringListInterfaceSpanish>();
            break;
        case 0x0C:
            msgGeneration = std::make_shared<ParserStringListInterfaceFrench>();
            break;
            /// @todo add additional cases as needed
        default:
            msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
    }
  #endif
#else
  #if defined(ENGLISH_ERRORS)
    msgGeneration = std::make_shared<ParserStringListInterfaceEnglish>();
  #elif defined(SPANISH_ERRORS)
    msgGeneration = std::make_shared<ParserStringListInterfaceSpanish>();
  #elif defined(FRENCH_ERRORS)
    msgGeneration = std::make_shared<ParserStringListInterfaceFrench>();
  #elif defined(CHINESE_ERRORS)
    msgGeneration = std::make_shared<ParserStringListInterfaceChineseSimplified>();
  /// @todo add additional #elif language support above here
  #endif
#endif
}

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
    defaultBreakList({' '}), debugMsgLevel(0), msgGeneration(nullptr)
{
    intializeMessageGenerator();
}

BaseParserStringList::BaseParserStringList(const BaseParserStringList& other) :
    defaultBreakList(std::move(other.defaultBreakList)),
    debugMsgLevel(other.debugMsgLevel),
    msgGeneration(std::move(other.msgGeneration))
{
}

BaseParserStringList::BaseParserStringList(BaseParserStringList&& other) noexcept :
    defaultBreakList(std::move(other.defaultBreakList)),
    debugMsgLevel(other.debugMsgLevel),
    msgGeneration(std::move(other.msgGeneration))
{
}


BaseParserStringList& BaseParserStringList::operator=(const BaseParserStringList& other)
{
    if (this != &other)
    {
        defaultBreakList = other.defaultBreakList;
        debugMsgLevel = other.debugMsgLevel;
        msgGeneration = other.msgGeneration;
    }
    return *this;
}

BaseParserStringList& BaseParserStringList::operator=(BaseParserStringList&& other) noexcept
{
    if (this != &other)
    {
        defaultBreakList = other.defaultBreakList;
        debugMsgLevel = other.debugMsgLevel;
        msgGeneration = other.msgGeneration;
    }
    return *this;
}

BaseParserStringList::~BaseParserStringList()
{
    if (nullptr != msgGeneration)
    {
        msgGeneration.reset();
    }
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

parserstr BaseParserStringList::getNotListTypeMessage(int nargs)
{
    return msgGeneration->getNotListTypeMessage(nargs);
}

parserstr BaseParserStringList::getUnknownArgumentMessage(parserstr keyString)
{
    return msgGeneration->getUnknownArgumentMessage(keyString);
}

parserstr BaseParserStringList::getInvalidAssignmentMessage(parserstr keyString)
{
    return msgGeneration->getInvalidAssignmentMessage(keyString);
}

parserstr BaseParserStringList::getAssignmentFailedMessage(parserstr keyString, parserstr valueString)
{
    return msgGeneration->getAssignmentFailedMessage(keyString, valueString);
}

parserstr BaseParserStringList::getMissingAssignmentMessage(parserstr keyString)
{
    return msgGeneration->getMissingAssignmentMessage(keyString);
}

parserstr BaseParserStringList::getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found)
{
    return msgGeneration->getMissingListAssignmentMessage(keyString, expected, found);
}

parserstr BaseParserStringList::getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found)
{
    return msgGeneration->getTooManyAssignmentMessage(keyString, expected, found);
}

parserstr BaseParserStringList::getMissingArgumentMessage(parserstr keyString)
{
    return msgGeneration->getMissingArgumentMessage(keyString);
}

parserstr BaseParserStringList::getArgumentCreationError(parserstr keyString)
{
    return msgGeneration->getArgumentCreationError(keyString);
}

// Command line parser specific strings
parserstr BaseParserStringList::getUsageMessage() const
{
    return msgGeneration->getUsageMessage();
}

parserstr BaseParserStringList::getPositionalArgumentsMessage() const
{
    return msgGeneration->getPositionalArgumentsMessage();
}

parserstr BaseParserStringList::getSwitchArgumentsMessage() const
{
    return msgGeneration->getSwitchArgumentsMessage();
}

parserstr BaseParserStringList::getHelpString() const
{
    return msgGeneration->getHelpString();
}

// Environment parser specific strings and messages
parserstr BaseParserStringList::getEnvArgumentsMessage()
{
    return msgGeneration->getEnvArgumentsMessage();
}

parserstr BaseParserStringList::getEnvironmentNoFlags(parserstr argKey)
{
    return msgGeneration->getEnvironmentNoFlags(argKey);
}

parserstr BaseParserStringList::getRequiredEnvironmentArgMissing(parserstr argKey)
{
    return msgGeneration->getRequiredEnvironmentArgMissing(argKey);
}

// JSON parser specific strings and messages
parserstr BaseParserStringList::getJsonArgumentsMessage()
{
    return msgGeneration->getJsonArgumentsMessage();
}

// XML parser specific strings and messages
parserstr BaseParserStringList::getXmlArgumentsMessage()
{
    return msgGeneration->getXmlArgumentsMessage();
}

/** @} */
