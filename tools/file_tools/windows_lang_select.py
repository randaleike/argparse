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

from .common.file_gen_tools import ParamRetDict
from .string_name_generator import StringClassNameGen
from .os_lang_select_tools import OsLangSelectFunctionHelper
from .jsonLanguageDescriptionList import LanguageDescriptionList

class WindowsLangSelectFunctionGenerator(OsLangSelectFunctionHelper):
    """!
    Methods for Windows language select function generation
    """
    def __init__(self, langData, functionName = "getParserStringListInterface_Windows"):
        """!
        @brief WindowsLangSelectFunctionGenerator constructor
        @param langData {string} JSON language description list file name
        @param functionName {string} Function name to be used for generation
        """
        super().__init__()
        self.paramDictList = [ParamRetDict.buildParamDict("langId", "LANGID", "Return value from GetUserDefaultUILanguage() call")]
        self.selectFunctionName = functionName
        self.defOsString = "(defined(_WIN64) || defined(_WIN32))"
        self.defDynamicOsString = "("+self.defOsString+" && defined("+StringClassNameGen.getDynamicCompileswitch()+"))"
        self.langData = LanguageDescriptionList(langData)

    def getFunctionName(self):
        return self.selectFunctionName

    def getOsDefine(self):
        return self.defOsString

    def getOsDynamicDefine(self):
        return self.defDynamicOsString

    def genFunctionDefine(self):
        """!
        @brief Get the function declaration string for the given name

        @param name {string}  Function name

        @return string list - Function comment block and declaration start
        """
        return self._genFunctionDefine(self.selectFunctionName,
                                    "Determine the correct "+StringClassNameGen.getBaseClassName()+"<lang> from the input LANGID value",
                                    self.paramDictList)

    def genFunctionEnd(self):
        """!
        @brief Get the function declaration string for the given name
        @return string - Function close with comment
        """
        return self.endFunction(self.selectFunctionName)

    def genFunction(self, outfile):
        """!
        @brief Generate the function body text

        @param langJsonData {dictionary} JSON file language dictionary data
        @param outfile {file} File to output the function to
        """
        # Generate the #if and includes
        functionBody = []
        functionBody.append("#if "+self.defDynamicOsString)
        functionBody.append("  #include <Windows.h>")
        functionBody.append("")  # whitespace for readability

        # Generate function doxygen comment and start
        functionBody.extend(self.genFunctionDefine())

        # Start function body generation
        bodyIndent = "    "
        functionBody.append(bodyIndent+"switch("+self.paramDictList[0]['name']+" & 0x0FF)")
        functionBody.append(bodyIndent+"{")

        # Generate case if chain for each language in the dictionary
        caseIndent = bodyIndent+"    "
        caseBodyIndent = caseIndent+"    "
        langList = self.langData.getLanguageList()
        for langName in langList:
            langCodes, langRegionList = self.langData.getLanguageLANGIDData(langName)
            for id in langCodes:
                caseline =  caseIndent+"case"
                caseline += hex(id)
                caseline += ":"
                functionBody.append(caseline)
            caseAssign = caseBodyIndent+self.genMakePtrReturnStatement(langName)
            functionBody.append(caseAssign)
            functionBody.append(caseBodyIndent+"break;")

        # Add the final default case
        functionBody.append(caseIndent+"default:")
        functionBody.append(caseBodyIndent+self.genMakePtrReturnStatement(self.langJsonData['default']['name']))
        functionBody.append(bodyIndent+"}")

        # Complete the function
        functionBody.append(self.genFunctionEnd())
        functionBody.append("#endif // "+self.defDynamicOsString)
        outfile.writelines(functionBody)

    def genReturnFunctionCall(self, indent = 4):
        """!
        @brief Generate the call code for the linux dynamic lang selection function
        @param indent {number} Code indentation spaces
        @return list of strings Formatted code lines
        """
        indentText = "".rjust(indent, " ")
        localVarName = "langId"

        getParam = indentText
        getParam += ParamRetDict.getParamType(self.paramDictList[0])
        getParam += " "
        getParam += localVarName
        getParam += "= GetUserDefaultUILanguage();"

        doCall = indentText
        doCall += "return "
        doCall += self.selectFunctionName
        doCall += "("
        doCall += localVarName
        doCall += ");"

        return [getParam, doCall]

    def _genUnitTestTest(self, testName, langid, expectedIso, getIsoMethod):
        """!
        @brief Generate single selection function unit test instance

        @param testName {string} Name of the test
        @param langid {number} LANGID value to test
        @param expectedIso {string} Expected ISO return code for the test variable
        @param getIsoMethod {string} Name of the ParserStringListInterface return ISO code method

        @return list of strings - Output C code
        """
        testBlockName = "WindowsSelectFunction"
        bodyIndent = "".rjust(4, " ")
        breifDesc = "Test "+self.selectFunctionName+" "+str(langid)+" selection case"
        testBody = self.genDoxyMethodComment(breifDesc, [])

        testVar = "testVar"
        testVarDecl = self.returnType+" "+testVar
        testVarTest = testVar+"."+getIsoMethod+"().c_str()"
        testBody.append("TEST("+testBlockName+", "+testName+")")
        testBody.append("{")
        testBody.append(bodyIndent+"// Generate the test language string object")

        testBody.append("") # whitespace for readability
        testBody.append(bodyIndent+testVarDecl+" = "+self.selectFunctionName+"("+str(langid)+");")
        testBody.append(bodyIndent+"EXPECT_STREQ(\""+expectedIso+"\", "+testVarTest+";")
        testBody.append("}")
        return testBody

    def genUnitTest(self, langJsonData, getIsoMethod, outfile):
        """!
        @brief Generate all unit tests for the selection function

        @param langJsonData {dictionary} JSON file language dictionary data
        @param getIsoMethod {string} Name of the ParserStringListInterface return ISO code method
        @param outfile {file} File to output the function to
        """
        # Generate block start code
        blockStart = []
        blockStart.append("#if "+self.defDynamicOsString)
        externDef = "extern "
        externDef += self.returnType
        externDef += " "
        externDef += self.selectFunctionName
        externDef += "("
        externDef += ParamRetDict.getParamType(self.paramDictList[0])
        externDef += " "
        externDef += ParamRetDict.getParamName(self.paramDictList[0])
        externDef += ");"
        blockStart.append(externDef)
        outfile.writelines(blockStart)

        # Generate the tests
        for langName, langData in langJsonData['languages'].items():
            for langId in langData['LANGID_regions']:
                # Generate test for each region of known language
                testName = langName.capitalize()+"_"+str(langId)+"_Selection)"
                testBody = self._genUnitTestTest(testName, langId, langData['isoCode'], getIsoMethod)
                testBody.append("") # whitespace for readability
                outfile.writelines(testBody)

            # Generate test for unknown region of known language(s)
            for langCode in langData['LANGID']:
                unknownRegionTestName = langName.capitalize()+"_unknownRegion_00"+str(langCode)+"_Selection)"
                unknownRegionBody = self._genUnitTestTest(unknownRegionTestName, langCode, langData['isoCode'], getIsoMethod)
                unknownRegionBody.append("") # whitespace for readability
                outfile.writelines(unknownRegionBody)

            # Generate test for unknown region of known language(s)
            for langCode in langData['LANGID']:
                unknownRegionTestName = langName.capitalize()+"_unknownRegion_FF"+str(langCode)+"_Selection)"
                unknownRegionBody = self._genUnitTestTest(unknownRegionTestName, 0xFF00+langCode, langData['isoCode'], getIsoMethod)
                unknownRegionBody.append("") # whitespace for readability
                outfile.writelines(unknownRegionBody)

        # Generate test for unknown region of unknown language and expect default
        unknownLangBody = self._genUnitTestTest("UnknownLanguageDefaultSelection",
                                                0,
                                                langJsonData['default']['isoCode'],
                                                getIsoMethod)
        outfile.writelines(unknownLangBody)

        # Generate block end code
        outfile.writelines(["#endif // "+self.defDynamicOsString])

    def genUnitTestFunctionCall(self, checkVarName, indent = 4):
        """!
        @brief Generate the call code for the linux dynamic lang selection unit test
        @param checkVarName {string} Unit test expected variable name
        @param indent {number} Code indentation spaces
        @return list of strings Formatted code lines
        """
        indentText = "".rjust(indent, " ")
        localVarName = "langId"

        getParam = indentText
        getParam += ParamRetDict.getParamType(self.paramDictList[0])
        getParam += " "
        getParam += localVarName
        getParam += "= GetUserDefaultUILanguage();"

        doCall = indentText
        doCall += self.returnType
        doCall += " "
        doCall += checkVarName
        doCall += " = "
        doCall += self.selectFunctionName
        doCall += "("
        doCall += localVarName
        doCall += ");"

        return [getParam, doCall]