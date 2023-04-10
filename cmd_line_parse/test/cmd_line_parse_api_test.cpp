/*
Copyright (c) 2022 Randal Eike

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
 * @file cmd_line_parse_api_test.cpp
 * @ingroup argparser_unittest
 * @defgroup Argument parsing utilities unit test
 * @{
 */

// Includes
#include "cmd_line_parse_api.h"
#include <gtest/gtest.h>

template <typename T> class CreateVargApi : public testing::Test
{
    public:
        CreateVargApi() {}
        ~CreateVargApi() override {}
};

TYPED_TEST_SUITE_P(CreateVargApi);

TYPED_TEST_P(CreateVargApi, Createvar)
{
    TypeParam argVar;
    argHandle testvar = createShortIntParserArg(&argVar);
    EXPECT_NE(nullptr, testvar);
}

REGISTER_TYPED_TEST_SUITE_P(CreateVargApi, Createvar);

typedef testing::Types<short int> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(CreateArgVar, CreateVargApi, intTypes);

/** @} */
