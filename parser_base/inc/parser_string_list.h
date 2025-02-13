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
 * @file cmd_line_parse.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once

// Includes
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <string>
#include <list>
#include <iostream>

#define DYNAMIC_INTERNATIONALIZATION    1
using parserstr = std::string;          ///< Standard parser string definition
using parserchar = char;                ///< Standard parser character definition

/**
 * @brief Debug messaging verbosity levels
 *
 */
enum debugVerbosityLevel_e
{
    noDebugMsg = 0,                     ///< Quiet, no debug messages
    minimalDebug,                       ///< Minimal debug message verbosity
    verboseDebug,                       ///< Medium debug message verbosity
    veryVerboseDebug                    ///< Maximum debug message verbosity
};

namespace argparser
{

/**
 * @brief Parser error/help string generation interface
 */
class ParserStringListInterface
{
    public:
        /**
         * @brief Construct a new Parser String List Interface object
         */
        ParserStringListInterface() = default;

        /**
         * @brief Copy constructor of a new Parser String List Interface object
         *
         * @param other - Reference of object to copy
         */
        ParserStringListInterface(const ParserStringListInterface& other) = default;

        /**
         * @brief Copy constructor of a new Parser String List Interface object
         *
         * @param other - Reference of reference of object to copy
         */
        ParserStringListInterface(ParserStringListInterface&& other) = default;

        /**
         * @brief Equate constructor of a new Parser String List Interface object
         *
         * @param other - Reference of object to copy
         */
        ParserStringListInterface& operator=(const ParserStringListInterface& other) = default;

        /**
         * @brief Equate constructor of a new Parser String List Interface object
         *
         * @param other - Reference of of reference of object to copy
         */
        ParserStringListInterface& operator=(ParserStringListInterface&& other) = default;

        /**
         * @brief Destroy the Parser String List Interface object
         */
        virtual ~ParserStringListInterface() = default;

        // General argument parsing messages
        [[nodiscard]] virtual parserstr getNotListTypeMessage(int nargs) = 0;
        [[nodiscard]] virtual parserstr getUnknownArgumentMessage(parserstr keyString)  = 0;
        [[nodiscard]] virtual parserstr getInvalidAssignmentMessage(parserstr keyString)  = 0;
        [[nodiscard]] virtual parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString)  = 0;
        [[nodiscard]] virtual parserstr getMissingAssignmentMessage(parserstr keyString)  = 0;
        [[nodiscard]] virtual parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found)  = 0;
        [[nodiscard]] virtual parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found)  = 0;
        [[nodiscard]] virtual parserstr getMissingArgumentMessage(parserstr keyString)  = 0;
        [[nodiscard]] virtual parserstr getArgumentCreationError(parserstr keyString)  = 0;
        [[nodiscard]] virtual parserstr getUsageMessage() const  = 0;

        // Command Line parser messages
        [[nodiscard]] virtual parserstr getPositionalArgumentsMessage() const  = 0;
        [[nodiscard]] virtual parserstr getSwitchArgumentsMessage() const  = 0;
        [[nodiscard]] virtual parserstr getHelpString() const  = 0;

        // Environment parser messages
        [[nodiscard]] virtual parserstr getEnvArgumentsMessage()  = 0;
        [[nodiscard]] virtual parserstr getEnvironmentNoFlags(parserstr argKey)  = 0;

        // JSON file parser messages
        [[nodiscard]] virtual parserstr getJsonArgumentsMessage()  = 0;

        // XML file parser messages
        [[nodiscard]] virtual parserstr getXmlArgumentsMessage()  = 0;
};

/**
 * @brief Internationalized string class for the parser
 */
class BaseParserStringList
{
    private:
        int                     debugMsgLevel;      ///< Debug message level
        std::list<parserchar>   defaultBreakList;   ///< Default list of break characters based on language.
        ParserStringListInterface* msgGeneration;   ///< Language specific message generation object.

        /**
         * @brief Determine the message generator language and
         *        initialize msgGeneration.
         */
        void intializeMessageGenerator();

    protected:
        /**
         * @brief Find the best position to break the long string
         *
         * @param workingString - Current long working string
         * @param breakCharList - List of possible break characters to use
         * @param maxLength     - Maximum length to return
         *
         * @return size_t - Best break position <= maxLength based on the list of break characters
         */
        [[nodiscard]] size_t findBestBreakPos(parserstr workingString, std::list<parserchar> breakCharList, size_t maxLength) const;

    public:
        BaseParserStringList();
        BaseParserStringList(const BaseParserStringList& other);
        BaseParserStringList(BaseParserStringList&& other) noexcept;
        BaseParserStringList& operator=(const BaseParserStringList& other);
        BaseParserStringList& operator=(BaseParserStringList&& other) noexcept;
        ~BaseParserStringList();

        // Generic utility
        /**
         * @brief Set the Debug Msg Level
         *
         * @param msgLevel - Debug level, normal = 0
         */
        void setDebugMsgLevel(int msgLevel)         {debugMsgLevel = msgLevel;}

        /**
         * @brief Break the long string into multiple strings and return the sub strings as a list. The long string
         *        will be broken at the break character in the input break character list which is closest to the
         *        max length input.  If no suitable break position is found then the string is broken at the input
         *        max length position.  The resulting list of strings will be padded with spaces to the maxlength
         *        size so that all stringa are the same length.
         *
         * @param baseString    - Long string to break up
         * @param breakCharList - List of potential break characters
         * @param maxLength     - Maximum length of an entry in the return string list
         *
         * @return std::list<parserstr> - List of strings all equal to maxLength in size
         */
        std::list<parserstr> formatStringToLength(parserstr baseString, std::list<parserchar> breakCharList, size_t maxLength);

        /**
         * @brief Get the language dependent break character list
         *
         * @return std::list<parserchar> - Language specific default break character list.
         */
        std::list<parserchar> getDefaultBreakCharList()     {return defaultBreakList;}

        // Generic error messages
        parserstr getNotListTypeMessage(int nargs);
        parserstr getUnknownArgumentMessage(parserstr keyString);
        parserstr getInvalidAssignmentMessage(parserstr keyString);
        parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString);
        parserstr getMissingAssignmentMessage(parserstr keyString);
        parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found);
        parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found);
        parserstr getMissingArgumentMessage(parserstr keyString);
        parserstr getArgumentCreationError(parserstr keyString);

        // Command line parser specific strings
        [[nodiscard]] parserstr getUsageMessage() const;
        [[nodiscard]] parserstr getPositionalArgumentsMessage() const;
        [[nodiscard]] parserstr getSwitchArgumentsMessage() const;
        [[nodiscard]] parserstr getHelpString() const;

        // Environment parser specific strings and messages
        parserstr getEnvArgumentsMessage();
        parserstr getEnvironmentNoFlags(parserstr argKey);

        // JSON parser specific strings and messages
        parserstr getJsonArgumentsMessage();

        // XML parser specific strings and messages
        parserstr getXmlArgumentsMessage();
};

}; // end of namespace argparser

/** @} */
