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

import json
langDetectFileHeader = ["","#include <memory>", "#include \"os_language_detect.h\"",""]
langDetectFileTail = ["",
                      "/**",
                      " * @brief SystemLanguageDetection::getLangId() implementation",
                      " * Determine the OS and call the appropriate function to get ",
                      " * the string list",
                      " *",
                      " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based ",
                      " *                                                      on the local settings",
                      " */",
                      "std::shared_ptr<ParserStringListInterface> SystemLanguageDetection::getLangId()",
                      "{",
                      "#if ((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                      "    return getDynamicLangID_Linux();",
                      "#elif ((defined(_WIN64) || defined(_WIN32)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                      "    return getDynamicLangID_Windows();",
                      "#elif defined(DYNAMIC_INTERNATIONALIZATION)",
                      "    #error \"Unknown OS type, no dynamic language id function defined\"",
                      "#else // not defined(DYNAMIC_INTERNATIONALIZATION)",
                      "    return getStaticLang();",
                      "#endif // defined os and defined(DYNAMIC_INTERNATIONALIZATION))",
                      "}"
                     ]

GetDynamicLangIdLinuxHeader = ["#if ((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                               "  #include <cstdlib>",
                               "  #include <regex>",
                               "/**",
                               " * @brief Pull the LANG environment string and translate it into the appropriate",
                               " *        ParserStringListInterface object",
                               " *",
                               " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based on the",
                               " *                                                      \"LANG\" environment value",
                               " */",
                               "std::shared_ptr<ParserStringListInterface> getDynamicLangID_Linux()",
                               "{",
                               "    const char* langId = getenv(\"LANG\");",
                               "    if (nullptr != langId)",
                               "    {"
                              ]

GetDynamicLangIdLinuxTail =   ["    }",
                               "}",
                               "#endif //((defined(__linux__) || defined(__unix__)) && defined(DYNAMIC_INTERNATIONALIZATION))"
                              ]

GetDynamicLangIdWinHeader = ["#if ((defined(_WIN64) || defined(_WIN32)) && defined(DYNAMIC_INTERNATIONALIZATION))",
                             "#include <Windows.h>",
                             "/**",
                             " * @brief Get the windows local language id and translate it to the appropriate"
                             " *        ParserStringListInterface object",
                             " *",
                             " * @return std::shared_ptr<ParserStringListInterface> - Pointer to ParserStringListInterface based on the",
                             " *                                                      LANGID value",
                             " */",
                             "std::shared_ptr<ParserStringListInterface> getDynamicLangID_Windows()",
                             "{",
                             "    switch(langId)",
                             "    {"
                            ]

GetDynamicLangIdWinTail =   ["    }",
                             "}",
                             "#endif //((defined(_WIN64) || defined(_WIN32)) && defined(DYNAMIC_INTERNATIONALIZATION))"
                            ]

getStaticLangHeader = ["#if !defined(DYNAMIC_INTERNATIONALIZATION))",
                       "/**",
                       " * @brief Get the default ParserStringListInterface object",
                       " *",
                       " * @return std::shared_ptr<ParserStringListInterface> - Pointer to the default ParserStringListInterface",
                       " */",
                       "std::shared_ptr<ParserStringListInterface> getStaticLang()",
                       "{",

]

class LanguageDescriptionList(object):
    """!
    Language description list data
    """
    def __init__(self, langListFileName = "argparse-lang-list.json"):
        """!
        @brief LanguageDescriptionList constructor

        @param langListFileName (string) - Name of the json file containing
                                           the language description data
        """
        self.filename = langListFileName
        try:
            langJsonFile = open(langListFileName, 'r', encoding='utf-8')
        except FileNotFoundError:
            self.langJsonData = {}
        else:
            self.langJsonData = json.load(langJsonFile)
            langJsonFile.close()


    def update(self):
        """!
        @brief Update the JSON file with the current contents of self.langJsonData
        """
        with open(self.filename, 'w', encoding='utf-8') as langJsonFile:
            json.dump(self.langJsonData, langJsonFile, indent=2)

    def addDefault(self, langName):
        """!
        @brief

        @param langName (string) - Language name to use default if detection fails
        """
        defaultDict = {"name":langName}
        self.langJsonData["default"] = defaultDict

    def addLanguage(self, langName, googleLangCode, linuxEnvCode, windowsLangId, iso639Code):
        """!
        @brief Add a language to the self.langJsonData data

        @param langName (string) - Language name to use for file/class name generation
        @param googleLangCode (string) - Google translate language code
        @param linuxEnvCode (regex string) - linux LANG environment value(s) for this language
        @param windowsLangId (list of numbers) - Windows LANGID value(s) for this language
        @param iso639Code (string) - ISO 639 set 3 language code
        """
        langData = [('googleCode', googleLangCode),
                    ('LANG', linuxEnvCode),
                    ('LANGID', windowsLangId),
                    ('isoCode', iso639Code)]
        langEntry = dict(langData)
        self.langJsonData[langName] = langEntry

    def outputGetDynamicLangIDLinux(self, outputFile):
        """!
        @brief Output the getDynamicLangID_Linux c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        outputFile.writelines(GetDynamicLangIdLinuxHeader)

        # Generate if/else if chain for each language in the dictionary
        checkPrefix = "if"
        indent = "        "
        for langName, langData in self.langJsonData:
            if langName != "default":
                ifline =  indent+checkPrefix
                ifline += "(std::regex_match(langId, std::regex<\""
                ifline += langData['LANG']
                ifline += "\")))"
                checkPrefix = "else if"

                ifStart = indent+"{"
                ifAssign = indent+"    return std::make_shared("
                ifAssign += "<ParserStringListInterface"
                ifAssign += str(langName).capitalize()
                ifAssign += ">();"
                ifEnd = indent+"}"
                outputFile.writelines([ifline, ifStart, ifAssign, ifEnd])

        # Add the final else case
        elseline =  indent+"else"
        elseStart = indent+"{"

        elseAssign = indent+"    return std::make_shared("
        elseAssign += "ParserStringListInterface"
        elseAssign += self.langJsonData["default"]["name"].capitalize()
        elseAssign += ">();"

        elseEnd = indent+"}"
        outputFile.writelines([elseline, elseStart, elseAssign, elseEnd])
        outputFile.writelines(GetDynamicLangIdLinuxTail)

    def outputGetDynamicLangIDWindows(self, outputFile):
        """!
        @brief Output the getDynamicLangID_Windows c++ function code to the
               outputFile file object

        @param outputFile (file) - File object to append the function to
        """
        outputFile.writelines(GetDynamicLangIdWinHeader)
        indent = "        "
        for langName, langData in self.langJsonData:
            caseLineList = []
            if langName != "default":
                for id in langData["LANGID"]:
                    caseline =  indent+"case"
                    caseline += hex(id)
                    caseline += ":"
                    caseLineList.append(caseline)
                classMod = str(langName)
            else:
                caseline =  indent+"default:"
                caseLineList.append(caseline)
                classMod = langData["name"]

            caseAssign = indent+"    return std::make_shared("
            caseAssign += "<ParserStringListInterface"
            caseAssign += classMod.capitalize()
            caseAssign += ">();"
            caseLineList.append(caseAssign)
            caseLineList.append(indent+"    break;")

            outputFile.writelines(caseLineList)

        outputFile.writelines(GetDynamicLangIdWinTail)

def AddEnglish(languages):
    """!
    @brief Add the english language definition
           Example for AddLanguage call

    @param languages (LanguageDescriptionList) - Object to add to
    """
    linuxEnv = R"^en_(AU|BZ|CA|CB|GB|IE|JM|NZ|PH|TT|US|ZA|ZW)\.UTF-8"
    winLanID = [3081,10249,4105,9225,2057,16393,6153,8201,5129,13321,7177,11273,1033,12297]
    languages.addLanguage("english", "en", linuxEnv, winLanID, "en")

def AddSpanish(languages):
    """!
    @brief Add the spanish language definition
           Example for AddLanguage call

    @param languages (LanguageDescriptionList) - Object to add to
    """
    linuxEnv = R"^es_(AR|BO|CL|CO|CR|DO|EC|ES|GT|HN|MX|NI|PA|PE|PR|PY|SV|UY|VE)\.UTF-8"
    winLanID = [11274,16394,13322,9226,5130,7178,12298,17418,4106,18442,2058,19466,6154,15370,10250,20490,1034,14346,8202]
    languages.addLanguage("spanish", "es", linuxEnv, winLanID, "es")

def AddFrench(languages):
    """!
    @brief Add the french language definition
           Example for AddLanguage call

    @param languages (LanguageDescriptionList) - Object to add to
    """
    linuxEnv = R"^fr_(BE|CA|CH|FR|LU|MC)\.UTF-8"
    winLanID = [2060,11276,3084,9228,12300,1036,5132,13324,6156,14348,10252,4108,7180]
    languages.addLanguage("french", "fr", linuxEnv, winLanID, "fr")

def AddSimplifiedChinese(languages):
    """!
    @brief Add the simplified chinese language definition
           Example for AddLanguage call

    @param languages (LanguageDescriptionList) - Object to add to
    """
    linuxEnv = R"^zh_(CN|HK|MO|SG|TW)\.UTF-8"
    winLanID = [2052,3076,5124,4100,1028]
    languages.addLanguage("SimplifiedChinese", "zh", linuxEnv, winLanID, "zh")

def CreateDefaultJson():
    """!
    @brief Create base default argparse-lang-list.json file
    """
    languages = LanguageDescriptionList()
    AddEnglish(languages)
    AddSpanish(languages)
    AddFrench(languages)
    AddSimplifiedChinese(languages)
    languages.addDefault("english")
    languages.update()

def AddLanguage(langFileName, googleLangCode, linuxEnvCode, windowsLangId, iso639Code):
    """!
    @brief Add a language to the self.langJsonData data

    @param langFileName (string) - Language name to use for filename generation
    @param googleLangCode (string) - Google translate language code
    @param linuxEnvCode (regex string) - linux LANG environment value(s) for this language
    @param windowsLangId (list of numbers) - Windows LANGID value(s) for this language
    @param iso639Code (string) - ISO 639 set 3 language code
    """
    languages = LanguageDescriptionList()
    languages.addLanguage(langFileName, googleLangCode, linuxEnvCode, windowsLangId, iso639Code)
    languages.update()

def GenerateLanguageSelectFiles(langSelectFileName):
    """!
    @brief            Example for AddLanguage call

    """


if __name__ == '__main__':
