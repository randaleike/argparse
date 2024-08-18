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
 * @file cmd_line_parse_api_test.cpp
 * @ingroup libcmd_line_parser_unittest
 * @{
 */

// Includes
#include "cmd_line_parse_api.h"
#include "../src/cmd_line_parse_api_lib_private.h"
#include <gtest/gtest.h>
#include <math.h>

//==================================================================
// Basic test
//==================================================================
template <typename T> class CreateVargApi : public testing::Test
{
    public:
        T           cvarg;
        argHandle   cvarghandle;

        CreateVargApi() {}
        ~CreateVargApi() override {releaseParserArg(cvarghandle);}

        T getVargValue(void);
        argVarType_e getVargType(void);

};

template <> CreateVargApi<short int>::CreateVargApi()           {cvarg = -1; cvarghandle = createShortIntParserArg(&cvarg);}
template <> CreateVargApi<int>::CreateVargApi()                 {cvarg = -1; cvarghandle = createIntParserArg(&cvarg);}
template <> CreateVargApi<long int>::CreateVargApi()            {cvarg = -1; cvarghandle = createLongIntParserArg(&cvarg);}
//template <> CreateVargApi<long long int>::CreateVargApi()       {cvarg = -1; cvarghandle = createLLongIntParserArg(&cvarg);}

template <> CreateVargApi<short unsigned>::CreateVargApi()      {cvarg = 1; cvarghandle = createShortUIntParserArg(&cvarg);}
template <> CreateVargApi<unsigned>::CreateVargApi()            {cvarg = 1; cvarghandle = createUIntParserArg(&cvarg);}
template <> CreateVargApi<long unsigned>::CreateVargApi()       {cvarg = 1; cvarghandle = createLongUIntParserArg(&cvarg);}
//template <> CreateVargApi<long long unsigned>::CreateVargApi()  {cvarg = 1; cvarghandle = createLLongUIntParserArg(&cvarg);}

template <> CreateVargApi<double>::CreateVargApi()              {cvarg = M_PI; cvarghandle = createFloatParserArg(&cvarg);}
template <> CreateVargApi<char>::CreateVargApi()                {cvarg = 'a'; cvarghandle = createCharParserArg(&cvarg);}
template <> CreateVargApi<bool>::CreateVargApi()                {cvarg = false; cvarghandle = createParserFlagArg(false, true, &cvarg);}

template <> short int           CreateVargApi<short int>::getVargValue(void)            {return (cvarghandle->argptr.shortintArg->value);}
template <> int                 CreateVargApi<int>::getVargValue(void)                  {return (cvarghandle->argptr.intArg->value);}
template <> long int            CreateVargApi<long int>::getVargValue(void)             {return (cvarghandle->argptr.longintArg->value);}
//template <> long long int       CreateVargApi<long long int>::getVargValue(void)        {return (cvarghandle->argptr.llintArg->value);}

template <> short unsigned      CreateVargApi<short unsigned >::getVargValue(void)      {return (cvarghandle->argptr.shortuintArg->value);}
template <> unsigned            CreateVargApi<unsigned>::getVargValue(void)             {return (cvarghandle->argptr.uintArg->value);}
template <> long unsigned       CreateVargApi<long unsigned>::getVargValue(void)        {return (cvarghandle->argptr.longuintArg->value);}
//template <> long long unsigned  CreateVargApi<long long unsigned>::getVargValue(void)   {return (cvarghandle->argptr.lluintArg->value);}

template <> double              CreateVargApi<double>::getVargValue(void)               {return (cvarghandle->argptr.floatArg->value);}
template <> char                CreateVargApi<char>::getVargValue(void)                 {return (cvarghandle->argptr.charArg->value);}
template <> bool                CreateVargApi<bool>::getVargValue(void)                 {return (cvarghandle->argptr.boolArg->value);}

template <> argVarType_e        CreateVargApi<short int>::getVargType(void)             {return (argVarType_e::VT_SHORT_INT);}
template <> argVarType_e        CreateVargApi<int>::getVargType(void)                   {return (argVarType_e::VT_INTEGER);}
template <> argVarType_e        CreateVargApi<long int>::getVargType(void)              {return (argVarType_e::VT_LONG_INT);}
//template <> argVarType_e        CreateVargApi<long long int>::getVargType(void)         {return (argVarType_e::VT_LLONG_INT);}

template <> argVarType_e        CreateVargApi<short unsigned>::getVargType(void)        {return (argVarType_e::VT_SHORT_UINT);}
template <> argVarType_e        CreateVargApi<unsigned>::getVargType(void)              {return (argVarType_e::VT_UINTEGER);}
template <> argVarType_e        CreateVargApi<long unsigned>::getVargType(void)         {return (argVarType_e::VT_LONG_UINT);}
//template <> argVarType_e        CreateVargApi<long long unsigned>::getVargType(void)    {return (argVarType_e::VT_LLONG_UINT);}

template <> argVarType_e        CreateVargApi<double>::getVargType(void)                {return (argVarType_e::VT_FLOAT);}
template <> argVarType_e        CreateVargApi<char>::getVargType(void)                  {return (argVarType_e::VT_CHAR);}
template <> argVarType_e        CreateVargApi<bool>::getVargType(void)                  {return (argVarType_e::VT_BOOL);}

TYPED_TEST_SUITE_P(CreateVargApi);

TYPED_TEST_P(CreateVargApi, Createvar)
{
    EXPECT_NE(nullptr, this->cvarghandle) << "Argument handle error";
    EXPECT_NE(nullptr, this->cvarghandle->argptr.vargIntf) << "Varg pointer error";
    EXPECT_EQ(&(this->cvarg), this->cvarghandle->cStoragePtr.retValue) << "C arg pointer error";
    EXPECT_EQ(this->cvarg, this->getVargValue()) << "Value initialization error";
    EXPECT_EQ(1, this->cvarghandle->elementCount);
    EXPECT_EQ(this->getVargType(), this->cvarghandle->type);
}

REGISTER_TYPED_TEST_SUITE_P(CreateVargApi, Createvar);

//typedef testing::Types<short int, int, long int, long long int, short unsigned, unsigned, long unsigned, long long unsigned, char, double> intTypes;
typedef testing::Types<short int, int, long int, short unsigned, unsigned, long unsigned, char, double> intTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(CreateIntArgVar, CreateVargApi, intTypes);


//==================================================================
// List arg test
//==================================================================
#define TEST_ARRAY_COUNT    (5)

template <typename T> class CreateListVargApi : public testing::Test
{
    public:
        CreateListVargApi() {}
        ~CreateListVargApi() override {releaseParserArg(cvarghandle);}

        argHandle   cvarghandle;
        T           cvarg[TEST_ARRAY_COUNT];

        std::list<T> getVargValue(void);
        argVarType_e getVargType(void);
};

template <> CreateListVargApi<short int>::CreateListVargApi()           {cvarghandle = createShortIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
template <> CreateListVargApi<int>::CreateListVargApi()                 {cvarghandle = createIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
template <> CreateListVargApi<long int>::CreateListVargApi()            {cvarghandle = createLongIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
template <> CreateListVargApi<long long int>::CreateListVargApi()       {cvarghandle = createLLongIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}

template <> CreateListVargApi<short unsigned>::CreateListVargApi()      {cvarghandle = createShortUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
template <> CreateListVargApi<unsigned>::CreateListVargApi()            {cvarghandle = createUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
template <> CreateListVargApi<long unsigned>::CreateListVargApi()       {cvarghandle = createLongUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}
//template <> CreateListVargApi<long long unsigned>::CreateListVargApi()  {cvarghandle = createLLongUIntArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}

template <> CreateListVargApi<double>::CreateListVargApi()              {cvarghandle = createDoubleArrayParserArg(&(cvarg[0]), TEST_ARRAY_COUNT);}

template <> std::list<short int>           CreateListVargApi<short int>::getVargValue(void)            {return (cvarghandle->argptr.shortintArrayArg->value);}
template <> std::list<int>                 CreateListVargApi<int>::getVargValue(void)                  {return (cvarghandle->argptr.intArrayArg->value);}
template <> std::list<long int>            CreateListVargApi<long int>::getVargValue(void)             {return (cvarghandle->argptr.longintArrayArg->value);}
template <> std::list<long long int>       CreateListVargApi<long long int>::getVargValue(void)        {return (cvarghandle->argptr.llintArrayArg->value);}

template <> std::list<short unsigned>      CreateListVargApi<short unsigned >::getVargValue(void)      {return (cvarghandle->argptr.shortuintArrayArg->value);}
template <> std::list<unsigned>            CreateListVargApi<unsigned>::getVargValue(void)             {return (cvarghandle->argptr.uintArrayArg->value);}
template <> std::list<long unsigned>       CreateListVargApi<long unsigned>::getVargValue(void)        {return (cvarghandle->argptr.longuintArrayArg->value);}
//template <> std::list<long long unsigned>  CreateListVargApi<long long unsigned>::getVargValue(void)   {return (cvarghandle->argptr.lluintArrayArg->value);}

template <> std::list<double>              CreateListVargApi<double>::getVargValue(void)               {return (cvarghandle->argptr.floatArrayArg->value);}

template <> argVarType_e        CreateListVargApi<short int>::getVargType(void)             {return (argVarType_e::VT_SHORT_INT);}
template <> argVarType_e        CreateListVargApi<int>::getVargType(void)                   {return (argVarType_e::VT_INTEGER);}
template <> argVarType_e        CreateListVargApi<long int>::getVargType(void)              {return (argVarType_e::VT_LONG_INT);}
template <> argVarType_e        CreateListVargApi<long long int>::getVargType(void)         {return (argVarType_e::VT_LLONG_INT);}

template <> argVarType_e        CreateListVargApi<short unsigned>::getVargType(void)        {return (argVarType_e::VT_SHORT_UINT);}
template <> argVarType_e        CreateListVargApi<unsigned>::getVargType(void)              {return (argVarType_e::VT_UINTEGER);}
template <> argVarType_e        CreateListVargApi<long unsigned>::getVargType(void)         {return (argVarType_e::VT_LONG_UINT);}
//template <> argVarType_e        CreateListVargApi<long long unsigned>::getVargType(void)    {return (argVarType_e::VT_LLONG_UINT);}

template <> argVarType_e        CreateListVargApi<double>::getVargType(void)                {return (argVarType_e::VT_FLOAT);}

TYPED_TEST_SUITE_P(CreateListVargApi);

TYPED_TEST_P(CreateListVargApi, Createvar)
{
    EXPECT_NE(nullptr, this->cvarghandle) << "Argument handle error";
    EXPECT_NE(nullptr, this->cvarghandle->argptr.vargIntf) << "Varg pointer error";
    EXPECT_EQ(&(this->cvarg), this->cvarghandle->cStoragePtr.retValue) << "C arg pointer error";
    EXPECT_EQ(TEST_ARRAY_COUNT, this->cvarghandle->elementCount);
    EXPECT_EQ(this->getVargType(), this->cvarghandle->type);
}

REGISTER_TYPED_TEST_SUITE_P(CreateListVargApi, Createvar);

//typedef testing::Types<short int, int, long int, long long int, short unsigned, unsigned, long unsigned, long long unsigned, double> listIntTypes;
typedef testing::Types<short int, int, long int, long long int, short unsigned, unsigned, long unsigned, double> listIntTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(CreateArrayArgVar, CreateListVargApi, listIntTypes);

/** @} */
