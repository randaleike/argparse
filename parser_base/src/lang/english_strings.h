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

// Includes
#include <cstring>
#include <string>
#include <sstream>
#include "parser_string_list.h"

using namespace argparser;
using parser_str_stream = std::stringstream;

/**
 * @brief Englist string list
 */
class ParserStringListInterfaceEnglish final : public ParserStringListInterface
{
    public:
        ParserStringListInterfaceEnglish() = default;
        ParserStringListInterfaceEnglish(const ParserStringListInterfaceEnglish& other) = default;
        ParserStringListInterfaceEnglish(ParserStringListInterfaceEnglish&& other) = default;
        ParserStringListInterfaceEnglish& operator=(const ParserStringListInterfaceEnglish& other) = default;
        ParserStringListInterfaceEnglish& operator=(ParserStringListInterfaceEnglish&& other) = default;
        ~ParserStringListInterfaceEnglish() final = default;

        // General argument parsing messages
        parserstr getNotListTypeMessage(int nargs) final
        {parser_str_stream parserstr;  parserstr << "Only list type arguments can have an argument count of " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "Unknown argument " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" invalid assignment"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" assignment failed"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" missing assignment value"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" missing assignment. Expected: " << expected << " found: " << found << " arguments"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" too many assignment values. Expected: " << expected << " found: " << found << " arguments"; return parserstr.str();}

        parserstr getMissingArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" required argument missing"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "Argument add failed: " << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const final
        {return "Usage:";}

        // Command Line parser messages
        [[nodiscard]] parserstr getPositionalArgumentsMessage() const final
        {return "Positional Arguments:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const final
        {return "Optional Arguments:";}

        [[nodiscard]] parserstr getHelpString() const final
        {return "show this help message and exit";}

        // Environment parser messages
        parserstr getEnvArgumentsMessage() final
        {return "Environment values:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "Environment value " << argKey << " narg must be > 0"; return parserstr.str();}

        parserstr getRequiredEnvironmentArgMissing(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "Environment value " << argKey << " must be defined"; return parserstr.str();}

        // JSON file parser messages
        parserstr getJsonArgumentsMessage() final
        {return "Available JSON argument values:";}

        // XML file parser messages
        parserstr getXmlArgumentsMessage() final
        {return "Available XML argument values:";}
};
