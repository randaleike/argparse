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
 * @file sample_unittest.cpp
 * @defgroup sample_code_unittest Command line parsing utilities sample code unit test
 * @ingroup sample_code_unittest
 * @{
 */

// Includes
#include <cstdlib>
#include <fstream>
#include <gtest/internal/gtest-port.h>
#include <iostream>
#include <string>
#include <cstddef>
#include <gtest/gtest.h>

TEST(sample1, example1)
{
    testing::internal::CaptureStdout();
    std::system("./sample1.exe");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 1 Number of arguments parsed: 1\n";
    expectedStr += "Positional Argument Value: \n";
    expectedStr += "Integer Argument Value:    2\n";
    expectedStr += "Counter Argument Value:    0\n";
    expectedStr += "String Argument Value:     default\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample1, example2)
{
    testing::internal::CaptureStdout();
    std::system("./sample1.exe -f -c -i 8 -o bar foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 8 Number of arguments parsed: 8\n";
    expectedStr += "Positional Argument Value: foo\n";
    expectedStr += "Integer Argument Value:    8\n";
    expectedStr += "Counter Argument Value:    1\n";
    expectedStr += "String Argument Value:     bar\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample1, example3)
{
    testing::internal::CaptureStdout();
    std::system("./sample1.exe -fci 7 -o bar foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 6 Number of arguments parsed: 6\n";
    expectedStr += "Positional Argument Value: foo\n";
    expectedStr += "Integer Argument Value:    7\n";
    expectedStr += "Counter Argument Value:    1\n";
    expectedStr += "String Argument Value:     bar\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample1, example4)
{
    testing::internal::CaptureStdout();
    std::system("./sample1.exe --flag -c -c --count --input 6 --output bar foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 10 Number of arguments parsed: 10\n";
    expectedStr += "Positional Argument Value: foo\n";
    expectedStr += "Integer Argument Value:    6\n";
    expectedStr += "Counter Argument Value:    3\n";
    expectedStr += "String Argument Value:     bar\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample1, example5)
{
    testing::internal::CaptureStdout();
    std::system("./sample1.exe -fcccc -o bar foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 5 Number of arguments parsed: 5\n";
    expectedStr += "Positional Argument Value: foo\n";
    expectedStr += "Integer Argument Value:    2\n";
    expectedStr += "Counter Argument Value:    4\n";
    expectedStr += "String Argument Value:     bar\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample2, example1)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample2.exe");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"positionalString\" required argument missing\n";
    expectedStr += "\"-o|--output\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample2 [switched options ] [positional arg]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?            show this help message and exit                        \n";
    expectedStr += " -f,--flag               Example of a simple true/false flag argument           \n";
    expectedStr += " -c,--count              Example of a incrementing counter argument             \n";
    expectedStr += " -i,--input=integerValue Example of a basic integer value argument              \n";
    expectedStr += " -o, --output=argString  Example of a switched string argument                  \n\n";
    expectedStr += "Positional Arguments:\n";
    expectedStr += " positionalString        Example of a string argument as positional             \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    EXPECT_STREQ("Parser failed, help displayed\n", terminaloutput.c_str());
}

TEST(sample2, example2)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample2.exe -o bar");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"positionalString\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample2 [switched options ] [positional arg]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?            show this help message and exit                        \n";
    expectedStr += " -f,--flag               Example of a simple true/false flag argument           \n";
    expectedStr += " -c,--count              Example of a incrementing counter argument             \n";
    expectedStr += " -i,--input=integerValue Example of a basic integer value argument              \n";
    expectedStr += " -o, --output=argString  Example of a switched string argument                  \n\n";
    expectedStr += "Positional Arguments:\n";
    expectedStr += " positionalString        Example of a string argument as positional             \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    EXPECT_STREQ("Parser failed, help displayed\n", terminaloutput.c_str());
}

TEST(sample2, example3)
{
    testing::internal::CaptureStdout();
    std::system("./sample2.exe -f -c -i 8 -o bar foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 8 Number of arguments parsed: 8\n";
    expectedStr += "Positional Argument Value: foo\n";
    expectedStr += "Integer Argument Value:    8\n";
    expectedStr += "Counter Argument Value:    1\n";
    expectedStr += "String Argument Value:     bar\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example1)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample3.exe");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"subcommand\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample3 [subcommand] [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-? show this help message and exit                                   \n";
    expectedStr += " -V,--version Example of a simple true/false flag argument                      \n\n";
    expectedStr += "Positional Arguments:\n";
    expectedStr += " subcommand   Example of a positional argument as subcommand                    \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    EXPECT_STREQ("Parser failed, help displayed\n", terminaloutput.c_str());
}

TEST(sample3, example2)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe one");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: one\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 2\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 0 Starting at argument: 2\n";
    expectedStr += "Counter Argument Value:    0\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example3)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe one -c");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 3 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: one\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 3\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 1 Starting at argument: 2\n";
    expectedStr += "Counter Argument Value:    1\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example4)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe two");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: two\n";

    // Sub-command two
    expectedStr += "Number of arguments passed in: 2\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 0 Starting at argument: 2\n";
    expectedStr += "Integer Argument Value:    2\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example5)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe two -i 7");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 4 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: two\n";

    // Sub-command two
    expectedStr += "Number of arguments passed in: 4\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 2 Starting at argument: 2\n";
    expectedStr += "Integer Argument Value:    7\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example6)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample3.exe three");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"-o|--output\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample3 [subcommand] [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?           show this help message and exit      \n";
    expectedStr += " -V,--version           Example of a simple true/false flag  \n";
    expectedStr += "                        argument                             \n";
    expectedStr += " -h,--help,-?           show this help message and exit      \n";
    expectedStr += " -o, --output=argString Example of a switched string argument\n\n";
    expectedStr += "Positional Arguments:\n";
    expectedStr += " subcommand             Example of a positional argument as  \n";
    expectedStr += "                        subcommand                           \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    
    std::string expectedTerminalStr = "Number of arguments passed in: 2 Number of arguments parsed: 2\n";
    expectedTerminalStr += "Subcommand Argument Value: three\n";
    expectedTerminalStr += "Sub Command 3 Parser failed, help displayed\n";
    expectedTerminalStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedTerminalStr.c_str(), terminaloutput.c_str());
}

TEST(sample3, example7)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe three -o foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 4 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: three\n";

    // Sub-command three
    expectedStr += "Number of arguments passed in: 4\n";
    expectedStr += "Number of subcommand 3 arguments parsed: 2 Starting at argument: 2\n";
    expectedStr += "String Argument Value:     foo\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example8)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe three -V -o foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 5 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: three\n";

    // Sub-command three
    expectedStr += "Number of arguments passed in: 5\n";
    expectedStr += "Number of subcommand 3 arguments parsed: 3 Starting at argument: 2\n";
    expectedStr += "String Argument Value:     foo\n";
    expectedStr += "Flag Argument Value:       true\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample3, example9)
{
    testing::internal::CaptureStdout();
    std::system("./sample3.exe four");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: four\n";
    expectedStr += "Unknown subcommand: \"four\", valid values: [one|two|three]\n";
    expectedStr += "Flag Argument Value:       false\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example1)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample4.exe");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"subcommand\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample4 [global options] [subcommand] [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?           show this help message and exit                         \n";
    expectedStr += " -U,--global            Example of a simple true/false flag argument            \n";
    expectedStr += " -i,--integer=globalInt Example of a simple integer argument                    \n\n";
    expectedStr += "Positional Arguments:\n";
    expectedStr += " subcommand             Example of a positional argument as subcommand after    \n";
    expectedStr += "                        options                                                 \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    
    std::string expectedTerminalStr = "Parser failed, help displayed\n";
    EXPECT_STREQ(expectedTerminalStr.c_str(), terminaloutput.c_str());
}

TEST(sample4, example2)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe one");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of global arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value:     one\n";
    expectedStr += "Global Flag Argument Value:    false\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 2\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 0 Starting at argument: 2\n";
    expectedStr += "Counter Argument Value:    0\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example3)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -Ui 4 one");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 4 Number of global arguments parsed: 4\n";
    expectedStr += "Subcommand Argument Value:     one\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 4\n\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 4\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 0 Starting at argument: 4\n";
    expectedStr += "Counter Argument Value:    0\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example4)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -Ui 4 one -c");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 5 Number of global arguments parsed: 4\n";
    expectedStr += "Subcommand Argument Value:     one\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 4\n\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 5\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 1 Starting at argument: 4\n";
    expectedStr += "Counter Argument Value:    1\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example5)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe one -ccc");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 3 Number of global arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value:     one\n";
    expectedStr += "Global Flag Argument Value:    false\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 1
    expectedStr += "Number of arguments passed in: 3\n";
    expectedStr += "Number of subcommand 1 arguments parsed: 1 Starting at argument: 2\n";
    expectedStr += "Counter Argument Value:    3\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example6)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe two");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of global arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value:     two\n";
    expectedStr += "Global Flag Argument Value:    false\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 2
    expectedStr += "Number of arguments passed in: 2\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 0 Starting at argument: 2\n";
    expectedStr += "Integer Argument Value:    2\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example7)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -Ui=3 two");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 3 Number of global arguments parsed: 3\n";
    expectedStr += "Subcommand Argument Value:     two\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 3\n\n";

    // Sub-command 2
    expectedStr += "Number of arguments passed in: 3\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 0 Starting at argument: 3\n";
    expectedStr += "Integer Argument Value:    2\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example8)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -Ui=3 two -i 8");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 5 Number of global arguments parsed: 3\n";
    expectedStr += "Subcommand Argument Value:     two\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 3\n\n";

    // Sub-command 2
    expectedStr += "Number of arguments passed in: 5\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 2 Starting at argument: 3\n";
    expectedStr += "Integer Argument Value:    8\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example9)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe two -i 10");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 4 Number of global arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value:     two\n";
    expectedStr += "Global Flag Argument Value:    false\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 2
    expectedStr += "Number of arguments passed in: 4\n";
    expectedStr += "Number of subcommand 2 arguments parsed: 2 Starting at argument: 2\n";
    expectedStr += "Integer Argument Value:    10\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example10)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample4.exe three");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"-o|--output\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample4 [global options] three [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?           show this help message and exit                         \n";
    expectedStr += " -o, --output=argString Example of a switched string argument                   \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    
    std::string expectedTerminalStr = "Number of arguments passed in: 2 Number of global arguments parsed: 2\n";
    expectedTerminalStr            += "Subcommand Argument Value:     three\n";
    expectedTerminalStr            += "Global Flag Argument Value:    false\n";
    expectedTerminalStr            += "Global Integer Argument Value: 0\n\n";
    expectedTerminalStr            += "Sub Command 3 Parser failed, help displayed\n";
    EXPECT_STREQ(expectedTerminalStr.c_str(), terminaloutput.c_str());
}

TEST(sample4, example11)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe three -o foo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 4 Number of global arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value:     three\n";
    expectedStr += "Global Flag Argument Value:    false\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 3
    expectedStr += "Number of arguments passed in: 4\n";
    expectedStr += "Number of subcommand 3 arguments parsed: 2 Starting at argument: 2\n";
    expectedStr += "String Argument Value:     foo\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example12)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -U three -o goo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 5 Number of global arguments parsed: 3\n";
    expectedStr += "Subcommand Argument Value:     three\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 0\n\n";

    // Sub-command 3
    expectedStr += "Number of arguments passed in: 5\n";
    expectedStr += "Number of subcommand 3 arguments parsed: 2 Starting at argument: 3\n";
    expectedStr += "String Argument Value:     goo\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample4, example13)
{
    testing::internal::CaptureStdout();
    std::system("./sample4.exe -U -i 6 three -o moo");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 7 Number of global arguments parsed: 5\n";
    expectedStr += "Subcommand Argument Value:     three\n";
    expectedStr += "Global Flag Argument Value:    true\n";
    expectedStr += "Global Integer Argument Value: 6\n\n";

    // Sub-command 3
    expectedStr += "Number of arguments passed in: 7\n";
    expectedStr += "Number of subcommand 3 arguments parsed: 2 Starting at argument: 5\n";
    expectedStr += "String Argument Value:     moo\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample5, example1)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample5.exe");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"subcommand\" required argument missing\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample5 [subcommand] [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?         show this help message and exit                           \n";
    expectedStr += " -l,--lines=linecount Example of an integer argument                            \n";
    expectedStr += " -f,--file=filename   Example of a string argument                              \n";
    expectedStr += "\nPositional Arguments:\n";
    expectedStr += " subcommand           Example of a positional enum argument as subcommand       \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    
    std::string expectedTerminalStr = "Parser failed, help displayed\n";
    EXPECT_STREQ(expectedTerminalStr.c_str(), terminaloutput.c_str());
}

TEST(sample5, example2)
{
    testing::internal::CaptureStdout();
    std::system("./sample5.exe read");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 2 Number of arguments parsed: 2\n";
    expectedStr += "Subcommand Argument Value: 1\n";
    expectedStr += "Line Count Argument Value: 0\n";
    expectedStr += "File Name Argument Value:  default.txt\n";

    // read sub-command
    expectedStr += "Read 0 lines from file \"default.txt\"\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample5, example3)
{
    testing::internal::CaptureStdout();
    std::system("./sample5.exe read -l 4 -f foo.c");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 6 Number of arguments parsed: 6\n";
    expectedStr += "Subcommand Argument Value: 1\n";
    expectedStr += "Line Count Argument Value: 4\n";
    expectedStr += "File Name Argument Value:  foo.c\n";

    // read sub-command
    expectedStr += "Read 4 lines from file \"foo.c\"\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample5, example4)
{
    testing::internal::CaptureStdout();
    std::system("./sample5.exe write -l 16 -f moo.c");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 6 Number of arguments parsed: 6\n";
    expectedStr += "Subcommand Argument Value: 2\n";
    expectedStr += "Line Count Argument Value: 16\n";
    expectedStr += "File Name Argument Value:  moo.c\n";

    // read sub-command
    expectedStr += "Write 16 lines to file \"moo.c\"\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample5, example5)
{
    testing::internal::CaptureStdout();
    std::system("./sample5.exe append -l 8 -f goo.c");
    std::string output = testing::internal::GetCapturedStdout();

    std::string expectedStr = "Number of arguments passed in: 6 Number of arguments parsed: 6\n";
    expectedStr += "Subcommand Argument Value: 3\n";
    expectedStr += "Line Count Argument Value: 8\n";
    expectedStr += "File Name Argument Value:  goo.c\n";

    // read sub-command
    expectedStr += "Append 8 lines to file \"goo.c\"\n";

    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
}

TEST(sample5, example6)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
    std::system("./sample5.exe copy -l 4 -f foo.c");
    std::string output = testing::internal::GetCapturedStderr();
    std::string terminaloutput = testing::internal::GetCapturedStdout();

    std::string expectedStr = "\"subcommand copy\" assignment failed\n";

    // help display
    expectedStr += "Usage:\n";
    expectedStr += "sample5 [subcommand] [subcommand options]\n\n";
    expectedStr += "Description of the utility to be included in the help display\n\n";
    expectedStr += "Optional Arguments:\n";
    expectedStr += " -h,--help,-?         show this help message and exit                           \n";
    expectedStr += " -l,--lines=linecount Example of an integer argument                            \n";
    expectedStr += " -f,--file=filename   Example of a string argument                              \n";
    expectedStr += "\nPositional Arguments:\n";
    expectedStr += " subcommand           Example of a positional enum argument as subcommand       \n\n";
    EXPECT_STREQ(expectedStr.c_str(), output.c_str());
    
    std::string expectedTerminalStr = "Parser failed, help displayed\n";
    EXPECT_STREQ(expectedTerminalStr.c_str(), terminaloutput.c_str());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/** @} */
