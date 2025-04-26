####
# Language files include path
####
set (languageStringsIncDir
     ${MASTER_PROJECT_BASE_DIR}/language/inc
     )

####
# Language files mock include path
####
set (languageStringsMockIncDir
     ${languageStringsIncDir}
     ${MASTER_PROJECT_BASE_DIR}/language/mock
     )

####
# Language source file list
####
set (languageStringsSrc
     ${MASTER_PROJECT_BASE_DIR}/language/src/ParserStringListInterface.cpp
     ${MASTER_PROJECT_BASE_DIR}/language/src/ParserStringListInterfaceEnglish.cpp
     ${MASTER_PROJECT_BASE_DIR}/language/src/ParserStringListInterfaceSpanish.cpp
     ${MASTER_PROJECT_BASE_DIR}/language/src/ParserStringListInterfaceFrench.cpp
     ${MASTER_PROJECT_BASE_DIR}/language/src/ParserStringListInterfaceSimplifiedchinese.cpp
     )

####
# Language mock file list
####
set (languageStringsMockSrc
     ${MASTER_PROJECT_BASE_DIR}/language/mock_ParserStringListInterface.cpp
     )

