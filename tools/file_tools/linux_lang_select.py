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

class LinuxLangSelectFunctionGenerator(OsLangSelectFunctionHelper):
    """!
    Methods for Linux language select function generation
    """
    def __init__(self, functionName = "getParserStringListInterface_Linux"):
        """!
        @brief LinuxLangSelectFunctionGenerator constructor
        @param functionName {string} Function name to be used for generation
        """
        super().__init__()
        self.paramDictList = [{'name': "langId", 'type':"const char*", 'desc': "Current LANG value from the program environment"}]
        self.selectFunctionName = functionName
        self.defOsString = "(defined(__linux__) || defined(__unix__))"
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
        @return string list - Function comment block and declaration start
        """
        return self._genFunctionDefine(self.selectFunctionName,
                                       "Determine the correct "+StringClassNameGen.getBaseClassName()+"<lang> from the input LANG environment setting",
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
        functionBody.append("  #include <cstdlib>")
        functionBody.append("  #include <regex>")
        functionBody.append("")  # whitespace for readability

        # Generate function doxygen comment and start
        functionBody.extend(self.genFunctionDefine())

        # Start function body generation
        bodyIndent = "    "
        functionBody.append(bodyIndent+"// Check for valid input")
        functionBody.append(bodyIndent+"if (nullptr != "+self.paramDictList[0]['name']+")")
        functionBody.append(bodyIndent+"{")

        # Generate if/else if chain for each language in the dictionary
        if1BodyIndent = bodyIndent+"    "
        functionBody.append(if1BodyIndent+"// Break the string into its components")
        functionBody.append(if1BodyIndent+"std::cmatch langMatch;")
        functionBody.append(if1BodyIndent+"std::regex searchRegex(\"(^[a-z]{2})_([A-Z]{2})\\.(UTF[0-9]{1,2})\");")
        functionBody.append(if1BodyIndent+"std::regex_match("+self.paramDictList[0]['name']+", searchMatch, searchRegex);")
        functionBody.append("")  #white space for readability
        functionBody.append(if1BodyIndent+"// Determine the language")

        firstCheck = True
        if2BodyIndent = if1BodyIndent+"    "
        for langName, langData in langJsonData['languages'].items():
            ifline = if1BodyIndent
            if firstCheck:
                ifline += "if "
                firstCheck = False
            else:
                ifline += "else if "

            ifline += "(langMatch[1].str() == \""
            ifline += langData['LANG']
            ifline += "\")"

            functionBody.append(ifline)
            functionBody.append(if1BodyIndent+"{")
            functionBody.append(if2BodyIndent+self.genMakePtrReturnStatement(langName))
            functionBody.append(if1BodyIndent+"}")

        # Add the final else (unknown language) case
        functionBody.append(if1BodyIndent+"else //unknown language, use default language")
        functionBody.append(if1BodyIndent+"{")
        functionBody.append(if2BodyIndent+self.genMakePtrReturnStatement(langJsonData['default']['name']))
        functionBody.append(if1BodyIndent+"}")

        # Add the else if nullptr case
        functionBody.append(bodyIndent+"else // null pointer input, use default language")
        functionBody.append(bodyIndent+"{")
        functionBody.append(if1BodyIndent+self.genMakePtrReturnStatement(langJsonData['default']['name']))
        functionBody.append(bodyIndent+"} // end of if(nullptr != "+self.paramDictList[0]['name']+")")

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
        getParam = indentText+self.paramDictList[0]['type']+" langid = getenv(\"LANG\");"
        doCall = indentText+"return "+self.selectFunctionName+"(langid);"
        return [getParam, doCall]

    def _genUnitTestTest(self, testName, linuxEnvString, expectedIso, getIsoMethod):
        """!
        @brief Generate single selection function unit test instance

        @param testName {string} Name of the test
        @param linuxEnvString {string} Environment string value to send to the select function
        @param expectedIso {string} Expected ISO return code for the test variable
        @param getIsoMethod {string} Name of the ParserStringListInterface return ISO code method

        @return list of strings - Output C code
        """
        testBlockName = "LinuxSelectFunction"
        bodyIndent = "".rjust(4, " ")
        breifDesc = "Test "+self.selectFunctionName+" "+linuxEnvString+" selection case"

        testBody = self.genDoxyMethodComment(breifDesc, [])
        testBody.append("TEST("+testBlockName+", "+testName+")")
        testBody.append("{")
        testBody.append(bodyIndent+"std::string testLangCode;")

        testBody.append("") # whitespace for readability
        testBody.append(bodyIndent+"// Generate the test language string object")
        testBody.append(bodyIndent+"testLangCode = \""+linuxEnvString+"\";")

        testBody.append("") # whitespace for readability
        testBody.append(bodyIndent+self.returnType+" testVar = "+self.selectFunctionName+"(testLangCode.c_str());")
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
            for region in langData['LANG_regions']:
                # Generate test for each region of known language
                linuxEnvString = langData['LANG']+"_"+region+".UTF-8"
                testName = langName.capitalize()+"_"+region+"_Selection)"
                testBody = self._genUnitTestTest(testName, linuxEnvString, langData['isoCode'], getIsoMethod)
                testBody.append("") # whitespace for readability
                outfile.writelines(testBody)

            # Generate test for unknown region of known language
            unknownRegionTestName =langName.capitalize()+"_unknownRegion_Selection)"
            unknownRegionEnv = langData['LANG']+"_XX.UTF-8"
            unknownRegionBody = self._genUnitTestTest(unknownRegionTestName, unknownRegionEnv, langData['isoCode'], getIsoMethod)
            unknownRegionBody.append("") # whitespace for readability
            outfile.writelines(unknownRegionBody)

        # Generate test for unknown region of unknown language and expect default
        unknownLangBody = self._genUnitTestTest("UnknownLanguageDefaultSelection",
                                                "xx_XX.UTF-8",
                                                langJsonData['default']['isoCode'],
                                                getIsoMethod)
        outfile.writelines(unknownLangBody)

        # Generate block end code
        outfile.writelines(["#endif // "+self.defDynamicOsString])
