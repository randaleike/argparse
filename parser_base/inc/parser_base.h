/* 
Copyright (c) 2022-2023 Randal Eike
 
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
#include "parser_string_list.h"

//#define DYNAMIC_INTERNATIONALIZATION
#define ENGLISH_ERRORS

namespace argparser
{

enum eAssignmentReturn
{
    eAssignSuccess = 0,             ///< Variable assignment passed
    eAssignFailed,                  ///< Variable assignment failed
    eAssignNoValue,                 ///< No value input to assignment function
    eAssignTooMany,                 ///< Too many input values
    eAssignTooFew,                  ///< Too few input values
};

enum eAddStatusReturn
{
    eAddArgSuccess = 0,             ///< Argument add passed
    eAddArgFailed,                  ///< Argument add failed
    eAddArgNotList,                 ///< Argument is not a list so arg count > 1 is not allowed
};

/**
 * @brief Positional argument list entry data structure
 */
struct ArgEntry
{
    parserstr   name;                                   ///< Argument name
    parserstr   help;                                   ///< Help description string
    parserstr   optionString;                           ///< Help option string
    varg_intf*  argData;                                ///< Argument data and storage object
    int         nargs;                                  ///< Number of argument data values expected
    int         position;                               ///< Argument position index, 0 == any
    bool        isRequired;                             ///< True if this is a required argument, else false if it is optional
    bool        isFound;                                ///< True if the argument key was found during parsing, else false if it was not
    std::list<parserstr>  keyList;                      ///< List of keys associated with the argument
};

/**
* @brief Class definition
*
* Detailed class description
*/
class parser_base
{
    protected:
        // Argument lists
        ArgEntry                nullEntry;                      ///< Return entry if argument key list is not found
        std::list<ArgEntry>     keyArgList;                     ///< List of key based arguments

        // Help page text enhancements
        size_t                  maxColumnWidth;                 ///< Maximum help message column width
        size_t                  maxOptionLength;                ///< Maximum raw option string length

        // String manipulation control parameters
        parserchar              keyListDelimeter;               ///< Add argument ket list delimeter, default = ','
        parserchar              assignmentDelimeter;            ///< Argument key, Value delimeter, default = '='
        parserchar              assignmentListDelimeter;        ///< Value list delimeter, default = ','

        // Parsing behavior control
        bool                    errorAbort;                     ///< True = Stop parsing if an error is found, False = accumulate errors until parsing complete
        int                     debugMsgLevel;                  ///< Current debug message level, default = 0 : None

        // Parse tracking data
        bool                    parsingError;                   ///< Set to true if any parsing error was detected.


        BaseParserStringList*   parserStringList;               ///< Parser string list

    public:
        /**
         * @brief Constructor
         */
        parser_base(bool abortOnError = false, int debugLevel = 0);

        /**
         * @brief Destructor
         */
        ~parser_base();

        /**
         * @brief Set the argument key list delimiter.
         * 
         * The argument key list delimeter is the parsercharacter or string 
         * the seperates the key values in the argKeys input for the
         * add argument functions.
         * 
         * @param delimeter - delimiter value, default = ','
         */
        void setKeyListDelimiter(parserchar delimeter)                  {keyListDelimeter = delimeter;}

        /**
         * @brief Set the argument assignment delimiter.  
         * 
         * The argument assignent delimiter is the string that delimits
         * an argument key value from the argument assignment value 
         * when the key and value are contained within the same command 
         * line argument
         * 
         * @param delimeter - delimiter value, default = '='
         */
        void setAssignmentDelimiter(parserchar delimeter)               {assignmentDelimeter = delimeter;}

        /**
         * @brief Set the assignment list delimiter.  
         * 
         * The assignent list delimiter is the string that delimits
         * multiple assignment input values when the argument 
         * nargs != 0 or 1
         * 
         * @param delimeter - delimiter value, default = ','
         */
        void setAssignmentListDelimiter(parserchar delimeter)           {assignmentListDelimeter = delimeter;}

        /**
         * @brief Set the Debug Message Level 
         * 
         * @param debugLevel - message level, 0 = no additional debug messages
         *                                    1 = Error debug messages
         *                                    2 = Full trace debug messages
         */
        void setDebugLevel(int debugLevel)                              {debugMsgLevel = debugLevel;}

        /**
         * @brief Format the input string to the required width.  Break the string 
         *        if longer than maxWidth at the nearest break parsercharater.  Pad any
         *        output strings to the minwidth with spaces.
         * 
         * @param baseString     - Input string
         * @param breakparsercharacter - parsercharacter to break the string at if the input string is to be broken
         * @param maxLength      - maximum length of a string in the output list
         * 
         * @return std::list<parserstr> 
         */
        std::list<parserstr> formatToLength(parserstr baseString, parserchar breakparsercharacter, size_t maxLength);

        //=================================================================================================
        //======================= Argument add interface methods ==========================================
        //=================================================================================================
        /**
         * @brief Break the input argument list into it's parts and add them to 
         * the input varg
         * 
         * @param arg          - Pointer to the argument to add the key values list
         * @param inputKeyList - Delimited list of argument data
         * 
         * @return size_t - number of arguments added to the arg
         */
        size_t addArgKeyList(ArgEntry& arg, parserstr inputKeyList);

        //=================================================================================================
        //======================= Argument parsing helper interface methods ===============================
        //=================================================================================================
        /**
         * @brief Break the input value list into it's parts
         * 
         * @param valueInput - delimited list of value data
         * @param valueList  - Reference to the current value string list
         * 
         * @return size_t - number of elements in the list
         */
        size_t getValueList(parserstr& valueString, std::list<parserstr>& valueList);

        /**
         * @brief Find the argument object that matches the input string
         * 
         * @param checkString - Input string to match
         * @param found       - Set to true if match was found, else false
         * 
         * @return ArgEntry - Reference to the ArgEntry from the list that matched if found == true
         */
        ArgEntry& findMatchingArg(const parserchar* checkString, bool& found);

        /**
         * @brief Assign the flag value to the key argument
         * 
         * @param currentArg - Pointer to the argument to set
         * 
         * @return eAssignmentReturn - Assignment return status
         */
        eAssignmentReturn assignKeyFlagValue(ArgEntry& currentArg);

        /**
         * @brief Assign a single value to an argument storage object.  
         * 
         * @param currentArg - Pointer to the argument to set
         * @param valueString - Reference to the embedded value string
         * 
         * @return eAssignmentReturn - Assignment return status
         */
        eAssignmentReturn assignKeyValue(ArgEntry& currentArg, parserstr& valueString);

        /**
         * @brief Assign multiple values to a list argument storage object.  
         * 
         * @param currentArg - Pointer to the argument to set
         * @param assignmentValues - List of assignment value strings
         * @param failedValue - Value string that failed assignment in the list
         * 
         * @return eAssignmentReturn - Assignment return status
         */
        eAssignmentReturn assignListKeyValue(ArgEntry& currentArg, std::list<parserstr>& assignmentValues, parserstr& failedValue);

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
        void displayArgHelpBlock(std::ostream &outStream, parserstr baseOptionText, parserstr baseHelpText, const size_t optionWidth, const size_t helpWidth);
};

}; // end of namespace argparser

/** @} */
