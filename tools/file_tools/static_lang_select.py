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

from .string_name_generator import StringClassNameGen
from .os_lang_select_tools import OsLangSelectFunctionHelper
from .jsonLanguageDescriptionList import LanguageDescriptionList

class StaticLangSelectFunctionGenerator(OsLangSelectFunctionHelper):
    """!
    Methods for compile switch determined language select function generation
    """
    def __init__(self, langData, functionName = "getParserStringListInterface_Static"):
        """!
        @brief StaticLangSelectFunctionGenerator constructor
        @param langData {string} JSON language description list file name
        @param functionName {string} Function name to be used for generation
        """
        super().__init__()
        self.selectFunctionName = functionName
        self.defStaticString = "!defined("+StringClassNameGen.getDynamicCompileswitch()+")"
        self.langData = LanguageDescriptionList(langData)

    def getFunctionName(self):
        return self.selectFunctionName

    def getOsDefine(self):
        return None

    def getOsDynamicDefine(self):
        return self.defStaticString

    def genFunctionDefine(self):
        """!
        @brief Get the function declaration string for the given name
        @return string list - Function comment block and declaration start
        """
        return(self._genFunctionDefine(self.selectFunctionName,
                                       "Determine the correct "+StringClassNameGen.getBaseClassName()+"<lang> from the compile switch setting",
                                       []))

    def genFunctionEnd(self):
        """!
        @brief Get the function declaration string for the given name
        @return string - Function close with comment
        """
        return self.endFunction(self.selectFunctionName)

    def genFunction(self, langJsonData, outfile):
        """!
        @brief Generate the function body text

        @param langJsonData {dictionary} JSON file language dictionary data
        @param outfile {file} File to output the function to
        """
        # Generate the #if and includes
        functionBody = []
        functionBody.append("#if "+self.defStaticString)
        functionBody.append("")  # whitespace for readability

        # Generate function doxygen comment and start
        functionBody.extend(self.genFunctionDefine())

        # Start function body generation
        bodyIndent = "".rjust(4, " ")
        staticClassName = StringClassNameGen.getBaseClassName()+"Static"

        # Generate #if #elf compile switch chain for each language in the dictionary
        firstLoop = True
        for langName, langData in langJsonData['languages'].items():
            if firstLoop:
                ifline = "  #if defined("+langData['compileSwitch']+")"
                firstLoop = False
            else:
                ifline = "  #elif defined("+langData['compileSwitch']+")"

            functionBody.append(ifline)
            functionBody.append(bodyIndent+"using "+staticClassName+" = "+StringClassNameGen.getLangClassName(langName)+";")


        # Add the final #else case
        functionBody.append("  #else //undefined language compile switch, use default")
        functionBody.append(bodyIndent+"using "+staticClassName+" = "+StringClassNameGen.getLangClassName(langJsonData['default']['name'])+";")
        functionBody.append("  #endif //end of language #if/#elifcompile switch chain")
        functionBody.append(bodyIndent+self.genMakePtrReturnStatement(staticClassName))

        # Complete the function
        functionBody.append(self.genFunctionEnd())
        functionBody.append("#endif // "+self.defStaticString)
        outfile.writelines(functionBody)

    def genReturnFunctionCall(self, indent = 4):
        """!
        @brief Generate the call code for the linux dynamic lang selection function
        @param indent {number} Code indentation spaces
        @return list of strings Formatted code lines
        """
        indentText = "".rjust(indent, " ")
        doCall = indentText+"return "+self.selectFunctionName+"();"
        return [doCall]

    def genUnitTest(self, langJsonData, getIsoMethod, outfile):
        """!
        @brief Generate all unit tests for the selection function

        @param langJsonData {dictionary} JSON file language dictionary data
        @param getIsoMethod {string} Name of the ParserStringListInterface return ISO code method
        @param outfile {file} File to output the function to
        """
        # Generate block start code
        blockStart = []
        blockStart.append("#if "+self.defStaticString)
        externDef = "extern "
        externDef += self.returnType
        externDef += " "
        externDef += self.selectFunctionName
        externDef += "();"
        blockStart.append(externDef)
        outfile.writelines(blockStart)

        # Generate the testgenDoxyMethodComment
        testBlockName = "StaticSelectFunction"
        bodyIndent = "".rjust(4, " ")
        breifDesc = "Test "+self.selectFunctionName+" selection case"
        testBody = self.genDoxyMethodComment(breifDesc, [])

        testVar = "testVar"
        testVarDecl = self.returnType+" "+testVar
        testVarTest = testVar+"."+getIsoMethod+"().c_str()"
        testBody.append("TEST("+testBlockName+", CompileSwitchedValue)")
        testBody.append("{")
        testBody.append(bodyIndent+"// Generate the test language string object")
        testBody.append(bodyIndent+testVarDecl+" = "+self.selectFunctionName+"();")
        testBody.append("") # whitespace for readability

        firstLoop = True
        for langName, langData in langJsonData['languages'].items():
            if firstLoop:
                testBody.append("  #if defined("+langData['compileSwitch']+")")
                firstLoop = False
            else:
                testBody.append("  #elif defined("+langData['compileSwitch']+")")

            testBody.append(bodyIndent+"EXPECT_STREQ(\""+langData['isoCode']+"\", "+testVarTest+";")

        # Add the final #else case
        testBody.append("  #else //undefined language compile switch, use default")
        testBody.append(bodyIndent+"EXPECT_STREQ(\""+langJsonData['default']['isoCode']+"\", "+testVarTest+";")
        testBody.append("  #endif //end of language #if/#elifcompile switch chain")

        # Complete the function
        testBody.append("}")
        outfile.writelines(testBody)

        # Generate block end code
        outfile.writelines(["#endif // "+self.defStaticString])

    def genUnitTestFunctionCall(self, checkVarName, indent = 4):
        """!
        @brief Generate the call code for the linux dynamic lang selection unit test
        @param checkVarName {string} Unit test expected variable name
        @param indent {number} Code indentation spaces
        @return list of strings Formatted code lines
        """
        indentText = "".rjust(indent, " ")
        doCall = indentText+self.returnType+" "+checkVarName+" = "+self.selectFunctionName+"();"
        return [doCall]