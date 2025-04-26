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
 * @file config_xml_parse_test.cpp
 * @ingroup libconfig_xml_parser_unittest
 * @{
 */

// Includes
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "varg_intf_mock.h"
#include "parser_string_list.h"
#include "config_xml_parse.h"
#include "test_config_xml.h"
#include "mock_ParserStringListInterface.h"

using ::testing::StrictMock;
using ::testing::Mock;
using ::testing::Return;
using stringMockptr = StrictMock<argparser::mock_ParserStringListInterface>*;

stringMockptr getStringsMock(argparser::config_xml_parse* parser)
{
    argparser::ParserStringListInterface* mock = parser->getmsgGenerator().get();
    return reinterpret_cast<stringMockptr> (mock);   // NOLINT
}

//======================================================================================
// Public Interface testing, English
//======================================================================================
TEST(config_xml_parse, defaultConstructor)
{
    argparser::config_xml_parse  testvar(testFileName);
    EXPECT_STREQ(testFileName, testvar.getFileName().c_str());
}

TEST(config_xml_parse, copyConstructor)
{
    argparser::config_xml_parse testvar(testFileName);
    argparser::config_xml_parse copiedvar(testvar);
    EXPECT_STREQ(testvar.getFileName().c_str(), copiedvar.getFileName().c_str());
}

TEST(config_xml_parse, moveConstructor)
{
    argparser::config_xml_parse testvar(testFileName);
    argparser::config_xml_parse copiedvar(std::move(testvar));
    EXPECT_STREQ(testFileName, copiedvar.getFileName().c_str());
}

TEST(config_xml_parse, equateConstructor)
{
    argparser::config_xml_parse testvar(testFileName);
    argparser::config_xml_parse copiedvar;
    copiedvar = testvar;
    EXPECT_STREQ(testvar.getFileName().c_str(), copiedvar.getFileName().c_str());
}

TEST(config_xml_parse, equateMoveConstructor)
{
    argparser::config_xml_parse testvar(testFileName);
    argparser::config_xml_parse copiedvar;
    copiedvar = std::move(testvar);
    EXPECT_STREQ(testFileName, copiedvar.getFileName().c_str());
}

TEST(config_xml_parse, defaultHelp)
{
    argparser::config_xml_parse testvar(testFileName);
    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("", output.c_str());
}

TEST(config_xml_parse, parseTest)
{
    argparser::config_xml_parse  testvar(testFileName);
    EXPECT_TRUE(testvar.parse());
}

TEST(config_xml_parse, parseTestFail)
{
    argparser::config_xml_parse testvar("./foo-config.xml");
    testing::internal::CaptureStderr();
    EXPECT_FALSE(testvar.parse());
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Failed to open json input file \"./foo-config.xml\"\n", output.c_str());
}


TEST(config_xml_parse, addArgument)
{
    argparser::config_xml_parse testvar(testFileName);
    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));

    testvar.addArgument(&testarg, "testarg");
}

TEST(config_xml_parse, addListArgument)
{
    argparser::config_xml_parse testvar(testFileName);
    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));
    EXPECT_CALL(testarg, isList()).WillOnce(Return(true));

    testvar.addArgument(&testarg, "testarg1", 2);
}

TEST(config_xml_parse, addListArgumentFail)
{
    argparser::config_xml_parse testvar(testFileName);
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getNotListTypeMessage(2)).WillOnce(Return("Mock only list type arguments can have an argument count of 2"));

    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, isList()).WillOnce(Return(false));

    testing::internal::CaptureStderr();
    testvar.addArgument(&testarg, "testarg1", 2);
    parserstr output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("Mock only list type arguments can have an argument count of 2\n", output.c_str());
}

TEST(config_xml_parse, helpWithArgument)
{
    argparser::config_xml_parse testvar(testFileName);
    stringMockptr stringMock = getStringsMock(&testvar);
    EXPECT_CALL(*stringMock, getXmlArgumentsMessage()).WillOnce(Return("Mock Available XML argument values:"));

    StrictMock<argparser::mock_varg_intf> testarg;
    EXPECT_CALL(testarg, isList()).WillOnce(Return(true));
    EXPECT_CALL(testarg, getTypeString()).WillOnce(Return("<numeric>"));

    testvar.addArgument(&testarg, "testarg1", 2);

    testing::internal::CaptureStdout();
    testvar.displayHelp(std::cout);
    parserstr output = testing::internal::GetCapturedStdout();
    parserstr expected = "Mock Available XML argument values:\n";
    expected += "<testarg1><numeric>,...</testarg1>                                              \n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
}

/** @} */
