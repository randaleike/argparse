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
import json

from file_tools.common.param_return_tools import ParamRetDict
from file_tools.common.file_gen_tools import GenCFunctionHelper

from file_tools.string_name_generator import StringClassNameGen
from tools.file_tools.jsonLanguageDescriptionList import LanguageDescriptionList

class StringClassDescription(object):
    """!
    String object class definitions
    """

    def __init__(self, stringDefFileName = "argparse-string-def.json"):
        """!
        @brief StringClassDescription constructor

        @param langListFileName (string) - Name of the json file containing
                                           the language description data
        """
        self.filename = stringDefFileName
        try:
            langJsonFile = open(stringDefFileName, 'r', encoding='utf-8')
        except FileNotFoundError:
            self.stringJasonData = {'propertyMethods':{}, 'translateMethods':{}}
        else:
            self.stringJasonData = json.load(langJsonFile)
            langJsonFile.close()

    def _definePropertyFunctionEntry(self, propertyName = "", briefDesc = "", retType = "", retDesc = "", isList = False):
        """!
        @brief Define a property string return function dictionary and
               return the entry to the caller

        @param propertyName {string} Name of the property
        @param briefDesc {string} Brief description of the function used in
                                  doxygen comment block generation
        @param retType {string} Return type string
        @param retDesc {string} Description of the return parserstr value
        @param islist {boolean} True = data is a list, False = single value

        @return {'name':<string>, 'briefDesc':<string>, 'params':[],
                 'return':ParamRetDict.buildReturnDict(retType, retDesc, isList),
                 'inline':<string>} property function dictionary
        """
        functionDict = {'name': propertyName,
                        'briefDesc': briefDesc,
                        'params': [],
                        'return': ParamRetDict.buildReturnDict(retType, retDesc, isList)
                        }
        return functionDict

    def getPropertyMethodList(self):
        """!
        @brief Return a list of property method name strings
        @return list of strings - Names of the property methods
        """
        return list(self.stringJasonData['propertyMethods'].keys())

    def getPropertyMethodData(self, methodName):
        """!
        @brief Return the input methodName data
        @return (tuple) - {string} Language descption property name,
                          {string} Brief description of the property method for Doxygen comment,
                          {list of dictionaries} Parameter list (probably empty list),
                          {dictionary} Return data dictionary
        """
        entry = self.stringJasonData['propertyMethods'][methodName]
        return entry['name'], entry['briefDesc'], entry['params'], entry['return']

    def _defineTranslationDict(self, translateBaseLang = "en", translateText = ""):
        """!
        @brief Create a translation dictionary
        @param translateBaseLang {string} Google translation language code for the input translateText string
        @param translateText {string} Text of the message
        @return dictionary - {'base':<translateBaseLang>, 'text':<translateText>} Translate method translation string dictionary
        """
        return {'base':translateBaseLang, 'text':translateText}

    def _getTranslationDictData(self, translateDict):
        """!
        @brief Create a translation dictionary
        @param translateDict {dictionary} Translate method translation string dictionary
        @return tuple - {string} Google translation language code for the input translateText string,
                        {string} Text of the message
        """
        return translateDict['base'], translateDict['text']

    def _defineTranslateFunctionEntry(self, briefDesc = "", paramsList = [], retDesc = "", translateBaseLang = "en", translateText = ""):
        """!
        @brief Define a property string return function dictionary and
               return the entry to the caller

        @param briefDesc {string} Brief description of the function used in
                                  doxygen comment block generation
        @param paramsList {list of dictionaries} List of the function parameter dictionary entrys
        @param retDesc {string} Description of the return parserstr value
        @param translateBaseLang {string} Google translation language code for the input translateText string
        @param translateText {string} Text of the message

        @return {'name':<string>, 'briefDesc':<string>, 'params':[],
                 'return':ParamRetDict.buildReturnDict('text', retDesc, False),
                 'translateDesc': {'base':<string> 'text':<string>}} Translate function dictionary
        """
        functionDict = {'briefDesc': briefDesc,
                        'params': paramsList,
                        'return': ParamRetDict.buildReturnDict("text", retDesc, False),
                        'translateDesc': self._defineTranslationDict(translateBaseLang, translateText)}
        return functionDict

    def getTranlateMethodList(self):
        """!
        @brief Return a list of property method name strings
        @return list of strings - Names of the property methods
        """
        return list(self.stringJasonData['translateMethods'].keys())

    def getTranlateMethodFunctionData(self, methodName):
        """!
        @brief Return the input methodName data
        @return (tuple) - {string} Brief description of the property method for Doxygen comment,
                          {list of dictionaries} Parameter list (probably empty list),
                          {dictionary} Return data dictionary
        """
        entry = self.stringJasonData['translateMethods'][methodName]
        return entry['briefDesc'], entry['params'], entry['return']

    def getTranlateMethodTextData(self, methodName):
        """!
        @brief Return the input methodName data
        @return (tuple) - {string} Google language code,
                          {string} Base text
        """
        entry = self.stringJasonData['translateMethods'][methodName]
        return self._getTranslationDictData(entry['translateDesc'])

    def _inputGoogleTranslateCode(self):
        """!
        @brief Get the google translate language code from user input and check for validity
        @return string - translate code
        """
        googleTranslateId = ""
        while(googleTranslateId == ""):
            transId = input("Enter original string google translate language code (2 lower case characters): ").lower()

            # Check validity
            if re.match('^[a-z]{2}$', transId):
                # Valid name
                googleTranslateId = transId
            else:
                # invalid name
                print("Error: Only two characters a-z are allowed in the code, try again.")
        return googleTranslateId

    def _inputCName(self):
        paramName = ""
        while(paramName == ""):
            name = input("Enter parameter name: ")
            name.strip()

            # Check validity
            if re.match('^[a-zA-Z_][a-zA-Z0-9_]*$', name):
                # Valid name
                paramName = name
            else:
                # invalid name
                print("Error: Must be a valid C name, try again.")
        return paramName

    def _inputCType(self):
        varType = ""
        while(varType == ""):
            inputType = input("Enter parameter type [s(tring)|t(ext)|n(umber)|c(ustom)] : ").lower()

            # Check validity
            if (inputType == "s") or (inputType=="string"):
                varType = "string"
            elif (inputType == "t") or (inputType=="text"):
                varType = "text"
            elif (inputType == "n") or (inputType=="number"):
                varType = "number"
            elif (inputType == "c") or (inputType=="custom"):
                print ("Note: Custom type must have an operator<< defined.")
                customType = input("Enter custom parameter type: ")

                typeNames = customType.split("::")
                # Strip reference and pointer decorations
                typeNames[-1].rstrip('&')
                typeNames[-1].rstrip('*')
                for typeName in typeNames:
                    typeName.strip() # Remove whitespace
                    if re.match('^[a-zA-Z_][a-zA-Z0-9_]*$', typeName):
                        # valid
                        varType = customType
                    else:
                        # invalid type
                        print (customType+" is not a valid c/c++ type name, try again.")
            else:
                # invalid name
                print("Error: \""+inputType+"\" unknown. Please select one of the options from the menu.")

        return varType

    def _inputParameterData(self):
        """!
        @brief Get input parameter data from user input
        @return dictionary - Param dictionary from  ParamRetDict.buildParamDict()
        """
        paramName = self._inputCName()
        paramType = self._inputCType()
        paramDesc = input("Enter brief parameter description for doxygen comment: ")
        return ParamRetDict.buildParamDict(paramName, paramType, paramDesc)

    def _inputReturnData(self):
        """!
        @brief Get the return data description from the user
        """
        retDesc = input("Enter brief description of the return value for doxygen comment: ")
        return retDesc

    def update(self):
        """!
        @brief Update the JSON file with the current contents of self.langJsonData
        """
        with open(self.filename, 'w', encoding='utf-8') as langJsonFile:
            json.dump(self.stringJasonData, langJsonFile, indent=2)

    def _getTranslateString(self, paramList):
        """!
        @brief Get the translation string template for the new translate function

        @param paramList {list of dictionaries} List of parameter description dictionaries
                                                for this function

        @return string - Validated translation template string
        """
        stringValid = False
        translateString = ""
        paramCount = len(paramList)

        if paramCount > 0:
            print("Enter translation template string. Use @paramName@ in the string to indicate where the ")
            print("function parameters should be inserted.")
            print("Example with single input parameter name \"keyString\": Unknown argumument key @keyString@")

            while not stringValid:
                translateString = input("String:")

                matchCount = 0
                prefix = "    "
                expectedParamList = ""
                for param in paramList:
                    paramRegex = "@"+param['name']+"@"
                    prefix=", "

                    if re.match(paramRegex, translateString):
                        matchCount += 1

                if matchCount < paramCount:
                    print ("Error: Translation template missing "+str(paramCount-matchCount)+" of "
                        +str(paramCount)+" template parameters")
                    print("Input template: \""+translateString+"\"")
                    print("Expected parameter list:")
                    print (expectedParamList)
                else:
                    stringValid = True
        else:
            translateString = input("Enter translation template string: ")

        return translateString

    def newTranslateMethodEntry(self):
        """!
        @brief Define and add a new translate string return function dictionary
               to the list of translate functions
        """
        newEntry = {}
        entryCorrect = False

        while not entryCorrect:
            functionName = self._inputCName()
            functionDesc = input("Enter brief function description for doxygen comment: ")

            paramList = []
            paramCount = input("Enter parameter count? [0-n]: ")
            while(paramCount > 0):
                paramList.append(self._inputParameterData())
                paramCount -= 1

            returnDesc = self._inputReturnData()

            languageBase = self._inputGoogleTranslateCode()
            translateString = self._getTranslateString(paramList)
            newEntry = self._defineTranslateFunctionEntry(functionDesc, paramList, returnDesc, languageBase, translateString)

            # Print entry for user to inspect
            print("New Entry:")
            print(newEntry)
            commit = input("Is this correct? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                entryCorrect = True

        # Test existing for match
        commitFlag = False
        if functionName in self.stringJasonData['translateMethods'].keys():
            # Determine if we should overwrite existing
            commit = input("Overwrite existing "+functionName+" entry? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                commitFlag = True
                self.stringJasonData['translateMethods'][functionName] = newEntry
        else:
            commit = input("Add new entry? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                commitFlag = True
                self.stringJasonData['translateMethods'][functionName] = newEntry

        return commitFlag

    def addTranslateMethodEntry(self, functionName, functionDesc, paramList, returnDescription, googleLangCode, translateString):
        """!
        @brief Add a new translate string return function dictionary
               to the list of translate functions
        @param functionName {string} Name of the function
        @param functionDesc {string} Brief description of the function for doxygen comment generation
        @param paramList {list of dictionaries} List of the input parameter description dictionaries
        @param returnDescription {string} Brief description of the return value for doxygen comment generation
        @param googleLangCode {string} Google translate language ID code of the input translateString
        @param translateString {string} String to generate translations for
        """
        newEntry = self._defineTranslateFunctionEntry(functionDesc, paramList, returnDescription, googleLangCode, translateString)

        if functionName in self.stringJasonData['translateMethods'].keys():
            # Determine if we should overwrite existing
            commit = input("Overwrite existing "+functionName+" entry? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                self.stringJasonData['translateMethods'][functionName] = newEntry
        else:
            self.stringJasonData['translateMethods'][functionName] = newEntry

    def _getPropertyReturnData(self):
        """!
        @brief Get the property function return data and property name
        @return string, string, string, string - Language description property name,
                                                 Method name,
                                                 Method return type,
                                                 Return type description for Doxygen comment
                                                 True if return is a list, else False
        """
        propertyOptions = LanguageDescriptionList.getLanguagePropertyList()

        print ("Select language property, from options:")
        optionText = ""
        optionPrefix = "    "
        maxIndex = 0
        for index, propertyId in enumerate(propertyOptions):
            optionText +=  optionPrefix
            optionText += str(index)+": "
            optionText += propertyId
            optionPrefix = ", "
            maxIndex += 1
        print (optionText)

        propertyId = None
        while propertyId is None:
            propertyIndex = int(input("Enter property [0 - "+str(maxIndex-1)+"]: "))
            if (propertyIndex >= 0) and (propertyIndex < maxIndex):
                propertyId = propertyOptions[propertyIndex]
            else:
                print ("Valid input values are 0 to "+str(maxIndex-1)+", try again")

        returnType, returnDesc, isList = LanguageDescriptionList.getLanguagePropertyReturnData(propertyId)
        methodName = LanguageDescriptionList.getLanguagePropertyMethodName(propertyId)
        return propertyId, methodName, returnType, returnDesc, isList

    def newPropertyMethodEntry(self):
        """!
        @brief Define and add a property string return function dictionary and
               add it to the list of translate functions
        """
        newEntry = {}
        entryCorrect = False

        while not entryCorrect:
            propertyName, methodName, returnType, returnDesc, isList = self._getPropertyReturnData()
            functionDesc = "Get the "+returnDesc+" for this object"

            newEntry = self._definePropertyFunctionEntry(propertyName, functionDesc, returnType, returnDesc, isList)

            # Print entry for user to inspect
            print(methodName+":")
            print(newEntry)
            commit = input("Is this correct? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                entryCorrect = True

        # Check for existing for match
        commitFlag = False
        if methodName in self.stringJasonData['propertyMethods'].keys():
            # Determine if we should overwrite existing
            commit = input("Overwrite existing "+methodName+" entry? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                self.stringJasonData['propertyMethods'][methodName] = newEntry
                commitFlag = True
        else:
            # Determine if we should add the new entry
            commit = input("Add new entry? [Y/N]").upper()
            if ((commit == 'Y') or (commit == "YES")):
                self.stringJasonData['propertyMethods'][methodName] = newEntry
                commitFlag = True

        return commitFlag

    def addPropertyMethodEntry(self, propertyName):
        """!
        @brief Add a new translate string return function dictionary
               to the list of translate functions
        @param propertyName {string} LanguageDescriptionList.getLanguagePropertyList() property key
        """
        # Make sure property exists in the language data
        propertyList = LanguageDescriptionList.getLanguagePropertyList()

        # Property exists, generate the new entry
        if propertyName in propertyList:
            returnType, returnDesc, isList = LanguageDescriptionList.getLanguagePropertyReturnData(propertyName)
            functionDesc = "Get the "+returnDesc+" for this object"
            functionName = LanguageDescriptionList.getLanguagePropertyMethodName(propertyName)

            newEntry = self._definePropertyFunctionEntry(propertyName, functionDesc, returnType, returnDesc)

            if functionName in self.stringJasonData['propertyMethods'].keys():
                # Verify the overwrite
                commit = input("Overwrite existing "+functionName+" entry? [Y/N]").upper()
                if ((commit == 'Y') or (commit == "YES")):
                    self.stringJasonData['propertyMethods'][functionName] = newEntry
            else:
                # Add the entry
                self.stringJasonData['propertyMethods'][functionName] = newEntry


def CreateDefaultStringFile():
    """!
    @brief Add a function to the self.langJsonData data
    """
    classStrings = StringClassDescription()
    classStrings.addPropertyMethodEntry("isoCode")

    # General argument parsing messages
    classStrings.addTranslateMethodEntry("getNotListTypeMessage", "Return non-list varg error message",
                                           [ParamRetDict.buildParamDict("nargs", "int", "input nargs value")],
                                           "Non-list varg error message",
                                           "en",
                                           "Only list type arguments can have an argument count of @nargs@")

    classStrings.addTranslateMethodEntry("getUnknownArgumentMessage", "Return unknown parser key error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Unknown key")],
                                           "Unknown parser key error message",
                                           "en",
                                           "Unknown argument @keyString@")

    classStrings.addTranslateMethodEntry("getInvalidAssignmentMessage", "Return varg invalid assignment error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key")],
                                           "Varg key invalid assignment error message",
                                           "en",
                                           "\\\"@keyString@\\\" invalid assignment")

    classStrings.addTranslateMethodEntry("getAssignmentFailedMessage", "Return varg assignment failed error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key"),
                                            ParamRetDict.buildParamDict("valueString", "string", "Assignment value")],
                                           "Varg key assignment failed error message",
                                           "en",
                                           "\\\"@keyString@\\\", \\\"@valueString@\\\" assignment failed")

    classStrings.addTranslateMethodEntry("getMissingAssignmentMessage", "Return varg missing assignment error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key")],
                                           "Varg key missing value assignment error message",
                                           "en",
                                           "\\\"@keyString@\\\" missing assignment value")

    classStrings.addTranslateMethodEntry("getMissingListAssignmentMessage", "Return varg missing list value assignment error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key"),
                                            ParamRetDict.buildParamDict("nargsExpected", "size_t", "Expected assignment list length"),
                                            ParamRetDict.buildParamDict("nargsFound", "size_t", "Input assignment list length")],
                                           "Varg key input value list too short error message",
                                           "en",
                                           "\\\"@keyString@\\\" missing assignment value(s). Expected: @nargsExpected@ found: @nargsFound@ arguments")

    classStrings.addTranslateMethodEntry("getTooManyAssignmentMessage", "Return varg missing list value assignment error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key"),
                                            ParamRetDict.buildParamDict("nargsExpected", "size_t", "Expected assignment list length"),
                                            ParamRetDict.buildParamDict("nargsFound", "size_t", "Input assignment list length")],
                                           "Varg key input value list too long error message",
                                           "en",
                                           "\\\"@keyString@\\\" too many assignment values. Expected: @nargsExpected@ found: @nargsFound@ arguments")

    classStrings.addTranslateMethodEntry("getMissingArgumentMessage", "Return required varg missing error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key")],
                                           "Required varg key missing error message",
                                           "en",
                                           "\\\"@keyString@\\\" required argument missing")

    classStrings.addTranslateMethodEntry("getArgumentCreationError", "Return parser add varg failure error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Error key")],
                                           "Parser varg add failure message",
                                           "en",
                                           "Argument add failed: @keyString@")

    # Command Line parser messages
    classStrings.addTranslateMethodEntry("getUsageMessage", "Return usage help message",
                                           [],
                                           "Usage help message",
                                           "en",
                                           "Usage:")

    classStrings.addTranslateMethodEntry("getPositionalArgumentsMessage", "Return positional argument help message",
                                           [],
                                           "Positional argument help message",
                                           "en",
                                           "Positional Arguments:")

    classStrings.addTranslateMethodEntry("getSwitchArgumentsMessage", "Return optional argument help message",
                                           [],
                                           "Optional argument help message",
                                           "en",
                                           "Optional Arguments:")

    classStrings.addTranslateMethodEntry("getHelpString", "Return default help switch help message",
                                           [],
                                           "Default help argument help message",
                                           "en",
                                           "show this help message and exit")

    # Environment parser messages
    classStrings.addTranslateMethodEntry("getEnvArgumentsMessage", "Return environment parser argument help header",
                                           [],
                                           "Environment parser argument help header message",
                                           "en",
                                           "Defined Environment values:")

    classStrings.addTranslateMethodEntry("getEnvironmentNoFlags", "Return environment parser add flag varg failure error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Flag key")],
                                           "Environment parser add flag varg failure message",
                                           "en",
                                           "Environment value @keyString@ narg must be > 0")

    classStrings.addTranslateMethodEntry("getRequiredEnvironmentArgMissing", "Return environment parser required varg missing error message",
                                           [ParamRetDict.buildParamDict("keyString", "string", "Flag key")],
                                           "Environment parser required varg missing error message",
                                           "en",
                                           "Environment value @keyString@ must be defined")


    # JSON file parser messages
    classStrings.addTranslateMethodEntry("getJsonArgumentsMessage", "Return json parser argument help header",
                                           [],
                                           "JSON parser argument help header message",
                                           "en",
                                           "Available JSON argument values:")

    # XML file parser messages
    classStrings.addTranslateMethodEntry("getXmlArgumentsMessage", "Return xml parser argument help header",
                                           [],
                                           "XML parser argument help header message",
                                           "en",
                                           "Available XML argument values:")

    classStrings.update()

def AddTranslateMethodEntry():
    """!
    @brief Add a translate string function to the self.langJsonData data
    """
    classStrings = StringClassDescription()
    commit = classStrings.newTranslateMethodEntry()
    if commit:
        print ("Updating JSON file")
        classStrings.update()

def AddPropertyMethodEntry():
    """!
    @brief Add a property string function to the self.langJsonData data
    """
    classStrings = StringClassDescription()
    commit = classStrings.newPropertyMethodEntry()
    if commit:
        print ("Updating JSON file")
        classStrings.update()

def PrintMethods():
    classStrings = StringClassDescription()
    print(classStrings.stringJasonData)

import argparse
def CommandMain():
    """!
    Utility command interface
    @param subcommand {string} JSON string file command
    """
    parser = argparse.ArgumentParser(prog="jsonStringClassDescription",
                                     description="Update argpasre library language description JSON file")
    parser.add_argument('subcommand', choices=['addproperty', 'addtranslate', 'print', 'createnew'])
    args = parser.parse_args()

    if args.subcommand.lower() == "addproperty":
        AddPropertyMethodEntry()
    if args.subcommand.lower() == "addtranslate":
        AddTranslateMethodEntry()
    elif args.subcommand.lower() == "print":
        PrintMethods()
    elif args.subcommand.lower() == "createnew":
        CreateDefaultStringFile()
    else:
        print ("Error: Unknown JSON string method definition file command: "+args.subcommand)
        SystemExit(1)


if __name__ == '__main__':
    CommandMain()