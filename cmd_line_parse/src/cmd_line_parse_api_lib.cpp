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
#include <iostream>
#include "cmd_line_parse_api_lib_private.h"

/**
 * @brief Update the C corresponding reflection of the varg object
 *
 * @param arg - Argument to update
 */
static void updateParserArgCData(argHandle arg)
{
    if (arg->argptr.vargIntf == nullptr)
    {
        // Something is very wrong
        std::cerr << "argument handle invalid, handle: " << arg << " argptr==nullptr" << std::endl;
        return;
    }
    if (arg->cStoragePtr.retValue == nullptr)
    {
        // Something is very wrong
        std::cerr << "argument handle invalid, handle: " << arg << " cStoragePtr==nullptr" << std::endl;
        return;
    }
    
    switch(arg->type)
    {
        case argVarType_e::VT_SHORT_INT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.shortIntVal) = arg->argptr.shortintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.shortIntVal)[i] = arg->argptr.shortintArrayArg->value.front();
                    arg->argptr.shortintArrayArg->value.pop_front();
                }
            }
            break;

        case argVarType_e::VT_INCREMENTING:
        case argVarType_e::VT_INTEGER:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.integerVal) = arg->argptr.intArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.integerVal)[i] = arg->argptr.intArrayArg->value.front();
                    arg->argptr.intArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_LONG_INT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.longIntVal) = arg->argptr.longintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.longIntVal)[i] = arg->argptr.longintArrayArg->value.front();
                    arg->argptr.longintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_LLONG_INT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.llIntVal) = arg->argptr.llintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.llIntVal)[i] = arg->argptr.llintArrayArg->value.front();
                    arg->argptr.llintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_SHORT_UINT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.shortUIntVal) = arg->argptr.shortuintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.shortUIntVal)[i] = arg->argptr.shortuintArrayArg->value.front();
                    arg->argptr.shortuintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_UINTEGER:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.unsignedVal) = arg->argptr.uintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.unsignedVal)[i] = arg->argptr.uintArrayArg->value.front();
                    arg->argptr.uintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_LONG_UINT:
            if (false == arg->argptr.vargIntf->isList())
            {
                *(arg->cStoragePtr.longUIntVal) = arg->argptr.longuintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.longUIntVal)[i] = arg->argptr.longuintArrayArg->value.front();
                    arg->argptr.longuintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_LLONG_UINT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.llUIntVal) = arg->argptr.lluintArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.llUIntVal)[i] = arg->argptr.lluintArrayArg->value.front();
                    arg->argptr.lluintArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_FLOAT:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.floatVal) = arg->argptr.floatArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.floatVal)[i] = arg->argptr.floatArrayArg->value.front();
                    arg->argptr.floatArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_BOOL:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.flagVal) = arg->argptr.boolArg->value;
            }
            else
            {
                for (int i = 0; (i < arg->elementCount) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.flagVal)[i] = arg->argptr.boolArrayArg->value.front();
                    arg->argptr.boolArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_CHAR:
            if (false == arg->argptr.vargIntf->isList()) 
            {
                *(arg->cStoragePtr.charVal) = arg->argptr.charArg->value;
            }
            else
            {
                for (int i = 0; (i < (arg->elementCount - 1)) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.charVal)[i] = arg->argptr.stringArg->value[i];
                }
                (arg->cStoragePtr.charVal)[arg->elementCount - 1] = '\0';
            }
            break;
        case argVarType_e::VT_STRING_ARRAY:
            if (true == arg->argptr.vargIntf->isList()) 
            {
                for (int i = 0; (i < (arg->elementCount)) && (false == arg->argptr.vargIntf->isEmpty()); i++)
                {
                    (arg->cStoragePtr.stringArray)[i] = (char*)(arg->argptr.stringArrayArg->value.front().c_str());
                    arg->argptr.stringArrayArg->value.pop_front();
                }
            }
            break;
        case argVarType_e::VT_INVALID:
        default:
            std::cerr << "argument invalid type, handle: " << arg << " type: " << static_cast<int>(arg->type) << std::endl;
            break;
    }
}

//=================================================================================================
//=================================================================================================
//======================== Parser Argument API interface methods ==================================
//=================================================================================================
//=================================================================================================
/*
#define createArg(name, vargtype, storeptr, storeArgType, vargptr) \
argHandle name(vargtype* arg) {\
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData)); \
    if (argument != nullptr) \
    {
        argument->cStoragePtr.storeptr = arg; \
        argument->type = storeArgType; \
        argument->elementCount = 1; \
        argument->argptr.vargptr = new argparser::varg<vargtype>(*arg); \
    } \
    return argument; 
}

#define createListArg(name, vargtype, storeptr, storeArgType, vargptr) \
argHandle name(vargtype arg[], int elementCount) {\
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData)); \
    if (argument != nullptr) \
    {
        argument->cStoragePtr.storeptr = &(arg[0]); \
        argument->type = storeArgType; \
        argument->elementCount = elementCount; \
        argument->argptr.vargptr = new argparser::listvarg<vargtype>(*arg); \
    } \
    return argument; 
}
*/

/**
 * @brief Create a short int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntParserArg(short int* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.shortIntVal = arg;
        argument->type = argVarType_e::VT_SHORT_INT;
        argument->elementCount = 1;
        argument->argptr.shortintArg = new argparser::varg<short int>(*arg);
    }
    return argument;
}

/**
 * @brief Create a int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntParserArg(int* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.integerVal = arg;
        argument->type = argVarType_e::VT_INTEGER;
        argument->elementCount = 1;
        argument->argptr.intArg = new argparser::varg<int>(*arg);
    }
    return argument;
}

/**
 * @brief Create a long int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntParserArg(long int* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.longIntVal = arg;
        argument->type = argVarType_e::VT_LONG_INT;
        argument->elementCount = 1;
        argument->argptr.longintArg = new argparser::varg<long int>(*arg);
    }
    return argument;
}

/**
 * @brief Create a long long int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongLongIntParserArg(long long int* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.llIntVal = arg;
        argument->type = argVarType_e::VT_LLONG_INT;
        argument->elementCount = 1;
        argument->argptr.llintArg = new argparser::varg<long long int>(*arg);
    }
    return argument;
}

/**
 * @brief Create a short unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntParserArg(short unsigned* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.shortUIntVal = arg;
        argument->type = argVarType_e::VT_SHORT_UINT;
        argument->elementCount = 1;
        argument->argptr.shortuintArg = new argparser::varg<short unsigned>(*arg);
    }
    return argument;
}

/**
 * @brief Create an unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntParserArg(unsigned* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.unsignedVal = arg;
        argument->type = argVarType_e::VT_UINTEGER;
        argument->elementCount = 1;
        argument->argptr.uintArg = new argparser::varg<unsigned>(*arg);
    }
    return argument;
}

/**
 * @brief Create a long unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntParserArg(long unsigned* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.longUIntVal = arg;
        argument->type = argVarType_e::VT_LONG_UINT;
        argument->elementCount = 1;
        argument->argptr.longuintArg = new argparser::varg<long unsigned>(*arg);
    }
    return argument;
}

/**
 * @brief Create a long long unsigned int parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongLongUIntParserArg(long long unsigned* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.llUIntVal = arg;
        argument->type = argVarType_e::VT_LLONG_UINT;
        argument->elementCount = 1;
        argument->argptr.lluintArg = new argparser::varg<long long unsigned>(*arg);
    }
    return argument;
}

/**
 * @brief Create a double parser argument object
 *
 * @param arg - Pointer to the argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createFloatParserArg(double* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.floatVal = arg;
        argument->type = argVarType_e::VT_FLOAT;
        argument->elementCount = 1;
        argument->argptr.floatArg = new argparser::varg<double>(*arg);
    }
    return argument;
}

/**
 * @brief Create a single character parser argument object
 *
 * @param arg - Pointer to the character argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createCharParserArg(char* arg)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.charVal = arg;
        argument->type = argVarType_e::VT_CHAR;
        argument->elementCount = 1;
        argument->argptr.charArg = new argparser::varg<char>(*arg);
    }
    return argument;
}

/**
 * @brief Create a string parser argument object
 *
 * @param arg - Pointer to the string argument storage
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringParserArg(char* arg, int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.charVal = arg;
        argument->type = argVarType_e::VT_CHAR;
        argument->elementCount = elementCount;
        argument->argptr.stringArg = new argparser::varg<std::string>(&(arg[0]));
    }
    return argument;
}

/**
 * @brief Create a parser argument objectintegerVal
 * @param flag         - Pointer to the argument storage 
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createParserFlagArg(bool defaultValue, bool setValue, bool* flag)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.flagVal = flag;
        argument->type = argVarType_e::VT_BOOL;
        argument->elementCount = 1;
        argument->argptr.boolArg = new argparser::varg<bool>(defaultValue, setValue);
    }
    return argument;
}

/**
 * @brief Create a incrementing parser argument object
 *
 * @param flag - Pointer to the argument storage 
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIncrementingFlagArg(int* flag)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.integerVal = flag;
        argument->type = argVarType_e::VT_INCREMENTING;
        argument->elementCount = 1;
        argument->argptr.incrementArg = new argparser::vargincrement;
    }
    return argument;
}

/**
 * @brief Create a short integer array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortIntArrayParserArg(short int arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.shortIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_SHORT_INT;
        argument->elementCount = elementCount;
        argument->argptr.shortintArrayArg = new argparser::listvarg<short int>;
    }
    return argument;
}

/**
 * @brief Create a integer array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createIntArrayParserArg(int arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.integerVal = &(arg[0]);
        argument->type = argVarType_e::VT_INTEGER;
        argument->elementCount = elementCount;
        argument->argptr.intArrayArg = new argparser::listvarg<int>;
    }
    return argument;
}

/**
 * @brief Create a long integer array parser argument object
 *
 * @param arg - Pointer to the long integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongIntArrayParserArg(long int arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.longIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_LONG_INT;
        argument->elementCount = elementCount;
        argument->argptr.longintArrayArg = new argparser::listvarg<long int>;
    }
    return argument;
}

/**
 * @brief Create a long long integer array parser argument object
 *
 * @param arg - Pointer to the long long integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLongIntArrayParserArg(long long int arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.llIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_LLONG_INT;
        argument->elementCount = elementCount;
        argument->argptr.llintArrayArg = new argparser::listvarg<long long int>;
    }
    return argument;
}

/**
 * @brief Create an short unsigned array parser argument object
 *
 * @param arg - Pointer to the short unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createShortUIntArrayParserArg(short unsigned arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.shortUIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_SHORT_UINT;
        argument->elementCount = elementCount;
        argument->argptr.shortuintArrayArg = new argparser::listvarg<short unsigned>;
    }
    return argument;
}

/**
 * @brief Create an unsigned array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createUIntArrayParserArg(unsigned arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.unsignedVal = &(arg[0]);
        argument->type = argVarType_e::VT_UINTEGER;
        argument->elementCount = elementCount;
        argument->argptr.uintArrayArg = new argparser::listvarg<unsigned>;
    }
    return argument;
}

/**
 * @brief Create an long unsigned array parser argument object
 *
 * @param arg - Pointer to the long unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLongUIntArrayParserArg(long unsigned arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.longUIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_LONG_UINT;
        argument->elementCount = elementCount;
        argument->argptr.longuintArrayArg = new argparser::listvarg<long unsigned>;
    }
    return argument;
}

/**
 * @brief Create an long long unsigned array parser argument object
 *
 * @param arg - Pointer to the long long unsigned integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createLLUIntArrayParserArg(long long unsigned arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.llUIntVal = &(arg[0]);
        argument->type = argVarType_e::VT_LLONG_UINT;
        argument->elementCount = elementCount;
        argument->argptr.lluintArrayArg = new argparser::listvarg<long long unsigned>;
    }
    return argument;
}

/**
 * @brief Create a double array parser argument object
 *
 * @param arg - Pointer to the integer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createDoubleArrayParserArg(double arg[], int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.floatVal = &(arg[0]);
        argument->type = argVarType_e::VT_FLOAT;
        argument->elementCount = elementCount;
        argument->argptr.floatArrayArg = new argparser::listvarg<double>();
    }
    return argument;
}

/**
 * @brief Create a double array parser argument object
 *
 * @param arg - Pointer to the constant character pointer array argument storage
 * @param elementCount - Size of the array in elements
 *
 * @return argHandle - Handle to use when adding the argument to the parser argument list
 */
argHandle createStringArrayParserArg(char** arg, int elementCount)
{
    argHandle argument = (typeof(argHandle))malloc(sizeof(struct argData));
    if (argument != nullptr)
    {
        argument->cStoragePtr.stringArray = arg;
        argument->type = argVarType_e::VT_STRING_ARRAY;
        argument->elementCount = elementCount;
        argument->argptr.stringArrayArg = new argparser::listvarg<std::string>();
    }
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
        delete arg->argptr.vargIntf;
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
 * @return cmdLineParserHandle - Handle to the created argument parser
 */
cmdLineParserHandle getParser(parsercstr usage, parsercstr description, bool abortOnError, bool disableDefaultHelp, int debugLevel)
{
    cmdLineParserHandle parser = (typeof(cmdLineParserHandle))malloc(sizeof(struct cmdLineParser));
    parser->object = new argparser::cmd_line_parse(usage, description, abortOnError, disableDefaultHelp, debugLevel);
    parser->argList.clear();
    return parser;
}

/**
 * @brief Release the parser instance
 *
 * @param parser - Handle value returned by getParser()
 * @param releaseArgHandles - True, release the argument handles that were assigned to the parer add argument calls
 *                            False, Do not release the argument handles assigned to the parser
 */
void releaseParser(cmdLineParserHandle parser, bool releaseArgHandles)
{
    if (parser != nullptr)
    {
        delete parser->object;
        if (releaseArgHandles)
        {
            while (false == parser->argList.empty())
            {
                argHandle argItem = parser->argList.front();
                parser->argList.pop_front();
                releaseParserArg(argItem);
            }
        }
        else
        {
            parser->argList.clear();
        }
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
void setEpilog(cmdLineParserHandle parser, parsercstr epilog)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->setEpilog(epilog);
    }
}

/**
 * @brief Set the Program Name for the usage string
 *
 * @param parser - Handle value returned by getParser()
 * @param progName - Program name to use in the usage string
 */
void setProgramName(cmdLineParserHandle parser, parsercstr progName)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
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
void setKeyPrefix(cmdLineParserHandle parser, parsercstr prefix)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->setKeyPrefix(prefix);
    }
}

/**
 * @brief Disable the default help argument setup
 */
void disableDefaultHelpArgument(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableDefaultHelpArgument();
    }
}


/**
 * @brief Disable the help display on parsing error
 *
 * @param parser - Handle value returned by getParser()
 */
void disableHelpDisplayOnError(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
        parser->object->disableHelpDisplayOnError();
    }
}


/**
 * @brief Disable the single character list input argument parsing
 *
 * @param parser - Handle value returned by getParser()
 */
void disableSingleCharListArgument(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
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
void enableUnknowArgumentIgnore(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object = static_cast<argparser::cmd_line_parse*>(parser->object);
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
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addKeyArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (true == arg->argptr.vargIntf->isList())
    {
        std::cerr << "Use addKeyArrayArgument() to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addKeyArgument(arg->argptr.vargIntf, name, argKeys, helpText, 1, required);
    parser->argList.push_back(arg);
}

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
void addKeyArrayArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, int nargs, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (false == arg->argptr.vargIntf->isList())
    {
        std::cerr << "Use addKeyArgument() to add single value arguments to the parser" << std::endl;
        return;
    }

    parser->object->addKeyArgument(arg->argptr.vargIntf, name, argKeys, helpText, nargs, required);
    parser->argList.push_back(arg);
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
void addFlagArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (true == arg->argptr.vargIntf->isList())
    {
        std::cerr << "Use addKeyArrayArgument() or addPositionalArrayArgument to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addFlagArgument(arg->argptr.vargIntf, name, argKeys, helpText, required);
    parser->argList.push_back(arg);
}

/**
 * @brief Add a new key based incrementing command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addIncrementingArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr argKeys, parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (arg->type != argVarType_e::VT_INCREMENTING)
    {
        std::cerr << "Invalid argument type use createIncrementingFlagArg() to create the proper handle type" << std::endl;
        return;
    }

    parser->object->addFlagArgument(arg->argptr.vargIntf, name, argKeys, helpText, required);
    parser->argList.push_back(arg);
}

/**
 * @brief Add a new positinal based command line argument to the argument list
 *
 * @param parser     - Handle value returned by getParser()
 * @param arg        - Handle of the argument to add
 * @param name       - Human name used in the help messages
 * @param argKeys    - Delimieted list of argument key values
 * @param helpText   - Help text to be printed in the help message
 * @param required   - True if argument is required, false if arguemnt is optional
 */
void addPositionalArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr helpText, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (true == arg->argptr.vargIntf->isList())
    {
        std::cerr << "Use addPositionalArrayArgument() to add array arguments to the parser" << std::endl;
        return;
    }

    parser->object->addPositionalArgument(arg->argptr.vargIntf, name, helpText, 1, required);
    parser->argList.push_back(arg);
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
void addPositionalArrayArgument(cmdLineParserHandle parser, argHandle arg, parsercstr name, parsercstr helpText, int nargs, bool required)
{
    if (parser == nullptr)
    {
        std::cerr << "Invalid parser input, nullptr" << std::endl;
        return;
    }

    if (arg == nullptr)
    {
        std::cerr << "Invalid arg input, nullptr" << std::endl;
        return;
    }

    if (false == arg->argptr.vargIntf->isList())
    {
        std::cerr << "Use addPositionalArgument() to add single value arguments to the parser" << std::endl;
        return;
    }

    parser->object->addPositionalArgument(arg->argptr.vargIntf, name, helpText, nargs, required);
    parser->argList.push_back(arg);
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
void setPositionalNameStop(cmdLineParserHandle parser, const parsercstr positionalArgumentName)
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
int parse(cmdLineParserHandle parser, int argc, char* argv[], int startingArgIndex, int endingArgIndex)
{
    int parserStatus = -1;

    if (parser != nullptr)
    {
        parserStatus = parser->object->parse(argc, argv, startingArgIndex, endingArgIndex);
        if (-1 != parserStatus)
        {
            // Update the C reflection of the varg data
            for (auto const& arg : parser->argList)
            {
                updateParserArgCData(arg);
            }
        }
    }
    
    return parserStatus;
}

/**
 * @brief Print the formatted help message to the input stream
 *
 * @param parser - Handle value returned by getParser()
 */
void displayHelp(cmdLineParserHandle parser)
{
    if (parser != nullptr)
    {
        parser->object->displayHelp();
    }
}

/** @} */
