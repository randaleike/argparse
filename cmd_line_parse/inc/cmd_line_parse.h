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
#include "varg_intf.h"
#include "parser_base.h"

namespace argparser
{

/**
* @brief Class definition
*
* Detailed class description
*/
class cmd_line_parse : public parser_base
{
    private:
        // Argument lists
        std::list<ArgEntry>     positionalArgList;              ///< List of positional arguments

        // Help page text enhancements
        parserstr               programName;                    ///< Program name to display in the usage %(prog) field of the help message, default = argv[0]
        parserstr               usageText;                      ///< Text to display in the usage field of the help message, default = "%(prog) [options]"
        parserstr               descriptionText;                ///< Text to display in the description field of the help message, default = ""
        parserstr               epilogText;                     ///< Text to display at the end of the help message, default = ""

        // String manipulation control parameters
        parserstr               keyPrefix;                      ///< Key value prefix, default = '-'

        // Parsing behavior control
        bool                    displayHelpOnError;             ///< True = display the help message by default on a parsing error, False = do not display help message on a parsing error
        bool                    enableDefaultHelp;              ///< True = add default help argument to the key arg list, false = do not add default help argument
        bool                    ignoreUnknownKey;               ///< True = unknown key is not an error, false = unknown key is an error, default = false
        bool                    singleCharArgListAllowed;       ///< True = single character key list allowed, false = all key arguments must be separarted, default = true

        // Parse tracking data
        int                     positionNumber;                 ///< Position list argument number
        int                     parseingPositionNumber;         ///< Parseing position argument number
        int                     currentArgumentIndex;           ///< current argv array index being processed
        int                     argcount;                       ///< argc value from the parse call
        char**                  argvArray;                      ///< argv value from the parse call
        int                     debugMsgLevel;                  ///< debugging message level
        parserstr               positionalStop;                 ///< Name of the positional argument to stop parsing on
        bool                    positionalStopArgumentFound;    ///< Flag to abort processing

        /**
         * @brief Test if the current command line argument is a key switch
         *
         * @return true  - Next command line argument is a key switch
         * @return false - Next argument is a value
         */
        bool isCurrentArgKeySwitch();

        /**
         * @brief Add the default help argument to the argument list
         */
        void addDefaultHelpArgument();

        /**
         * @brief Find the argument object that matches the input string
         *
         * @param keystring   - Input string to match
         * @param found       - Set to true if match was found, else false
         *
         * @return ArgEntry - Reference to the ArgEntry from the ArgEntry if match was found. Or nullptr if not.
         */
        ArgEntry& findMatchingArg(const char* keystring, bool& found);

        /**
         * @brief Get the Initial Value List object
         *
         * @param valueInput - delimited list of value data
         * @param valueList  - Reference to the current value string list
         *
         * @return size_t - number of elements in the list
         */
        size_t getInitialValueList(parserstr& valueString, std::list<parserstr>& valueList);
        size_t getInitialValueList(const char* valueString, std::list<parserstr>& valueList);

        /**
         * @brief Assign the flag value to the key argument
         *
         * @param currentArg - Pointer to the argument to set
         * @param keyString  - Key value that was matched
         * @param valueString - Reference to the embedded value string if present, else empty string
         *
         * @return bool - False = assignment worked, true = assignment failed
         */
        bool assignKeyFlagValue(ArgEntry& currentArg, const char* keyString, parserstr& valueString);

        /**
         * @brief Assign multiple values to a list argument storeage.
         *
         * @param currentArg - Pointer to the argument to set
         * @param keyString  - Key value that was matched
         * @param valueString - Reference to the embedded value string if present, else empty string
         *
         * @return bool - False = assignment worked, true = assignment failed
         */
        bool assignKeyValue(ArgEntry& currentArg, const char* keyString, parserstr& valueString);

        /**
         * @brief Parse a single key argument
         *
         * @param searchString - Key argument value to find and set
         * @param valueString - Value string from the argument or empty string
         */
        void parseSingleKeyArg(const char* searchString, parserstr valueString);

    protected:

        /**
         * @brief Parse the input key argument type and update the parsing state data
         */
        void parseKeyArg();

        /**
         * @brief Parse the input positional argument type and update the parsing state data
         */
        void parsePositionalArg();

    public:
        /**
         * @brief Constructor
         */
        cmd_line_parse(parserstr usage = "%(prog) [options]", parserstr description = "", bool abortOnError = false, bool disableDefaultHelp = false, int debugLevel = 0);

        /**
         * @brief Destructor
         */
        ~cmd_line_parse();

        //=================================================================================================
        //======================= Parser setup interface methods ==========================================
        //=================================================================================================
        /**
         * @brief Set the Epilog text
         *
         * @param epilog - Text to display at the end of the help block
         */
        void setEpilog(parserstr epilog)                                {epilogText = epilog;}

        /**
         * @brief Set the Program Name for the usage string
         *
         * @param progName - Program name to use in the usage string
         */
        void setProgramName(parserstr progName)                         {programName = progName;}
        void setProgramName(char* progName)                             {programName = progName;}

        /**
         * @brief Set the argument key prefix value.
         *
         * The argument key prefix is the character or string
         * the identifies an input argument key string.  Any input
         * argument that does not begin with this character is
         * assumed to be a positional argument value.
         *
         * @param prefix - argument prefix value
         */
        void setKeyPrefix(parserstr prefix)                             {keyPrefix = prefix;}
        void setKeyPrefix(char* prefix)                                 {keyPrefix = prefix;}

        /**
         * @brief Disable the default help argument setup
         */
        void disableDefaultHelpArgument()                               {enableDefaultHelp = false;}

        /**
         * @brief Disable the help display on parsing error
         */
        void disableHelpDisplayOnError()                                {displayHelpOnError = false;}

        /**
         * @brief Disable the single character list input argument parsing
         */
        void disableSingleCharListArgument()                            {singleCharArgListAllowed = false;}

        /**
         * @brief Enable flag to ignore unknown arguments, default
         *        is to generate an error if an unknown argument
         *        is found
         */
        void enableUnknowArgumentIgnore()                               {ignoreUnknownKey = true;}

        //=================================================================================================
        //======================= Argument add interface methods ==========================================
        //=================================================================================================
        /**
         * @brief Add a new key based command line argument to the argument list
         *
         * @param arg      - Pointer to the defined var argument to fill
         * @param name     - Human name used in the help messages
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
        void addKeyArgument(varg_intf* arg, parserstr name, parserstr argKeys, parserstr helpText, int nargs = 1, bool required = false);

        /**
         * @brief Add a new key based flag command line argument to the argument list
         *
         * @param arg      - Pointer to the defined var argument to fill
         * @param name     - Human name used in the help messages
         * @param argKeys  - Delimieted list of argument key values
         * @param helpText - Help text to be printed in the help message
         * @param required - True if argument is required, false if arguemnt is optional
         */
        void addFlagArgument(varg_intf* arg, parserstr name, parserstr argKeys, parserstr helpText, bool required = false);

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
        void addPositionalArgument(varg_intf* arg, parserstr name, parserstr helpText, int nargs = 1, bool required = false);

        //=================================================================================================
        //======================= Commandline parser interface methods ====================================
        //=================================================================================================
        /**
         * @brief Set the name of the positional argument to stop parsing
         *
         * @param positionalArgumentName - Positional name to find
         */
        void setPositionalNameStop(const char* positionalArgumentName);

        /**
         * @brief Parse the input command line arguments
         *
         * @param argc - Number of char pointers in the argv[] array
         * @param argv - Array of command line text entries
         * @param startingArgIndex - argv index to start parsing arguments
         * @param endingArgIndex - argv index to stop parsing arguments, -1 == argc
         *
         * @return int - Index of the last argument parsed or -1 if an error occured
         */
        int parse(int argc, char* argv[], int startingArgIndex = 1, int endingArgIndex = -1);

        /**
         * @brief Print the formatted option help message to the input stream
         *
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayOptionHelp(std::ostream &outStream = std::cerr);

        /**
         * @brief Print the formatted environment help message to the input stream
         *
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayPositionHelp(std::ostream &outStream = std::cerr);

        /**
         * @brief Print the formatted help message to the input stream
         *
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayHelp(std::ostream &outStream = std::cerr);
};

}; // end of namespace argparser

/** @} */
