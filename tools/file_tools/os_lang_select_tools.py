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

from .common.param_return_tools import ParamRetDict
from .common.file_gen_tools import GenCFunctionHelper
from .string_name_generator import StringClassNameGen

class OsLangSelectFunctionHelper(GenCFunctionHelper):
    """!
    @brief Helper methods for OS lang file function generation

    This class extends GenCFunctionHelper and implents some common data and functionality
    for specific OS language function implementations
    """
    def __init__(self):
        """!
        @brief OsLangSelectFunctionHelper constructor
        """
        self.stdPtrType = "std::shared_ptr"
        self.returnType = self.stdPtrType+"<"+StringClassNameGen.getBaseClassName()+">"
        self.retPtrDict = ParamRetDict.buildReturnDict(self.returnType,
                                                        "Pointer to "+StringClassNameGen.getBaseClassName()+"<lang> based on OS local language")

    def _genFunctionDeclare(self, name, briefDesc, paramDictList):
        """!
        @brief Get the function declaration string for the given name

        @param name {string} Function name
        @param briefDesc {string} Brief description for DOXYGEN commant
        @param paramDictList {list} Parameter description dicionary list

        @return string list - Function comment block and declaration start
        """
        return self.declareFunctionWithDecorations(name, briefDesc, paramDictList, self.retPtrDict)

    def _genFunctionDefine(self, name, briefDesc, paramDictList):
        """!
        @brief Get the function definition string for the given name

        @param name {string} Function name
        @param briefDesc {string} Brief description for DOXYGEN commant
        @param paramDictList {list} Parameter description dicionary list

        @return string list - Function comment block and declaration start
        """
        return self.defineFunctionWithDecorations(name, briefDesc, paramDictList, self.retPtrDict)

    def genMakePtrReturnStatement(self, classMod):
        """!
        @brief Generate a language select return statement
        @param classMod {string} Language name of the final parser string object
        """
        return super().genMakePtrReturnStatement(StringClassNameGen.getLangClassName(classMod))

    def genPtrParameterStatement(self, paramName):
        """!
        @brief Generate a language select variable declaration
        @param paramName {string} Variable name
        """
        paramLine = "std::shared<"
        paramLine += StringClassNameGen.getBaseClassName()
        paramLine += "> "
        paramLine += paramName
        paramLine += ";"
        return paramLine

    def getStringType(self):
        return StringClassNameGen.getParserStringType()

    def getStringListType(self):
        return "std::list<"+StringClassNameGen.getParserStringType()+">"
