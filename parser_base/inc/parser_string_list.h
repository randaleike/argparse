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
 * @file cmd_line_parse.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

#pragma once

// Includes
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <list>
#include <iostream>

#define DYNAMIC_INTERNATIONALIZATION
typedef std::string parserstr;          ///< Standard parser string definition
typedef char parserchar;                ///< Standard parser character definition

namespace argparser
{

/**
 * @brief Internationalized string class for the parser
 */
class BaseParserStringList
{
    protected:
        int                     debugMsgLevel;      ///< Debug message level
        std::list<parserchar>   defaultBreakList;   ///< Default list of break characters based on language.

        /**
         * @brief Find the best position to break the long string
         *
         * @param workingString - Current long working string
         * @param breakCharList - List of possible break characters to use
         * @param maxLength     - Maximum length to return
         *
         * @return size_t - Best break position <= maxLength based on the list of break characters
         */
        size_t findBestBreakPos(parserstr workingString, std::list<parserchar> breakCharList, size_t maxLength);

    public:
        BaseParserStringList();
        virtual ~BaseParserStringList()                             {}

        // Generic utility
        /**
         * @brief Set the Debug Msg Level
         *
         * @param msgLevel - Debug level, normal = 0
         */
        void setDebugMsgLevel(int msgLevel);

        /**
         * @brief Get the Internationalized Class object
         *
         * @return BaseParserStringList* - Pointer to the international language string list created.
         */
        static BaseParserStringList* getInternationalizedClass();

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
        virtual std::list<parserstr> formatStringToLength(parserstr baseString, std::list<parserchar> breakCharList, size_t maxLength);

        /**
         * @brief Get the language dependent break character list
         *
         * @return std::list<parserchar> - Language specific default break character list.
         */
        virtual std::list<parserchar> getDefaultBreakCharList()     {return defaultBreakList;}

        // Generic error messages
        virtual parserstr getNotListTypeMessage(int nargs) = 0;
        virtual parserstr getUnknownArgumentMessage(const parserchar* keyString) = 0;
        virtual parserstr getInvalidAssignmentMessage(const parserchar* keyString) = 0;
        virtual parserstr getAssignmentFailedMessage(const parserchar* keyString, parserstr valueString) = 0;
        virtual parserstr getMissingAssignmentMessage(const parserchar* keyString) = 0;
        virtual parserstr getMissingListAssignmentMessage(const parserchar* keyString, int expected, int found) = 0;
        virtual parserstr getTooManyAssignmentMessage(const parserchar* keyString, int expected, int found) = 0;
        virtual parserstr getMissingArgumentMessage(const parserchar* keyString) = 0;
        virtual parserstr getArgumentCreationError(parserstr keyString) = 0;

        // Command line parser specific strings
        virtual parserstr getUsageMessage() const = 0;
        virtual parserstr getPositionalArgumentsMessage() const = 0;
        virtual parserstr getSwitchArgumentsMessage() const = 0;
        virtual parserstr getHelpString() const = 0;

        // Environment parser specific strings and messages
        virtual parserstr getEnvArgumentsMessage() = 0;
        virtual parserstr getEnvironmentNoFlags(parserstr argKey) = 0;
};

}; // end of namespace argparser

/** @} */
