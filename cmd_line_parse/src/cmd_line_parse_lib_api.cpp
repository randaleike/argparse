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
 * @ingroup cmd_line_parse
 * @defgroup Command line parsing utility
 * @{
 */

// Includes
#include "varg.h"
#include "vargincrement.h"
#include "vargenum.h"
#include "listvarg.h"
#include "cmd_line_parse.h"
#include "cmd_line_parse_api.h"

//=================================================================================================
//========================= Structure definitions =================================================
//=================================================================================================
struct argData
{
    argtype_e                                       type;                       ///< Argument type assignment
    void*                                           cStoragePtr;                ///< Pointer to the C type storage
    int                                             elementCount;               ///< Maximum number of elements to update

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
};

/**
 * @brief Object c dereference
 */
struct argParser
{
    argparser::cmd_line_parse*  object;             ///< Command line parser object
    std::list<argHandle>*       argList;            ///< List of varg objects assigned to the parser
};

/**
 * @brief Update the C corresponding reflection of the varg object
 *
 * @param arg - Argument to update
 */
static void updateParserArgCData(argHandle arg)
{

}

//=================================================================================================
//=================================================================================================
//======================== Parser Argument API interface methods ==================================
//=================================================================================================
//=================================================================================================

/**
 * @brief Create a Parser argument object
 *
 * @param type       - Type of argument
 * @param argStorage - Pointer to the argument storeage to fill
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserArg(argtype_e type)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct vargData));
    argument->type = type;

    switch(type)
    {
        case ARG_SHORT_INT:
            // create variable with the current value as the default
            argument->shortintArg = new argparser::varg<short int>(*(static_cast<short int*>(argStorage)));
            break;
        case ARG_INT:
            // create variable with the current value as the default
            argument->intArg = new argparser::varg<int>(*(static_cast<int*>(argStorage)));
            break;
        case ARG_LONG_INT:
            // create variable with the current value as the default
            argument->longintArg = new argparser::varg<long int>(*(static_cast<long int*>(argStorage)));
            break;
        case ARG_LONG_LONG_INT:
            // create variable with the current value as the default
            argument->llintArg = new argparser::varg<long long int>(*(static_cast<long long int*>(argStorage)));
            break;
        case ARG_SHORT_UINT:
            // create variable with the current value as the default
            argument->shortuintArg = new argparser::varg<short unsigned>(*(static_cast<short unsigned*>(argStorage)));
            break;
        case ARG_UINT:
            // create variable with the current value as the default
            argument->uintArg = new argparser::varg<unsigned>(*(static_cast<unsigned*>(argStorage)));
            break;
        case ARG_LONG_UINT:
            // create variable with the current value as the default
            argument->longuintArg = new argparser::varg<long unsigned>(*(static_cast<long unsigned*>(argStorage)));
            break;
        case ARG_LONG_LONG_UINT:
            // create variable with the current value as the default
            argument->lluintArg = new argparser::varg<long long unsigned>(*(static_cast<long long unsigned*>(argStorage)));
            break;
        case ARG_DOUBLE:
            // create variable with the current value as the default
            argument->floatArrayArg = new argparser::varg<double>(*(static_cast<double*>(argStorage)));
            break;
        case ARG_CHAR:
            // create variable with the current value as the default
            argument->charArg = new argparser::varg<char>(*(static_cast<char*>(argStorage)));
            break;
        case ARG_BOOL:
            // create variable with the current value as the default
            argument->boolArg = new argparser::varg<bool>(*(static_cast<bool*>(argStorage)));
            break;
        case ARG_CHAR_STRING:
            // create variable with the current value as the default
            argument->stringArg = new argparser::varg<std::string>((static_cast<char*>(argStorage)));
            break;
        case ARG_INCREMENT:
            // create increment variable
            argument->incrementArg = new argparser::vargincrement;
            break;
        case ARG_ARRAY_SHORT_INT:
            // create list variable
            argument->shortintArrayArg = new argparser::listvarg<short int>;
            break;
        case ARG_ARRAY_INT:
            // create list variable
            argument->intArrayArg = new argparser::listvarg<int>;
            break;
        case ARG_ARRAY_LONG_INT:
            // create list variable
            argument->longintArrayArg = new argparser::listvarg<long int>;
            break;
        case ARG_ARRAY_LONG_LONG_INT:
            // create list variable
            argument->llintArrayArg = new argparser::listvarg<long long int>;
            break;
        case ARG_ARRAY_SHORT_UINT:
            // create list variable
            argument->shortuintArrayArg = new argparser::listvarg<short unsigned>;
            break;
        case ARG_ARRAY_UINT:
            // create list variable
            argument->uintArrayArg = new argparser::listvarg<unsigned>;
            break;
        case ARG_ARRAY_LONG_UINT:
            // create list variable
            argument->longuintArrayArg = new argparser::listvarg<long unsigned>;
            break;
        case ARG_ARRAY_LONG_LONG_UINT:
            // create list variable
            argument->lluintArrayArg = new argparser::listvarg<long long unsigned>;
            break;
        case ARG_ARRAY_DOUBLE:
            // create list variable
            argument->floatArrayArg = new argparser::listvarg<double>;
            break;
        case ARG_ARRAY_CHAR:
            // create list variable
            argument->charArrayArg = new argparser::listvarg<char>;
            break;
        case ARG_ARRAY_BOOL:
            // create list variable
            argument->boolArrayArg = new argparser::listvarg<bool>;
            break;
        case ARG_ARRAY_CHAR_STRING:
            // create list variable
            argument->stringArrayArg = new argparser::listvarg<std::string>;
            break;
        default:
            free(argument);
            argument = nullptr;
            break;
    }

    return argument;
}

/**
 * @brief Create a Parser argument object
 *
 * @param defaultValue - Initial value
 * @param setValue     - Value if flag variable was found
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool defaultValue, bool setValue)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct vargData));
    argument->type = ARG_BOOL;
    argument->cStoragePtr = argStorage;

    // create variable with the current value as the default
    argument->boolArg = new argparser::varg<bool>(defaultValue, setValue);

    return argument;
}

/**
 * @brief Release the parser argument handle.
 *
 * @param arg - Argument handle to release
 */
void releaseParserArg(argHandle arg)
{
    if (arg != nullptr)
    {
        delete arg->vargIntf;
        free(arg);
    }
}

//=================================================================================================
//=================================================================================================
//========================= Parser API interface methods ==========================================
//=================================================================================================
//=================================================================================================

/**
 * @brief Get the Parser object
 *
 * @param usage - Usage sting for help display
 * @param description - Description of tool for the help display
 * @param abortOnError - True = abort parsing on the first error, False = continue parsing to the end
 * @param disableDefaultHelp - True = display the help mesage if a parsing error occurs,
 *                             False = do not display the help message until parserDisplayHelp() is called
 * @param debugLevel - Debug output verbosity level, 0 = none, 5 = full
 *
 * @return argParserHandle - Handle to the created argument parser
 */
argParserHandle getParser(parsercstr usage, parsercstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel)
{
    argParserHandle parser = (typeof(argParserHandle))malloc(sizeof(struct argParser));
    argparser::cmd_line_parse* object = new argparser::cmd_line_parse(usage, description, abortOnError, disableDefaultHelp, debugLevel);
    parser->argList = new std::list<argList

/**
 * @brief Release the parser instance
 *
 * @param parser - Handle value returned by getParser()
 * @param releaseArgHandles - True, release the argument handles that were assigned to the parer add argument calls
 *                            False, Do not release the argument handles assigned to the parser
 */
void releaseParser(argParserHandle parser, bool releaseArgHandles);
{
    if (parser != nullptr)
    {
        delete parser->object;
        if (releaseArgHandles)
        {
            while (!parser->argList.empty())
            {
                argHandle argItem = parser->argList.pop_front();
                releaseParserArg(argItem);
            }
        }
        else
        {
            parser->argList.erase();
        }
        delete parser->argList;
        free(parser);
    }
}

//=================================================================================================
//======================= Parser setup interface methods ==========================================
//=================================================================================================

/**
 * @brief Set the Epilog text
 *
 * @param parser - Handle value returned by getParser()
 * @param epilog - Text to display at the end of the help block
 */
void setEpilog(argParserHandle parser, parsercstr epilog)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->setEpilog(epilog);
    }
}

/**
 * @brief Set the Program Name for the usage string
 *
 * @param parser - Handle value returned by getParser()
 * @param progName - Program name to use in the usage string
 */
void setProgramName(argParserHandle parser, parsercstr progName)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->setProgramName(progName);
    }
}

/**
 * @brief Set the argument key prefix value.
 *
 * The argument key prefix is the character or string
 * the identifies an input argument key string.  Any input
 * argument that does not begin with this character is
 * assumed to be a positional argument value.
 *
 * @param parser - Handle value returned by getParser()
 * @param prefix - argument prefix value
 */
void setKeyPrefix(argParserHandle parser, parsercstr prefix)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->setKeyPrefix(prefix);
    }
}

/**
 * @brief Disable the default help argument setup
 */
void disableDefaultHelpArgument(argParserHandle parser)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableDefaultHelpArgument();
    }
}


/**
 * @brief Disable the help display on parsing error
 *
 * @param parser - Handle value returned by getParser()
 */
void disableHelpDisplayOnError(argParserHandle parser)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableHelpDisplayOnError();
    }
}


/**
 * @brief Disable the single character list input argument parsing
 *
 * @param parser - Handle value returned by getParser()
 */
void disableSingleCharListArgument(argParserHandle parser)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableSingleCharListArgument();
    }
}

/**
 * @brief Enable flag to ignore unknown arguments, default
 *        is to generate an error if an unknown argument
 *        is found
 *
 * @param parser - Handle value returned by getParser()
 */
void enableUnknowArgumentIgnore(argParserHandle parser)
{
    if (parser != nullptr)
    {
        argparser::cmd_line_parse* parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->enableUnknowArgumentIgnore();
    }
}

//=================================================================================================
//======================= Argument add interface methods ==========================================
//=================================================================================================
/**
 * @brief Add a new key based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param nargs      - Number of argument values that follow
 *                     0 : Argument is a flag with no following values
 *                     1 : Simple argument with a single value
 *                     N : ARRAY argument with exactly N arguments, if less than N arguments
 *                         are found it is flagged as an error
 *                    -N : ARRAY argument with up to N arguments, if more than N arguments
 *                         are found it is flagged as an error
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addKeyArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, int nargs, bool required)
{
    if (parser != nullptr)
    {
        parser->object->addKeyArgument(arg->vargIntf, name, argKeys, helpText, nargs, required);
        parser->argList->push_back(arg);
    }
}

/**
 * @brief Add a new key based flag command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addFlagArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, bool required)
{
    if (parser != nullptr)
    {
        parser->object->addFlagArgument(arg->vargIntf, name, argKeys, helpText, required);
        parser->argList->push_back(arg);
    }
}

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param nargs      - Number of argument values that follow
 *                     0 : Argument is a flag with no following values
 *                     1 : Simple argument with a single value
 *                     N : ARRAY argument with exactly N arguments, if less than N arguments
 *                         are found it is flagged as an error
 *                    -N : ARRAY argument with up to N arguments, if more than N arguments
 *                         are found it is flagged as an error
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addPositionalArgument(argParserHandle parser, argHandle arg, parsercstr name, parsercstr helpText, int nargs, bool required)
{
    if (parser != nullptr)
    {
        parser->object->addPositionalArgument(arg->vargIntf, name, argKeys, helpText, nargs, required);
        parser->argList->push_back(arg);
    }
}

//=================================================================================================
//======================= Commandline parser interface methods ====================================
//=================================================================================================
/**
 * @brief Set the name of the positional argument to stop parsing
 *
 * @param parser - Handle value returned by getParser()
 * @param positionalArgumentName - Positional name to find
 */
void setPositionalNameStop(argParserHandle parser, const parsercstr positionalArgumentName)
{
    if (parser != nullptr)
    {
        parser->object->setPositionalNameStop(positionalArgumentName);
    }
}

/**
 * @brief Parse the input command line arguments
 *
 * @param parser - Handle value returned by getParser()
 * @param argc - Number of char pointers in the argv[] array
 * @param argv - Array of command line text entries
 * @param startingArgIndex - argv index to start parsing arguments
 * @param endingArgIndex - argv index to stop parsing arguments, -1 == argc
 *
 * @return int - Index of the last argument parsed or -1 if an error occured
 */
int parse(argParserHandle parser, int argc, char* argv[], int startingArgIndex, int endingArgIndex)
{
    if (parser != nullptr)
    {
        int parserStatus = parser->object->parse(argc, argv, startingArgIndex, endingArgIndex);
        if (-1 != parserStatus)
        {
            // Update the C reflection of the varg data
            for (auto const& arg : parser->argList)
            {
                arg->
            }
        }
    }
}

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param parser - Handle value returned by getParser()
 */
void displayHelp(argParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object->displayHelp();
    }
}

#ifdef __cplusplus
}
#endif

/** @} */
