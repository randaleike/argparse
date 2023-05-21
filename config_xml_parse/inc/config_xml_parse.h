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
 * @file config_xml_parse.h
 * @ingroup argparser
 * @defgroup Argument parsing utilities
 * @{
 */

// Includes 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <list>
#include "varg_intf.h"

namespace argparser
{

/**
* @brief XML configuration file parser
*
*/
class config_xml_parse
{
    private:
        std::list<varg_intf*>   keyArgList;
        bool                    errorAbort;
        int                     debugMsgLevel;
        std::string             fileName;

    protected:

    public:
        /**
         * @brief Constructor
         */
        config_xml_parse(char* jsonFileName, bool abortOnError = false, int debugLevel = 0, bool defaultHelpArg = true);

        /**
         * @brief Destructor
         */
        ~config_xml_parse();

        /**
         * @brief Set the Debug Message Level 
         * 
         * @param debu#include <list>
                        2 = Full trace debug messages
         */
        void setDebugLevel(int debugLevel = 0)      {debugMsgLevel = debugLevel;}

        /**
         * @brief Add a new key based environment argument
         * 
         * @param arg      - Pointer to the defined var argument to fill
         * @param argKey   - JSON key value
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
        void addArgument(varg_intf* arg, char* argKey, int nargs = 1, bool required = false);

        /**
         * @brief Parse the configuration file 
         * 
         * @return true - Key name found and keyValue parsed without error
         * @return false - (Key name not found and required) or keyValue parsed without error
         */
        bool parse();

        /**
         * @brief Print the formatted help message to the input stream
         * 
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayHelp(std::ostream outStream = std::cerr);
};

}; // end of namespace argparser

/** @} */
