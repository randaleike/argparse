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

import os
import re
import asyncio

from googletrans import Translator

from file_tools.jsonLanguageDescriptionList import LanguageDescriptionList
from file_tools.jsonStringClassDescription import StringClassDescription

from file_tools.common.file_gen_tools import GenerateCppFileHelper
from file_tools.common.param_return_tools import ParamRetDict
from file_tools.string_name_generator import StringClassNameGen

from file_tools.linux_lang_select import LinuxLangSelectFunctionGenerator
from file_tools.windows_lang_select import WindowsLangSelectFunctionGenerator
# Add additional OS lang select classes here

from file_tools.static_lang_select import StaticLangSelectFunctionGenerator
from file_tools.master_lang_select import MasterSelectFunctionGenerator


class GenerateOSLanguageDetectFiles(GenerateCppFileHelper):
    def __init__(self, filePath, owner = None):
        """!
        @brief GenerateOSLanguageDetectFiles constructor

        @param filePath {string} path to put the .h and .cpp generated files in
        """
        super().__init__(StringClassNameGen.getBaseClassName())
        self.filePath = filePath
        self.owner = owner
        self.versionMajor = 0
        self.versionMinor = 5
        self.autoToolName = self.__class__.__name__+str(self.versionMajor)+"."+str(self.versionMinor)

        self.groupName = "OsLanguageSelection"
        self.groupDesc = "OS laguage detection and selection utility"

        self.jsonLangData = LanguageDescriptionList("argparser-lang-list.json")
        self.jsonStringsData = StringClassDescription("argparser-string-def.json")

        self.osLangSelectList = [LinuxLangSelectFunctionGenerator("argparser-lang-list.json"),
                                 WindowsLangSelectFunctionGenerator("argparser-lang-list.json")
                                 # Add additional OS lang select classes here
                                 ]
        self.staticSelect = StaticLangSelectFunctionGenerator("argparser-lang-list.json")

        self.masterFunctionName = "getLocalParserStringListInterface"
        self.nameSpaceName = "argparser"
        self.masterFunction = MasterSelectFunctionGenerator(self.masterFunctionName,
                                                            StringClassNameGen.getBaseClassName())
        self.declareIndent = 8
        self.ifDynamicDefined = "defined("+StringClassNameGen.getDynamicCompileswitch()+")"

    def _generateFileHeader(self):
        """!
        @brief Generate the boiler plate file header with copyright and eula
        """
        return super()._generateFileHeader(self.autoToolName, 2025, self.owner)

    def _generateHFileName(self, langName = None):
        if langName is not None:
            return StringClassNameGen.getBaseClassName()+langName.capitalize()+".h"
        else:
            return StringClassNameGen.getBaseClassName()+".h"

    def _writeCppFile(self, jsonLangData, cppFile):
        """!
        @brief Write the OS language selection CPP file
        @param jsonLangData {dictionary} JSON file language dictionary data
        @param cppFile {File} File to write the data to
        """
        # Write the common header data
        cppFile.writelines(self._generateFileHeader(cppFile))
        cppFile.writelines([""]) # whitespace for readability

        # Add the common includess
        cppFile.writelines(["// Includes"])
        self._genInclude("<memory>", cppFile)
        self._genInclude("<cstring>", cppFile)
        self._genInclude("<string>", cppFile)
        self._genInclude(self._generateHFileName(), cppFile)

        # Add the parser string files
        languageList = self.jsonLangData.getLanguageList()
        for langName in languageList:
            langCompileSwitch = self.jsonLangData.getLanguageCompileSwitchData(langName)
            ifdef = "#if (defined("+langCompileSwitch+") || "+self.ifDynamicDefined+")"
            cppFile.writelines(["#if "+ifdef])
            self._genInclude(self._generateHFileName(langName), cppFile)
            cppFile.writelines(["#endif // "+ifdef])

        # Add doxygen group start
        cppFile.writelines([""]) # whitespace for readability
        cppFile.writelines(self.genDoxyDefgroup(self.groupName, self.groupDesc, self.fileName+".cpp"))

        # Add the language dependent selection functions
        for langSelectFunction in self.osLangSelectList:
            cppFile.writelines([""]) # whitespace for readability
            langSelectFunction.genFunction(jsonLangData, cppFile)

        # Add the static selection function
        cppFile.writelines([""]) # whitespace for readability
        self.staticSelect(jsonLangData, cppFile)

        # Add the master selection function
        cppFile.writelines([""]) # whitespace for readability
        self.masterFunction(jsonLangData, cppFile)

        # Complete the doxygen group
        cppFile.writelines(self.genDoxyGroupEnd())

    def _genPropertyInlineCode(self, langName, propertyName, propertyReturn):
        """!
        @brief Generate property function inline code
        @param langName {string} Language name
        @param propertyName {string} Language property name
        @param propertyReturn {dictionary} Property method return dictionary
        @return list of strings - Inline code
        """
        returnType = ParamRetDict.getReturnType(propertyReturn)
        codeTxt = []
        if ParamRetDict.isReturnList(propertyReturn):
            # List case
            dataList = self.jsonLangData.getLanguagePropertyData(langName, propertyName)
            codeTxt.append("{")

            # Determine data type
            if returnType == "text":
                codeTxt.append("std::list<"+StringClassNameGen.getParserStringType()+"> returnData;")
                for dataItem in dataList:
                    codeTxt.append("returnData.emplace_back(\""+dataItem+"\");")
            elif returnType == "number":
                codeTxt.append("std::list<LANGID> returnData;")
                for dataItem in dataList:
                    codeTxt.append("returnData.emplace_back("+dataItem+");")
            else:
                codeTxt.append("std::list<"+returnType+"> returnData;")
                for dataItem in dataList:
                    codeTxt.append("returnData.emplace_back("+dataItem+");")

            codeTxt.append("return returnData;")
            codeTxt.append("}")
        else:
            # Single item case
            dataItem = self.jsonLangData.getLanguagePropertyData(langName, propertyName)
            # Determine data type
            if returnType == "text":
                codeTxt.append("{return (\""+dataItem+"\");}")
            else:
                codeTxt.append("{return ("+dataItem+");}")

        return codeTxt

    def _writePropertyMethods(self, jsonStringsDef, jsonLangData, hFile, langName = None):
        """!
        @brief Write the property method definitions

        @param jsonStringsDef {dictionary} JSON function definitions
        @param jsonLangData {dictionary} JSON file language dictionary data
        @param hFile {File} File to write the data to
        @param langName {string} Language name or None this is for the base file
        """
        # Add the property fetch methods
        if langName is None:
            postfixFinal = "= 0"
            prefix = '[[nodiscard]] virtual'
            noDoxyGeneration = False
        else:
            postfixFinal = "final"
            prefix = None
            noDoxyGeneration = True

        propertyMethodList = self.jsonStringsData.getPropertyMethodList()
        for propertyMethod in propertyMethodList:
            propertyName, propertyDesc, propertyParams, propertyReturn = self.jsonStringsData.getPropertyMethodData(propertyMethod)
            if len(propertyParams) == 0:
                postfix = "const " + postfixFinal
            else:
                postfix = postfixFinal

            # Get the language data replacements
            if langName is not None:
                inlineText = self._genPropertyInlineCode(langName, propertyName, propertyReturn)
            else:
                inlineText = None

            # Output final declaration
            hFile.writelines(self.declareFunctionWithDecorations(propertyMethod,
                                                                 propertyDesc,
                                                                 propertyParams,
                                                                 propertyReturn,
                                                                 self.declareIndent,
                                                                 noDoxyGeneration,
                                                                 prefix,
                                                                 postfix,
                                                                 inlineText))

            if not noDoxyGeneration:
                hFile.writelines([""]) # whitespace for readability

    def _translateString(self, baseLanguage, baseText, targetLang):
        """!
        @brief Translate the baseText string into the target language from the base language
        @param baseLanguage {string} Google translate language code of the baseText string
        @param baseText {string} String to translate and output
        @param targetLang {string} Google translate language code for the translated baseText string
        @return string - Translated string
        """
        if baseLanguage == targetLang:
            return baseText
        else:
            # @todo add google translate call
            return baseText

    def _parseTranlateString(baseString):
        """!
        @brief Convert the input string to an output string stream
        @param baseString {string} String to convert
        """
        matchList = re.finditer(r'@[a-zA-Z_][a-zA-Z0-9_]*@', baseString)
        streamString = "{"+StringClassNameGen.getParserStrStreamType+" parserstr;  parserstr"
        previousEnd = 0

        for matchData in matchList:
            # Add text data prior to first match if any
            if matchData.start() > previousEnd:
                streamString += " << \""
                streamString += r'{}'.format(baseString[previousEnd:matchData.start()])
                streamString += "\""

            # Add the matched parameter
            streamString += " << "
            streamString += matchData.group()[1:-1]
            previousEnd = matchData.end()

        # Add the trailing string
        if previousEnd < len(baseString):
            streamString += " << \""
            streamString += baseString[previousEnd:]
            streamString += "\""

        streamString += "; return parserstr.str();}"
        return streamString

    def _generateTranslateInlineCode(self, baseLanguage, baseText, targetLang):
        """!
        @brief Generate the inline string output code
        @param baseLanguage {string} Google translate language code of the baseText string
        @param baseText {string} String to translate and output
        @param targetLang {string} Google translate language code for the translated baseText string
        @return list of strings - Inline code
        """
        translatedText = self._translateString(baseLanguage, baseText, targetLang)
        return self._parseTranlateString(translatedText)

    def _writeTranslateMethods(self, hFile, langName = None, googleTransCode = None):
        """!
        @brief Write the property method definitions

        @param hFile {File} File to write the data to
        @param langName {string} Language name or None this is for the base file
        @param googleTransCode {string} Google translate generation
        """
        # Add the property fetch methods
        if langName is None:
            postfixFinal = "= 0"
            prefix = '[[nodiscard]] virtual'
            noDoxyGeneration = False
        else:
            postfixFinal = "final"
            prefix = None
            noDoxyGeneration = True

        tranlateMethodList = self.jsonStringsData.getTranlateMethodList()
        for translateMethodName in tranlateMethodList:
            transDesc, transParams, transReturn = self.jsonStringsData.getTranlateMethodFunctionData(translateMethodName)
            if len(transParams) == 0:
                postfix = "const "+postfixFinal
            else:
                postfix = postfixFinal

            # Get the language generation string
            if langName is not None:
                targetLang = self.jsonLangData.getLanguageGoogleCodeData(langName)
                baseLanguage, baseText = self.jsonStringsData.getTranlateMethodTextData(translateMethodName)
                inlineText = self._generateTranslateInlineCode(baseLanguage, baseText, targetLang)
            else:
                inlineText = None

            # Determine if we need
            hFile.writelines(self.declareFunctionWithDecorations(translateMethodName,
                                                                 transDesc,
                                                                 transParams,
                                                                 transReturn,
                                                                 self.declareIndent,
                                                                 noDoxyGeneration,
                                                                 prefix,
                                                                 postfix,
                                                                 inlineText))
            if not noDoxyGeneration:
                hFile.writelines([""]) # whitespace for readability

    def _writeBaseHFile(self, jsonStringsDef, jsonLangData, hFile):
        """!
        @brief Write the OS language selection CPP file

        @param jsonStringsDef {dictionary} JSON function definitions
        @param jsonLangData {dictionary} JSON file language dictionary data
        @param hFile {File} File to write the data to
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader(hFile))
        hFile.writelines([""]) # whitespace for readability
        hFile.writelines(self._genDoxyDefgroup(self.groupName, self.groupDesc, self.fileName+".h"))

        hFile.writelines(["", "#pragma once"])
        hFile.writelines(["", "// Includes"])

        self._genInclude("<cstddef>", hFile)
        self._genInclude("<cstdlib>", hFile)
        self._genInclude("<memory>", hFile)
        self._genInclude("<string>", hFile)

        hFile.writelines(["",
                        "using "+StringClassNameGen.getParserStringType()+" = std::string;          ///< Standard parser string definition",
                        "using "+StringClassNameGen.getParserCharType()+" = char;                ///< Standard parser character definition",
                        "",
                        "namespace "+self.nameSpaceName,
                        "{",
                        ""])

        # Start class definition
        className = StringClassNameGen.getBaseClassName()
        hFile.writelines(self.genClassStart(className,
                                            "Parser error/help string generation interface"))
        hFile.writelines(["    public:"])

        # Add default Constructor/destructor definitions
        hFile.writelines(self.genClassDefaultConstructorDestructor(className,
                                                                   self.declareIndent,
                                                                   True,
                                                                   False))

        # Add the string generation functions
        self._writePropertyMethods(jsonStringsDef, jsonLangData, hFile, None)

        for stringFunction in jsonStringsDef["translateFunctions"]:
            if stringFunction['isConst']:
                postfix = "const = 0"
            else:
                postfix = "= 0"
            hFile.writelines(self.declareFunctionWithDecorations(stringFunction['name'],
                                                                 stringFunction['briefDesc'],
                                                                 stringFunction['params'],
                                                                 stringFunction['return'],
                                                                 self.declareIndent,
                                                                 False,
                                                                 "[[nodiscard]] virtual",
                                                                 postfix))

        # Add the documented function declareation
        hFile.writelines(self.masterFunction.declareFunction())

        # Complete the doxygen group
        hFile.writelines(self.genDoxyGroupEnd())

    def _writeLangHFile(self, jsonStringsDef, jsonLangData, hFile, langName):
        """!
        @brief Write the OS language selection CPP file

        @param jsonStringsDef {dictionary} JSON function definitions
        @param jsonLangData {dictionary} JSON file language dictionary data
        @param hFile {File} File to write the data to
        @param langName {string} - Language name
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader(hFile))
        hFile.writelines([""]) # whitespace for readability
        hFile.writelines(self._genDoxyDefgroup(self.groupName, self.groupDesc, self.fileName+".h"))

        hFile.writelines(["", "#pragma once"])
        hFile.writelines(["", "// Includes"])

        # Set the class name
        className = StringClassNameGen.getLangClassName(langName)
        self._genInclude("<cstdio>", hFile)
        self._genInclude("<cstring>", hFile)
        self._genInclude("<sstream>", hFile)
        self._genInclude(StringClassNameGen.getBaseClassName()+".h", hFile)
        hFile.writelines(["",
                        "using namespace "+self.nameSpaceName+";",
                        "using "+StringClassNameGen.getParserStrStreamType()+" = std::stringstream;",
                        ""])

        # Start class definition
        hFile.writelines(self.genClassStart(className,
                                            "Language specific parser error/help string generation interface",
                                            "public "+StringClassNameGen.getBaseClassName(),
                                            "final"))
        hFile.writelines(["    public:"])

        # Add default Constructor/destructor definitions
        hFile.writelines(self.genClassDefaultConstructorDestructor(className, self.declareIndent, False, True))

        # Add the property fetch methods
        self._writePropertyMethods(jsonStringsDef, jsonLangData, hFile, langName)
        hFile.writelines([""]) # whitespace for readability

        # Add the string generation methods
        self._writePropertyMethods(jsonStringsDef, hFile, langName)
        for stringMethod, methodData in jsonStringsDef["translateFunctions"].items():
            if len(methodData['params']) == 0:
                postfix = "const final"
            else:
                postfix = "final"

            # Get the language data replacements
            inlineText = []

            # Determine if we need
            hFile.writelines(self.declareFunctionWithDecorations(stringMethod,
                                                                 methodData['briefDesc'],
                                                                 methodData['params'],
                                                                 methodData['return'],
                                                                 self.declareIndent,
                                                                 True,
                                                                 None,
                                                                 postfix,
                                                                 inlineText))

        # Complete the doxygen group
        hFile.writelines(self.genDoxyGroupEnd())


    def generateCppFile(self, jsonLangData):
        """!
        @brief Generate the OS language selection CPP file
        @param jsonLangData {dictionary} JSON file language dictionary data
        """
        cppFileName = os.path.join(self.filePath, self.fileName+".cpp")
        try:
            # open the file
            cppFile = open(cppFileName, "w", encoding="utf-8")
            self._writeCppFile(jsonLangData, cppFile)
            cppFile.close()
        except:
            print("ERROR: Unable to open "+cppFileName+".cpp for writing!")

    def generateHFile(self):
        hFileName = os.path.join(self.filePath, self.fileName+".h")
        try:
            # open the file
            hFile = open(hFileName, "w", encoding="utf-8")
            self._writeCppFile(hFile)
            hFile.close()
        except:
            print("ERROR: Unable to open "+hFileName+".h for writing!")
