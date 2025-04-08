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
#import asyncio

#from googletrans import Translator

from jsonLanguageDescriptionList import LanguageDescriptionList
from jsonStringClassDescription import StringClassDescription

from file_tools.common.param_return_tools import ParamRetDict
from file_tools.common.doxygen_gen_tools import CDoxyCommentGenerator

from file_tools.string_name_generator import StringClassNameGen
from file_tools.os_lang_select_tools import OsLangSelectFunctionHelper

from file_tools.linux_lang_select import LinuxLangSelectFunctionGenerator
from file_tools.windows_lang_select import WindowsLangSelectFunctionGenerator
# Add additional OS lang select classes here

from file_tools.static_lang_select import StaticLangSelectFunctionGenerator
from file_tools.master_lang_select import MasterSelectFunctionGenerator


class GenerateOSLanguageFiles(OsLangSelectFunctionHelper):
    def __init__(self, filePath, owner = None, eulaName = None):
        """!
        @brief GenerateOSLanguageDetectFiles constructor

        @param filePath {string} path to put the .h and .cpp generated files in
        """
        super().__init__(eulaName, StringClassNameGen.getBaseClassName())
        self.filePath = filePath
        self.owner = owner
        self.versionMajor = 0
        self.versionMinor = 5
        self.autoToolName = self.__class__.__name__+str(self.versionMajor)+"."+str(self.versionMinor)

        self.doxyCommentGen = CDoxyCommentGenerator()
        self.groupName = "OsLanguageSelection"
        self.groupDesc = "OS laguage detection and selection utility"

        self.jsonLangData = LanguageDescriptionList()
        self.jsonStringsData = StringClassDescription()

        self.osLangSelectList = [LinuxLangSelectFunctionGenerator(self.jsonLangData),
                                 WindowsLangSelectFunctionGenerator(self.jsonLangData)
                                 # Add additional OS lang select classes here
                                 ]
        self.staticSelect = StaticLangSelectFunctionGenerator(self.jsonLangData)

        self.masterFunctionName = "getLocalParserStringListInterface"
        self.nameSpaceName = StringClassNameGen.getNamespaceName()
        self.masterFunction = MasterSelectFunctionGenerator(self.masterFunctionName,
                                                            StringClassNameGen.getBaseClassName())
        self.ifDynamicDefined = "defined("+StringClassNameGen.getDynamicCompileswitch()+")"
        self.declareIndent = 8
        self.functionIndent = 4

    def _generateFileHeader(self):
        """!
        @brief Generate the boiler plate file header with copyright and eula
        """
        return super()._generateFileHeader(self.autoToolName, 2025, self.owner)

    def _generateHFileName(self, langName = None):
        if langName is not None:
            return StringClassNameGen.getLangClassName(langName)+".h"
        else:
            return StringClassNameGen.getBaseClassName()+".h"

    def _generateCppFileName(self, langName = None):
        if langName is not None:
            return StringClassNameGen.getLangClassName(langName)+".cpp"
        else:
            return StringClassNameGen.getBaseClassName()+".cpp"

    def _writeCppFile(self, cppFile):
        """!
        @brief Write the OS language selection CPP file
        @param cppFile {File} File to write the data to
        """
        # Write the common header data
        cppFile.writelines(self._generateFileHeader())
        cppFile.writelines(["\n"]) # whitespace for readability

        # Add the common includes
        includeFileList = ["<memory>", "<cstring>", "<string>", self._generateHFileName()]
        cppFile.writelines(self.genIncludeBlock(includeFileList))

        # Add the parser string files
        languageList = self.jsonLangData.getLanguageList()
        for langName in languageList:
            langCompileSwitch = self.jsonLangData.getLanguageCompileSwitchData(langName)
            ifdef = "(defined("+langCompileSwitch+") || "+self.ifDynamicDefined+")\n"
            cppFile.writelines(["#if "+ifdef])
            cppFile.writelines(self._genInclude(self._generateHFileName(langName)))
            cppFile.writelines(["#endif // "+ifdef])

        # Add doxygen group start
        cppFile.writelines(["\n"]) # whitespace for readability
        cppFile.writelines(self.doxyCommentGen.genDoxyDefgroup(self.groupName, self.groupDesc, self._generateCppFileName()))

        cppFile.writelines(["\n"]) # whitespace for readability
        cppFile.writelines(self._genUsingNamespace(self.nameSpaceName))

        # Add the language dependent selection functions
        for langSelectFunction in self.osLangSelectList:
            cppFile.writelines(["\n"]) # whitespace for readability
            langSelectFunction.genFunction(cppFile)

        # Add the static selection function
        cppFile.writelines(["\n"]) # whitespace for readability
        self.staticSelect.genFunction(cppFile)

        # Add the master selection function
        cppFile.writelines(["\n"]) # whitespace for readability
        self.masterFunction.genFunction(cppFile, self.osLangSelectList, self.staticSelect)

        # Complete the doxygen group
        cppFile.writelines(["\n"]) # whitespace for readability
        cppFile.writelines(self.doxyCommentGen.genDoxyGroupEnd())

    def _genPropertyInlineCode(self, langName, propertyName, propertyReturn, isText):
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
            codeTxt.append(returnType+" returnData;")

            # Determine data type
            for dataItem in dataList:
                if isText:
                    codeTxt.append(self.getAddStringListStatment("returnData", dataItem))
                else:
                    codeTxt.append(self.getAddValueListStatment("returnData", dataItem))
            codeTxt.append("return returnData;")
        else:
            # Single item case
            dataItem = self.jsonLangData.getLanguagePropertyData(langName, propertyName)

            # Determine data type
            if isText:
                codeTxt.append("return (\""+dataItem+"\");")
            else:
                codeTxt.append("return ("+dataItem+");")

        return codeTxt

    def _writePropertyMethods(self, hFile, langName = None):
        """!
        @brief Write the property method definitions

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

            # Translate the return type
            xlatedRetDict, isText = self.xlateReturnDict(propertyReturn)

            # Get the language data replacements
            if langName is not None:
                inlineText = self._genPropertyInlineCode(langName, propertyName, xlatedRetDict, isText)
            else:
                inlineText = None

            # Output final declaration
            hFile.writelines(self.declareFunctionWithDecorations(propertyMethod,
                                                                 propertyDesc,
                                                                 propertyParams,
                                                                 xlatedRetDict,
                                                                 self.declareIndent,
                                                                 noDoxyGeneration,
                                                                 prefix,
                                                                 postfix,
                                                                 inlineText))

            if not noDoxyGeneration:
                hFile.writelines(["\n"]) # whitespace for readability

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

    def _parseTranlateString(self, baseString):
        """!
        @brief Convert the input string to an output string stream
        @param baseString {string} String to convert
        """
        matchList = re.finditer(r'@[a-zA-Z_][a-zA-Z0-9_]*@', baseString)
        streamString = StringClassNameGen.getParserStrStreamType()+" parserstr;  parserstr"
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

        streamString += "; return parserstr.str();"
        return [streamString]

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

    def _writeTranslateMethods(self, hFile, langName = None):
        """!
        @brief Write the property method definitions

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

        tranlateMethodList = self.jsonStringsData.getTranlateMethodList()
        for translateMethodName in tranlateMethodList:
            transDesc, transParams, transReturn = self.jsonStringsData.getTranlateMethodFunctionData(translateMethodName)

            # Xlate the return data
            xlatedReturn, isText = self.xlateReturnDict(transReturn)

            # Xlate the param data
            xlatedParams = []
            if len(transParams) == 0:
                postfix = "const "+postfixFinal
            else:
                postfix = postfixFinal
                xlatedParams = self.xlateParamList(transParams)

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
                                                                 xlatedParams,
                                                                 xlatedReturn,
                                                                 self.declareIndent,
                                                                 noDoxyGeneration,
                                                                 prefix,
                                                                 postfix,
                                                                 inlineText))
            if not noDoxyGeneration:
                hFile.writelines(["\n"]) # whitespace for readability

    def _writeBaseHFile(self, hFile):
        """!
        @brief Write the OS language selection CPP file
        @param hFile {File} File to write the data to
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader())
        hFile.writelines(["\n"]) # whitespace for readability

        includeList = ["<cstddef>", "<cstdlib>", "<memory>", "<string>"]
        hFile.writelines(self.genIncludeBlock(includeList))

        hFile.writelines(["\n"]) # whitespace for readability
        hFile.writelines(self.doxyCommentGen.genDoxyDefgroup(self.groupName, self.groupDesc, StringClassNameGen.getBaseClassName()+".h"))

        hFile.writelines(["\n",
                        "using "+StringClassNameGen.getParserStringType()+" = std::string;          ///< Standard parser string definition\n",
                        "using "+StringClassNameGen.getParserCharType()+" = char;                ///< Standard parser character definition\n",
                        "\n"]),
        hFile.writelines(self.genNamespaceOpen(self.nameSpaceName))
        hFile.writelines(["\n"]) # whitespace for readability

        # Start class definition
        className = StringClassNameGen.getBaseClassName()
        hFile.writelines(self.genClassOpen(className,
                                           "Parser error/help string generation interface"))
        hFile.writelines(["    public:\n"])

        # Add default Constructor/destructor definitions
        hFile.writelines(self.genClassDefaultConstructorDestructor(className,
                                                                   self.declareIndent,
                                                                   True,
                                                                   False))

        # Generate the property fetch methods
        self._writePropertyMethods(hFile, None)

        # Generate the translated string generation methods
        self._writeTranslateMethods(hFile, None)

        # Add the documented function declareation
        hFile.writelines(self.masterFunction.declareFunction(self.declareIndent))

        # Close the class and namespace
        hFile.writelines(self.genClassClose(className))
        hFile.writelines(self.genNamespaceClose(self.nameSpaceName))

        # Complete the doxygen group
        hFile.writelines(self.doxyCommentGen.genDoxyGroupEnd())

    def _writeLangHFile(self, hFile, langName):
        """!
        @brief Write the OS language selection CPP file

        @param jsonStringsDef {dictionary} JSON function definitions
        @param jsonLangData {dictionary} JSON file language dictionary data
        @param hFile {File} File to write the data to
        @param langName {string} - Language name
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader())
        hFile.writelines(["\n"]) # whitespace for readability

        includeList = ["<cstdio>",
                       "<cstring>",
                       "<sstream>",
                       StringClassNameGen.getBaseClassName()+".h"]
        hFile.writelines(self.genIncludeBlock(includeList))
        hFile.writelines(["\n"]) # whitespace for readability

        # Set the class name
        className = StringClassNameGen.getLangClassName(langName)
        hFile.writelines(["using namespace "+self.nameSpaceName+";\n"])
        hFile.writelines(["using "+StringClassNameGen.getParserStrStreamType()+" = std::stringstream;\n", "\n"])

        # Start class definition
        hFile.writelines(self.genClassOpen(className,
                                            "Language specific parser error/help string generation interface",
                                            "public "+StringClassNameGen.getBaseClassName(),
                                            "final"))
        hFile.writelines(["    public:\n"])

        # Add default Constructor/destructor definitions
        hFile.writelines(self.genClassDefaultConstructorDestructor(className, self.declareIndent, False, True))

        # Add the property fetch methods
        self._writePropertyMethods(hFile, langName)
        hFile.writelines(["\n"]) # whitespace for readability

        # Add the string generation methods
        self._writeTranslateMethods(hFile, langName)

        # Close the class
        hFile.writelines(self.genClassClose(className))

    def generateCppFile(self):
        """!
        @brief Generate the OS language selection CPP file
        """
        returnStatus = False
        cppFileName = os.path.join(self.filePath, self._generateCppFileName())
        try:
            # open the file
            cppFile = open(cppFileName, 'w', encoding='utf-8')
            self._writeCppFile(cppFile)
            cppFile.close()
            returnStatus = True
        except:
            print("ERROR: Unable to open "+cppFileName+" for writing!")
        return returnStatus

    def generateBaseHFile(self):
        returnStatus = False
        hFileName = os.path.join(self.filePath, self._generateHFileName())
        try:
            # open the file
            hFile = open(hFileName, 'w', encoding='utf-8')
            self._writeBaseHFile(hFile)
            hFile.close()
            returnStatus = True
        except:
            print("ERROR: Unable to open "+hFileName+" for writing!")
        return returnStatus

    def generateLangHFiles(self):
        returnStatus = True
        languageList = self.jsonLangData.getLanguageList()
        for languageName in languageList:
            hFileName = os.path.join(self.filePath, self._generateHFileName(languageName))
            try:
                # open the file
                hFile = open(hFileName, 'w', encoding='utf-8')
                self._writeLangHFile(hFile, languageName)
                hFile.close()
            except:
                print("ERROR: Unable to open "+hFileName+" for writing!")
                returnStatus = False

        return returnStatus

    def genMainFiles(self):
        status = self.generateCppFile()
        status = status and self.generateBaseHFile()
        status = status and self.generateLangHFiles()
        return status

import argparse
def CommandMain():
    """!
    Utility command interface
    @param subcommand {string} JSON string file command
    """
    parser = argparse.ArgumentParser(prog="oslang_file_generator",
                                     description="Update argpaser library language string h/cpp files")
    parser.add_argument('subcommand', choices=['build'])
    args = parser.parse_args()

    fileGen = GenerateOSLanguageFiles("./test/inc", "Randal Eike")

    if args.subcommand.lower() == "build":
        fileGen.genMainFiles()
    else:
        print ("Error: Unknown file generation command: "+args.subcommand)
        SystemExit(1)


if __name__ == '__main__':
    CommandMain()