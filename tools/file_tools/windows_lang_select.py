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

from .file_gen_tools import StringClassNameGen
from .os_lang_select_tools import OsLangSelectFunctionHelper

class WindowsLangSelectFunctionGenerator(OsLangSelectFunctionHelper):
    """!
    Methods for Windows language select function generation
    """
    def __init__(self, functionName = "getParserStringListInterface_Windows"):
        """!
        @brief WindowsLangSelectFunctionGenerator constructor
        @param functionName {string} Function name to be used for generation
        """
        super().__init__()
        self.paramDictList = [{'name': "langId", 'type':"LANGID", 'desc': "Return value from GetUserDefaultUILanguage() call"}]
        self.selectFunctionName = functionName
        self.defOsString = "(defined(_WIN64) || defined(_WIN32))"
        self.defDynamicOsString = "("+self.defOsString+" && defined("+StringClassNameGen.getDynamicCompileswitch()+"))"

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
        return self._genFunctionEnd(self.selectFunctionName)

    def genFunction(self, langJsonData, outfile):
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
        for langName, langData in langJsonData['languages'].items():
            for id in langData["LANGID"]:
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
        getParam = indentText+self.paramDictList[0]['type']+" langId = GetUserDefaultUILanguage();"
        doCall = indentText+"return "+self.selectFunctionName+"(langId);"
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
        testBody.append("TEST("+testBlockName+", "+testName+")")
        testBody.append("{")
        testBody.append(bodyIndent+"// Generate the test language string object")

        testBody.append("") # whitespace for readability
        testBody.append(bodyIndent+self.returnType+" testVar = "+self.selectFunctionName+"("+str(langid)+");")
        testBody.append(bodyIndent+"EXPECT_STREQ(\""+expectedIso+"\", testVar."+getIsoMethod+"().c_str();")
        testBody.append("}")
        return testBody

    def _genUnitTest(self, langJsonData, getIsoMethod, outfile):
        """!
        @brief Generate all unit tests for the selection function

        @param langJsonData {dictionary} JSON file language dictionary data
        @param getIsoMethod {string} Name of the ParserStringListInterface return ISO code method
        @param outfile {file} File to output the function to
        """
        # Generate block start code
        blockStart = []
        blockStart.append("#if "+self.defDynamicOsString)
        blockStart.append("using namespace "+StringClassNameGen.getNamespaceName()+";")

        externDef = "extern "
        externDef += self.returnType
        externDef += " "
        externDef += self.selectFunctionName
        externDef += "("
        externDef += self.paramDictList[0]['type']
        externDef += " langid);"
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
