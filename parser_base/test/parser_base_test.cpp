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
 * @file parser_base_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include <string>
#include <list>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "varg_intf.h"
#include "varg_intf_mock.h"
#include "parser_base.h"
#include "parser_string_list.h"
#include "mock_ParserStringListInterface.h"

using ::testing::StrictMock;
using ::testing::Mock;
using ::testing::Return;
using stringMockptr = StrictMock<argparser::mock_ParserStringListInterface>*;

const size_t defaultOptionWidth = 25;
const size_t shortOptionWidth   = 20;
const size_t defaultHelpWidth   = 30;

enum testDebugLevel {
    DEBUG_NONE = 2,
    DEBUG_SILENT = 3,
    DEBUG_MINIMUM = 4,
    DEBUG_VERBOSE = 5,
};

stringMockptr getStringsMock(argparser::parser_base* parser)
{
    argparser::ParserStringListInterface* mock = parser->getmsgGenerator().get();
    return reinterpret_cast<stringMockptr> (mock);   // NOLINT
}

class test_parser_base : public argparser::parser_base
{
    public:
        test_parser_base(bool abortOnError = false, int debugLevel = 0) : argparser::parser_base(abortOnError, debugLevel) {}
        test_parser_base(const test_parser_base& other) noexcept = default;
        test_parser_base(test_parser_base&& other) noexcept = default;
        test_parser_base& operator=(const test_parser_base& other) noexcept = default;
        test_parser_base& operator=(test_parser_base&& other) noexcept = default;

        /**
         * @brief Destructor
         */
        ~test_parser_base() = default;

        // Protected data/method accessors
        std::list<argparser::ArgEntry> getKeyArgList()  {return argparser::parser_base::getKeyArgList();}
        parserchar getKeyListDelimiter()                {return argparser::parser_base::getKeyListDelimeter();}
        parserchar getAssignmentDelimiter()             {return argparser::parser_base::getAssignmentDelimeter();}
        parserchar getAssignmentListDelimiter()         {return argparser::parser_base::getAssignmentListDelimeter();}
        bool getAbortOnError()                          {return argparser::parser_base::getErrorAbortFlag();}
        int  getDebugLevel()                            {return argparser::parser_base::getDebugMsgLevel();}

        size_t getMaxColumnWidth()                      {return argparser::parser_base::getMaxColumnWidth();}
        size_t getMaxOptionLength()                     {return argparser::parser_base::getMaxOptionLength();}
        void setMaxColumnWidth(size_t width)            {argparser::parser_base::setMaxColumnWidth(width);}
        void setMaxOptionLength(size_t width)           {argparser::parser_base::setMaxOptionLength(width);}
        void resizeMaxOptionLength(size_t newSize)      {argparser::parser_base::resizeMaxOptionLength(newSize);}
        size_t getOptionKeyWidth()                      {return argparser::parser_base::getOptionKeyWidth();}
        size_t getHelpKeyWidth()                        {return argparser::parser_base::getHelpKeyWidth(getOptionKeyWidth());}

        void addArgument(argparser::ArgEntry& arg)          {argparser::parser_base::addKeyArgListEntry(arg);}
        void addKeyArgListEntry(argparser::ArgEntry entry)  {argparser::parser_base::addKeyArgListEntry(entry);}
        bool isKeyArgListEmpty()                            {return argparser::parser_base::isKeyArgListEmpty();}

        bool isParsingError()                               {return argparser::parser_base::isParsingError();}
        void setParsingError(bool orValue)                  {argparser::parser_base::setParsingError(orValue);}
        void clearParsingError()                            {argparser::parser_base::clearParsingError();}
        bool isParserAbort()                                {return argparser::parser_base::isParserAbort();}
};

//======================================================================================
// Public Interface testing
//======================================================================================

TEST(TestParserBase,  ConstructorBasic)
{
    test_parser_base testvar;
    EXPECT_TRUE(testvar.isKeyArgListEmpty());
    EXPECT_FALSE(testvar.getAbortOnError());
    EXPECT_EQ(0, testvar.getDebugLevel());

    EXPECT_EQ(80, testvar.getMaxColumnWidth());
    EXPECT_EQ(0, testvar.getMaxOptionLength());
    EXPECT_EQ(',', testvar.getKeyListDelimiter());
    EXPECT_EQ('=', testvar.getAssignmentDelimiter());
    EXPECT_EQ(',', testvar.getAssignmentListDelimiter());
}

TEST(TestParserBase,  ConstructorInput)
{
    test_parser_base testvar(true, 4);
    EXPECT_TRUE(testvar.isKeyArgListEmpty());
    EXPECT_TRUE(testvar.getAbortOnError());
    EXPECT_EQ(4, testvar.getDebugLevel());

    EXPECT_EQ(80, testvar.getMaxColumnWidth());
    EXPECT_EQ(0, testvar.getMaxOptionLength());
    EXPECT_EQ(',', testvar.getKeyListDelimiter());
    EXPECT_EQ('=', testvar.getAssignmentDelimiter());
    EXPECT_EQ(',', testvar.getAssignmentListDelimiter());
}

TEST(TestParserBase,  setKeyListDelimiter)
{
    test_parser_base testvar;
    testvar.setKeyListDelimiter(';');
    EXPECT_EQ(';', testvar.getKeyListDelimiter());
}

TEST(TestParserBase,  setAssignmentDelimiter)
{
    test_parser_base testvar;
    testvar.setAssignmentDelimiter('#');
    EXPECT_EQ('#', testvar.getAssignmentDelimiter());
}

TEST(TestParserBase,  setAssignmentListDelimiter)
{
    test_parser_base testvar;
    testvar.setAssignmentListDelimiter(':');
    EXPECT_EQ(':', testvar.getAssignmentListDelimiter());
}

TEST(TestParserBase,  setDebugLevel)
{
    test_parser_base testvar;
    testvar.setDebugLevel(3);
    EXPECT_EQ(3, testvar.getDebugLevel());
}

TEST(TestParserBase,  addArgKeyListSingleNospaces)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(1, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListSingleSpaces)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = " --foo ";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(1, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListDouble)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo,--moo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(2, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--moo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListDoubleWithSpace)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = " --foo, --moo ";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(2, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--moo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListTriple)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo,--moo,--goo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(3, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--moo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--goo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListTripleNewDelimeter)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo:--moo:--goo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};
    testparser.setKeyListDelimiter(':');

    EXPECT_EQ(3, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--moo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--goo", testArg.keyList.front().c_str());
}

TEST(TestParserBase,  addArgKeyListTripleNewDelimeterAndSpace)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo :--moo :--goo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};
    testparser.setKeyListDelimiter(':');

    EXPECT_EQ(3, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--moo", testArg.keyList.front().c_str());
    testArg.keyList.pop_front();
    EXPECT_STREQ("--goo", testArg.keyList.front().c_str());
}


TEST(TestParserBase,  addArgKeyListEntry)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};

    EXPECT_EQ(1, testparser.addArgKeyList(testArg, testkeys));
    EXPECT_STREQ("--foo", testArg.keyList.front().c_str());

    testparser.addKeyArgListEntry(testArg);
    EXPECT_FALSE(testparser.isKeyArgListEmpty());
    EXPECT_EQ(1, testparser.getKeyArgList().size());
    EXPECT_STREQ("foo", testparser.getKeyArgList().front().name.c_str());
}


TEST(TestParserBase,  CopyConstructorTest)
{
    const size_t maxCols = 100;
    const size_t maxOpt = 50;

    // Non-default everything
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--foo";
    argparser::ArgEntry testArg = {"foo", "foo input value", "", (&testvarg), 0, 0, true};
    test_parser_base testparser(true, 4);

    testparser.setKeyListDelimiter(';');
    testparser.setAssignmentDelimiter('#');
    testparser.setAssignmentListDelimiter(':');
    testparser.setMaxColumnWidth(maxCols);
    testparser.setMaxOptionLength(maxOpt);

    testparser.addArgKeyList(testArg, testkeys);
    testparser.addKeyArgListEntry(testArg);

    // Copy and make sure it's all correct
    test_parser_base copiedParser(testparser);
    EXPECT_EQ(';', copiedParser.getKeyListDelimiter());
    EXPECT_EQ('#', copiedParser.getAssignmentDelimiter());
    EXPECT_EQ(':', copiedParser.getAssignmentListDelimiter());
    EXPECT_EQ(4, copiedParser.getDebugLevel());
    EXPECT_TRUE(copiedParser.getAbortOnError());
    EXPECT_EQ(maxCols, copiedParser.getMaxColumnWidth());
    EXPECT_EQ(maxOpt, copiedParser.getMaxOptionLength());

    EXPECT_FALSE(copiedParser.isKeyArgListEmpty());
    EXPECT_EQ(1, copiedParser.getKeyArgList().size());
    EXPECT_STREQ("foo", copiedParser.getKeyArgList().front().name.c_str());
}

TEST(TestParserBase,  MoveConstructor)
{
    const size_t maxCols = 90;
    const size_t maxOpt = 45;

    // Non-default everything
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--moo";
    argparser::ArgEntry testArg = {"moo", "moo input value", "", (&testvarg), 0, 0, true};
    test_parser_base testparser(true, 2);

    testparser.setKeyListDelimiter(':');
    testparser.setAssignmentDelimiter('!');
    testparser.setAssignmentListDelimiter('.');
    testparser.setMaxColumnWidth(maxCols);
    testparser.setMaxOptionLength(maxOpt);

    testparser.addArgKeyList(testArg, testkeys);
    testparser.addKeyArgListEntry(testArg);

    // Move the object and test for correct values
    test_parser_base copiedParser(std::move(testparser));
    EXPECT_EQ(':', copiedParser.getKeyListDelimiter());
    EXPECT_EQ('!', copiedParser.getAssignmentDelimiter());
    EXPECT_EQ('.', copiedParser.getAssignmentListDelimiter());
    EXPECT_EQ(2, copiedParser.getDebugLevel());
    EXPECT_TRUE(copiedParser.getAbortOnError());
    EXPECT_EQ(maxCols, copiedParser.getMaxColumnWidth());
    EXPECT_EQ(maxOpt, copiedParser.getMaxOptionLength());

    EXPECT_FALSE(copiedParser.isKeyArgListEmpty());
    EXPECT_EQ(1, copiedParser.getKeyArgList().size());
    EXPECT_STREQ("moo", copiedParser.getKeyArgList().front().name.c_str());
}

TEST(TestParserBase,  EquateConstructor)
{
    const size_t maxCols = 70;
    const size_t maxOpt = 35;

    // Non-default everything
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--goo";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 0, 0, true};
    test_parser_base testparser(true, 1);

    testparser.setKeyListDelimiter('/');
    testparser.setAssignmentDelimiter('$');
    testparser.setAssignmentListDelimiter('!');
    testparser.setMaxColumnWidth(maxCols);
    testparser.setMaxOptionLength(maxOpt);

    testparser.addArgKeyList(testArg, testkeys);
    testparser.addKeyArgListEntry(testArg);

    // Copy and make sure it's all correct
    test_parser_base copiedParser;
    copiedParser = testparser;
    EXPECT_EQ('/', copiedParser.getKeyListDelimiter());
    EXPECT_EQ('$', copiedParser.getAssignmentDelimiter());
    EXPECT_EQ('!', copiedParser.getAssignmentListDelimiter());
    EXPECT_EQ(1, copiedParser.getDebugLevel());
    EXPECT_TRUE(copiedParser.getAbortOnError());
    EXPECT_EQ(maxCols, copiedParser.getMaxColumnWidth());
    EXPECT_EQ(maxOpt, copiedParser.getMaxOptionLength());

    EXPECT_FALSE(copiedParser.isKeyArgListEmpty());
    EXPECT_EQ(1, copiedParser.getKeyArgList().size());
    EXPECT_STREQ("goo", copiedParser.getKeyArgList().front().name.c_str());
}

TEST(TestParserBase,  MoveEquateConstructor)
{
    const size_t maxCols = 110;
    const size_t maxOpt = 55;

    // Non-default everything
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr testkeys = "--roo";
    argparser::ArgEntry testArg = {"roo", "roo input value", "", (&testvarg), 0, 0, true};
    test_parser_base testparser(true, 1);

    testparser.setKeyListDelimiter('%');
    testparser.setAssignmentDelimiter('*');
    testparser.setAssignmentListDelimiter('&');
    testparser.setMaxColumnWidth(maxCols);
    testparser.setMaxOptionLength(maxOpt);

    testparser.addArgKeyList(testArg, testkeys);
    testparser.addKeyArgListEntry(testArg);

    // Copy and make sure it's all correct
    test_parser_base copiedParser;
    copiedParser = std::move(testparser);
    EXPECT_EQ('%', copiedParser.getKeyListDelimiter());
    EXPECT_EQ('*', copiedParser.getAssignmentDelimiter());
    EXPECT_EQ('&', copiedParser.getAssignmentListDelimiter());
    EXPECT_EQ(1, copiedParser.getDebugLevel());
    EXPECT_TRUE(copiedParser.getAbortOnError());
    EXPECT_EQ(maxCols, copiedParser.getMaxColumnWidth());
    EXPECT_EQ(maxOpt, copiedParser.getMaxOptionLength());

    EXPECT_FALSE(copiedParser.isKeyArgListEmpty());
    EXPECT_EQ(1, copiedParser.getKeyArgList().size());
    EXPECT_STREQ("roo", copiedParser.getKeyArgList().front().name.c_str());
}

TEST(TestParserBase,  getValueList)
{
    test_parser_base testparser;
    parserstr valueTestString = "1,2,3";
    std::list<parserstr> returnList; // NOLINT

    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_STREQ("1", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("2", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("3", returnList.front().c_str());
}

TEST(TestParserBase,  getValueListNewDelimeter)
{
    test_parser_base testparser;
    parserstr valueTestString = "1;2;3";
    std::list<parserstr> returnList; // NOLINT
    testparser.setAssignmentListDelimiter(';');

    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_STREQ("1", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("2", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("3", returnList.front().c_str());
}

TEST(TestParserBase,  getValueListPreceedingAssignment)
{
    test_parser_base testparser;
    parserstr valueTestString = "=1,2,3";
    std::list<parserstr> returnList; // NOLINT

    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_STREQ("1", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("2", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("3", returnList.front().c_str());
}

TEST(TestParserBase,  getValueListPreceedingAssignmentDelim)
{
    test_parser_base testparser;
    parserstr valueTestString = ":1;2;3";
    std::list<parserstr> returnList; // NOLINT
    testparser.setAssignmentDelimiter(':');
    testparser.setAssignmentListDelimiter(';');

    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_STREQ("1", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("2", returnList.front().c_str());
    returnList.pop_front();
    EXPECT_STREQ("3", returnList.front().c_str());
}

TEST(TestParserBase,  findMatchingArg)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg1;
    StrictMock<argparser::mock_varg_intf> testvarg2;
    parserstr testkeys1 = "--foo,-f";
    argparser::ArgEntry testArg1 = {"foo", "foo input value", "", (&testvarg1), 0, 0, true};
    parserstr testkeys2 = "--goo,-g";
    argparser::ArgEntry testArg2 = {"goo", "goo input value", "", (&testvarg2), 0, 0, true};

    testparser.addArgKeyList(testArg1, testkeys1);
    testparser.addArgKeyList(testArg2, testkeys2);
    testparser.addArgument(testArg1);
    testparser.addArgument(testArg2);

    bool found = false;
    argparser::ArgEntry& retArg = testparser.findMatchingArg("--goo", found);
    EXPECT_TRUE(found);
    EXPECT_STREQ("goo", retArg.name.c_str());

    retArg = testparser.findMatchingArg("-g", found);
    EXPECT_TRUE(found);
    EXPECT_STREQ("goo", retArg.name.c_str());

    retArg = testparser.findMatchingArg("--foo", found);
    EXPECT_TRUE(found);
    EXPECT_STREQ("foo", retArg.name.c_str());

    retArg = testparser.findMatchingArg("-f", found);
    EXPECT_TRUE(found);
    EXPECT_STREQ("foo", retArg.name.c_str());

    retArg = testparser.findMatchingArg("-", found);
    EXPECT_FALSE(found);
}

TEST(TestParserBase,  debugMessages)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg1;
    StrictMock<argparser::mock_varg_intf> testvarg2;
    parserstr testkeys1 = "--foo,-f";
    argparser::ArgEntry testArg1 = {"foo", "foo input value", "", (&testvarg1), 0, 0, true};
    parserstr testkeys2 = "--goo,-g";
    argparser::ArgEntry testArg2 = {"goo", "goo input value", "", (&testvarg2), 0, 0, true};

    testparser.addArgKeyList(testArg1, testkeys1);
    testparser.addArgKeyList(testArg2, testkeys2);
    testparser.addArgument(testArg1);
    testparser.addArgument(testArg2);

    testparser.setDebugLevel(DEBUG_SILENT);
    bool found = false;

    testing::internal::CaptureStderr();
    argparser::ArgEntry& retArg = testparser.findMatchingArg("-f", found);
    parserstr output = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(found);
    EXPECT_STREQ("foo", retArg.name.c_str());
    EXPECT_STREQ("", output.c_str());

    testparser.setDebugLevel(DEBUG_MINIMUM);
    testing::internal::CaptureStderr();
    retArg = testparser.findMatchingArg("-f", found);
    output = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(found);
    EXPECT_STREQ("foo", retArg.name.c_str());
    EXPECT_STREQ("Found match var: foo key: -f\n", output.c_str());

    testparser.setDebugLevel(DEBUG_VERBOSE);
    testing::internal::CaptureStderr();
    retArg = testparser.findMatchingArg("-g", found);
    output = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(found);
    EXPECT_STREQ("goo", retArg.name.c_str());

#if (ENABLE_DEBUG_STRING_CHECK)
    parserstr expected = "keyArgList size: 2\n";                    // Two arguments added, foo then goo. Order determined by the addArgument calls
    expected += "Testing var: foo test key: --foo input key: -g\n"; // foo key order "--foo" determined by testkeys1 key order
    expected += "Test key size: 5 input key size: 2\n";             // 5 = length of the "--foo" key string, 2 = the length of the "-g" search string
    expected += "Testing var: foo test key: -f input key: -g\n";    // foo key order "-f" determined by testkeys1 key order
    expected += "Test key size: 2 input key size: 2\n";             // 2 = length of the "-f" key string, 2 = the length of the "-g" search string
    expected += "Testing var: goo test key: --goo input key: -g\n"; // goo key order "--goo" determined by testkeys2 key order
    expected += "Test key size: 5 input key size: 2\n";             // 5 = length of the "--goo" key string, 2 = the length of the "-g" search string
    expected += "Testing var: goo test key: -g input key: -g\n";    // goo key order "-g" determined by testkeys2 key order
    expected += "Test key size: 2 input key size: 2\n";             // 2 = length of the "-g" key string, 2 = the length of the "-g" search string
    expected += "Found match var: goo key: -g\n";
    EXPECT_STREQ(expected.c_str(), output.c_str());
#endif // if(ENABLE_DEBUG_STRING_CHECK)
}

TEST(TestParserBase,  assignKeyFlagValue)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue())
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 0, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    EXPECT_EQ(argparser::eAssignSuccess, testparser.assignKeyFlagValue(testArg));
}

TEST(TestParserBase,  assignKeyValueWithInput)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("54")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 1, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    parserstr inputStr = "54";
    parserstr keyStr = "--goo";
    EXPECT_EQ(argparser::eAssignSuccess, testparser.assignKeyValue(testArg, inputStr, keyStr));
}

TEST(TestParserBase,  assignKeyValueBadInput)
{
    parserstr keyStr = "--goo";
    parserstr testkeys = keyStr+",-g";

    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("34")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_OUT_OF_RANGE_e))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_STORAGE_NULLPTR_e));
    EXPECT_CALL(testvarg, getTypeString())
        .WillOnce(Return("uinteger"));
    EXPECT_CALL(testvarg, getRangeString())
        .WillOnce(Return("<0:200>"))
        .WillOnce(Return("<10:20>"));

    stringMockptr stringMock = getStringsMock(&testparser);
    EXPECT_CALL(*stringMock, getInvalidValueAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                          ::testing::StrEq("34"),
                                                          ::testing::StrEq("uinteger"),
                                                          ::testing::StrEq("<0:200>")))
        .WillOnce(Return("mock invalid input value message"));
    EXPECT_CALL(*stringMock, getOutOfRangeAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                        ::testing::StrEq("34"),
                                                        ::testing::StrEq("<10:20>")))
        .WillOnce(Return("mock invalid value range message"));
    EXPECT_CALL(*stringMock, getStorageNullptrMessage(::testing::StrEq(keyStr.c_str())))
        .WillOnce(Return("mock nullptr storage message"));

    std::string expected = "mock invalid input value message\n" \
                           "mock invalid value range message\n" \
                           "mock nullptr storage message\n";

    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 1, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    parserstr inputStr = "34";
    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignFailed, testparser.assignKeyValue(testArg, inputStr, keyStr));
    EXPECT_EQ(argparser::eAssignFailed, testparser.assignKeyValue(testArg, inputStr, keyStr));
    EXPECT_EQ(argparser::eAssignFailed, testparser.assignKeyValue(testArg, inputStr, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ(expected.c_str(), output.c_str());
}

TEST(TestParserBase,  assignKeyValueBadInputTooMany)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("34")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_STORAGE_TOO_MANY_e));

    parserstr keyStr = "--goo";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 1, 0, true};
    //testparser.setDebugLevel(5);

    stringMockptr stringMock = getStringsMock(&testparser);
    EXPECT_CALL(*stringMock, getTooManyAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                     ::testing::Eq(1),
                                                     ::testing::Eq(2)))
        .WillOnce(Return("mock too many value message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr inputStr = "34";

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignTooMany, testparser.assignKeyValue(testArg, inputStr, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock too many value message\n", output.c_str());
}

TEST(TestParserBase,  assignKeyValueEmptyInputFail)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    parserstr keystr = "--goo";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 0, 0, true};
    //testparser.setDebugLevel(5);

    stringMockptr stringMock = getStringsMock(&testparser);
    EXPECT_CALL(*stringMock, getMissingAssignmentMessage(::testing::StrEq(keystr.c_str())))
        .WillOnce(Return("mock no value error message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr inputStr;

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignNoValue, testparser.assignKeyValue(testArg, inputStr, keystr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock no value error message\n", output.c_str());
}

TEST(TestParserBase,  assignListKeyValueWithInput)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    parserstr keystr = "--goo";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 3, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";
    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_EQ(argparser::eAssignSuccess, testparser.assignListKeyValue(testArg, returnList, keystr));
}

TEST(TestParserBase,  assignListKeyValueWithInputAnySize)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), -1, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";
    parserstr keyStr = "--goo";
    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_EQ(argparser::eAssignSuccess, testparser.assignListKeyValue(testArg, returnList, keyStr));
}

TEST(TestParserBase,  assignListKeyValueTooMany)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;

    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 2, 0, true};
    //testparser.setDebugLevel(5);

    parserstr keyStr = "-g";
    stringMockptr stringMock = getStringsMock(&testparser);
    EXPECT_CALL(*stringMock, getTooManyAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                     ::testing::Eq(2),
                                                     ::testing::Eq(3)))
        .WillOnce(Return("mock too many value message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";

    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignTooMany, testparser.assignListKeyValue(testArg, returnList, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock too many value message\n", output.c_str());
}

TEST(TestParserBase,  assignListKeyValueTooManyNegCnt)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;

    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), -2, 0, true};
    //testparser.setDebugLevel(5);

    parserstr keyStr = "-g";
    stringMockptr stringMock = getStringsMock(&testparser);
    EXPECT_CALL(*stringMock, getTooManyAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                     ::testing::Eq(2),
                                                     ::testing::Eq(3)))
        .WillOnce(Return("mock too many value message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";
    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignTooMany, testparser.assignListKeyValue(testArg, returnList, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock too many value message\n", output.c_str());
}

TEST(TestParserBase,  assignListKeyValueNegCnt)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("42")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("13")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));

    parserstr testkeys = "--goo,-g";
    parserstr keyStr = "-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), -4, 0, true};
    //testparser.setDebugLevel(5);

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";
    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));
    EXPECT_EQ(argparser::eAssignSuccess, testparser.assignListKeyValue(testArg, returnList, keyStr));
}

TEST(TestParserBase,  assignListKeyValueTooFew)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;

    parserstr keyStr = "-g";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 4, 0, true};
    //testparser.setDebugLevel(5);

    stringMockptr stringMock = getStringsMock(&testparser);

    EXPECT_CALL(*stringMock, getMissingListAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                         ::testing::Eq(4),
                                                         ::testing::Eq(3)))
        .WillOnce(Return("mock too few value message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,42,13";
    std::list<parserstr> returnList;     // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignTooFew, testparser.assignListKeyValue(testArg, returnList, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock too few value message\n", output.c_str());
}

TEST(TestParserBase,  assignListKeyValueEmpty)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;

    parserstr keyStr = "-g";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo", "goo input value", "", (&testvarg), 4, 0, true};
    //testparser.setDebugLevel(5);

    stringMockptr stringMock = getStringsMock(&testparser);

    EXPECT_CALL(*stringMock, getMissingAssignmentMessage(::testing::StrEq(keyStr.c_str())))
        .WillOnce(Return("mock missing value message"));

    std::list<parserstr> returnList;  // NOLINT

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignNoValue, testparser.assignListKeyValue(testArg, returnList, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock missing value message\n", output.c_str());
}

TEST(TestParserBase,  assignListKeyValueBadInput)
{
    test_parser_base testparser;
    StrictMock<argparser::mock_varg_intf> testvarg;
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("21")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_SUCCESS_e));
    EXPECT_CALL(testvarg, setValue(::testing::StrEq("moo")))
        .WillOnce(Return(argparser::valueParseStatus_e::PARSE_INVALID_INPUT_e));
    EXPECT_CALL(testvarg, getTypeString())
        .WillOnce(Return("integer"));
    EXPECT_CALL(testvarg, getRangeString())
        .WillOnce(Return("<0:200>"));

    parserstr keyStr = "--goo";
    parserstr testkeys = "--goo,-g";
    argparser::ArgEntry testArg = {"goo",
                                   "goo input value",
                                   "",
                                   (&testvarg),
                                   3,
                                   0,
                                   true};
    //testparser.setDebugLevel(5);

    stringMockptr stringMock = getStringsMock(&testparser);

    EXPECT_CALL(*stringMock, getInvalidValueAssignmentMessage(::testing::StrEq(keyStr.c_str()),
                                                          ::testing::StrEq("moo"),
                                                          ::testing::StrEq("integer"),
                                                          ::testing::StrEq("<0:200>")))
        .WillOnce(Return("mock invalid value message"));

    testparser.addArgKeyList(testArg, testkeys);
    parserstr valueTestString = "21,moo,13";
    std::list<parserstr> returnList;  // NOLINT
    EXPECT_EQ(3, testparser.getValueList(valueTestString, returnList));

    testing::internal::CaptureStderr();
    EXPECT_EQ(argparser::eAssignFailed, testparser.assignListKeyValue(testArg, returnList, keyStr));

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_STREQ("mock invalid value message\n", output.c_str());
}

TEST(TestParserBase,  displayArgHelpBlockNoWrap)
{
    test_parser_base testparser;
    testing::internal::CaptureStdout();
    testparser.displayArgHelpBlock(std::cout, "--goo,-g", "goo input value",
                                   shortOptionWidth, shortOptionWidth);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("--goo,-g             goo input value     \n", output.c_str());
}

TEST(TestParserBase,  displayArgHelpBlockHelpWrap)
{
    test_parser_base testparser;

    testing::internal::CaptureStdout();
    testparser.displayArgHelpBlock(std::cout, "--foo,-f",
                                   "foo input value with a longer help string",
                                   shortOptionWidth, defaultHelpWidth);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("--foo,-f             foo input value with a longer \n                     help string                   \n", output.c_str());
}

TEST(TestParserBase,  displayArgHelpBlockArgWrap)
{
    test_parser_base testparser;

    testing::internal::CaptureStdout();
    testparser.displayArgHelpBlock(std::cout, "<--longkeyvalue><--extralongkeyvalue>",
                                   "Extra long key value",
                                   defaultOptionWidth, defaultHelpWidth);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("<--longkeyvalue>          Extra long key value          \n<--extralongkeyvalue>    \n", output.c_str());
}

TEST(TestParserBase,  displayArgHelpBlockBothWrap)
{
    test_parser_base testparser;

    testing::internal::CaptureStdout();
    testparser.displayArgHelpBlock(std::cout, "<--longkeyvalue><--extralongkeyvalue>",
                                   "Extra long key value with an extra long help text string just to be sure",
                                   defaultOptionWidth, defaultHelpWidth);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("<--longkeyvalue>          Extra long key value with an  \n<--extralongkeyvalue>     extra long help text string   \n                          just to be sure               \n", output.c_str());
}

TEST(TestParserBase,  displayArgHelpBlockBothWrap1)
{
    test_parser_base testparser;

    testing::internal::CaptureStdout();
    testparser.displayArgHelpBlock(std::cout, "<--longkeyvalue> <--extralongkeyvalue>",
                                   "Extra long key value with an extra long help text string just to be sure",
                                   defaultOptionWidth, defaultHelpWidth);
    parserstr output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ("<--longkeyvalue>          Extra long key value with an  \n<--extralongkeyvalue>     extra long help text string   \n                          just to be sure               \n", output.c_str());
}

TEST(TestParserBase,  ParseErrorSetAbortTrue)
{
    test_parser_base testparser(true, 0);
    EXPECT_TRUE(testparser.getAbortOnError());
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    // Make sure it's an or
    testparser.setParsingError(false);
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    testparser.setParsingError(true);
    EXPECT_TRUE(testparser.isParsingError());
    EXPECT_FALSE(testparser.isParserAbort());

    // Make sure it stays set
    testparser.setParsingError(false);
    EXPECT_TRUE(testparser.isParsingError());
    EXPECT_FALSE(testparser.isParserAbort());

    // Validate clear function
    testparser.clearParsingError();
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());
}

TEST(TestParserBase,  ParseErrorSetAbortFalse)
{
    test_parser_base testparser;
    EXPECT_FALSE(testparser.getAbortOnError());
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    // Make sure it's an or
    testparser.setParsingError(false);
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    testparser.setParsingError(true);
    EXPECT_TRUE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    // Make sure it stays set
    testparser.setParsingError(false);
    EXPECT_TRUE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());

    // Validate clear function
    testparser.clearParsingError();
    EXPECT_FALSE(testparser.isParsingError());
    EXPECT_TRUE(testparser.isParserAbort());
}

TEST(TestParserBase,  ResizeFormatParams)
{
    const size_t testSize1 = 10;
    const size_t testSize2 = 20;
    const size_t testSize3 = 45;

    test_parser_base testparser;
    EXPECT_EQ(0, testparser.getMaxOptionLength());
    EXPECT_EQ(0, testparser.getOptionKeyWidth());
    EXPECT_EQ(80, testparser.getMaxColumnWidth());

    // Set size
    testparser.resizeMaxOptionLength(testSize1);
    EXPECT_EQ(testSize1, testparser.getMaxOptionLength());
    EXPECT_EQ(testSize1, testparser.getOptionKeyWidth());

    testparser.resizeMaxOptionLength(testSize2);
    EXPECT_EQ(testSize2, testparser.getMaxOptionLength());
    EXPECT_EQ(testSize2, testparser.getOptionKeyWidth());
    EXPECT_EQ(80, testparser.getMaxColumnWidth());
    EXPECT_EQ(80-testSize2-1, testparser.getHelpKeyWidth());

    // Make sure it's one way
    testparser.resizeMaxOptionLength(testSize1);
    EXPECT_EQ(testSize2, testparser.getMaxOptionLength());
    EXPECT_EQ(testSize2, testparser.getOptionKeyWidth());
    EXPECT_EQ(80, testparser.getMaxColumnWidth());
    EXPECT_EQ(80-testSize2-1, testparser.getHelpKeyWidth());

    // Make it large
    testparser.resizeMaxOptionLength(testSize3);
    EXPECT_EQ(testSize3, testparser.getMaxOptionLength());
    EXPECT_EQ(40, testparser.getOptionKeyWidth());
    EXPECT_EQ(80, testparser.getMaxColumnWidth());
    EXPECT_EQ(39, testparser.getHelpKeyWidth());

}
/** @} */
