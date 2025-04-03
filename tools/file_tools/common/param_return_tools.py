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

class ParamRetDict(object):
    """!
    Parameter and return dictionary definitions
    """
    def __init__(self):
        pass

    @staticmethod
    def buildReturnDict(retType, retDesc=""):
        """!
        @brief Build a return data dictionary
        @param retType {string} Code type definition
        @param retDesc {string} Brief description of the return value for @return doxygen generation
        """
        return {'type':retType, 'desc':retDesc}

    @staticmethod
    def getReturnData(returnDict):
        """!
        @brief Build a return data dictionary
        @param returnDict {dictionary} Return dictionary entry
        @return tuple - Return type string, Return description string
        """
        return returnDict['type'], returnDict['desc']

    @staticmethod
    def getReturnType(returnDict):
        """!
        @brief Build a return data dictionary
        @param returnDict {dictionary} Return dictionary entry
        @return string - Return type string
        """
        return returnDict['type']

    @staticmethod
    def buildParamDict(paramName, paramType, paramDesc=""):
        """!
        @brief Build a return data dictionary
        @param paramName {string} Code param name
        @param paramType {string} Code param type
        @param paramDesc {string} Brief description of the param value for @param doxygen generation
        """
        return {'name':paramName, 'type':paramType, 'desc':paramDesc}

    @staticmethod
    def getParamData(paramDict):
        """!
        @brief Build a return data dictionary
        @param paramDict {dictionary} Parameter dictionary entry
        @return tuple - Parameter name string, parameter type string, parameter description string
        """
        return paramDict['name'], paramDict['type'], paramDict['desc']

    @staticmethod
    def getParamType(paramDict):
        """!
        @brief Build a return data dictionary
        @param paramDict {dictionary} Parameter dictionary entry
        @return string - Parameter type string
        """
        return paramDict['type']

    @staticmethod
    def getParamName(paramDict):
        """!
        @brief Build a return data dictionary
        @param paramDict {dictionary} Parameter dictionary entry
        @return string - Parameter name string
        """
        return paramDict['name']
