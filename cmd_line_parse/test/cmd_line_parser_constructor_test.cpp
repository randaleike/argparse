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
 * @file cmd_line_parse_misc_test.cpp
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include <cstddef>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <parser_base.h>
#include "mock_varg_intf.h"
#include "cmd_line_parse_test.h"

#if defined(CONSTRUCTOR_GET_HELP_STRING)
const bool disableDefaultHelp = false;      // NOLINT
#else //defined(CONSTRUCTOR_GET_HELP_STRING)
const bool disableDefaultHelp = true;       // NOLINT
#endif //defined(CONSTRUCTOR_GET_HELP_STRING)

class cmd_line_parser_constructor : public ::testing::Test
{
    protected:
        /**
         * @brief Test the constructed parser
         *
         * @param testvar - Parser to test
         * @return true - Passed
         * @return false - Failed
         */
        static bool parser_test(argparser::cmd_line_parse testvar)
        {
            testvar.disableHelpDisplayOnError();

            StrictMock<argparser::mock_varg_intf> flagArg;
            EXPECT_CALL(flagArg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

            StrictMock<argparser::mock_varg_intf> keyarg;
            EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

            testvar.addFlagArgument(&flagArg, "flag", "-f,--flag", "Example of a simple true/false flag argument");
            testvar.addKeyArgument(&keyarg, "key", "-v,--value", "Example of a key argument", 1, true);

            // NOLINTBEGIN
            parserchar progname[] = "runprog";
            parserchar vargflg[] = "-f";
            parserchar posArg[] = "-v=15";
            parserchar* argv[] = {progname, vargflg, posArg};
            int argc = sizeof(argv) / sizeof(argv[0]);
            int parseCount = testvar.parse(argc, argv);
            // NOLINTEND

            return (parseCount == argc);
        }

        /**
         * @brief Test the constructed parser
         *
         * @param testvar - Parser to test
         * @return true - Passed
         * @return false - Failed
         */
        static bool parser_test_slash(argparser::cmd_line_parse testvar)
        {
            testvar.disableHelpDisplayOnError();

            StrictMock<argparser::mock_varg_intf> flagArg;
            EXPECT_CALL(flagArg, setValue())
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

            StrictMock<argparser::mock_varg_intf> keyarg;
            EXPECT_CALL(keyarg, setValue(::testing::StrEq("15")))
                .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

            testvar.addFlagArgument(&flagArg, "flag", "/f,/flag", "Example of a simple true/false flag argument");
            testvar.addKeyArgument(&keyarg, "key", "/v,/value", "Example of a key argument", 1, true);

            // NOLINTBEGIN
            parserchar progname[] = "runprog";
            parserchar vargflg[] = "/f";
            parserchar posArg[] = "/v=15";
            parserchar* argv[] = {progname, vargflg, posArg};
            int argc = sizeof(argv) / sizeof(argv[0]);
            int parseCount = testvar.parse(argc, argv);
            // NOLINTEND

            return (parseCount == argc);
        }
};

#if defined(CONSTRUCTOR_GET_HELP_STRING)
TEST_F(cmd_line_parser_constructor, DefaultConstructor)
{
    argparser::cmd_line_parse testvar;
    EXPECT_TRUE(parser_test(testvar));
}
#endif //defined(CONSTRUCTOR_GET_HELP_STRING)

TEST_F(cmd_line_parser_constructor, ConstCharPtr)
{
    const char* useage = "testprog [options]";
    const char* desc = "Description of the test program";
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);
    EXPECT_TRUE(parser_test(testvar));
}

TEST_F(cmd_line_parser_constructor, NullPtrInput)
{
    argparser::cmd_line_parse testvar(nullptr, nullptr, false, disableDefaultHelp);
    EXPECT_TRUE(parser_test(testvar));
}

TEST_F(cmd_line_parser_constructor, ParserString)
{
    parserstr useage("testprog [options]");
    parserstr desc("Description of the test program");
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);
    EXPECT_TRUE(parser_test(testvar));
}

TEST_F(cmd_line_parser_constructor, ParserStringEmpty)
{
    parserstr useage;
    parserstr desc;
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);
    EXPECT_TRUE(parser_test(testvar));
}

TEST_F(cmd_line_parser_constructor, CopyConstructor)
{
    parserstr useage("testprog [options]");
    parserstr desc("Description of the test program");
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);

    argparser::cmd_line_parse copiedVar(testvar);
    EXPECT_TRUE(parser_test(copiedVar));
}

TEST_F(cmd_line_parser_constructor, MoveConstructor)
{
    parserstr useage("testprog [options]");
    parserstr desc("Description of the test program");
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);

    argparser::cmd_line_parse copiedVar(std::move(testvar));
    EXPECT_TRUE(parser_test(copiedVar));
}

#if defined(CONSTRUCTOR_GET_HELP_STRING)
TEST_F(cmd_line_parser_constructor, EquateConstructor)
{
    parserstr useage("testprog [options]");
    parserstr desc("Description of the test program");
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);

    argparser::cmd_line_parse copiedVar;
    copiedVar = testvar;
    EXPECT_TRUE(parser_test(copiedVar));
}

TEST_F(cmd_line_parser_constructor, EquateMoveConstructor)
{
    parserstr useage("testprog [options]");
    parserstr desc("Description of the test program");
    argparser::cmd_line_parse testvar(useage, desc, false, disableDefaultHelp);

    argparser::cmd_line_parse copiedVar;
    copiedVar = std::move(testvar);
    EXPECT_TRUE(parser_test(copiedVar));
}
#endif //defined(CONSTRUCTOR_GET_HELP_STRING)

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/** @} */
