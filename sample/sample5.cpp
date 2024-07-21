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
 * @file sample5.cpp
 * @ingroup libcmd_line_parser
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include "varg.h"
#include "vargincrement.h"
#include "vargenum.h"
#include "cmd_line_parse.h"

enum subcommand_e
{
    subcmd_invalid,
    subcmd_read,
    subcmd_write,
    subcms_append
};

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
int sub_command_read(unsigned lineCount, std::string filename)
{
    std::cout << "Read " << lineCount << " lines from file \"" << filename << "\"" << std::endl;
    return 0;
}


int sub_command_write(unsigned lineCount, std::string filename)
{
    std::cout << "Write " << lineCount << " lines to file \"" << filename << "\"" << std::endl;
    return 0;
}

int sub_command_append(unsigned lineCount, std::string filename)
{
    std::cout << "Append " << lineCount << " lines to file \"" << filename << "\"" << std::endl;
    return 0;
}

//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================
/**
 * @brief Main Entry point
 *
 * Example of parsing until up through the first command line argument using an enum variable.
 * Then parsing the rest of the command line based on the value of the first arguement.
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
    argparser::vargenum<subcommand_e>  subcommand(subcommand_e::subcmd_invalid, "subcmd");            // Default = invalid subcommand

    // Add command line key names for the enum value
    subcommand.setEnumValue("read", subcommand_e::subcmd_read);
    subcommand.setEnumValue("write", subcommand_e::subcmd_write);
    subcommand.setEnumValue("append", subcommand_e::subcms_append);

    argparser::varg<unsigned>          linecount(0);   // Default = 0
    argparser::varg<std::string>       filename("default.txt");   // Default = default.txt

    argparser::cmd_line_parse   cmdLineParser("sample5 [subcommand] [subcommand options]",
                                              "Description of the utility to be included in the help display",
                                              true);

    // Add the arguments to the command line parser and parse the input
    cmdLineParser.addPositionalArgument(&subcommand, "subcommand", "Example of a positional enum argument as subcommand", 1, true); // sub command argument
    cmdLineParser.addKeyArgument(&linecount, "linecount", "-l,--lines", "Example of an integer argument");
    cmdLineParser.addKeyArgument(&filename, "filename", "-f,--file", "Example of a string argument");

    // Parse the input arguments and display the result
    int lastArg = cmdLineParser.parse(argc, argv); // stop parsing after the subcommand argument
    if (-1 != lastArg)
    {
        // Display the parsing results
        std::cout << "Number of arguments passed in: " << argc << " Number of arguments parsed: " << lastArg << std::endl;
        std::cout << "Subcommand Argument Value: " << subcommand.value << std::endl;
        std::cout << "Line Count Argument Value: " << linecount.value << std::endl;
        std::cout << "File Name Argument Value:  " << filename.value << std::endl;
        switch(subcommand.value)
        {
            case subcommand_e::subcmd_invalid:
                std::cout << "Unknown subcommand: \"" << argv[1] << "\", valid values: [read|write|append]" << std::endl;
                break;

            case subcommand_e::subcmd_read:
                sub_command_read(linecount.value, filename.value);
                break;
            case subcommand_e::subcmd_write:
                sub_command_write(linecount.value, filename.value);
                break;
            case subcommand_e::subcms_append:
                sub_command_append(linecount.value, filename.value);
                break;
        }
    }
    else
    {
        std::cout << "Parser failed, help displayed" << std::endl;
    }

    return 0;
}
/*
Example 1: >sample5.exe
Expected Output:
    "subcommand" required argument missing
    Usage:
    sample5 [subcommand] [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-?         show this help message
     -l,--lines=linecount Example of an integer argument
     -f,--file=filename   Example of an integer argument

    Positional Arguments:
    subcommand            Example of a positional enum argument as subcommand

    Parser Failed, help displayed

Example 2: >sample5.exe read
Expected Output:
    Number of arguments passed in: 2 Number of arguments parsed: 2
    Subcommand Argument Value: 1
    Line Count Argument Value: 0
    File Name Argument Value:  default.txt
    Read 0 lines from file "default.txt"

Example 3: >sample5.exe read -l 4 -f foo.c
Expected Output:
    Number of arguments passed in: 6 Number of arguments parsed: 6
    Subcommand Argument Value: 1
    Line Count Argument Value: 4
    File Name Argument Value:  foo.c
    Read 4 lines from file "foo.c"

Example 4: >sample5.exe write -l 4 -f foo.c
Expected Output:
    Number of arguments passed in: 6 Number of arguments parsed: 6
    Subcommand Argument Value: 2
    Line Count Argument Value: 4
    File Name Argument Value:  foo.c
    Write 4 lines to file "foo.c"

Example 5: >sample5.exe append -l 4 -f foo.c
Expected Output:
    Number of arguments passed in: 6 Number of arguments parsed: 6
    Subcommand Argument Value: 3
    Line Count Argument Value: 4
    File Name Argument Value:  foo.c
    Append 4 lines to file "foo.c"

Example 6: >sample5.exe copy -l 4 -f foo.c
Expected Output:
    "subcommand copy" assignment failed
    Usage:
    sample5 [subcommand] [subcommand options]

    Description of the utility to be included in the help display

    Optional Arguments:
     -h,--help,-?         show this help message
     -l,--lines=linecount Example of an integer argument
     -f,--file=filename   Example of an integer argument

    Positional Arguments:
    subcommand            Example of a positional enum argument as subcommand

    Parser Failed, help displayed

*/

/** @} */
