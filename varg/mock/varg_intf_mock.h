/*
 Copyright (c) 2025 Randal Eike

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
 * @file varg_intf_mock.h
 * @defgroup libvarg Argument value storage gtest mock implementation
 * @{
 */

#pragma once

// Includes
#include <cstdlib>
#include <climits>
#include <limits>
#include <string>
#include <gmock/gmock.h>
#include "varg_intf.h"

namespace argparser
{

/**
* @brief Base variable argument varg interface and methods
*/
class mock_varg_intf : public varg_intf
{
    public:
        mock_varg_intf() = default;
        mock_varg_intf(const mock_varg_intf& other) = delete;
        mock_varg_intf(mock_varg_intf&& other) = delete;
        mock_varg_intf& operator=(const mock_varg_intf& other) = delete;
        mock_varg_intf& operator=(mock_varg_intf&& other) = delete;
        ~mock_varg_intf() override = default;

        MOCK_METHOD(const char*, getTypeString, (), (override));
        MOCK_METHOD(const char*, getRangeString, (), (override));
        MOCK_METHOD(bool, isList, (), (const, override));
        MOCK_METHOD(valueParseStatus_e, setValue, (const char* newValue), (override));
        MOCK_METHOD(valueParseStatus_e, setValue, (), (override));
        MOCK_METHOD(bool, isEmpty, (), (override));
        MOCK_METHOD(size_t, getAssignmentCount, (), (override));

        // Private/protected methods made public for mock
        MOCK_METHOD(void, setMinMaxSigned, (long long int min, long long int max));
        MOCK_METHOD(void, setMinMaxUnsigned, (long long unsigned min, long long unsigned max));
        MOCK_METHOD(void, setMinMaxDouble, (double min, double max));
        MOCK_METHOD(void, setTypeString, (typeStringFormat_e fmtType));

        MOCK_METHOD(valueParseStatus_e, getBoolValue, (const char* newValue, bool& parsedValue));
        MOCK_METHOD(valueParseStatus_e, getCharValue, (const char* newValue, char& parsedValue));
        MOCK_METHOD(valueParseStatus_e, getSignedValue, (const char* newValue, long long int &parsedValue), (const));
        MOCK_METHOD(valueParseStatus_e, getUnsignedValue, (const char* newValue, long long unsigned &parsedValue), (const));
        MOCK_METHOD(valueParseStatus_e, getDoubleValue, (const char* newValue, double& parsedValue), (const));

        MOCK_METHOD(const char*, getTypeString, (), (const));
        MOCK_METHOD(const char*, getRangeString, (), (const));

};

}; // end of namespace argparser

/** @} */
