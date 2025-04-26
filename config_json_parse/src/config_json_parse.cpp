/*
 Copyright (c) 2024 Randal Eike

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
 * @file config_json_parse.cpp
 * @ingroup libjson_arg_parser
 * @{
 */

// Includes
#include <iostream>
#include <fstream>
#include "config_json_parse.h"

using namespace argparser;

/**
* @brief Construct a new config xml parse object
*
* @param xmlFileName - XML initialization file to parse
* @param abortOnError - True = abort parsing if an error occurs, False = ignore error and continue parsing, default = false.
* @param debugLevel - Debug message verbosity, 0 = none, 1 = minimal, 2 = verbose, 3 = very verbose. Default = 0, none.
*/
config_json_parse::config_json_parse(const char* jsonFileName, bool abortOnError, int debugLevel) :
    parser_base(abortOnError, debugLevel), fileName(jsonFileName)
{
    /** @todo implement code */
}

/**
* @brief Copy Assignment Constructor
*
* @param other - Source object for the copy
*/
config_json_parse& config_json_parse::operator=(const config_json_parse& other)
{
    if (this != &other)
    {
        fileName = other.fileName;
    }
    return *this;
}

/**
* @brief Reference Copy Assignment Constructor
*
* @param other - Source object for the copy
*/
config_json_parse& config_json_parse::operator=(config_json_parse&& other) noexcept
{
    if (this != &other)
    {
        fileName = other.fileName;
    }
    return *this;
}

/**
* @brief Add a new key based environment argument
*
* @param arg      - Pointer to the defined var argument to fill
* @param argKey   - JSON key value
* @param nargs    - Number of argument values that follow
*                   0 : Argument is a flag with no following values
*                   1 : Simple argument with a single value
*                   N : List argument with exactly N arguments, if less than N arguments
*                       are found it is flagged as an error
*                  -N : List argument with up to N arguments, if more than N arguments
*                       are found it is flagged as an error
* @param required - True if argument is required, false if arguemnt is optional
*/
void config_json_parse::addArgument(varg_intf* arg, const char* argKey, int nargs, bool required)
{
    // Only list type varg_intf are allowed more than 1 value
    if ((nargs != 1) && !arg->isList())
    {
        std::cerr << msgGeneration->getNotListTypeMessage(nargs) << std::endl;
    }
    else
    {
        // Add the keys to the argument
        ArgEntry newKeyArg = {};
        std::string keyString = argKey;
        newKeyArg.keyList.push_back(keyString);

        // Setup the key argument parameters
        newKeyArg.name = keyString;
        newKeyArg.help = "";
        newKeyArg.argData = arg;

        newKeyArg.nargs = nargs;
        newKeyArg.position = 0;
        newKeyArg.isRequired = required;
        newKeyArg.isFound = false;

        // Construct the key option help string
        std::string optionString = "\"";
        optionString += newKeyArg.name;
        optionString += "\":\"";
        optionString += arg->getTypeString();
        if (nargs > 1)
        {
            optionString += parser_base::getAssignmentListDelimeter();
            optionString += "...";
        }
        optionString += "\"";
        newKeyArg.optionString = optionString;

        parser_base::resizeMaxOptionLength(optionString.size());

        // Add the new argument to the list
        parser_base::addKeyArgListEntry(newKeyArg);
    }
}

/**
* @brief Parse the configuration file
*
* @return true - XML input file parsed without error.
* @return false - An error occurred during XML input file parsing or key processing and
*                 abort on error is set to true.
*/
bool config_json_parse::parse()
{
    bool parseStatus = true;
    std::ifstream parseFile(fileName);
    if (!parseFile)
    {
        std::cerr << "Failed to open json input file \"" << fileName << "\"" << std::endl;
        parseStatus = false;
    }
    else
    {
        /** @todo implement parse */
    }
    return parseStatus;
}

/**
* @brief Print the formatted help message to the input stream
*
* @param outStream - Output streem to use for text output.  Default is the standard error stream
*/
void config_json_parse::displayHelp(std::ostream &outStream)
{
    const size_t optionKeyWidth = parser_base::getOptionKeyWidth();
    const size_t helpKeyWidth = parser_base::getHelpKeyWidth(optionKeyWidth);

    if (!parser_base::isKeyArgListEmpty())
    {
        // Display the key arguments help
        outStream << msgGeneration->getJsonArgumentsMessage() << std::endl;
        for (auto const& keyArg : parser_base::getKeyArgList())
        {
            // Display the arg block
            displayArgHelpBlock(outStream, keyArg.optionString, keyArg.help, optionKeyWidth, helpKeyWidth);
        }
    }
}

/** @} */
