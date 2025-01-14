/*
Copyright (c) 2023-2025 Randal Eike

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
 * @file sample6.cpp
 * @ingroup libcmd_line_parser
 * @{
 */

// Includes
#include <stdio.h>                                          // NOLINT
#include "../varg/inc_clib/varg_clib.h"
#include "../cmd_line_parse/inc_clib/cmd_line_parse_api.h"

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


//============================================================================================================================
//============================================================================================================================
//  Public functions
//============================================================================================================================
//============================================================================================================================
// NOLINTBEGIN
/**
 * @brief Main Entry point
 *
 * Example of simple command line parser
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
    char positionalArg[100] = {'\0'};                                                   // Default = empty string
    char argString[100] = {'d', 'e', 'f', 'a', 'u', 'l', 't'};  // Default = "default" string
    int  intArg = 2;                        // Default = 2
    bool flagArg = false;                   // Default = false, set to true if command line option found
    int  counter = 0;                       // Default = 0, incremented each time the argument flag is found

    argHandle posArgHandle    = createStringArrayParserArg(positionalArg, 100);
    argHandle stringArgHandle = createStringArrayParserArg(argString, 100);
    argHandle intArgHandle    = createIntParserArg(&intArg);
    argHandle flagArgHandle   = createParserFlagArg(&flagArg, true);
    argHandle counterHandle   = createIncrementingArg(&counter);

    cmdLineParserHandle cmdLineParser = getParser("sample6 [switched options ] [positional arg]",
                                                  "Description of the utility to be included in the help display",
                                                  true, false, 0);

    // Add the arguments to the command line parser and parse the input
    addFlagArgument(cmdLineParser, flagArgHandle, "version", "-f,--flag", "Example of a simple true/false flag argument", false);
    addIncrementingArgument(cmdLineParser, counterHandle, "counter", "-c,--count", "Example of a incrementing counter argument", false);
    addKeyArgument(cmdLineParser, intArgHandle, "integerValue", "-i,--input", "Example of a basic integer value argument", false);
    addKeyArgument(cmdLineParser, stringArgHandle, "argString", "-o, --output", "Example of a switched string argument", false);
    addPositionalArgument(cmdLineParser, posArgHandle, "positionalString", "Example of a string argument as positional", false);

    // Parse the input arguments and display the result
    int lastArg = parse(cmdLineParser, argc, argv, 1, -1);
    if (-1 != lastArg)
    {
        // Display the parsing results
        printf("Number of arguments passed in: %d Number of arguments parsed: %d\n", argc, lastArg);
        printf("Positional Argument Value: %s\n", positionalArg);
        printf("Integer Argument Value:    %d\n", intArg);
        printf("Counter Argument Value:    %d\n", counter);
        printf("String Argument Value:     %s\n", argString);
        printf("Flag Argument Value:       %s\n", (flagArg ? "true" : "false"));
    }
    else
    {
        printf("Parser failed, help displayed\n");
    }

    return 0;
}
// NOLINTEND
/*
Example 1: >sample6.exe
Expected Output:
    Number of arguments passed in: 1 Number of arguments parsed: 1
    Positional Argument Value:
    Integer Argument Value:    2
    Counter Argument Value:    0
    String Argument Value:     default
    Flag Argument Value:       false

Example 2: >sample6.exe -f -c -i 8 -o bar foo
Expected Output:
    Number of arguments passed in: 8 Number of arguments parsed: 8
    Positional Argument Value: foo
    Integer Argument Value:    8
    Counter Argument Value:    1
    String Argument Value:     bar
    Flag Argument Value:       true

Example 3: >sample6.exe -fci 7 -o bar foo
Expected Output:
    Number of arguments passed in: 6 Number of arguments parsed: 6
    Positional Argument Value: foo
    Integer Argument Value:    7
    Counter Argument Value:    1
    String Argument Value:     bar
    Flag Argument Value:       true

Example 4: >sample6.exe --flag -c -c --count --input 6 --output bar foo
Expected Output:
    Number of arguments passed in: 10 Number of arguments parsed: 10
    Positional Argument Value: foo
    Integer Argument Value:    6
    Counter Argument Value:    3
    String Argument Value:     bar
    Flag Argument Value:       true

Example 5: >sample6.exe -fcccc -o bar foo
Expected Output:
    Number of arguments passed in: 5 Number of arguments parsed: 5
    Positional Argument Value: foo
    Integer Argument Value:    2
    Counter Argument Value:    4
    String Argument Value:     bar
    Flag Argument Value:       true
*/

/** @} */
