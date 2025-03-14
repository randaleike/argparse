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
 * @brief French string list
 */
class ParserStringListInterfaceFrench final : public ParserStringListInterface
{
    public:
        ParserStringListInterfaceFrench() = default;
        ParserStringListInterfaceFrench(const ParserStringListInterfaceFrench& other) = default;
        ParserStringListInterfaceFrench(ParserStringListInterfaceFrench&& other) = default;
        ParserStringListInterfaceFrench& operator=(const ParserStringListInterfaceFrench& other) = default;
        ParserStringListInterfaceFrench& operator=(ParserStringListInterfaceFrench&& other) = default;
         ~ParserStringListInterfaceFrench() final = default;

        parserstr getLangIsoCode() final {return("fr-FR");}

         // General argument parsing messages
        parserstr getNotListTypeMessage(int nargs) final
        {parser_str_stream parserstr;  parserstr << "Seuls les arguments de type liste peuvent avoir un nombre d'arguments de " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "Argument inconnu " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" affectation invalide"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(parserstr keyString, parserstr valueString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" l'affectation a échoué"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" valeur d'affectation manquante"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" devoir manquant. Attendu: " << expected << " trouvé : " << found << " arguments"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(parserstr keyString, size_t expected, size_t found) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" trop de valeurs d'affectation. Attendu: " << expected << " trouvé : " << found << " arguments"; return parserstr.str();}

        parserstr getMissingArgumentMessage(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" argument obligatoire manquant"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) final
        {parser_str_stream parserstr;  parserstr << "Échec de l'ajout d'arguments:" << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const final
        {return "Usage:";}

        // Command Line parser messages
        [[nodiscard]] parserstr getPositionalArgumentsMessage() const final
        {return "Arguments positionnels:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const final
        {return "Arguments facultatifs:";}

        [[nodiscard]] parserstr getHelpString() const final
        {return "afficher ce message d'aide et quitter";}

        // Environment parser messages
        parserstr getEnvArgumentsMessage() final
        {return "Valeurs environnementales:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "Valeurs environnementale " << argKey << " narg doit être > 0"; return parserstr.str();}

        parserstr getRequiredEnvironmentArgMissing(parserstr argKey) final
        {parser_str_stream parserstr;  parserstr << "La valeur d'environnement " << argKey << " doit être définie"; return parserstr.str();}

        // JSON file parser messages
        parserstr getJsonArgumentsMessage() final
        {return "Valeurs d'argument JSON disponibles :";}

        // XML file parser messages
        parserstr getXmlArgumentsMessage() final
        {return "Valeurs d'argument XML disponibles :";}
};
