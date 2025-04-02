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

from file_tools.file_gen_tools import GenerateCppFileHelper
from file_tools.file_gen_tools import StringClassNameGen

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

        self.osLangSelectList = [LinuxLangSelectFunctionGenerator(),
                                 WindowsLangSelectFunctionGenerator()
                                 # Add additional OS lang select classes here
                                 ]
        self.staticSelect = StaticLangSelectFunctionGenerator()

        self.masterFunctionName = "getLocalParserStringListInterface"
        self.nameSpaceName = "argparser"
        self.masterFunction = MasterSelectFunctionGenerator(self.masterFunctionName,
                                                            self.nameSpaceName)
        self.declareIndent = 8

    def _generateFileHeader(self, outfile):
        """!
        @brief Generate the boiler plate file header with copyright and eula
        """
        return super()._generateFileHeader(self.autoToolName, 2025, self.owner)

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
        self._genInclude(StringClassNameGen.getBaseClassName()+".h", cppFile)

        # Add the parser string files
        for langData in jsonLangData['languages']:
            filename = StringClassNameGen.getBaseClassName()+langData['name']+".h"
            ifdef = "("+langData['compileSwitch']+" || defined("+StringClassNameGen.getDynamicCompileswitch()+"))"
            cppFile.writelines(["#if "+ifdef])
            self._genInclude(filename, cppFile)
            cppFile.writelines(["#endif // "+ifdef])

        # Add doxygen group start
        cppFile.writelines([""]) # whitespace for readability
        self._genDoxyDefgroup(self.groupName, cppFile, ".cpp")

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
        self._genDoxyGroupEnd(cppFile)

    def _writeBaseHFile(self, jsonStringsDef, hFile):
        """!
        @brief Write the OS language selection CPP file

        @param jsonStringsDef {dictionary} JSON function definitions
        @param hFile {File} File to write the data to
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader(hFile))
        hFile.writelines([""]) # whitespace for readability
        self._genDoxyDefgroup(self.groupName, self.groupDesc, hFile, ".h")

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
                        "namespace argparser",
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
        for baseFunction in jsonStringsDef["propertyFunctions"]:
            if baseFunction['isConst']:
                postfix = "const = 0"
            else:
                postfix = "= 0"

            hFile.writelines(self.declareFunctionWithDecorations(baseFunction['name'],
                                                                 baseFunction['briefDesc'],
                                                                 baseFunction['params'],
                                                                 baseFunction['return'],
                                                                 self.declareIndent,
                                                                 False,
                                                                 "[[nodiscard]] virtual",
                                                                 postfix))

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
        self._genDoxyGroupEnd(hFile)

    def _writeLangHFile(self, jsonStringsDef, jsonLangEntry, hFile, langName):
        """!
        @brief Write the OS language selection CPP file

        @param jsonStringsDef {dictionary} JSON function definitions
        @param jsonLangEntry {dictionary} JSON file language dictionary data
        @param hFile {File} File to write the data to
        @param langName {string} - Language name
        """
        # Write the common header datajsonStringsDef
        hFile.writelines(self._generateFileHeader(hFile))
        hFile.writelines([""]) # whitespace for readability
        self._genDoxyDefgroup(self.groupName, self.groupDesc, hFile, ".h")

        hFile.writelines(["", "#pragma once"])
        hFile.writelines(["", "// Includes"])

        # Assume base class
        className = StringClassNameGen.getLangClassName(langName)
        classNameInherit = " final : public "+StringClassNameGen.getBaseClassName()

        self._genInclude("<cstdio>", hFile)
        self._genInclude("<cstring>", hFile)
        self._genInclude("<sstream>", hFile)
        self._genInclude(StringClassNameGen.getBaseClassName()+".h", hFile)
        hFile.writelines(["",
                        "using namespace argparser;",
                        "using "+StringClassNameGen.getParserStrStreamType()+" = std::stringstream;",
                        ""])

        # Start class definition
        hFile.writelines(self.genClassStart(className,
                                            "Parser error/help string generation interface",
                                            "public "+StringClassNameGen.getBaseClassName(),
                                            "final"))
        hFile.writelines(["    public:"])

        # Add default Constructor/destructor definitions
        hFile.writelines(self.genClassDefaultConstructorDestructor(className, self.declareIndent, True, True))

        # Add the string generation functions
        for baseFunction in jsonStringsDef["propertyFunctions"]:
            if baseFunction['isConst']:
                postfix = "const final"
            else:
                postfix = "final"

            # Get the language data replacements
            inlineText = baseFunction['inline']
            regexMatch = r'@([a-zA-Z0-9\-_]*)@'
            langReplace = re.finditer(regexMatch, inlineText)
            for property in langReplace:
                propertyName = property[1]
                inlineText = inlineText.replace('@'+propertyName+'@', jsonLangEntry[propertyName])

            # Output final declaration
            hFile.writelines(self.declareFunctionWithDecorations(baseFunction['name'],
                                                                 baseFunction['briefDesc'],
                                                                 baseFunction['params'],
                                                                 baseFunction['return'],
                                                                 self.declareIndent,
                                                                 False,
                                                                 None,
                                                                 postfix,
                                                                 inlineText))

        for stringFunction in jsonStringsDef["translateFunctions"]:
            if stringFunction['isConst']:
                postfix = "const final"
            else:
                postfix = "final"

            # Determine if we need
            hFile.writelines(self.declareFunctionWithDecorations(stringFunction['name'],
                                                                 stringFunction['briefDesc'],
                                                                 stringFunction['params'],
                                                                 stringFunction['return'],
                                                                 self.declareIndent,
                                                                 False,
                                                                 None,
                                                                 postfix,
                                                                 None))

        # Complete the doxygen group
        self._genDoxyGroupEnd(hFile)


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
