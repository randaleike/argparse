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
from file_tools.comment_block import CommentParams
from file_tools.comment_block import CommentGenerator
from file_tools.eula import EulaText
from file_tools.text_format import MultiLineFormat

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
        self.OsDefList = [self.linuxDefString, self.windowsDefString]

    def getIfEndifDynamic(self, defOsString = None):
        """!
        @brief Get the dynamic os #if compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString or None
        @return string [2] - formatted #if/#endif compile switch statement
        """
        if defOsString is None:
            ifStr = "#if "+self.dynamicDefString
            endifStr = "#endif //"+self.dynamicDefString
        else:
            ifStr = "#if ("+defOsString+" && "+self.dynamicDefString+")"
            endifStr = "#endif //("+defOsString+" && "+self.dynamicDefString+")"
        return [ifStr, endifStr]

    def getIfElifEndOSDynamic(self, defOSStringList):
        """!
        @brief Get compile switch #endif string
        @param defOSStringList (string list) - List of OS definition strings
        @return string[1+len(defOSStringList)] - formatted #if/#elif/#endif compile
                                                 switch statement with end comment
        """
        retList = []
        startStr="#if "
        for defOsString in defOSStringList:
            newStr = startStr+"("+defOsString+" && "+self.dynamicDefString+")"
            retList.append(newStr)
            startStr = "#elif "

        retList.append("#else //unknown OS or not "+self.dynamicDefString)
        retList.append("  #error \"Unknown OS type, no dynamic language id function defined\"")
        retList.append("#endif //(defined os and "+self.dynamicDefString+")")
        return retList

    def getElifOSDynamic(self, defOsString):
        """!
        @brief Get the dynamic os #elif compile switch string
        @param defOsString (string) - linuxDefString or windowsDefString
        @return string - formatted #elif compile switch statement
        """
        return "#elif ("+defOsString+" && "+self.dynamicDefString+")"

    def getIfElseEndifDynamic(self):
        """!
        @brief Get compile switch #else string
        @return string[3] - formatted #if/#else/#endif compile switch statements
                            with comments
        """
        ifStr = "#if "+self.dynamicDefString
        elseStr = "#else // not "+self.dynamicDefString
        endifStr = "#endif //"+self.dynamicDefString
        return [ifStr, elseStr, endifStr]

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

    def getIfEndifStatic(self):
        """!
        @brief Get the dynamic os #if compile switch string
        @return string[2] - formatted #if/#endif compile static build switch statement
        """
        ifStr = "#if !"+self.dynamicDefString
        endifStr = "#endif // not "+self.dynamicDefString
        return [ifStr, endifStr]

class CCommentGenerator(CommentGenerator):
    def __init__(self):
        super().__init__(CommentParams.cCommentParms)

class DoxyCommentGenerator(CommentGenerator):
    def __init__(self, commentMarkers, addParamType=False):
        """!
        @brief DoxyCommentGenerator constructor
        @param commentMarkers {CommentBlockDelim dictionary} Comment deliminter markers for the input file type.
        @param addParamType {boolean} True add the param['type'] to the doxygen param comment text
                                      False do not add param['type'] to the doxygen param comment text
        """
        super().__init__(commentMarkers)
        self.formatMaxLength = 120
        self.addParamType = addParamType
        self.descFormatMax = self.formatMaxLength-len(self.commentData['doxyLineStart'])

    def _genCommentBlockPrefix(self, blockIndent=0):
        """!
        @brief Generate doxygen block prefix string

        @param blockIndent Current cmment block indentation

        @return string - Formatted block prefix
        """
        prefix = ""
        prefix.rjust(blockIndent,' ')
        prefix += self.commentData['doxyLineStart']
        return prefix

    def _genCommentReturnText(self, retDict, prefix):
        """!
        @brief Generate @return doxygen text

        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param prefix {string} Current comment block prefix string

        @return list of strings - Formatted string list for the comment block
        """
        # Construct first return line
        l1 = "@return "+retDict['type']+" - "

        # Format the description into sized string(s)
        descList = MultiLineFormat(retDict['desc'], self.descFormatMax-len(l1))

        # Construct the final block return text
        retList = []
        firstdesc = True
        for descStr in descList:
            if firstdesc:
                retList.append(prefix+l1+descStr)
                firstdesc = False
            else:
                retList.append(prefix+descStr.rjust(len(l1), ' '))

        # return the final formated data string list
        return retList

    def _genCommentParamText(self, paramDict, prefix):
        """!
        @brief Generate @param doxygen text

        @param paramDict {'name':string, 'type':<type string>, 'desc':<description string} - Return parameter data
        @param prefix {string} Current comment block prefix string

        @return list of strings - Formatted string list for the comment block
        """
        # Construct first param line
        l1 = "@param "+paramDict['name']
        if self.addParamType:
            l1 += " {"+paramDict['type']+"}"
        l1 += " "

        # Format the description into sized string(s)
        descList = MultiLineFormat(paramDict['desc'], self.descFormatMax-len(l1))

        # Add the description string(s)
        firstdesc = True
        retList = []
        for descStr in descList:
            if firstdesc:
                retList.append(prefix+l1+descStr)
                firstdesc = False
            else:
                retList.append(prefix+descStr.rjust(len(l1), ' '))

        # return the final formated data string list
        return retList

    def genDoxyComment(self, briefDesc, paramDictList, retDict, longDesc=None, blockIndent=0):
        """!
        @brief Generate the doxygen comment block

        @param briefDesc {string} @brief description for the comment block
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param longDesc {string} Detailed description for the comment block or None if no detailed description
        @param blockIndent Current cmment block indentation

        @return list of strings - Comment block as a list of formatted strings
        """
        # Set the start
        blockStart = self.commentData['doxyBlockStart']
        blockStart.rjust(blockIndent, ' ')
        blockStrList = [blockStart]

        # Generate the block prefix text fot the rest
        prefix = self._genCommentBlockPrefix(blockIndent)

        # Add the brief text
        briefStart = "@brief "
        formattedBriefTxt = MultiLineFormat(briefDesc, self.descFormatMax-len(briefStart))
        firstdesc = True
        for briefLine in formattedBriefTxt:
            if firstdesc:
                blockStrList.append(prefix+briefStart+briefLine)
                firstdesc = False
            else:
                blockStrList.append(prefix+briefLine.rjust(len(briefStart), ' '))

        blockStrList.append(prefix) # add empty line for readability

        # Add the long description
        if longDesc is not None:
            formattedLongTxt = MultiLineFormat(longDesc, self.descFormatMax)
            for longDescLine in formattedLongTxt:
                blockStrList.append(prefix+longDescLine)

            blockStrList.append(prefix) # add empty line for readability

        # Add Param data
        for paramDict in paramDictList:
            blockStrList.extend(self._genCommentParamText(paramDict, prefix))

        blockStrList.append(prefix) # add empty line for readability

        # Add return data
        blockStrList.extend(self._genCommentReturnText(retDict, prefix))

        # Complete the block
        blockEnd = self.commentData['blockEnd']
        blockEnd.rjust(blockIndent, ' ')
        blockStrList.append(blockEnd)

        return blockStrList


class GenCFunctionHelper(DoxyCommentGenerator):
    """!
    Helper functions for function generation
    """
    def __init__(self):
        """!
        @brief GenFunctionHelper constructor
        @param commentMarkers {CommentBlockDelim dictionary} Comment deliminter markers for the input file type.
        @param addParamType {boolean} True add the param['type'] to the doxygen param comment text
                                      False do not add param['type'] to the doxygen param comment text
        """
        super().__init__(CommentParams.cCommentParms)

    def _declareFunctionWithDecorations(self, name, desc, paramDictList, retDict, indent = 0, noDoxygen = False,
                                        prefixDecaration = None, postfixDecaration = None, inlinecode = None):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param indent {integer} Comment and function declaration indentation
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param prefixDecaration {string} Valid C/C++ declaration prefix decoration, i.e "virtual"
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const" | "override" ...
        @param inlinecode {sting list or None} Inline code for the declaration or None id there is no inline definition

        @return string list - Function doxygen comment block and declaration
        """
        funcDeclareText = []

        # Add doxygen comment block
        if not noDoxygen:
            funcDeclareText.extend(self.genDoxyComment(desc, paramDictList, retDict, indent))

        # Create function definition line
        funcLine = ""

        # Add function prefix definitions if defined
        if prefixDecaration is not None:
            funcLine += prefixDecaration
            funcLine += " "

        # Construct main function declaration
        funcLine += retDict['type']+" "+name+"("
        paramPrefix = ""
        for paramDict in paramDictList:
             funcLine += paramPrefix
             funcLine += paramDict['type']
             funcLine += " "
             funcLine += paramDict['name']
             paramPrefix = ", "
        funcLine += ")"

        # Add function post fix decorations if defined
        if postfixDecaration is not None:
            funcLine += " "
            funcLine += postfixDecaration

        # Add inline code if defined
        if inlinecode is None:
            funcLine += ";"
            funcDeclareText.append(funcLine)
        else:
            funcDeclareText.append(funcLine)
            inlineStart = "{".rjust(indent, ' ')
            if len(inlinecode) == 1:
                funcDeclareText.append(inlineStart+inlinecode+"}")
            else:
                funcDeclareText.append(inlineStart)
                for codeLine in inlinecode:
                    funcDeclareText.append(codeLine.rjust(indent+4, ' '))
                funcDeclareText.append("}".rjust(indent, ' '))

        return funcDeclareText

    def declareCFunction(self, name, desc, paramDictList, retDict, noDoxygen = False):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block

        @return string list - Function doxygen comment block and declaration
        """
        return self._declareFunctionWithDecorations(name, desc, paramDictList, retDict, 0, noDoxygen)

    def declareCppPureVirtualFunction(self, name, desc, paramDictList, retDict, indent = 8, noDoxygen = False,
                                      postfixDecaration = None):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param indent {integer} Comment and function declaration indentation
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const" | "override" ...

        @return string list - Function doxygen comment block and declaration
        """
        if postfixDecaration is not None:
            virtualPostFix = postfixDecaration+" = 0"
        else:
            virtualPostFix = " = 0"
        return self._declareFunctionWithDecorations(name, desc, paramDictList, retDict, indent,
                                                    noDoxygen, "[[nodiscard]] virtual", virtualPostFix,
                                                    False)

    def declareCppFinalFunction(self, name, desc, paramDictList, retDict, indent = 8, noDoxygen = False,
                                postfixDecaration = None):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param indent {integer} Comment and function declaration indentation
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const"

        @return string list - Function doxygen comment block and declaration
        """
        if postfixDecaration is not None:
            finalPostFix = postfixDecaration+" final"
        else:
            finalPostFix = "final"
        return self._declareFunctionWithDecorations(name, desc, paramDictList, retDict, indent,
                                                    noDoxygen, None, finalPostFix, False)

    def declareCppFinalInlineFunction(self, name, desc, paramDictList, retDict, inlinecode,
                                      indent = 8, noDoxygen = False,
                                      postfixDecaration = None):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @parm  inlinecode {string list} Inline code text
        @param indent {integer} Comment and function declaration indentation
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const"

        @return string list - Function doxygen comment block and declaration
        """
        if postfixDecaration is not None:
            finalPostFix = postfixDecaration+" final"
        else:
            finalPostFix = "final"

        return self._declareFunctionWithDecorations(name, desc, paramDictList, retDict, indent,
                                                    noDoxygen, None, finalPostFix, inlinecode)

    def defineFunction(self, name, desc, paramDictList, retDict, noDoxygen = False):
        """!
        @brief Generate a function definition start with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block

        @return string list - Function doxygen comment block and declaration start
        """
        funcDefineText = []

        # Add doxygen comment block
        if not noDoxygen:
            funcDefineText.extend(self.genDoxyComment(desc, paramDictList, retDict))

        # Create function definition line
        funcLine = retDict['type']+" "+name+"("
        paramPrefix = ""
        for paramDict in paramDictList:
             funcLine += paramPrefix
             funcLine += paramDict['type']
             funcLine += " "
             funcLine += paramDict['name']
             paramPrefix = ", "
        funcLine += ")"
        funcDefineText.append(funcLine)

        # Add function open text
        funcDefineText.append("{")
        return funcDefineText

    def endFunction(self, name):
        """!
        @brief Get the function declaration string for the given name
        @param name (string) - Function name
        @return string - Function close with comment
        """
        return ("} // end of "+name+"()")

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
