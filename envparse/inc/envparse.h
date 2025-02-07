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
 * @file envparse.h
 * @defgroup libenvparse Environment input argument parsing utilities
 * @{
 */

// Includes
#include <cstdlib>
#include <string>
#include <list>
#include <iostream>
#include "varg_intf.h"
#include "parser_base.h"
#if defined(_WIN64) || defined(_WIN32)
    #include <windows.h>
#endif


namespace argparser
{

const size_t bufferSize = 4096;         ///< Size of the environment read buffer if needed

/**
* @brief Class definition
*
* Detailed class description
*/
class envparser : public parser_base
{
    private:
        int     debugMsgLevel;
        #if defined(_WIN64) || defined(_WIN32)
            TCHAR envRetBuffer[bufferSize];         ///< Environment value read buffer for getEnvironmentVar() method
        #endif

    protected:
        /**
         * @brief Get the Environment Var value
         *
         * @param searchName - Name of the environment varable to find
         * @param envValue - Reference to a string to store the response in
         *
         * @return bool - true if the search name was found, else false if not
         */
        static bool getEnvironmentVar(parserstr searchName, parserstr& envValue);

    public:
        /**
         * @brief Construct a new envparser object
         *
         * @param abortOnError - True = abort parsing on the first error, False = continue parsing to the end
         *                             False = do not display the help message until parserDisplayHelp() is called
         * @param debugLevel - Debug output verbosity level, 0 = none, 5 = full
         */
        envparser(bool abortOnError = false, int debugLevel = 0): parser_base(abortOnError,debugLevel), debugMsgLevel(debugLevel)  {}

        /**
         * @brief Copy Constructor
         *
         * @param other - Source object for the copy
         */
        envparser(const envparser& other);

        /**
         * @brief Reference Copy Constructor
         *
         * @param other - Source object for the copy
         */
        envparser(envparser&& other);

        /**
         * @brief Copy Assignment Constructor
         *
         * @param other - Source object for the copy
         */
        envparser& operator=(const envparser& other);

        /**
         * @brief Reference Copy Assignment Constructor
         *
         * @param other - Source object for the copy
         */
        envparser& operator=(envparser&& other);

        /**
         * @brief Destructor
         */
        ~envparser() = default;

        /**
         * @brief Add a new key based environment argument
         *
         * @param arg      - Pointer to the defined var argument to fill
         * @param argKey   - Pointer to a character buffer containing the ket string to match when parsing the environment data
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
        void addArgument(varg_intf* arg, const char* argKey, const char* helpText, int nargs = 1, bool required = false);

        /**
         * @brief Parse the environment for matching entries
         *
         * @return true - All environment arguments were parsed correctly and
         *                all environment required arguments were found
         * @return false - An error occured parsing the input environment
         *                 arguments or one or more required arguments were
         *                 not found
         */
        bool parse();

        /**
         * @brief Print the formatted help message to the input stream
         *
         * @param outStream - Output streem to use for text output.  Default is the standard error stream
         */
        void displayHelp(std::ostream &outStream = std::cerr);
};

}; // end of namespace argparser

/** @} */
