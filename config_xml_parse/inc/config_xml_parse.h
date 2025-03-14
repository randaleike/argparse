/*
 Copyright (c) 2022-2025 Randal Eike

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
 * @file config_xml_parse.h
 * @defgroup libxml_arg_parser XML config file input argument parsing utilities
 * @{
 */

// Includes
#include <cstdlib>
#include <cstdio>
#include <string>
#include <list>
#include <iostream>
#include "varg_intf.h"
#include "parser_base.h"

namespace argparser
{

/**
* @brief XML configuration file parser
*
*/
class config_xml_parse : public parser_base
{
    private:
        std::string             fileName;

    protected:

    public:
        /**
         * @brief Construct a new config xml parse object
         *
         * @param xmlFileName - XML initialization file to parse
         * @param abortOnError - True = abort parsing if an error occurs, False = ignore error and continue parsing, default = false.
         * @param debugLevel - Debug message verbosity, 0 = none, 1 = minimal, 2 = verbose, 3 = very verbose. Default = 0, none.
         */
        config_xml_parse(const char* xmlFileName="", bool abortOnError = false, int debugLevel = 0);

        /**
         * @brief Copy Constructor
         *
         * @param other - Source object for the copy
         */
        config_xml_parse(const config_xml_parse& other) = default;

        /**
         * @brief Reference Copy Constructor
         *
         * @param other - Source object for the copy
         */
        config_xml_parse(config_xml_parse&& other) = default;

        /**
         * @brief Copy Assignment Constructor
         *
         * @param other - Source object for the copy
         */
        config_xml_parse& operator=(const config_xml_parse& other);

        /**
         * @brief Reference Copy Assignment Constructor
         *
         * @param other - Source object for the copy
         */
        config_xml_parse& operator=(config_xml_parse&& other) noexcept;

        /**
         * @brief Destructor
         */
        ~config_xml_parse() = default;

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
        void addArgument(varg_intf* arg, const char* argKey, int nargs = 1, bool required = false);

        /**
         * @brief Parse the configuration file
         *
         * @return true - XML input file parsed without error.
         * @return false - An error occurred during XML input file parsing or key processing and
         *                 abort on error is set to true.
         */
        bool parse();

        /**
         * @brief Print the formatted help message to the input stream
         *
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayHelp(std::ostream &outStream = std::cerr);

        /**
         * @brief Get the File Name object
         *
         * @return std::string Filename from the constructor
         */
         std::string getFileName()   {return fileName;}
};

}; // end of namespace argparser

/** @} */
