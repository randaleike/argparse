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
 * @file cmd_line_parse_lib_api.cpp
 * @ingroup libcmd_line_parser
 * @{
 */

#pragma once

#include "varg.h"
#include "vargincrement.h"
#include "vargenum.h"
#include "listvarg.h"
#include "cmd_line_parse.h"
#include "cmd_line_parse_api.h"

//=================================================================================================
//========================= Constant definitions ==================================================
//=================================================================================================
enum class argVarType_e
{
    VT_INVALID = 0,
    VT_SHORT_INT,
    VT_INTEGER,
    VT_LONG_INT,
    VT_LLONG_INT,
    VT_SHORT_UINT,
    VT_UINTEGER,
    VT_LONG_UINT,
    VT_LLONG_UINT,
    VT_FLOAT,
    VT_BOOL,
    VT_CHAR,
    VT_STRING_ARRAY,
    VT_INCREMENTING,
};

//=================================================================================================
//========================= Structure definitions =================================================
//=================================================================================================
struct argData
{
    union storagePtr_u
    {
        short int*                                  shortIntVal;
        int*                                        integerVal;
        long int*                                   longIntVal;
        long long int*                              llIntVal;
        short unsigned*                             shortUIntVal;
        unsigned*                                   unsignedVal;
        long unsigned*                              longUIntVal;
        long long unsigned*                         llUIntVal;
        char*                                       charVal;
        bool*                                       flagVal;
        double*                                     floatVal;
        char**                                      stringArray;
        void*                                       retValue;
    } cStoragePtr;                                                          ///< Pointer to the C type storage
    union vargTypes_u
    {
        argparser::varg<short int>*                 shortintArg;
        argparser::varg<int>*                       intArg;
        argparser::varg<long int>*                  longintArg;
        argparser::varg<long long int>*             llintArg;
        argparser::varg<short unsigned>*            shortuintArg;
        argparser::varg<unsigned>*                  uintArg;
        argparser::varg<long unsigned>*             longuintArg;
        argparser::varg<long long unsigned>*        lluintArg;
        argparser::varg<double>*                    floatArg;
        argparser::varg<char>*                      charArg;
        argparser::varg<bool>*                      boolArg;
        argparser::varg<std::string>*               stringArg;
        argparser::vargincrement*                   incrementArg;
        argparser::listvarg<short int>*             shortintArrayArg;
        argparser::listvarg<int>*                   intArrayArg;
        argparser::listvarg<long int>*              longintArrayArg;
        argparser::listvarg<long long int>*         llintArrayArg;
        argparser::listvarg<short unsigned>*        shortuintArrayArg;
        argparser::listvarg<unsigned>*              uintArrayArg;
        argparser::listvarg<long unsigned>*         longuintArrayArg;
        argparser::listvarg<long long unsigned>*    lluintArrayArg;
        argparser::listvarg<double>*                floatArrayArg;
        argparser::listvarg<char>*                  charArrayArg;
        argparser::listvarg<bool>*                  boolArrayArg;
        argparser::listvarg<std::string>*           stringArrayArg;
        argparser::varg_intf*                       vargIntf;
    } argptr;                                                               ///<  Pointer to the varg item

    int                                             elementCount;           ///< Maximum number of elements to update
    argVarType_e                                    type;                   ///< Maximum number of elements to update
};

/**
 * @brief Object c dereference
 */
struct cmdLineParser
{
    argparser::cmd_line_parse*  object;             ///< Command line parser object
    std::list<argData*>         argList;            ///< List of varg objects assigned to the parser
};

/** @} */
