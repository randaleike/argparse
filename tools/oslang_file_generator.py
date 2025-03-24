"""@package argparselangautogen
Utility to automatically generate language strings using google translate api
for the argparse libraries
"""

#==========================================================================
# Copyright (c) 2025 Randal Eike
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of self software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and self permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#==========================================================================

from file_tools.copyright_tools import CopyrightGenerator
from file_tools.comment_block import CommentBlock
from file_tools.comment_block import CommentGenerator
from file_tools.eula import EulaText
from file_tools.text_format import MultiLineFormat
from autogenlang import LanguageDescriptionList

linuxDefString = "(defined(__linux__) || defined(__unix__))"
windowsDefString = "(defined(_WIN64) || defined(_WIN32))"
dynamicDefString = "defined(DYNAMIC_INTERNATIONALIZATION)"

baseStringType = "ParserStringListInterface"
macrofunctionName = "SystemLanguageDetection::getLangId"
getStaticFunctionName = "getStaticLang"
linuxGetDynamicFunctionName = "getDynamicLang_Linux"
windowsGetDynamicFunctionName = "getDynamicLang_Windows"

class CompileSwitchGeneration(object):
    linuxDefString = "(defined(__linux__) || defined(__unix__))"
    windowsDefString = "(defined(_WIN64) || defined(_WIN32))"

    """!
    Utility for generating compile switch strings
    """
    def __init__(self, dynamicLanguageSwitch = "DYNAMIC_INTERNATIONALIZATION"):
        """!
        @brief CompileSwitchGeneration constructor
        @param dynamicLanguageSwitch (string) - C/C++ Compiler switch to enable dynamic language detection
        """
        self.dynamicDefString = "defined("+dynamicLanguageSwitch+")"

    def getIfDynamic(self, defOsString):
        """!
        @brief Get the dynamic os #if compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString
        @return string - formatted #if compile switch statement
        """
        return "#if ("+defOsString+" && "+self.dynamicDefString+")"

    def getEndDynamic(self):
        """!
        @brief Get compile switch #endif string
        @return string - formatted #endif compile switch statement with end comment
        """
        return "#endif //"+dynamicDefString

    def getElseIfDynamic(self, defOsString):
        """!
        @brief Get the dynamic os #elif compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString
        @return string - formatted #elif compile switch statement
        """
        return "#elif ("+defOsString+" && "+self.dynamicDefString+")"

    def getEndOsDynamic(self, defOsString=None):
        """!
        @brief Get the dynamic os #endif compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString or None for generic
        @return string - formatted #endif compile switch statement with end comment
        """
        if defOsString is None:
            return "#endif //(defined os and "+self.dynamicDefString+")"
        else:
            return "#endif //("+defOsString+" and "+self.dynamicDefString+")"

    def getElseNotDynamic(self):
        """!
        @brief Get compile switch #else string
        @return string - formatted #else compile switch statement with comment
        """
        return "#else //not "+self.dynamicDefString

    def getIfLangCompileOrDynamic(self, langSwitch):
        """!
        @brief Get the dynamic os #if compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString
        @return string list - [0] = formatted #if compile switch statement
                              [1] = formatted #endif compile switch statement with comment
        """
        compileSwitchStart = "#if (defined("+langSwitch+") || "+self.dynamicDefString+")"
        compileSwitchEnd = "#endif //(defined("+langSwitch+") || "+self.dynamicDefString+")"
        return [compileSwitchStart, compileSwitchEnd]

    def getIfStatic(self):
        """!
        @brief Get the dynamic os #if compile switch string
        @return string - formatted #if compile switch statement
        """
        return "#if !"+self.dynamicDefString

    def getEndifStatic(self):
        """!
        @brief Get the dynamic os #if compile switch string
        @return string - formatted #if compile switch statement
        """
        return "#endif // not "+self.dynamicDefString

class LangSelectFunctionHelper(object):
    """!
    Helper functions for OS lang file function generation
    """
    def __init__(self, baseStringType = "ParserStringListInterface"):
        """!
        @brief DoxygenCommentHelper constructor
        """
        self.stdPtrType = "std::shared_ptr"
        self.returnType = self.stdPtrType+"<"+baseStringType+">"
        self.baseStringType = baseStringType
        self.stdMakePtr = "std::make_shared<"

    def _getCommentReturnText(self, qualstring):
        """!
        @brief Generate std::shared_ptr return type
        @param qualstring (string) - What the return ptr is based on for comment
        """
        l1 = "@return "+self.returnType+" - Pointer to "+self.baseStringType+" based on"
        l2 = "        "+qualstring
        return [l1,l2]

    def generateDoxygenComment(self, name, desc, paramList, retQual):
        """!
        @brief Generate the doxygen comment block fot the function
        @param name (string) - Function name
        @param desc (string) - Function description
        @param paramList (list of param dictionaries)
        @param retQual (string) - What the return ptr is based on for comment
        @return string list - Comment block
        """
        blockStrs = []
        blockStrs.append("/**")
        blockStrs.append(" * @brief "+name+" implementation")
        formattedDesc = MultiLineFormat(desc, 78)

        for descLine in formattedDesc:
            blockStrs.append(" * "+descLine)
        blockStrs.append(" *")

        for param in paramList:
            blockStrs.append(" * @param "+param['name']+" - "+param['desc'])

        retLines = self._getCommentReturnText(retQual)
        for retDescLine in retLines:
            blockStrs.append(" * "+retDescLine)

        blockStrs.append(" */")
        return blockStrs

    def declFunction(self, name, desc, paramList, retQual):
        """!
        @brief Get the function declaration string for the given name
        @param name (string) - Function name
        @param desc (string) - Function description
        @param retQual (string) - What the return ptr is based on for comment
        @return string list - Function comment block and declaration start
        """
        declStrs = self.generateDoxygenComment(self, name, desc, paramList, retQual)
        declFunc = self.returnType+" "+name+"("
        paramPrefix = ""
        for param in paramList:
             declFunc += paramPrefix
             declFunc += param['type']
             declFunc += " "
             declFunc += param['name']
             paramPrefix = ", "
        declFunc += ")"
        declStrs.append(declFunc)
        declStrs.append("{")
        return declStrs

    def endFunction(self, name):
        """!
        @brief Get the function declaration string for the given name
        @param name (string) - Function name
        @return string - Function close with comment
        """
        return ("} // end of "+name+"()")

    def getMakePtrReturnStatement(self, classMod):
        retLine = "return "
        retLine += self.stdMakePtr
        retLine += self.baseStringType
        retLine += classMod.capitalize()
        retLine += ">();"
        return retLine

class DynamicLangIdLinux(LangSelectFunctionHelper, CompileSwitchGeneration):
    """!
    @brief class to generate getDynamicLangID_Linux() function
    """
    def __init__(self, jsonLangData, functionName = "getDynamicLangID_Linux"):
        """!
        @brief DynamicLangIdLinux constructor
        @param jsonLangData - Translated JSON data from language json definitions
        @param functionName - Name of the function, default = getDynamicLangID_Linux
        """
        self.functionName = functionName
        self.desc = "Translate the input environment LANG string into the appropriate ParserStringListInterface<LANG> object"
        self.retQual = "the \"LANG\" environment value"
        self.paramList = [{'name':"langId", 'type':"const char*", 'desc':"\"LANG\" environment value"}]
        self.langJsonData = jsonLangData

    def getOSCompileSwitch(self):
        """!
        @brief Get the OS compile swich string for this function
        @return string OS specific compile switch string
        """
        return CompileSwitchGeneration.linuxDefString

    def outputFunction(self, outputFile):
        """!
        @brief Output the getDynamicLangID_Linux c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        functionheader = []
        functionheader.append(self.getIfDynamic(self.getOSCompileSwitch()))
        functionheader.append("  #include <cstdlib>")
        functionheader.append("  #include <regex>")
        functionheader.extend(self.declFunction(self.functionName, self.desc, self.paramList, self.retQual))
        functionheader.append("    if (nullptr != "+self.paramList[0]['name']+")")
        functionheader.append("    {")

        outputFile.writelines(functionheader)

        # Generate if/else if chain for each language in the dictionary
        checkPrefix = "if"
        indent = "        "
        ifBodyIndent = indent+"    "
        for langName, langData in self.langJsonData['languages']:
            ifline =  indent+checkPrefix
            ifline += "(std::regex_match("
            ifline += self.paramList[0]['name']
            ifline += ", std::regex<\""
            ifline += langData['LANG']
            ifline += "\")))"
            checkPrefix = "else if"

            ifStart = indent+"{"
            ifAssign = ifBodyIndent+self.getMakePtrReturnStatement(str(langName))
            ifEnd = indent+"}"
            outputFile.writelines([ifline, ifStart, ifAssign, ifEnd])

        # Add the final else case
        elseline =  indent+"else"
        elseStart = indent+"{"
        elseAssign = ifBodyIndent+self.getMakePtrReturnStatement(self.langJsonData["default"]["name"])
        elseEnd = indent+"}"
        outputFile.writelines([elseline, elseStart, elseAssign, elseEnd])

        # End the function and #if
        functionTail = []
        functionTail.append("    } // end of if(nullptr != "+self.paramList[0]['name']+")")
        functionTail.append(self.endFunction(self.functionName))
        functionTail.append(self.getEndOsDynamic(self.getOSCompileSwitch()))
        outputFile.writelines(functionTail)

    def getCallBlock(self, paramName, indent="    "):
        """!
        @brief Get the macro function call block for the function
        @param name (string) - Function input parameter for definition, fill and call
        @param indent (string) - Indentaion spacing for the block
        @return string list - Code lines required for the call
        """
        getParam = indent+self.paramList[0]['type']+" "+paramName+" = getenv(\"LANG\");"
        doCall = indent+"return "+self.functionName+"("+paramName+");"
        return [getParam, doCall]

class DynamicLangIdWindows(LangSelectFunctionHelper, CompileSwitchGeneration):
    """!
    @brief class to generate getDynamicLangID_Linux() function
    """
    def __init__(self, jsonLangData, functionName = "getDynamicLangID_Windows"):
        """!
        @brief DynamicLangIdWindows constructor
        @param jsonLangData - Translated JSON data from language json definitions
        @param functionName - Name of the function, default = getDynamicLangID_Windows
        """
        self.functionName = functionName
        self.desc = "Translate the input LANGID value into the appropriate ParserStringListInterface<LANG> object"
        self.retQual = "the LANGID value"
        self.paramList = [{'name':"langId", 'type':"LANGID", 'desc':"LANGID value from OS call"}]
        self.langJsonData = jsonLangData

    def getOSCompileSwitch(self):
        """!
        @brief Get the OS compile swich string for this function
        @return string OS specific compile switch string
        """
        return CompileSwitchGeneration.windowsDefString

    def outputFunction(self, outputFile):
        """!
        @brief Output the getDynamicLangID_Windows c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        functionheader = []
        functionheader.append(self.getIfDynamic(self.getOSCompileSwitch()))
        functionheader.append("  #include <Windows.h>")
        functionheader.extend(self.declFunction(self.functionName, self.desc, self.paramList, self.retQual))
        functionheader.append("    switch("+self.paramList[0]['name']+")")
        functionheader.append("    {")
        outputFile.writelines(functionheader)

        # Generate case if chain for each language in the dictionary
        indent = "    "
        caseBodyIndent = indent+"    "
        for langName, langData in self.langJsonData['languages']:
            caseLineList = []
            for id in langData["LANGID"]:
                caseline =  indent+"case"
                caseline += hex(id)
                caseline += ":"
                caseLineList.append(caseline)
            caseAssign = caseBodyIndent+self.getMakePtrReturnStatement(str(langName))
            caseLineList.append(caseAssign)
            caseLineList.append(caseBodyIndent+"break;")

            outputFile.writelines(caseLineList)

        # Add the final default case
        defaultline   = indent+"default:"
        defaultAssign = caseBodyIndent+self.getMakePtrReturnStatement(self.langJsonData["default"]["name"])
        caseEnd = indent+"}"
        outputFile.writelines([defaultline, defaultAssign, caseEnd])

        # End the function and #if
        functionTail = []
        functionTail.append(self.endFunction(self.functionName))
        functionTail.append(self.getEndOsDynamic(self.getOSCompileSwitch()))
        outputFile.writelines(functionTail)

    def getCallBlock(self, paramName, indent="    "):
        """!
        @brief Get the macro function call block for the function
        @param paramName (string) - Function input parameter for definition, fill and call
        @param indent (string) - Indentaion spacing for the block
        @return string list - Code lines required for the call
        """
        getParam = indent+self.paramList[0]['type']+" "+paramName+" = GetUserDefaultUILanguage();"
        doCall = indent+"return "+self.functionName+"("+paramName+");"
        return [getParam, doCall]

class StaticSelection(LangSelectFunctionHelper, CompileSwitchGeneration):
    """!
    @brief class to generate getStaticLanguage() function
    """
    def __init__(self, jsonLangData, functionName = "getStaticLanguage"):
        """!
        @brief StaticSelection constructor
        @param jsonLangData - Translated JSON data from language json definitions
        @param functionName - Name of the function, default = getStaticLanguage
        """
        self.functionName = functionName
        self.desc = "Get the correct ParserStringListInterface<LANG> object based on compile switch settings"
        self.retQual = "the compile switch settings"
        self.paramList = []
        self.langJsonData = jsonLangData

    def getOSCompileSwitch(self):
        """!
        @brief Get the OS compile swich string for this function
        @return string OS specific compile switch string
        """
        return ""

    def outputFunction(self, outputFile):
        """!
        @brief Output the getStaticLanguage c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        functionheader = []
        functionheader.append(self.getIfStatic())
        functionheader.extend(self.declFunction(self.functionName, self.desc, self.paramList, self.retQual))
        outputFile.writelines(functionheader)

        # Generate #if/#elif chain for each language in the dictionary
        indent = "    "
        blockStart = "#if"
        for langName, langData in self.langJsonData['languages']:
            staticBlock = []
            staticBlock.append(blockStart+" defined("+langData['compile_switch']+")")
            staticBlock.append(indent+self.getMakePtrReturnStatement(str(langName)))
            outputFile.writelines(staticBlock)
            blockStart = "#elif"

        # Output the final #else default case
        elseStart = "#else // no language compile switch set, go to default"
        elseAssign = indent+self.getMakePtrReturnStatement(self.langJsonData["default"]["name"])
        elseEnd = "#endif // end of statci language selection"
        outputFile.writelines([elseStart, elseAssign, elseEnd])

        # Complete the function and #if
        functionTail = []
        functionTail.append(self.endFunction(self.functionName))
        functionTail.append(self.getEndifStatic())
        outputFile.writelines(functionTail)

    def getCallBlock(self, paramName, indent="    "):
        """!
        @brief Get the macro function call block for the function
        @param paramName (string) - Function input parameter for definition, fill and call
        @param indent (string) - Indentaion spacing for the block
        @return string list - Code lines required for the call
        """
        doCall = indent+"return "+self.functionName+"();"
        return [doCall]

class MacroFunction(LangSelectFunctionHelper, CompileSwitchGeneration):
    """!
    @brief class to generate SystemLanguageDetection::getLangId() function
    """
    def __init__(self, jsonLangData, osFunctionList, staticFunction, functionName = "SystemLanguageDetection::getLangId"):
        """!
        @brief StaticSelection constructor
        @param jsonLangData - Translated JSON data from language json definitions
        @param osFunctionList - List of os function generators
        @param staticFunction - Static function generator
        @param functionName (string) - Name of the function, default = getStaticLanguage
        """
        self.functionName = functionName
        self.desc = "Determine the OS and call the appropriate function to get" \
                    " the correct ParserStringListInterface<LANG> object"
        self.retQual = "compile switches and OS language identification"
        self.paramList = []
        self.langJsonData = jsonLangData
        self.osLangGenList = osFunctionList
        self.staticFuncGen = staticFunction

    def outputFunction(self, outputFile):
        """!
        @brief Output the getStaticLanguage c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        functionheader = self.declFunction(self.functionName, self.desc, self.paramList, self.retQual)
        outputFile.writelines(functionheader)

        # Generate #if/#elif chain for each OS
        codeindent = "    "
        blockStart = True
        codeBlock = []

        for osFuncGen in self.osLangGenList:
            if blockStart:
                codeBlock.append(self.getIfDynamic(osFuncGen.getOSCompileSwitch()))
            else:
                codeBlock.append(self.getElseIfDynamic(osFuncGen.getOSCompileSwitch()))
            codeBlock.extend(self.getCallBlock("langid", codeindent))

        codeBlock.append(self.getElseNotDynamic(osFuncGen.getOSCompileSwitch()))
        codeBlock.extend(self.staticFuncGen.getCallBlock("", codeindent))
        codeBlock.append(self.getEndOsDynamic())
        outputFile.writelines(codeBlock)

        # Complete the function
        outputFile.writelines([self.endFunction(self.functionName)])

class GenerateOSLanguageDetectCpp(object):
    def __init__(self, jsonLangData, fileName="os_language_detect.cpp"):
        self.fileName = fileName
        self.osFunctionGeneratorList = [DynamicLangIdLinux(jsonLangData), DynamicLangIdWindows(jsonLangData)]
        self.staticGenorator = StaticSelection(jsonLangData)
        self.eula = EulaText("MIT_open")
        self.commentGenerator = CommentGenerator(CommentBlock.getCommentMarkers(fileName), 80)

    add def generateDoxygenComment(self, name, desc, paramList, retQual): to CommentGenerator class

langDetectFileHeader = ["","#include <memory>", "#include \"os_language_detect.h\"",""]
langDetectFileTail = ["",
                      "/**",
                      " * @brief SystemLanguageDetection::getLangId() implementation",
                      " * Determine the OS and call the appropriate function to get ",
                      " * the string list",
                      " *",
                      " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based ",
                      " *                                                      on the local settings",
                      " */",
                      "std::shared_ptr<ParserStringListInterface> SystemLanguageDetection::getLangId()",
                      "{",
                      "#if ((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                      "    return getDynamicLangID_LinuxbaseStringTypews();",
                      "#elif defined(DYNAMIC_INTERNATIONALIZATION)",
                      "    #error \"Unknown OS type, no dynamic language id function defined\"",
                      "#else // not defined(DYNAMIC_INTERNATIONALIZATION)",
                      "    return getStaticLang();",
                      "#endif // defined os and defined(DYNAMIC_INTERNATIONALIZATION))",
                      "}"
                     ]

GetDynamicLangIdLinuxHeader = ["#if ((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                               "  #include <cstdlib>",
                               "  #include <regex>",
                               "/**",
                               " * @brief Pull the LANG environment string and translate it into the appropriate",
                               " *        ParserStringListInterface object",
                               " *",
                               " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based on the",
                               " *                                                      \"LANG\" environment value",
                               " */",
                               "std::shared_ptr<ParserStringListInterface> getDynamicLangID_Linux()",
                               "{",
                               "    const char* langId = getenv(\"LANG\");",
                               "    if (nullptr != langId)",
                               "    {"
                              ]from file_tools.comment_block import CommentGenerator


GetDynamicLangIdLinuxTail =   ["    }",
                               "}",
                               "#endif //((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))"
                              ]

GetDynamicLangIdWinHeader = ["#if ((defined(_WIN64) || defined(_WIN32)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                             "#include <Windows.h>",
                             "/**",
                             " * @brief Get the windows local language id and translate it to the appropriate"
                             " *        ParserStringListInterface object",
                             " *",
                             " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based on the",
                             " *                                                      LANGID value",
                             " */",
                             "std::shared_ptr<ParserStringListInterface> getDynamicLangID_Windows()",
                             "{",
                             "    switch(langId)",
                             "    {"
                            ]

GetDynamicLangIdWinTail =   ["    }",
                             "}",
                             "#endif //((defined(_WIN64) || defined(_WIN32)) && defined(DYNAMIC_INTERNATIONALIZATION))"
                            ]

getStaticLangHeader = ["#if !defined(DYNAMIC_INTERNATIONALIZATION))",
                       "/**",
                       " * @brief Get the default ParserStringListInterface object",
                       " *",
                       " * @return std::shared_ptr<ParserStringListInterface> - Pointer to the default ParserStringListInterface",
                       " */",
                       "std::shared_ptr<ParserStringListInterface> getStaticLang()",
                       "{",

]
