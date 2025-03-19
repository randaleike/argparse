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
 * @brief Simplified Chinese string list
 */
class ParserStringListInterfaceChineseSimplified final : public ParserStringListInterface
{
    public:
        ParserStringListInterfaceChineseSimplified() = default;
        ParserStringListInterfaceChineseSimplified(const ParserStringListInterfaceChineseSimplified& other) = default;
        ParserStringListInterfaceChineseSimplified(ParserStringListInterfaceChineseSimplified&& other) = default;
        ParserStringListInterfaceChineseSimplified& operator=(const ParserStringListInterfaceChineseSimplified& other) = default;
        ParserStringListInterfaceChineseSimplified& operator=(ParserStringListInterfaceChineseSimplified&& other) = default;
         ~ParserStringListInterfaceChineseSimplified() final = default;

        parserstr getLangIsoCode() final {return("zh");}

        // General argument parsing messages
        parserstr getNotListTypeMessage(int nargs) final
        {parser_str_stream parserstr;  parserstr << "只有列表类型的参数可以有一个参数计数 " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "未知参数 " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 无效分配"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" 分配失败"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少赋值"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少任务。 预期的： " << expected << " 成立: " << found << " 论据"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 分配值过多。 预期的： " << expected << " 成立: " << found << " 论据"; return parserstr.str();}

        parserstr getMissingArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少必要的参数"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "参数添加失败： " << keyString; return parserstr.str();}

        // Command Line parser messages
        [[nodiscard]] parserstr getUsageMessage() const final
        {return "用法：";}

        [[nodiscard]] parserstr getPositionalArgumentsMessage() const final
        {return "位置参数：";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const final
        {return "可选参数：";}

        [[nodiscard]] parserstr getHelpString() const final
        {return "显示此帮助信息并退出";}

        // Environment parser messages
        parserstr getEnvArgumentsMessage() final
        {return "环境值：";}

        parserstr getEnvironmentNoFlags(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "环境价值\"" << argKey << "\" narg 必须 > 0"; return parserstr.str();}

        parserstr getRequiredEnvironmentArgMissing(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "必须定义环境值: " << argKey; return parserstr.str();}

        // JSON file parser messages
        parserstr getJsonArgumentsMessage() final
        {return "可用的 JSON 参数值：";}

        // XML file parser messages
        parserstr getXmlArgumentsMessage() final
        {return "可用的 XML 参数值：";}
};
