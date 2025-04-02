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

class MasterSelectFunctionGenerator(OsLangSelectFunctionHelper):
    """!
    Methods for master language select function generation
    """
    def __init__(self, functionName = "getLocalParserStringListInterface", namespaceName = "SystemLanguageDetection"):
        """!
        @brief MasterSelectFunctionGenerator constructor
        @param functionName {string} Function name to be used for generation
        @param namespaceName {string} Namespace decoration
        """
        super().__init__()
        if namespaceName is not None:
            self.selectFunctionName = namespaceName+"::"+functionName
        else:
            self.selectFunctionName = functionName

        self.briefDesc = "Determine the OS use OS specific functions to determine the correct local language" \
                         "based on the OS specific local language setting and return the correct "
        self.briefDesc += StringClassNameGen.getBaseClassName()
        self.briefDesc += "<lang> object"

    def getFunctionName(self):
        return self.selectFunctionName

    def declareFunction(self):
        """!
        @brief Generate a function declatation text block with doxygen comment
        @return string list - Function doxygen comment block and declaration
        """

        return self._genFunctionDeclare(self.selectFunctionName, self.briefDesc, [])

    def genFunctionDefine(self):
        """!
        @brief Get the function declaration string for the given name
        @return string list - Function comment block and declaration start
        """
        return self._genFunctionDefine(self.selectFunctionName, self.briefDesc, [])

    def genFunctionEnd(self):
        """!
        @brief Get the function declaration string for the given name
        @return string - Function close with comment
        """
        return self._genFunctionEnd(self.selectFunctionName)

    def genFunction(self, outfile, osLangSelectors, staticSelector):
        """!
        @brief Generate the function body text
        @param outfile {file} File to output the function to
        @param osLangSelectors {list} List of OS language selector function generation objects
        @param staticSelector {StaticLangSelectFunctionGenerator} Static generation object
        """
        # Generate function doxygen comment and start
        functionBody = []
        functionBody.extend(self.genFunctionDefine())
        bodyIndent = 4

        # Generate OS calls
        firstOs = True
        for osSelector in osLangSelectors:
            if firstOs:
                functionBody.append("#if "+osSelector.getOsDynamicDefine())
            else:
                functionBody.append("#elif "+osSelector.getOsDynamicDefine())
            functionBody.extend(osSelector.genReturnFunctionCall(bodyIndent))

        # Add the dynamic but unknown OS #elif case
        functionBody.append("#elif defined("+StringClassNameGen.getDynamicCompileswitch()+")")
        functionBody.append("#error No dynamic language generation defined for this OS".rjust(bodyIndent, ' '))

        # Add the #else case
        functionBody.append("#else // not defined("+StringClassNameGen.getDynamicCompileswitch()+")")
        functionBody.extend(staticSelector.genReturnFunctionCall(bodyIndent))

        # Complete the function
        functionBody.append("#endif // defined os and defined("+StringClassNameGen.getDynamicCompileswitch()+")")
        functionBody.append(self.genFunctionEnd())
        outfile.writelines(functionBody)

    def genReturnFunctionCall(self, indent = 4):
        """!
        @brief Generate the call code for the linux dynamic lang selection function
        @param indent {number} Code indentation spaces
        @return list of strings Formatted code lines
        """
        doCall = "return "+self.selectFunctionName+"();"
        return [doCall.rjust(indent, " ")]

    def genUnittest(self, outfile):
        """!
        @brief Generate the function body text
        @param outfile {file} File to output the function to
        """
        pass
