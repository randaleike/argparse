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
 * @brief Spanish string list
 */
class ParserStringListInterfaceSpanish final : public ParserStringListInterface
{
    public:
        ParserStringListInterfaceSpanish() = default;
        ParserStringListInterfaceSpanish(const ParserStringListInterfaceSpanish& other) = default;
        ParserStringListInterfaceSpanish(ParserStringListInterfaceSpanish&& other) = default;
        ParserStringListInterfaceSpanish& operator=(const ParserStringListInterfaceSpanish& other) = default;
        ParserStringListInterfaceSpanish& operator=(ParserStringListInterfaceSpanish&& other) = default;
         ~ParserStringListInterfaceSpanish() final = default;

        // General argument parsing messages
        parserstr getNotListTypeMessage(int nargs) final
        {parser_str_stream parserstr;  parserstr << "Solo los argumentos de tipo lista pueden tener un recuento de argumentos de " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "Argumento desconocidoreturn " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" asignación inválida"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" asignación fallida"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" valor de asignación faltante"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" tarea faltante. Esperado: " << expected << " encontrado: " << found << " argumentos"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" demasiados valores de asignación. Esperado: " << expected << " encontrado: " << found << " argumentos"; return parserstr.str();}

        parserstr getMissingArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" falta el argumento requerido"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "No se pudo agregar el argumento: " << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const final
        {return "Uso:";}

        // Command Line parser messages
        [[nodiscard]] parserstr getPositionalArgumentsMessage() const final
        {return "Argumentos posicionales:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const final
        {return "Argumentos opcionales:";}

        [[nodiscard]] parserstr getHelpString() const final
        {return "mostrar este mensaje de ayuda y salir";}

        // Environment parser messages
        parserstr getEnvArgumentsMessage() final
        {return "Valores ambientales:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "Valores ambiental " << argKey << " narg debe ser > 0"; return parserstr.str();}

        // JSON file parser messages
        parserstr getJsonArgumentsMessage() final
        {return "Valores de argumentos JSON disponibles:";}

        // XML file parser messages
        parserstr getXmlArgumentsMessage() final
        {return "Valores de argumentos XML disponibles:";}
};
