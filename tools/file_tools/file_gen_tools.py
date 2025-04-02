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

from datetime import datetime

from .comment_block import CommentParams
from .comment_block import CommentGenerator
from .doxygen_gen_tools import CDoxyCommentGenerator
from .copyright_tools import CopyrightGenerator
from .eula import EulaText

#============================================================================
#============================================================================
# C function generation helper class
#============================================================================
#============================================================================
class GenCFunctionHelper(CDoxyCommentGenerator):
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
        super().__init__()

    def declareFunctionWithDecorations(self, name, briefdesc, paramDictList, retDict, indent = 0, noDoxygen = False,
                                       prefixDecaration = None, postfixDecaration = None, inlinecode = None,
                                       longDesc = None):
        """!
        @brief Generate a function declatation text block with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {{'type':<type string>, 'desc':<description string} or None} - Return parameter data or None
        @param indent {integer} Comment and function declaration indentation
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param prefixDecaration {string} Valid C/C++ declaration prefix decoration, i.e "virtual"
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const" | "override" ...
        @param inlinecode {sting list or None} Inline code for the declaration or None id there is no inline definition
        @param longDesc {string or None} Long description of the function

        @return string list - Function doxygen comment block and declaration
        """
        funcDeclareText = []

        # Add doxygen comment block
        if not noDoxygen:
            funcDeclareText.extend(self.genDoxyMethodComment(briefdesc, paramDictList, retDict, longDesc, indent))

        # Create function definition line
        funcLine = ""

        # Add function prefix definitions if defined
        if prefixDecaration is not None:
            funcLine += prefixDecaration
            funcLine += " "

        # Construct main function declaration
        if retDict is not None:
            funcLine += retDict['type']+" "+name+"("
        else:
            funcLine += name+"("

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


    def defineFunctionWithDecorations(self, name, briefdesc, paramDictList, retDict, noDoxygen = False,
                                      prefixDecaration = None, postfixDecaration = None,
                                      longDesc = None):
        """!
        @brief Generate a function definition start with doxygen comment

        @param name {string} Function name
        @param desc {string} Function description
        @param paramDictList {list of {'name':string, 'type':<type string>, 'desc':<description string}} - Return parameter data
        @param retDict {'type':<type string>, 'desc':<description string} - Return parameter data
        @param noDoxygen {boolean} True skip doxygen comment generation, False generate doxygen comment block
        @param prefixDecaration {string} Valid C/C++ declaration prefix decoration, i.e "virtual"
        @param postfixDecaration {string} Valid C/C++ declaration postfix decoration, i.e "const" | "override" ...
        @param longDesc {string or None} Long description of the function

        @return string list - Function doxygen comment block and declaration start
        """
        funcDefineText = []

        # Add doxygen comment block
        if not noDoxygen:
            funcDefineText.extend(self.genDoxyMethodComment(briefdesc, paramDictList, retDict, longDesc))

        # Add function prefix definitions if defined
        if prefixDecaration is not None:
            funcLine += prefixDecaration
            funcLine += " "

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

        # Add function post fix decorations if defined
        if postfixDecaration is not None:
            funcLine += " "
            funcLine += postfixDecaration
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

    @staticmethod
    def genMakePtrReturnStatement(classMod):

        retLine = "return "
        retLine += "std::make_shared<"
        retLine += StringClassNameGen.getLangClassName(classMod)
        retLine += ">();"
        return retLine

#============================================================================
#============================================================================
# File generation helper class
#============================================================================
#============================================================================
class GenerateCppFileHelper(GenCFunctionHelper):
    """!
    @brief File generation helper class.

    This class implements boiler plate data and helper functions used by
    the parent file specific generation class to generate the file
    """
    def __init__(self, fileName, eulaName = "MIT_open"):
        """!
        @brief GenerateFileHelper constructor

        @param fileName {string} Name to use for the .h and .cpp generated files
        @param eulaName {string} Name of the EULA from EulaText class to use.
        """
        self.fileName = fileName
        self.copyrightGenerator = CopyrightGenerator()
        self.eula = EulaText(eulaName)
        self.commentGenerator = CommentGenerator(CommentParams.cCommentParms)

    def _generateFileHeader(self, autotoolname, startYear=2025, owner = None):
        """!
        @brief Generate the boiler plate file header with copyright and eula

        @param autotoolname {string} Auto generation tool name for comments
        @param startYear {number} First copyright year
        @param owner {string} File owner for copyright message or None
        @return list of strings - Code to output
        """
        commentText = []

        if owner is not None:
            # Generate copyright and EULA text
            currentYear = datetime.now().year
            commentText.append(self.copyrightGenerator.createNewCopyright(owner, startYear, currentYear))
            commentText.append("") # white space for readability
            commentText.append(self.eula.formatEulaName())
            commentText.append("") # white space for readability
            commentText.extend(self.eula.formatEulaText())
            commentText.append("") # white space for readability

        commentText.append("") # white space for readability
        commentText.append("This file was autogenerated by "+autotoolname+" do not edit")
        commentText.append("") # white space for readability

        # Special comment generator for header block
        headerGenCommentParam = CommentParams.cCommentParms
        headerGenCommentParam['blockLineStart'] = "* "
        headerCommentGen = CommentGenerator(self.autoGenCommentParam, 80)

        # Generate comment header
        commentText.extend(headerCommentGen.buildCommentBlockHeader())

        # Wrap and output commentText lines
        for line in commentText:
            commentText.append(headerCommentGen.wrapCommentLine(line))

        # Generate comment footer
        commentText.extend(headerCommentGen.buildCommentBlockFooter())
        return commentText

    def _genInclude(self, includeName):
        """!
        @brief Add Include line to the output file
        @param includeName {string} Name of the include file to add
        @return list of strings - Code to output
        """
        if -1 == includeName.find("<"):
            return ["#include \""+includeName+"\""]
        else:
            return ["#include "+includeName]

    def _genDoxyDefgroup(self, group, groupdef, ext = None):
        """!
        @brief Doxygen defgroup comment block
        @param group {string} Name of the group to define
        @param groupdef {string} Description of the new group
        @param ext {string} File extention or None if filename is complete
        @return list of strings - Code to output
        """
        doxyHeader = []
        doxyHeader.append(self.autoGenCommentParam['doxyBlockStart'])

        if ext is not None:
            doxyHeader.append(self.autoGenCommentParam['doxyLineStart']+"@file "+self.fileName+"."+ext)
        else:
            doxyHeader.append(self.autoGenCommentParam['doxyLineStart']+"@file "+self.fileName)

        doxyHeader.append(self.autoGenCommentParam['doxyLineStart']+"@defgroup "+group+" "+groupdef)
        doxyHeader.append(self.autoGenCommentParam['doxyLineStart']+"@ingroup "+group)
        doxyHeader.append(self.autoGenCommentParam['doxyLineStart']+"@{")
        doxyHeader.append(self.autoGenCommentParam['blockEnd'])
        return doxyHeader

    def _genDoxyGroupEnd(self):
        """!
        @brief Doxygen group comment block end marker
        @return list of strings - Code to output
        """
        doxyEnd = self.autoGenCommentParam['doxyBlockStart']+"@}"+self.autoGenCommentParam['blockEnd']
        return [doxyEnd]

    def genClassStart(self, className, classDesc, inheritence = None, classDecoration = None, noDoxyCommentConstructor = False):
        """!
        @brief Generate default constructor(s)/destructor declarations for a class

        @param className {string} Name of the class
        @param inheritence {sting} Parent class and visability or None
        @param classDecoration {sting} Class decoration or None
        @param noDoxyCommentConstructor {boolean} Doxygen comment disable. False = generate doxygen comments,
                                                  True = ommit comments
        @return list of strings - Code to output
        """
        codeText = []

        # Generate Doxygen class description
        if not noDoxyCommentConstructor:
            codeText.extend(self.genDoxyClassComment(classDesc))

        # Generate class start
        if inheritence is not None:
            if classDecoration is not None:
                codeText.append("class "+className+" "+classDecoration+" : "+inheritence)
            else:
                codeText.append("class "+className+" : "+inheritence)
        else:
            codeText.append("class "+className)
        codeText.append("{")

        return codeText

    def genClassDefaultConstructorDestructor(self, className, indent = 8, virtualDestructor = False, noDoxyCommentConstructor = False):
        """!
        @brief Generate default constructor(s)/destructor declarations for a class

        @param className {string} Name of the class
        @param indent {number} Indentation space count for the declarations (default = 8)
        @param virtualDestructor {boolean} False if destructor is not virtual (default)
                                           True if virtual decoration on destructor
        @param noDoxyCommentConstructor {boolean} Doxygen comment disable. False = generate doxygen comments,
                                                  True = ommit comments
        @return list of strings - Code to output
        """
        # Setup params for the different constructors
        otherReference = [{'name': "other", 'type': "const "+className+"&", 'desc': "Reference to object to copy"}]
        otherMove = [{'name': "other", 'type': className+"&&", 'desc': "Reference to object to move"}]
        equateReturn = {'type':className+"&", 'desc':"*this"}
        destructorPrefix = None
        if virtualDestructor:
            destructorPrefix = "virtual"

        # Declare default default constructor
        codeText = self.declareFunctionWithDecorations(className,
                                                       "Construct a new "+className+" object",
                                                       [],
                                                       None,
                                                       indent,
                                                       noDoxyCommentConstructor,
                                                       None,
                                                       "= default")
        if not noDoxyCommentConstructor:
            codeText.append("")      #whitespace for readability

        # Declare default copy constructor
        codeText.extend(self.declareFunctionWithDecorations(className,
                                                            "Copy constructor for a new "+className+" object",
                                                            otherReference,
                                                            None,
                                                            indent,
                                                            noDoxyCommentConstructor,
                                                            None,
                                                            "= default"))

        if not noDoxyCommentConstructor:
            codeText.append("")      #whitespace for readability

        # Declare default move constructor
        codeText.extend(self.declareFunctionWithDecorations(className,
                                                            "Move constructor for a new "+className+" object",
                                                            otherMove,
                                                            None,
                                                            indent,
                                                            noDoxyCommentConstructor,
                                                            None,
                                                            "= default"))

        if not noDoxyCommentConstructor:
            codeText.append("")      #whitespace for readability

        # Declare default equate constructor
        codeText.extend(self.declareFunctionWithDecorations("operator=",
                                                            "Equate constructor for a new "+className+" object",
                                                            otherReference,
                                                            equateReturn,
                                                            indent,
                                                            noDoxyCommentConstructor,
                                                            None,
                                                            "= default"))

        if not noDoxyCommentConstructor:
            codeText.append("")      #whitespace for readability

        # Declare default equate move constructor
        codeText.extend(self.declareFunctionWithDecorations("operator=",
                                                            "Equate move constructor for a new "+className+" object",
                                                            otherMove,
                                                            equateReturn,
                                                            indent,
                                                            noDoxyCommentConstructor,
                                                            None,
                                                            "= default"))

        if not noDoxyCommentConstructor:
            codeText.append("")      #whitespace for readability

        # Declare default destructor
        codeText.extend(self.declareFunctionWithDecorations("~"+className,
                                                            "Destructor for "+className+" object",
                                                            [],
                                                            None,
                                                            indent,
                                                            noDoxyCommentConstructor,
                                                            destructorPrefix,
                                                            "= default"))
        codeText.append("")      #whitespace for readability
        return codeText


#============================================================================
#============================================================================
# Misc helper classes
#============================================================================
#============================================================================
class StringClassNameGen(object):
    """!
    @brief Helper static class for generating consistent ParserStringInterface names across multiple files
    """
    def __init__(self):
        pass

    @staticmethod
    def getNamespaceName():
        """!
        @brief Return the base class name
        @return string Base sting class name
        """
        return "argparser"

    @staticmethod
    def getBaseClassName():
        """!
        @brief Return the base class name
        @return string Base sting class name
        """
        return "ParserStringListInterface"

    @staticmethod
    def getBaseClassNameWithNamespace():
        """!
        @brief Return the base class name
        @return string Base sting class name
        """
        return StringClassNameGen.getNamespaceName()+"::"+StringClassNameGen.getBaseClassName()

    @staticmethod
    def getLangClassName(lang):
        """!
        @brief Build the language specific file name based on the input lang value
        @param lang {string} Language name
        @return string Language specific class name
        """
        return StringClassNameGen.getBaseClassName()+lang.capitalize()

    @staticmethod
    def getParserStringType():
        return "parserstr"

    @staticmethod
    def getParserCharType():
        return "parserchar"

    @staticmethod
    def getParserStrStreamType():
        return "parser_str_stream"

    @staticmethod
    def getDynamicCompileswitch():
        """!
        @brief Return the base class name
        @return string Base sting class name
        """
        return "DYNAMIC_INTERNATIONALIZATION"
