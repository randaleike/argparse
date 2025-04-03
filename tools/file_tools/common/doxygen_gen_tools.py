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

from .param_return_tools import ParamRetDict
from .comment_block import CommentParams
from .comment_block import CommentGenerator
from .text_format import MultiLineFormat

#============================================================================
#============================================================================
# Doxygen comment block helper classes
#============================================================================
#============================================================================
class DoxyCommentGenerator(CommentGenerator):
    def __init__(self, commentMarkers, addParamType=False):
        """!
        @brief DoxyCommentGenerator constructor
        @param commentMarkers {CommentBlockDelim dictionary} Comment deliminter markers for the input file type.
        @param addParamType {boolean} True add the parameter type to the doxygen param comment text
                                      False do not add parameter type to the doxygen param comment text
        """
        super().__init__(commentMarkers)
        self.formatMaxLength = 120
        self.addParamType = addParamType
        self.descFormatMax = self.formatMaxLength-len(self.commentData['doxyLineStart'])

    def _genCommentBlockPrefix(self, blockIndent=0):
        """!
        @brief Generate doxygen block prefix string

        @param blockIndent Current comment block indentation

        @return string - Formatted block prefix
        """
        prefix = ""
        prefix.rjust(blockIndent,' ')
        prefix += self.commentData['doxyLineStart']
        return prefix

    def _genBlockStart(self, blockIndent = 0):
        """!
        @brief Generate doxygen block start string

        @param blockIndent Current comment block indentation

        @return string - Formatted block prefix
        """
        # Set the start
        blockStart = self.commentData['doxyBlockStart']
        blockStart.rjust(blockIndent, ' ')
        return blockStart

    def _genBlockEnd(self, blockIndent = 0):
        """!
        @brief Generate doxygen block start string

        @param blockIndent Current comment block indentation

        @return string - Formatted block prefix
        """
        # Set the start
        blockEnd = self.commentData['blockEnd']
        blockEnd.rjust(blockIndent, ' ')
        return blockEnd

    def _genBriefDesc(self, briefDesc, prefix):
        """!
        @brief Generate the doxygen comment block

        @param briefDesc {string} @brief description for the comment block
        @param prefix {string} Current comment block indentation prefix
        @return list of strings - Long description comment as a list of formatted strings
        """
        briefDescList = []

        # Generate the brief description text
        briefStart = "@brief "
        formattedBriefTxt = MultiLineFormat(briefDesc, self.descFormatMax-len(briefStart))
        firstdesc = True
        for briefLine in formattedBriefTxt:
            if firstdesc:
                briefDescList.append(prefix+briefStart+briefLine)
                firstdesc = False
            else:
                briefDescList.append(prefix+briefLine.rjust(len(briefStart), ' '))

        return briefDescList

    def _genLongDesc(self, prefix, longDesc=None):
        """!
        @brief Generate the doxygen comment block

        @param prefix {string} Current comment block prefix string
        @param longDesc {string} Detailed description for the comment block or None if no detailed description

        @return list of strings - Long description comment as a list of formatted strings
        """
        longDescList = []

        # Generate the long description text
        if longDesc is not None:
            formattedLongTxt = MultiLineFormat(longDesc, self.descFormatMax)
            for longDescLine in formattedLongTxt:
                longDescList.append(prefix+longDescLine)
        return longDescList

    def _genCommentReturnText(self, retDict, prefix):
        """!
        @brief Generate @return doxygen text

        @param retDict {dictionary} - Return parameter data
        @param prefix {string} Current comment block prefix string

        @return list of strings - Formatted string list for the comment block
        """
        # Construct first return line
        returnType, returnDesc = ParamRetDict.getReturnData(retDict)
        l1 = "@return "+returnType+" - "

        # Format the description into sized string(s)
        descList = MultiLineFormat(returnDesc, self.descFormatMax-len(l1))

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

        @param paramDict {dictionary} - Return parameter data
        @param prefix {string} Current comment block prefix string

        @return list of strings - Formatted string list for the comment block
        """
        # Construct first param line
        paramName, paramType, paramDesc = ParamRetDict.getParamData(paramDict)
        l1 = "@param "+paramName
        if self.addParamType:
            l1 += " {"+paramType+"}"
        l1 += " "

        # Format the description into sized string(s)
        descList = MultiLineFormat(paramDesc, self.descFormatMax-len(l1))

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

    def genDoxyMethodComment(self, briefDesc, paramDictList, retDict=None, longDesc=None, blockIndent=0):
        """!
        @brief Generate the doxygen comment block

        @param briefDesc {string} @brief description for the comment block
        @param paramDictList {list of dictionaries} - Return parameter data
        @param retDict {dictionary} - Return parameter data
        @param longDesc {string} Detailed description for the comment block or None if no detailed description
        @param blockIndent Current comment block indentation

        @return list of strings - Comment block as a list of formatted strings
        """
        # Generate the block start
        blockStrList = [self._genBlockStart(blockIndent)]

        # Generate the block prefix text fot the rest
        prefix = self._genCommentBlockPrefix(blockIndent)

        # Add the brief text
        blockStrList.extend(self._genBriefDesc(briefDesc, prefix))
        blockStrList.append(prefix) # add empty line for readability

        # Add the long description
        if longDesc is not None:
            blockStrList.extend(self._genLongDesc(prefix, longDesc))
            blockStrList.append(prefix) # add empty line for readability

        # Add Param data
        if (len(paramDictList) > 0):
            for paramDict in paramDictList:
                blockStrList.extend(self._genCommentParamText(paramDict, prefix))
            blockStrList.append(prefix) # add empty line for readability

        # Add return data
        if retDict is not None:
            blockStrList.extend(self._genCommentReturnText(retDict, prefix))

        # Complete the block
        blockStrList.append(self._genBlockEnd(blockIndent))
        return blockStrList

    def genDoxyClassComment(self, briefDesc, longDesc = None, blockIndent = 0):
        """!
        @brief Generate a doxygen cgenDoxyClassCommentlass/structure documentation block

        @param briefDesc {string} @brief description for the comment block
        @param longDesc {string} Detailed description for the comment block or None if no detailed description
        @param blockIndent Current cmment block indentation

        @return list of strings - Comment block as a list of formatted strings
        """
        # Generate the block start
        blockStrList = [self._genBlockStart(blockIndent)]

        # Generate the block prefix text fot the rest
        prefix = self._genCommentBlockPrefix(blockIndent)

        # Add the brief text
        blockStrList.extend(self._genBriefDesc(briefDesc, prefix))

        # Add the long description
        if longDesc is not None:
            blockStrList.append(prefix) # add empty line for readability
            blockStrList.extend(self._genLongDesc(prefix, longDesc))

        # Complete the block
        blockStrList.append(self._genBlockEnd(blockIndent))
        return blockStrList

    def genDoxyDefgroup(self, group, groupdef, fileName):
        """!
        @brief Doxygen defgroup comment block
        @param group {string} Name of the group to define
        @param groupdef {string} Description of the new group
        @param fileName {string} File name and extention
        @param blockIndent {number} Block indentation spaces, default=0
        @return list of strings - Code to output
        """
        doxyGroupBlk = [self.commentData['doxyBlockStart']]

        # Generate the block prefix text fot the rest
        prefix = self.commentData['doxyLineStart']
        doxyGroupBlk.append(prefix+"@file "+fileName)
        doxyGroupBlk.append(prefix+"@defgroup "+group+" "+groupdef)
        doxyGroupBlk.append(prefix+"@ingroup "+group)
        doxyGroupBlk.append(prefix+"@{")
        doxyGroupBlk.append(self.commentData['blockEnd'])
        return doxyGroupBlk

    def genDoxyGroupEnd(self):
        """!
        @brief Doxygen group comment block end marker
        @return list of strings - Code to output
        """
        doxyEnd = self.commentData['doxyBlockStart']+"@}"+self.commentData['blockEnd']
        return [doxyEnd]

class CDoxyCommentGenerator(DoxyCommentGenerator):
    """!
    C/C++ file Doxygen comment generator class
    """
    def __init__(self):
        super().__init__(CommentParams.cCommentParms, False)

class PyDoxyCommentGenerator(DoxyCommentGenerator):
    """!
    Python file Doxygen comment generator class
    """
    def __init__(self):
        super().__init__(CommentParams.pyCommentParms, True)

class TsDoxyCommentGenerator(DoxyCommentGenerator):
    """!
    Typescript file Doxygen comment generator class
    """
    def __init__(self):
        super().__init__(CommentParams.cCommentParms, True)

class JsDoxyCommentGenerator(DoxyCommentGenerator):
    """!
    Javascript file Doxygen comment generator class
    """
    def __init__(self):
        super().__init__(CommentParams.cCommentParms, True)
