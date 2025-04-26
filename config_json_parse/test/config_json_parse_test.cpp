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
 * @file config_json_parse_test.cpp
 * @ingroup libconfig_json_parser_unittest
 * @{
 */

// Includes
#include <cstddef>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "varg_intf_mock.h"
#include "parser_string_list.h"
#include "config_json_parse.h"
#include "test_config_json.h"
#include "mock_ParserStringListInterface.h"

using ::testing::StrictMock;
using ::testing::Mock;
using ::testing::Return;
using stringMockptr = StrictMock<argparser::mock_ParserStringListInterface>*;


stringMockptr getStringsMock(argparser::config_json_parse* parser)
{
    argparser::ParserStringListInterface* mock = parser->getmsgGenerator().get();
    return reinterpret_cast<stringMockptr> (mock);   // NOLINT
}

//======================================================================================
// Public Interface testing, English
//======================================================================================
TEST(config_json_parse, defaultConstructor)
{
    argparser::config_json_parse testvar(testFileName);
    EXPECT_STREQ(testFileName, testvar.getFileName().c_str());
}

TEST(config_json_parse, copyConstructor)
{
    argparser::config_json_parse testvar(testFileName);
    argparser::config_json_parse copiedvar(testvar);
    EXPECT_STREQ(testvar.getFileName().c_str(), copiedvar.getFileName().c_str());
}

TEST(config_json_parse, moveConstructor)
{
    argparser::config_json_parse testvar(testFileName);
    argparser::config_json_parse copiedvar(std::move(testvar));
    EXPECT_STREQ(testFileName, copiedvar.getFileName().c_str());
}

TEST(config_json_parse, equateConstructor)
{
    argparser::config_json_parse testvar(testFileName);
    argparser::config_json_parse copiedvar;
    copiedvar = testvar;
    EXPECT_STREQ(testvar.getFileName().c_str(), copiedvar.getFileName().c_str());
}

TEST(config_json_parse, equateMoveConstructor)
{
    argparser::config_json_parse testvar(testFileName);
    argparser::config_json_parse copiedvar;
    copiedvar = std::move(testvar);
    EXPECT_STREQ(testFileName, copiedvar.getFileName().c_str());
}

TEST(config_json_parse, defaultHelp)
{
    argparser::config_json_parse testvar(testFileName);
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("", output.c_str());
}

TEST(config_json_parse, parseTest)
{
    argparser::config_json_parse testvar(testFileName);
    EXPECT_TRUE(testvar.parse());
}

TEST(config_json_parse, parseTestFail)
{
    argparser::config_json_parse testvar("./foo-config.json");
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Failed to open json input file \"./foo-config.json\"\n", output.c_str());
}

TEST(config_json_parse, addArgument)
{
    argparser::config_json_parse testvar(testFileName);
    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));
    testvar.addArgument(&testarg, "testarg1");
}

TEST(config_json_parse, addListArgument)
{
    argparser::config_json_parse testvar(testFileName);
    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));
    testvar.addArgument(&testarg, "testarg1", 2);
}

TEST(config_json_parse, addListArgumentFail)
{
    argparser::config_json_parse testvar(testFileName);
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getNotListTypeMessage(2)).WillOnce(Return("Mock only list type arguments can have an argument count of 2"));

    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    testvar.addArgument(&testarg, "testarg1", 2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock only list type arguments can have an argument count of 2\n", output.c_str());
}

TEST(config_json_parse, helpWithArgument)
{
    argparser::config_json_parse testvar(testFileName);
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getJsonArgumentsMessage()).WillOnce(Return("Mock Available JSON argument values:"));


    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));

    testvar.addArgument(&testarg, "testarg1", 2);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expected = "Mock Available JSON argument values:\n";
    expected += R"("testarg1":"<numeric>,...")";
    expected += "                                                      \n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
}

/** @} */
