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
 * @file parser_string_list.cpp
 * @ingroup argparser
 * @defgroup Internationalized string list for all parsers
 * @{
 */

// Includes
#include <cstring>
#include <string>
#include "parser_string_list.h"
#if defined(DYNAMIC_INTERNATIONALIZATION)
  #if defined(__linux__) || defined(__unix__)
    #include <stdlib.h> 
  #elif defined(_WIN64) || defined(_WIN32)
    #include <Windows.h>
  #else
    #error "Unknown OS type"
  #endif
#endif

using namespace argparser;

#if defined(ENGLISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Englist string list
 */
class BaseParserStringListEnglish : public BaseParserStringList
{
    public:
        virtual void printNotListTypeMessage(std::ostream &outStream, int nargs) 
        {outStream << "Only list type arguments can have an argument count of " << nargs << std::endl;}
        
        virtual void printUnknownArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "Unkown argument " << keyString << std::endl;}
        
        virtual void printInvalidAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" invalid assignment" << std::endl;}
        
        virtual void printAssignmentFailedMessage(std::ostream &outStream, const parserchar* keyString, parserstr valueString)
        {outStream << "\"" << keyString << " " << valueString << "\" assignment failed" << std::endl;}

        virtual void printMissingAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" missing assignment value" << std::endl;}

        virtual void printMissingListAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" missing assignment. Expected: " << expected << " found: " << found << " arguments" << std::endl;}

        virtual void printTooManyAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" too many assignment values. Expected: " << expected << " found: " << found << " arguments" << std::endl;}

        virtual void printMissingArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" required argument missing" << std::endl;}

        virtual void printArgumentCreationError(std::ostream &outStream, parserstr keyString)
        {outStream << "Argument add failed: " << keyString << std::endl;}

        virtual parserstr getUsageMessage() const
        {return "Usage:";}

        virtual parserstr getPositionalArgumentsMessage() const
        {return "Positional Arguments:";}

        virtual parserstr getSwitchArgumentsMessage() const
        {return "Optional Arguments:";}

        virtual parserstr getHelpString() const
        {return "show this help message and exit";}

        virtual parserstr getEnvArgumentsMessage()
        {return "Environment values:";}

        virtual void printEnvironmentNoFlags(std::ostream &outStream, parserstr argKey)
        {outStream << "Environment value " << argKey << " narg must be > 0" << std::endl;}
};
#endif

#if defined(SPANISH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Spanish string list
 */
class BaseParserStringListSpanish : public BaseParserStringList
{
    public:
        virtual void printNotListTypeMessage(std::ostream &outStream, int nargs) 
        {outStream << "Solo los argumentos de tipo lista pueden tener un recuento de argumentos de " << nargs << std::endl;}
        
        virtual void printUnknownArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "Argumento desconocidoreturn " << keyString << std::endl;}
        
        virtual void printInvalidAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" asignación inválida" << std::endl;}
        
        virtual void printAssignmentFailedMessage(std::ostream &outStream, const parserchar* keyString, parserstr valueString)
        {outStream << "\"" << keyString << " " << valueString << "\" asignación fallida" << std::endl;}

        virtual void printMissingAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" valor de asignación faltante" << std::endl;}

        virtual void printMissingListAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" tarea faltante. Esperado: " << expected << " encontrado: " << found << " argumentos" << std::endl;}

        virtual void printTooManyAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" demasiados valores de asignación. Esperado: " << expected << " encontrado: " << found << " argumentos" << std::endl;}

        virtual void printMissingArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" falta el argumento requerido" << std::endl;}

        virtual void printArgumentCreationError(std::ostream &outStream, parserstr keyString)
        {outStream << "No se pudo agregar el argumento: " << keyString << std::endl;}

        virtual parserstr getUsageMessage() const
        {return "Uso:";}

        virtual parserstr getPositionalArgumentsMessage() const
        {return "Argumentos posicionales:";}

        virtual parserstr getSwitchArgumentsMessage() const
        {return "Argumentos opcionales:";}

        virtual parserstr getHelpString() const
        {return "mostrar este mensaje de ayuda y salir";}

        virtual parserstr getEnvArgumentsMessage()
        {return "Valores ambientales:";}

        virtual void printEnvironmentNoFlags(std::ostream &outStream, parserstr argKey)
        {outStream << "Valores ambiental " << argKey << " narg debe ser > 0" << std::endl;}
};
#endif

#if defined(FRENCH_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief French string list
 */
class BaseParserStringListFrench : public BaseParserStringList
{
    public:
        virtual void printNotListTypeMessage(std::ostream &outStream, int nargs) 
        {outStream << "Seuls les arguments de type liste peuvent avoir un nombre d'arguments de " << nargs << std::endl;}
        
        virtual void printUnknownArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "Argument inconnu " << keyString << std::endl;}
        
        virtual void printInvalidAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" affectation invalide" << std::endl;}
        
        virtual void printAssignmentFailedMessage(std::ostream &outStream, const parserchar* keyString, parserstr valueString)
        {outStream << "\"" << keyString << " " << valueString << "\" l'affectation a échoué" << std::endl;}

        virtual void printMissingAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" valeur d'affectation manquante" << std::endl;}

        virtual void printMissingListAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" devoir manquant. Attendu: " << expected << " trouvé : " << found << " arguments" << std::endl;}

        virtual void printTooManyAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" trop de valeurs d'affectation. Attendu: " << expected << " trouvé : " << found << " arguments" << std::endl;}

        virtual void printMissingArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" argument obligatoire manquant" << std::endl;}

        virtual void printArgumentCreationError(std::ostream &outStream, parserstr keyString)
        {outStream << "Échec de l'ajout d'arguments:" << keyString << std::endl;}

        virtual parserstr getUsageMessage() const
        {return "Usage:";}

        virtual parserstr getPositionalArgumentsMessage() const
        {return "Arguments positionnels:";}

        virtual parserstr getSwitchArgumentsMessage() const
        {return "Arguments facultatifs:";}

        virtual parserstr getHelpString() const
        {return "afficher ce message d'aide et quitter";}

        virtual parserstr getEnvArgumentsMessage()
        {return "Valeurs environnementales:";}

        virtual void printEnvironmentNoFlags(std::ostream &outStream, parserstr argKey)
        {outStream << "Valeurs environnementale " << argKey << " narg doit être > 0" << std::endl;}
};
#endif

#if defined(CHINESE_ERRORS) || defined(DYNAMIC_INTERNATIONALIZATION)
/**
 * @brief Simplified Chinese string list
 */
class BaseParserStringListChineseSimplified : public BaseParserStringList
{
    public:
        BaseParserStringListChineseSimplified() 
        {defaultBreakList.clear();}

        virtual void printNotListTypeMessage(std::ostream &outStream, int nargs) 
        {outStream << "只有列表类型的参数可以有一个参数计数 " << nargs << std::endl;}
        
        virtual void printUnknownArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "未知参数 " << keyString << std::endl;}
        
        virtual void printInvalidAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" 无效分配" << std::endl;}
        
        virtual void printAssignmentFailedMessage(std::ostream &outStream, const parserchar* keyString, parserstr valueString)
        {outStream << "\"" << keyString << " " << valueString << "\" 分配失败" << std::endl;}

        virtual void printMissingAssignmentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" 缺少赋值" << std::endl;}

        virtual void printMissingListAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" 缺少任务。 预期的： " << expected << " 成立: " << found << " 论据" << std::endl;}

        virtual void printTooManyAssignmentMessage(std::ostream &outStream, const parserchar* keyString, int expected, int found)
        {outStream << "\"" << keyString << "\" 分配值过多。 预期的： " << expected << " 成立: " << found << " 论据" << std::endl;}

        virtual void printMissingArgumentMessage(std::ostream &outStream, const parserchar* keyString)
        {outStream << "\"" << keyString << "\" 缺少必要的参数" << std::endl;}

        virtual void printArgumentCreationError(std::ostream &outStream, parserstr keyString)
        {outStream << "参数添加失败： " << keyString << std::endl;}

        virtual parserstr getUsageMessage() const
        {return "用法：";}

        virtual parserstr getPositionalArgumentsMessage() const
        {return "位置参数：";}

        virtual parserstr getSwitchArgumentsMessage() const
        {return "可选参数：";}

        virtual parserstr getHelpString() const
        {return "显示此帮助信息并退出";}

        virtual parserstr getEnvArgumentsMessage()
        {return "环境值：";}

        virtual void printEnvironmentNoFlags(std::ostream &outStream, parserstr argKey)
        {outStream << "环境价值 " << argKey << " narg 必须 > 0" << std::endl;}
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
size_t BaseParserStringList::findBestBreakPos(parserstr workingString, std::list<parserchar> breakCharList, size_t maxLength)
{
    std::list<size_t> breakPos;

    if (!breakCharList.empty())
    {
        // Start with the first char in the list
        for (std::list<parserchar>::iterator breakCharacter = breakCharList.begin(); breakCharacter != breakCharList.end(); ++breakCharacter)
        {
            // Find the best break location
            size_t workingBreakPos = workingString.size();
            if (debugMsgLevel > 4) std::cerr << "Start break: " << workingBreakPos << std::endl;
            do
            {
                // Search for the break character
                workingBreakPos = workingString.rfind(*breakCharacter, workingBreakPos-1);
                if (debugMsgLevel > 4) std::cerr << "Current break: " << workingBreakPos << std::endl;

            }
            while((workingBreakPos > maxLength) && (workingBreakPos != parserstr::npos));

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
            return breakPos.back();
        }
        else
        {
            // No good break found, break at max length
            return maxLength;
        }
    }
    else
    {
        // No break character list, break at max length
        return maxLength;
    }
}

//============================================================================================================================
//============================================================================================================================
//  Public non-virtual functions
//============================================================================================================================
//============================================================================================================================

BaseParserStringList::BaseParserStringList() : defaultBreakList({' '}), debugMsgLevel(0)
{ }

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

    if (debugMsgLevel > 4) std::cerr << "Start string: \"" << workingString << "\"" << std::endl;

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

        if (debugMsgLevel > 4) std::cerr << "Current string: \"" << workingString << "\"" << std::endl;
    }

    // Add the last tail
    returnList.push_back(workingString);

    // Pad the string to the proper length
    for (std::list<parserstr>::iterator unpaddedString = returnList.begin(); unpaddedString != returnList.end(); ++unpaddedString)
    {
        while (unpaddedString->length() < maxLength)
        {
            *unpaddedString += ' ';
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
 #if defined(__linux__) || defined(__unix__)
    const parserchar* langSetting = getenv("LANG");
    if (NULL != langSetting)
    {
        parserstr langString = langSetting;
        if (langString == "en_US.UTF-8")
        {
            return new BaseParserStringListEnglish;
        }
        else if (langString == "en_UK.UTF-8")
        {
            return new BaseParserStringListEnglish;
        }
        else if (langString == "es_ES.UTF-8")
        {
            return new BaseParserStringListSpanish;
        }
        else if (langString == "fr_FR.UTF-8")
        {
            return new BaseParserStringListFrench;
        }
        else if (langString == "zh_cn_utf8.UTF-8")
        {
            return new BaseParserStringListChineseSimplified;
        }
        else if (langString == "zh_tw_utf8.UTF-8")
        {
            return new BaseParserStringListChineseSimplified;
        }

        /// @todo add additional else if language support above here
        else
        {
            // default to US english
            return new BaseParserStringListEnglish;
        }
    }
    else
    {
        // default to US english
        return new BaseParserStringListEnglish;
    }
  #elif defined(_WIN64) || defined(_WIN32)
    LANGID langId = GetSystemDefaultUILanguage();
    /// @todo implement windows language detection
    return new BaseParserStringListEnglish;
  #endif
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
