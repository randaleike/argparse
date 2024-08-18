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
 * @file parser_string_list.cpp
 * @ingroup argparser
 * @defgroup Internationalized string list for all parsers
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include <sstream>
#include "parser_string_list.h"
#if defined(DYNAMIC_INTERNATIONALIZATION)
  #if defined(__linux__) || defined(__unix__)
    #include <cstdlib>
  #elif defined(_WIN64) || defined(_WIN32)
    #include <Windows.h>
  #else
    #error "Unknown OS type"
  #endif
#endif

using namespace argparser;
using parser_str_stream = std::stringstream;

#if defined(ENGLISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Englist string list
 */
class BaseParserStringListEnglish : public BaseParserStringList
{
    public:
        parserstr getNotListTypeMessage(int nargs) override
        {parser_str_stream parserstr;  parserstr << "Only list type arguments can have an argument count of " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "Unkown argument " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" invalid assignment"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(const parserstr keyString, parserstr valueString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" assignment failed"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" missing assignment value"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" missing assignment. Expected: " << expected << " found: " << found << " arguments"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" too many assignment values. Expected: " << expected << " found: " << found << " arguments"; return parserstr.str();}

        parserstr getMissingArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" required argument missing"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "Argument add failed: " << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const override
        {return "Usage:";}

        [[nodiscard]] parserstr getPositionalArgumentsMessage() const override
        {return "Positional Arguments:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const override
        {return "Optional Arguments:";}

        [[nodiscard]] parserstr getHelpString() const override
        {return "show this help message and exit";}

        parserstr getEnvArgumentsMessage() override
        {return "Environment values:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) override
        {parser_str_stream parserstr;  parserstr << "Environment value " << argKey << " narg must be > 0"; return parserstr.str();}
};
#endif

#if defined(SPANISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Spanish string list
 */
class BaseParserStringListSpanish : public BaseParserStringList
{
    public:
        parserstr getNotListTypeMessage(int nargs) override
        {parser_str_stream parserstr;  parserstr << "Solo los argumentos de tipo lista pueden tener un recuento de argumentos de " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "Argumento desconocidoreturn " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" asignación inválida"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(const parserstr keyString, parserstr valueString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" asignación fallida"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" valor de asignación faltante"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" tarea faltante. Esperado: " << expected << " encontrado: " << found << " argumentos"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" demasiados valores de asignación. Esperado: " << expected << " encontrado: " << found << " argumentos"; return parserstr.str();}

        parserstr getMissingArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" falta el argumento requerido"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "No se pudo agregar el argumento: " << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const override
        {return "Uso:";}

        [[nodiscard]] parserstr getPositionalArgumentsMessage() const override
        {return "Argumentos posicionales:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const override
        {return "Argumentos opcionales:";}

        [[nodiscard]] parserstr getHelpString() const override
        {return "mostrar este mensaje de ayuda y salir";}

        parserstr getEnvArgumentsMessage() override
        {return "Valores ambientales:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) override
        {parser_str_stream parserstr;  parserstr << "Valores ambiental " << argKey << " narg debe ser > 0"; return parserstr.str();}
};
#endif

#if defined(FRENCH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief French string list
 */
class BaseParserStringListFrench : public BaseParserStringList
{
    public:
        parserstr getNotListTypeMessage(int nargs) override
        {parser_str_stream parserstr;  parserstr << "Seuls les arguments de type liste peuvent avoir un nombre d'arguments de " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "Argument inconnu " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" affectation invalide"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(const parserstr keyString, parserstr valueString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" l'affectation a échoué"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" valeur d'affectation manquante"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" devoir manquant. Attendu: " << expected << " trouvé : " << found << " arguments"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" trop de valeurs d'affectation. Attendu: " << expected << " trouvé : " << found << " arguments"; return parserstr.str();}

        parserstr getMissingArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" argument obligatoire manquant"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "Échec de l'ajout d'arguments:" << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const override
        {return "Usage:";}

        [[nodiscard]] parserstr getPositionalArgumentsMessage() const override
        {return "Arguments positionnels:";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const override
        {return "Arguments facultatifs:";}

        [[nodiscard]] parserstr getHelpString() const override
        {return "afficher ce message d'aide et quitter";}

        parserstr getEnvArgumentsMessage() override
        {return "Valeurs environnementales:";}

        parserstr getEnvironmentNoFlags(parserstr argKey) override
        {parser_str_stream parserstr;  parserstr << "Valeurs environnementale " << argKey << " narg doit être > 0"; return parserstr.str();}
};
#endif

#if defined(CHINESE_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Simplified Chinese string list
 */
class BaseParserStringListChineseSimplified : public BaseParserStringList
{
    public:
        parserstr getNotListTypeMessage(int nargs) override
        {parser_str_stream parserstr;  parserstr << "只有列表类型的参数可以有一个参数计数 " << nargs; return parserstr.str();}

        parserstr getUnknownArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "未知参数 " << keyString; return parserstr.str();}

        parserstr getInvalidAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 无效分配"; return parserstr.str();}

        parserstr getAssignmentFailedMessage(const parserstr keyString, parserstr valueString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << " " << valueString << "\" 分配失败"; return parserstr.str();}

        parserstr getMissingAssignmentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少赋值"; return parserstr.str();}

        parserstr getMissingListAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少任务。 预期的： " << expected << " 成立: " << found << " 论据"; return parserstr.str();}

        parserstr getTooManyAssignmentMessage(const parserstr keyString, size_t expected, size_t found) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 分配值过多。 预期的： " << expected << " 成立: " << found << " 论据"; return parserstr.str();}

        parserstr getMissingArgumentMessage(const parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "\"" << keyString << "\" 缺少必要的参数"; return parserstr.str();}

        parserstr getArgumentCreationError(parserstr keyString) override
        {parser_str_stream parserstr;  parserstr << "参数添加失败： " << keyString; return parserstr.str();}

        [[nodiscard]] parserstr getUsageMessage() const override
        {return "用法：";}

        [[nodiscard]] parserstr getPositionalArgumentsMessage() const override
        {return "位置参数：";}

        [[nodiscard]] parserstr getSwitchArgumentsMessage() const override
        {return "可选参数：";}

        [[nodiscard]] parserstr getHelpString() const override
        {return "显示此帮助信息并退出";}

        parserstr getEnvArgumentsMessage() override
        {return "环境值：";}

        parserstr getEnvironmentNoFlags(parserstr argKey) override
        {parser_str_stream parserstr;  parserstr << "环境价值 " << argKey << " narg 必须 > 0"; return parserstr.str();}
};
#endif

//============================================================================================================================
//============================================================================================================================
//  Protected functions
//============================================================================================================================
//============================================================================================================================

/**
 * @brief Find the best position to break the sting given the input list of break characters to choose from
 *
 * @param workingString - Current working string
 * @param breakCharList - List of characters to position the break on
 * @param maxLength     - Maximum character length
 *
 * @return size_t  - best breack position found
 */
size_t BaseParserStringList::findBestBreakPos(parserstr workingString, std::list<parserchar> breakCharList, size_t maxLength) const
{
    std::list<size_t> breakPos;
    size_t            retVal = maxLength;

    if (!breakCharList.empty())
    {
        // Start with the first char in the list
        for (auto breakCharacter : breakCharList)
        {
            // Find the best break location
            size_t workingBreakPos = workingString.rfind(breakCharacter);
            if (debugMsgLevel > 4)
            {
                std::cerr << "Start break: " << workingBreakPos << std::endl;
                std::cerr << "Current break: " << workingBreakPos << std::endl;
            }

            while((workingBreakPos > maxLength) && (workingBreakPos != parserstr::npos))
            {
                // Search for the next break character
                workingBreakPos = workingString.rfind(breakCharacter, workingBreakPos-1);
                if (debugMsgLevel > 4)
                {
                    std::cerr << "Current break: " << workingBreakPos << std::endl;
                }
            }

            // Record the break position for that character
            if (workingBreakPos <= maxLength)
            {
                breakPos.push_back(workingBreakPos);
            }
        }

        // Sort the list and return the best break position
        if (!breakPos.empty())
        {
            // Return the break position character closest to the max length
            breakPos.sort();
            retVal = breakPos.back();
        }
    }

    return retVal;
}

//============================================================================================================================
//============================================================================================================================
//  Public non-functions
//============================================================================================================================
//============================================================================================================================

BaseParserStringList::BaseParserStringList() : defaultBreakList({' '}), debugMsgLevel(0) { }

/**
 * @brief Format the input string to the required width.  Break the string
 *        if longer than maxWidth at the nearest break charater.  Pad any
 *        output strings to the minwidth with spaces.
 *
 * @param baseString     - Input string
 * @param breakCharList  - Character list to break the string at if the input string is to be broken
 * @param maxLength      - maximum length of a string in the output list
 *
 * @return std::list<parserstr>
 */
std::list<parserstr> BaseParserStringList::formatStringToLength(parserstr baseString, std::list<parserchar> breakCharList, size_t maxLength)
{
    std::list<parserstr> returnList;
    parserstr workingString = baseString;
    size_t breakPos = parserstr::npos;

    if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug) 
    {
        std::cerr << "Start string: \"" << workingString << "\"" << std::endl;
    }

    while (workingString.length() > maxLength)
    {
        size_t breakPos = findBestBreakPos(workingString, breakCharList, maxLength);
        //Found a good breaking point
        if (workingString[breakPos] <= ' ')
        {
            returnList.push_back(workingString.substr(0, breakPos));
            workingString.erase(0, breakPos+1);
        }
        else
        {
            returnList.push_back(workingString.substr(0, breakPos));
            workingString.erase(0, breakPos);
        }

        if (debugMsgLevel > debugVerbosityLevel_e::verboseDebug) 
        {
            std::cerr << "Current string: \"" << workingString << "\"" << std::endl;
        }
    }

    // Add the last tail
    returnList.push_back(workingString);

    // Pad the string to the proper length
    for (auto & unpaddedString : returnList)
    {
        while (unpaddedString.length() < maxLength)
        {
            unpaddedString += ' ';
        }
    }

    return returnList;
}

/**
 * @brief Set the Debug Msg Level
 *
 * @param msgLevel - Debug level, normal = 0
 */
void BaseParserStringList::setDebugMsgLevel(int msgLevel)
{
    debugMsgLevel = msgLevel;
}

/**
 * @brief Get the specialized international string class based on the current OS language setting
 *
 * @return BaseParserStringList* - Pointer to the specialized language string table
 */
BaseParserStringList* BaseParserStringList::getInternationalizedClass()
{
#if defined(DYNAMIC_INTERNATIONALIZATION)
    BaseParserStringList* parserLang;
 #if defined(__linux__) || defined(__unix__)
    const parserchar* langSetting = getenv("LANG");
    if (nullptr != langSetting)
    {
        parserstr langString = langSetting;
        if (langString == "en_US.UTF-8")
        {
            parserLang = new BaseParserStringListEnglish;
        }
        else if (langString == "en_UK.UTF-8")
        {
            parserLang = new BaseParserStringListEnglish;
        }
        else if (langString == "es_ES.UTF-8")
        {
            parserLang = new BaseParserStringListSpanish;
        }
        else if (langString == "fr_FR.UTF-8")
        {
            parserLang = new BaseParserStringListFrench;
        }
        else if (langString == "zh_cn_utf8.UTF-8")
        {
            parserLang = new BaseParserStringListChineseSimplified;
        }
        else if (langString == "zh_tw_utf8.UTF-8")
        {
            parserLang = new BaseParserStringListChineseSimplified;
        }

        /// @todo add additional else if language support above here
        else
        {
            // default to US english
            parserLang = new BaseParserStringListEnglish;
        }
    }
    else
    {
        // default to US english
        parserLang = new BaseParserStringListEnglish;
    }
  #elif defined(_WIN64) || defined(_WIN32)
    LANGID langId = GetSystemDefaultUILanguage();
    /// @todo implement windows language detection
    parserLang = new BaseParserStringListEnglish;
  #endif
  return parserLang;

#elif defined(ENGLISH_ERRORS)
    return new BaseParserStringListEnglish;
#elif defined(SPANISH_ERRORS)
    return new BaseParserStringListSpanish;
#elif defined(FRENCH_ERRORS)
    return new BaseParserStringListFrench;
#elif defined(CHINESE_ERRORS)
    return new BaseParserStringListChineseSimplified;
#endif
}

/** @} */
