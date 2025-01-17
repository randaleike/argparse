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
 * @file varg_clib_test.cpp
 * @ingroup argparser_unittest
 * @{
 */

// Includes
#include <cmath>
#include "varg_clib.h"
#include "varg_clib_shared.h"
#include "../src/varg_clib_private.h"
#include <gtest/gtest.h>


//==================================================================
// Basic test
//==================================================================
template <typename T> class CreateVargApi : public testing::Test
{
    protected:
        T           cvarg;              // NOLINT
        argHandle   cvarghandle;        // NOLINT

    public:
        CreateVargApi() : cvarghandle(nullptr), cvarg(0) {}
        CreateVargApi(const CreateVargApi& other) = default;
        CreateVargApi(CreateVargApi&& other) = default;
        CreateVargApi& operator=(const CreateVargApi& other) = default;
        CreateVargApi& operator=(CreateVargApi&& other) = default;

        ~CreateVargApi() override {releaseParserArg(cvarghandle);}
        T getVargValue() {return(cvarg);}
};

template <> CreateVargApi<short int>::CreateVargApi() : cvarg(-1), cvarghandle(createShortIntParserArg(&cvarg)) {}
template <> CreateVargApi<int>::CreateVargApi() : cvarg(-1), cvarghandle(createIntParserArg(&cvarg))            {}
template <> CreateVargApi<long int>::CreateVargApi() : cvarg(-1), cvarghandle(createLongIntParserArg(&cvarg))   {}
//template <> CreateVargApi<long long int>::CreateVargApi() : cvarg(-1), cvarghandle(createLLongIntParserArg(&cvarg)) {}

template <> CreateVargApi<short unsigned>::CreateVargApi() : cvarg(1), cvarghandle(createShortUIntParserArg(&cvarg)) {}
template <> CreateVargApi<unsigned>::CreateVargApi() : cvarg(1), cvarghandle(createUIntParserArg(&cvarg))            {}
template <> CreateVargApi<long unsigned>::CreateVargApi() : cvarg(1), cvarghandle(createLongUIntParserArg(&cvarg))   {}
//template <> CreateVargApi<long long unsigned>::CreateVargApi() : cvarg(1), cvarghandle(createLLongUIntParserArg(&cvarg)) {}

template <> CreateVargApi<double>::CreateVargApi() : cvarg(3.14159265358979323846), cvarghandle(createFloatParserArg(&cvarg))     {}
template <> CreateVargApi<char>::CreateVargApi() : cvarg('a'), cvarghandle(createCharParserArg(&cvarg))         {}
template <> CreateVargApi<bool>::CreateVargApi() : cvarg(false), cvarghandle(createParserFlagArg(&cvarg, true)) {}

TYPED_TEST_SUITE_P(CreateVargApi);

TYPED_TEST_P(CreateVargApi, Createvar)
{
    EXPECT_NE(nullptr, this->cvarghandle) << "Argument handle error";
    EXPECT_NE(nullptr, this->cvarghandle->vararg) << "Varg pointer error";
    EXPECT_EQ(this->cvarg, this->getVargValue()) << "Value initialization error";
}

REGISTER_TYPED_TEST_SUITE_P(CreateVargApi, Createvar);

typedef testing::Types<short int, int, long int, short unsigned, unsigned, long unsigned, char, double> intTypes;   //NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(CreateIntArgVar, CreateVargApi, intTypes);


//==================================================================
// List arg test
//==================================================================
#define TEST_ARRAY_COUNT    (5)

template <typename T> class CreateListVargApi : public testing::Test
{
    protected:
        argHandle   cvarghandle;                    // NOLINT
        T           cvarg[TEST_ARRAY_COUNT] = {0,0,0,0,0};        // NOLINT

    public:
        CreateListVargApi();
        CreateListVargApi(const CreateListVargApi& other) = default;
        CreateListVargApi(CreateListVargApi&& other) = default;
        CreateListVargApi& operator=(const CreateListVargApi& other) = default;
        CreateListVargApi& operator=(CreateListVargApi&& other) = default;

        ~CreateListVargApi() override {releaseParserArg(cvarghandle);}

        T getVargValue(size_t index)  {return cvarg[index];}    // NOLINT
};

template <> CreateListVargApi<short int>::CreateListVargApi() : cvarghandle(createShortIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<int>::CreateListVargApi() : cvarghandle(createIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<long int>::CreateListVargApi() : cvarghandle(createLongIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<long long int>::CreateListVargApi() : cvarghandle(createLLongIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<short unsigned>::CreateListVargApi() : cvarghandle(createShortUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<unsigned>::CreateListVargApi() : cvarghandle(createUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<long unsigned>::CreateListVargApi() : cvarghandle(createLongUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
//template <> CreateListVargApi<long long unsigned>::CreateListVargApi() : cvarghandle(createLLongUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}
template <> CreateListVargApi<double>::CreateListVargApi() : cvarghandle(createDoubleArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT)) {}


TYPED_TEST_SUITE_P(CreateListVargApi);

TYPED_TEST_P(CreateListVargApi, Createvar)
{
    EXPECT_NE(nullptr, this->cvarghandle) << "Argument handle error";
    EXPECT_NE(nullptr, this->cvarghandle->vararg) << "Varg pointer error";
}

REGISTER_TYPED_TEST_SUITE_P(CreateListVargApi, Createvar);

typedef testing::Types<short int, int, long int, long long int, short unsigned, unsigned, long unsigned, double> listIntTypes;  //NOLINT
INSTANTIATE_TYPED_TEST_SUITE_P(CreateArrayArgVar, CreateListVargApi, listIntTypes);

/** @} */
