"""@package autogenlang
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

import re
def GenerateLanguageSelectFiles():
    """!
    @brief Generate the default JSON files
    """

def parseTranlateString(baseString):
    matchList = re.finditer(r'@[a-zA-Z_][a-zA-Z0-9_]*@', baseString)
    streamString = "parser_str_stream parserstr;  parserstr"
    previousEnd = 0

    for matchData in matchList:
        print(matchData)
        print("Text : "+matchData.group())
        print("Start: "+str(matchData.start()))
        print("End  : "+str(matchData.end()))

        # Add text data prior to first match if any
        if matchData.start() > previousEnd:
            streamString += " << \""
            streamString += r'{}'.format(baseString[previousEnd:matchData.start()])
            streamString += "\""

        # Add the matched parameter
        streamString += " << "
        streamString += matchData.group()[1:-1]
        previousEnd = matchData.end()

    # Add the trailing string
    if previousEnd < len(baseString):
        streamString += " << \""
        streamString += baseString[previousEnd:]
        streamString += "\""

    streamString += "; return parserstr.str();"
    return streamString


if __name__ == '__main__':
    print(parseTranlateString("Unknown argument @keyString@"))
    print(parseTranlateString("\\\"@keyString@\\\" missing assignment value(s). Expected: @nargsExpected@ found: @nargsFound@ arguments"))
    print(parseTranlateString("Argument add failed: @keyString@"))