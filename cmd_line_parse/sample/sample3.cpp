/*
Copyright (c) 2023 Randal Eike

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
 * @file sample3.cpp
 * @ingroup cmd_line_parse
 * @defgroup Command line parsing samples
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include "varg.h"
#include "vargincrement.h"
#include "cmd_line_parse.h"

//============================================================================================================================
//============================================================================================================================
//  Private functions
//============================================================================================================================
//============================================================================================================================


//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================
int sub_command1(int argc, char * argv[], argparser::cmd_line_parse cmdLineParser, int lastArg)
{
    argparser::vargincrement     counter;                       // Default = 0, incremented each time the argument flag is found

    // Add sub_command1 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addFlagArgument(&counter, "counter", "-c,--count", "Example of a incrementing counter argument");
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);
    if (-1 != sclastArg)
    {
        std::cout << "Counter Argument Value:    " << counter.value << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Sub Command 1 Parser failed, help displayed" << std::endl;
        return 2;
    }
}


int sub_command2(int argc, char * argv[], argparser::cmd_line_parse cmdLineParser, int lastArg)
{
    argparser::varg<int>         intArg(2);                     // Default = 2

    // Add sub_command2 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addKeyArgument(&intArg, "integerValue", "-i,--input", "Example of a basic integer value argument");
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);
    if (-1 != sclastArg)
    {
        std::cout << "Integer Argument Value:    " << intArg.value << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Sub Command 2 Parser failed, help displayed" << std::endl;
        return 3;
    }
}

int sub_command3(int argc, char * argv[], argparser::cmd_line_parse cmdLineParser, int lastArg)
{
    argparser::varg<std::string> argString("default");          // Default = "default" string

    // Add sub_command3 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addKeyArgument(&argString, "argString", "-o, --output", "Example of a switched string argument", 1, true); // make it required
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);
    if (-1 != sclastArg)
    {
        std::cout << "String Argument Value:     " << argString.value << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Sub Command 3 Parser failed, help displayed" << std::endl;
        return 4;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Main Entry point
 *
 * @param argc - Non-negative value representing the number of arguments passed to
 *               the program from the environment in which the program is run.
 * @param argv - Pointer to the first element of an array of argc + 1 pointers,
 *               of which the last one is null and the previous ones, if any,
 *               point to null-terminated multibyte strings that represent the
 *               arguments passed to the program from the execution environment.
 *               If argv[0] is not a null pointer (or, equivalently, if argc > 0),
 *               it points to a string that represents the name used to invoke the
 *               program, or to an empty string.
 *
 * @return int - 0 for pass, !0 for a failure
 */
int main(int argc, char * argv[])
{
    argparser::varg<std::string> subcommand("none");            // Default = none
    argparser::varg<bool>        flagArg(false, true);          // Default = false, set to true if command line option found
    argparser::cmd_line_parse    cmdLineParser("sample3 [subcommand] [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add the arguments to the command line parser and parse the input
    cmdLineParser.addFlagArgument(&flagArg, "version", "-V,--version", "Example of a simple true/false flag argument");
    cmdLineParser.addPositionalArgument(&subcommand, "subcommand", "Example of a positional argument as subcommand", 1, true); // sub command argument

    // Parse the input arguments and display the result
    int lastArg = cmdLineParser.parse(argc, argv, 1, 2); // stop parsing after the subcommand argument
    if (-1 != lastArg)
    {
        // Display the parsing results
        std::cout << "Number of arguments passed in: " << argc << " Number of arguments parsed: " << lastArg << std::endl;
        std::cout << "Subcommand Argument Value: " << subcommand.value << std::endl;
        if (subcommand.value == "one")
        {
            sub_command1(argc, argv, cmdLineParser, lastArg);
        }
        else if (subcommand.value == "two")
        {
            sub_command2(argc, argv, cmdLineParser, lastArg);
        }
        else if (subcommand.value == "three")
        {
            sub_command3(argc, argv, cmdLineParser, lastArg);
        }
        else
        {
            std::cout << "Unknown subcommand: \"" << subcommand.value << "\", valid values: [one|two|three]" << std::endl;
        }
        std::cout << "Flag Argument Value:       " << (flagArg.value ? "true" : "false") << std::endl;
    }
    else
    {
        std::cout << "Parser failed, help displayed" << std::endl;
    }

    return 0;
}
/*
Example 1: >sample3.exe
Expected Output:
    "subcommand" required argument missing
    Usage:
    sample3 [subcommand] [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-? show this help message
     -V,--version Example of a simple true/false flag argument

    Positional Arguments:
    subcommand    Example of a positional argument as subcommand

    Parser Failed, help displayed

Example 2: >sample3.exe one
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value: one
    Counter Argument Value:    0
    Flag Argument Value:       false

Example 3: >sample3.exe one -c
Expected Output:
    Number of arguments passed in: 3 Number of arguments parsed: 3
    Subcommand Argument Value: one
    Counter Argument Value:    1
    Flag Argument Value:       false

Example 4: >sample3.exe two
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value: two
    Integer Argument Value:    2
    Flag Argument Value:       false

Example 5: >sample3.exe two -i 7
Expected Output:
    Number of arguments passed in: 4 Number of arguments parsed: 2
    Subcommand Argument Value: two
    Integer Argument Value:    7
    Flag Argument Value:       false

Example 6: >sample3.exe three
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value: three
    "-o|--output" required argument missing

    Usage:
    sample3 [subcommand] [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-?          show this help message
     -V,--version          Example of a simple true/false flag argument
     -o,--output=argString Example of a switched string argument

    Positional Arguments:
    subcommand             Example of a positional argument as subcommand

    Sub Command 3 Parser Failed, help displayed
    Flag Argument Value:       false

Example 7: >sample3.exe three -o foo
Expected Output:
    Number of arguments passed in: 4 Number of arguments parsed: 2
    Subcommand Argument Value: three
    String Argument Value:     foo
    Flag Argument Value:       false

Example 8: >sample3.exe three -V -o foo
Expected Output:
    Number of arguments passed in: 5 Number of arguments parsed: 2
    Subcommand Argument Value: three
    String Argument Value:     foo
    Flag Argument Value:       true

*/

/** @} */
