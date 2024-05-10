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
 * @file sample4.cpp
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
int sub_command1(int argc, char * argv[], int lastArg)
{
    argparser::vargincrement     counter;                       // Default = 0, incremented each time the argument flag is found
    argparser::cmd_line_parse    cmdLineParser("sample4 [global options] one [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add sub_command1 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addFlagArgument(&counter, "counter", "-c,--count", "Example of a incrementing counter argument");
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);

    if (-1 != sclastArg)
    {
        std::cout << "Number of arguments passed in: " << argc << std::endl;
        std::cout << "Number of subcommand 1 arguments parsed: " << sclastArg - lastArg <<  " Starting at argument: " << lastArg << std::endl;
        std::cout << "Counter Argument Value:    " << counter.value << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Sub Command 1 Parser failed, help displayed" << std::endl;
        return 2;
    }
}


int sub_command2(int argc, char * argv[], int lastArg)
{
    argparser::varg<int>         intArg(2);                     // Default = 2
    argparser::cmd_line_parse    cmdLineParser("sample4 [global options] two [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add sub_command2 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addKeyArgument(&intArg, "integerValue", "-i,--input", "Example of a basic integer value argument");
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);
    if (-1 != sclastArg)
    {
        std::cout << "Number of arguments passed in: " << argc << std::endl;
        std::cout << "Number of subcommand 2 arguments parsed: " << sclastArg - lastArg <<  " Starting at argument: " << lastArg << std::endl;
        std::cout << "Integer Argument Value:    " << intArg.value << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Sub Command 2 Parser failed, help displayed" << std::endl;
        return 3;
    }
}

int sub_command3(int argc, char * argv[], int lastArg)
{
    argparser::varg<std::string> argString("default");          // Default = "default" string
    argparser::cmd_line_parse    cmdLineParser("sample4 [global options] three [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add sub_command3 arguments and parse starting at where we left off which should be arg 2
    cmdLineParser.addKeyArgument(&argString, "argString", "-o, --output", "Example of a switched string argument", 1, true); // make it required
    int sclastArg = cmdLineParser.parse(argc, argv, lastArg);
    if (-1 != sclastArg)
    {
        std::cout << "Number of arguments passed in: " << argc << std::endl;
        std::cout << "Number of subcommand 3 arguments parsed: " << sclastArg - lastArg <<  " Starting at argument: " << lastArg << std::endl;
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
 * Example of parsing until a specified argument is found.  The parsing the rest of the command line
 * based on the found arguement value
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
    argparser::varg<int>         intArg(0);                     // Default = 0, set to value if command line option found
    argparser::cmd_line_parse    cmdLineParser("sample4 [global options] [subcommand] [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add the arguments to the command line parser and parse the input
    cmdLineParser.addFlagArgument(&flagArg, "globalFlag", "-U,--global", "Example of a simple true/false flag argument");
    cmdLineParser.addKeyArgument(&intArg, "globalInt", "-i,--integer", "Example of a simple integer argument");
    cmdLineParser.addPositionalArgument(&subcommand, "subcommand", "Example of a positional argument as subcommand after options", 1, true); // sub command argument

    // Parse the input arguments and display the result
    cmdLineParser.setPositionalNameStop("subcommand");
    int lastArg = cmdLineParser.parse(argc, argv);
    if (-1 != lastArg)    
    {
        // Display the parsing results
        std::cout << "Number of arguments passed in: " << argc << " Number of global arguments parsed: " << lastArg << std::endl;
        std::cout << "Subcommand Argument Value:     " << subcommand.value << std::endl;
        std::cout << "Global Flag Argument Value:    " << (flagArg.value ? "true" : "false") << std::endl;
        std::cout << "Global Integer Argument Value: " << intArg.value << std::endl << std::endl;
        if (subcommand.value == "one")
        {
            sub_command1(argc, argv, lastArg);
        }
        else if (subcommand.value == "two")
        {
            sub_command2(argc, argv, lastArg);
        }
        else if (subcommand.value == "three")
        {
            sub_command3(argc, argv, lastArg);
        }
        else
        {
            std::cout << "Unknown subcommand: \"" << subcommand.value << "\", valid values: [one|two|three]" << std::endl;
        }
    }
    else
    {
        std::cout << "Parser failed, help displayed" << std::endl;
    }

    return 0;
}
/*
Example 1: >sample4.exe
Expected Output:
    "subcommand" required argument missing
    Usage:
    sample4 [global options] [subcommand] [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-? show this help message
     -U,--global  Example of a simple true/false flag argument
     -i,--integer Example of a simple integer argument

    Positional Arguments:
    subcommand    Example of a positional argument as subcommand after
                  options

    Parser Failed, help displayed

Example 2: >sample4.exe one
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value:     one
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0
    
    Number of arguments passed in: 2
    Number of subcommand 1 arguments parsed: 0 Starting at argument: 2
    Counter Argument Value:    0

Example 3: >sample4.exe -Ui 4 one
Expected Output:
    Number of arguments passed in: 4 Number of arguments parsed: 4
    Subcommand Argument Value:     one
    Global Flag Argument Value:    true
    Global Integer Argument Value: 4

    Number of arguments passed in: 4
    Number of subcommand 1 arguments parsed: 0 Starting at argument: 4
    Counter Argument Value:    0

Example 4: >sample4.exe -Ui 4 one -c
Expected Output:
    Number of arguments passed in: 5 Number of arguments parsed: 4
    Subcommand Argument Value:     one
    Global Flag Argument Value:    true
    Global Integer Argument Value: 4

    Number of arguments passed in: 5
    Number of subcommand 1 arguments parsed: 1 Starting at argument: 4
    Counter Argument Value:    1

Example 5: >sample4.exe one -ccc
Expected Output:
    Number of arguments passed in: 3 Number of arguments parsed: 2
    Subcommand Argument Value:     one
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0

    Number of arguments passed in: 3
    Number of subcommand 1 arguments parsed: 1 Starting at argument: 2
    Counter Argument Value:    3

Example 6: >sample4.exe two
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value:     two
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0

    Number of arguments passed in: 2
    Number of subcommand 2 arguments parsed: 0 Starting at argument: 2
    Integer Argument Value:    2

Example 7: >sample4.exe -Ui=3 two
Expected Output:
    Number of arguments passed in: 3 Number of arguments parsed: 3
    Subcommand Argument Value:     two
    Global Flag Argument Value:    true
    Global Integer Argument Value: 3

    Number of arguments passed in: 3
    Number of subcommand 2 arguments parsed: 0 Starting at argument: 3
    Integer Argument Value:    2

Example 8: >sample4.exe -Ui=3 two -i 8
Expected Output:
    Number of arguments passed in: 5 Number of arguments parsed: 3
    Subcommand Argument Value:     two
    Global Flag Argument Value:    true
    Global Integer Argument Value: 3

    Number of arguments passed in: 5
    Number of subcommand 2 arguments parsed: 1 Starting at argument: 3
    Integer Argument Value:    8

Example 9: >sample4.exe two -i 10
Expected Output:
    Number of arguments passed in: 4 Number of arguments parsed: 2
    Subcommand Argument Value:     two
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0

    Number of arguments passed in: 4
    Number of subcommand 2 arguments parsed: 1 Starting at argument: 3
    Integer Argument Value:    10

Example 10: >sample4.exe three
Expected Output:
    Number of arguments passed in: 2 Number of global arguments parsed: 2
    Subcommand Argument Value:     three
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0

    "-o|--output" required argument missing
    Usage:
    sample4 [global options] three [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-?           show this help message and exit                         
     -o, --output=argString Example of a switched string argument                   

    Sub Command 3 Parser failed, help displayed
    
Example 11: >sample4.exe three -o foo
Expected Output:
    Number of arguments passed in: 4 Number of arguments parsed: 2
    Subcommand Argument Value:     three
    Global Flag Argument Value:    false
    Global Integer Argument Value: 0

    Number of arguments passed in: 4
    Number of subcommand 3 arguments parsed: 1 Starting at argument: 2
    String Argument Value:     foo

Example 12: >sample4.exe -U three -o goo
Expected Output:
    Number of arguments passed in: 5 Number of arguments parsed: 3
    Subcommand Argument Value:     three
    Global Flag Argument Value:    true
    Global Integer Argument Value: 0

    Number of arguments passed in: 5
    Number of subcommand 3 arguments parsed: 1 Starting at argument: 3
    String Argument Value:     goo

Example 13: >sample4.exe -U -i 6 three -o moo
Expected Output:
    Number of arguments passed in: 7 Number of arguments parsed: 5
    Subcommand Argument Value:     three
    Global Flag Argument Value:    true
    Global Integer Argument Value: 6

    Number of arguments passed in: 7
    Number of subcommand 3 arguments parsed: 1 Starting at argument: 5
    String Argument Value:     moo

*/

/** @} */
