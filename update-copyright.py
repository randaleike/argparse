#!/usr/bin/python3
'''
@package tools
'''

"""
 Copyright (c) 2024 Randal Eike
 
 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 """
 
"""@package update-copyright
Scan source files and update copyright years

Scan the source files and update the copyright year in the header section of any modified files
"""

import os
import platform
import subprocess
import time
from datetime import datetime

MIT_LicenceText = ["Permission is hereby granted, free of charge, to any person obtaining a ",
                   "copy of this software and associated documentation files (the \"Software\"), ",
                   "to deal in the Software without restriction, including without limitation "
                   "the rights to use, copy, modify, merge, publish, distribute, sublicense, ",
                   "and/or sell copies of the Software, and to permit persons to whom the ",
                   "Software is furnished to do so, subject to the following conditions: ",
                   "",
                   "The above copyright notice and this permission notice shall be included ",
                   "in all copies or substantial portions of the Software. ",
                   "",
                   "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, ",
                   "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF ",
                   "MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. ",
                   "IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY ",
                   "CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, ", 
                   "TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE ",
                   "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. "]

CopyRight_Owner = "Randal Eike"
CopyRight_Marker = "Copyright (c)"

CommentBlockDelim = {'.c':   {'blockStart': "/*", 'blockEnd': "*/", 'blockLineStart': " "}, 
                     '.cpp': {'blockStart': "/*", 'blockEnd': "*/", 'blockLineStart': " "}, 
                     '.h':   {'blockStart': "/*", 'blockEnd': "*/", 'blockLineStart': " "}, 
                     '.hpp': {'blockStart': "/*", 'blockEnd': "*/", 'blockLineStart': " "}, 
                     '.py':  {'blockStart': "\"\"\"", 'blockEnd':"\"\"\"", 'blockLineStart': " "},
                     '.sh':  {'blockStart': "###", 'blockEnd': "###", 'blockLineStart': "# "},
                     '.bat': {'blockStart': "REM start", 'blockEnd': "REM end", 'blockLineStart': "REM "},
                     '.txt': {'blockStart': None, 'blockEnd': None, 'blockLineStart': None},
                     }

BlockStartEndMarker = "=============================================================================="

DBG_MSG_NONE = 0
DBG_MSG_MINIMAL = 1
DBG_MSG_VERBOSE = 2
DBG_MSG_VERYVERBOSE = 3
debugLevel = DBG_MSG_VERBOSE

def debugPrint(messageLevel, message):
    if (debugLevel >= messageLevel): print ("Debug: "+message)

def errorPrint(message):
    print ("ERROR: "+message)
    
def getFileYears_filesystem(filePath):
    """
    Get the file creation year and last modification n year

    @param filePath (string): Path and file name of the file to query
    
    @retval tuple creation year string, last modification year string
    """
    creationTime     = os.path.getctime(filePath)
    modificationTime = os.path.getmtime(filePath)
    debugPrint (DBG_MSG_MINIMAL, "Creation Time:     "+str(creationTime))
    debugPrint (DBG_MSG_MINIMAL, "Modification Time: "+str(modificationTime))
    debugPrint (DBG_MSG_VERBOSE, "Now :"+datetime.now().strftime('%s'))
    
    try: 
        creationStruct     = time.localtime(creationTime)
        modificationStruct = time.localtime(modificationTime)
        debugPrint (DBG_MSG_VERYVERBOSE, "Creation Struct:     "+str(creationStruct))
        debugPrint (DBG_MSG_VERYVERBOSE, "Modification Struct: "+str(modificationStruct))
        return time.strftime("%Y", creationTime), time.strftime("%Y", modificationTime)
        
    except (OverflowError, OSError):
        errorPrint("Overflow error on conversion of time epoch.")
        return None, None
    except:
        errorPrint("Unknown converstion error of time epoch.")
        return None, None

def getFileYears_git(filePath):
    """
    Get the file creation year and last modification n year

    @param filePath (string): Path and file name of the file to query
    
    @retval tuple creation year string, last modification year string
    """

    
    # Get the date file was added to the archive    
    gitcmd = ["git", "log", "--diff-filter=A", "--format=%aI", filePath]
    for item in gitcmd: debugPrint (DBG_MSG_VERYVERBOSE, item)
                                    
    gitStart = subprocess.run(gitcmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    startOutput = gitStart.stdout.decode('utf-8')
    if (gitStart.returncode != 0):
        errorPrint("Git creation date failed:\n"+startOutput)    
        return None, None
    else:
        debugPrint (DBG_MSG_MINIMAL, "Creation Time:     "+startOutput)
        
    # Get the date file was last modified in the archive
    gitcmd = ["git", "log", "-1", "--format=%aI", filePath]
    for item in gitcmd: debugPrint (DBG_MSG_VERYVERBOSE, item)

    gitMod = subprocess.run(gitcmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    modOutput = gitMod.stdout.decode('utf-8')
    if (gitMod.returncode != 0):
        errorPrint("Git modification date failed:\n"+modOutput)    
        return None, None
    else:
        debugPrint (DBG_MSG_MINIMAL, "Creation Time:     "+modOutput)

    return startOutput[:4], modOutput[:4]
    
    
def getFileYears(filePath):
    """
    Get the file creation year and last modification n year

    @param filePath (string): Path and file name of the file to query
    
    @retval tuple creation year string, last modification year string
    """
    if (os.path.exists(filePath) and (os.path.isfile(filePath))):
        if (os.path.exists(".git") and (os.path.isdir(".git"))):
            return getFileYears_git(filePath)
        else:
            return getFileYears_filesystem(filePath)
    else:
        errorPrint("File: \""+filePath+"\" does not exist or is not a file.")
        return None, None

def find_special_file_header_end(inputFile, headerMarker):
    """
    Find the first line past the expected header markers for special files like .sh
    
    @param inputFile (file) - Input file to read
    @param headerMarker (string) - Header block marker
    
    @retval startposition - file position of the comment block or None if not found.
    """

    inputFile.seek(0)
    commentStart = 0
    commentStartOffset = 0
    startFound = False
    
    while (not startFound):
        currentLine = inputFile.readline()

        # Comment block at position 0
        if (-1 != currentLine.find(headerMarker)):
            startFound = True
            return commentStart
            
        commentStart = inputFile.tell()
        
    # Found only header markers
    return None
       
def find_comment_block_start(inputFile, commentBlockData):
    """
    Find the start of a comment block
    
    @param inputFile (file) - Input file to read
    @param commentBlockData (CommentBlockDelim) - Comment block marker definitions dictionary entry
    
    @retval startposition - file position of the comment block or None if not found.
    """
    startFound = False
    commentStart = inputFile.tell()
    currentLine = inputFile.readline()
    commentStartOffset = 0

    while ((not startFound) and (currentLine != "")):

        # Comment block at position 0
        if (0 == currentLine.find(commentBlockData['blockStart'])):
            startFound = True
            commentStartOffset = currentLine.find(commentBlockData['blockStart'])
            return commentStart + commentStartOffset

        commentStart = inputFile.tell()
        currentLine = inputFile.readline()
        
    # Found no more comment block starts before the end of the file, return None
    return None

def find_comment_block_end(inputFile, commentBlockData):
    """
    Find the end of a comment block
    
    @param inputFile (file) - Input file to read
    @param commentBlockData (CommentBlockDelim) - Comment block marker definitions dictionary entry
    
    @retval tuple (file position of the comment block end or None if not found, 
                   file position of the CopyRight_Marker or None if not found, 
                   True/False: CopyRight_Marker found in comment block)
    """
    copyrightFound = False
    copyrightOffset = inputFile.tell()
    commentEnd = copyrightOffset
    currentLine = inputFile.readline()
    
    while (currentLine != ""):
        if (-1 != currentLine.find(CopyRight_Marker)):
            copyrightOffset += currentLine.find(CopyRight_Marker)
            copyrightFound = True
        else:
            copyrightOffset = inputFile.tell()

        # Comment block has end marker, search for it
        if (-1 != currentLine.find(commentBlockData['blockEnd'])):
            return commentEnd, copyrightOffset, copyrightFound

        commentEnd = inputFile.tell()
        currentLine = inputFile.readline()

    # Found end of file before end of comment blocks, return None, copyrightFound
    return None, None, copyrightFound   
    
def find_copyright_block(inputFile, commentBlockData):
    """
    Find the start of the copyright block
    
    @param inputFile (file) - Input file to read
    @param commentBlockData (CommentBlockDelim) - Comment block marker definitions dictionary entry
    
    @retval tuple (commentStart - file position of the copyright message block start or None if not found,
                   commentEnd - file position of the copyright message block end or None if not found,
                   copyrightPosition - file position of the copyright message line or None if not found,
                   copyrightFound - True if copyright block found else false
    """
    
    copyrightFound = False    
    commentStart = None
    commentEnd = 0
    
    # start at the beginning of the file
    inputFile.seek(0)
    
    # Search for a comment block start 
    while (not copyrightFound):
        commentStart = find_comment_block_start(inputFile, commentBlockData)
        if (commentStart is not None):
            commentEnd, copyrightPosition, copyrightFound = find_comment_block_end(inputFile, commentBlockData)
            if (not copyrightFound):
                commentStart = None
                commentEnd = 0

    return commentStart, commentEnd, copyrightPosition, copyrightFound

def find_copyright_line(inputFile):
    """
    Find the start of the copyright line
    
    @param inputFile (file) - Input file to read
    
    @retval tuple (copyrightPosition - file position of the copyright message block or None if not found,
                   copyrightFound - True if copyright block found else false
    """
    copyrightOffset = inputFile.tell()
    currentLine = inputFile.readline()
    
    # search for copyright
    while (currentLine != ""):
        if (-1 != currentLine.find(CopyRight_Marker)):
            # found it
            copyrightFound = True
            return copyrightOffset, copyrightFound
        else:
            # next line
            copyrightOffset = inputFile.tell()
            currentLine = inputFile.readline()

    # Found end of file before copyright line
    return None, None, False   

def insert_new_copyright_line(inputFile, outputFilename, newCopyRightMsg):
    """
    Write a new file with the updated copyright message

    @param inputFile (file): Existing text file
    @param outputFilename (filename string): Name of the file to output updated text to
    @param newCopyRightMsg (string): New copyright message to write to the new file
                                            
    @retval Bool - True if new faile was written, False if an error occured.
    """
    try:
        outputFile = open(outputFilename, mode='wt')

        inputFile.seek(0)
        currentLine = inputFile.readline()
        while (currentLine != ""):
            copyrightOffset = currentLine.find(CopyRight_Marker)
            if (-1 != copyrightOffset):
                debugPrint(DBG_MSG_VERBOSE, "Copyright offset %d, prefix: \"%s\"" % (copyrightOffset, currentLine[:copyrightOffset-1]))
                if (0 != copyrightOffset): newMsg = currentLine[:copyrightOffset-1]+newCopyRightMsg
                else: newMsg = newCopyRightMsg
                outputFile.write(newMsg)
                outputFile.write("\n")
            else:
                outputFile.write(currentLine)

            currentLine = inputFile.readline()
            
        outputFile.close()
        return True
        
    except:
        errorPrint("Unable to open file \""+outputFilename+"\" for writing as text file.")
        return False
    
def insert_new_copyright_block(inputFile, outputFilename, commentStart, commentEnd, 
                               commentBlockData, newCopyRightMsg, updateLicence):
    """
    Write a new file with the updated copyright message

    @param inputFile (file): Existing text file
    @param outputFilename (filename string): Name of the file to output updated text to
    @param commentStart (position): Starting position of the comment block to replace 
    @param commentEnd (position): Ending position of the comment block to replace 
    @param commentBlockData (CommentBlockDelim) - Comment block marker definitions dictionary entry
    @param newCopyRightMsg (string): New copyright message to write to the new file
    @param updateLicence (list of strings): New license text to add to the copyright comment block
                                            
    @retval Bool - True if new faile was written, False if an error occured.
    """
    try:
        outputFile = open(outputFilename, mode='wt')

        # Copy the first chunk of the file
        inputFile.seek(0)
        header = inputFile.read(commentStart)
        outputFile.write(header)

        # Copy the comment block start
        newLine = inputFile.readline()
        outputFile.write(newLine)
        
        # Insert the new copyright and licence text
        newLine = commentBlockData['blockLineStart']+" "+newCopyRightMsg+"\n"
        outputFile.write(newLine)
        
        newLine = commentBlockData['blockLineStart']+"\n"
        outputFile.write(newLine)
        
        for licenceLine in updateLicence:
            newLine = commentBlockData['blockLineStart']+" "+licenceLine+"\n"
            outputFile.write(newLine)
            
        # Copy the comment block end
        inputFile.seek(commentEnd)
        newLine = inputFile.readline()
        outputFile.write(newLine)

        # Copy the remainder of the file
        while (newLine != ""):
            newLine = inputFile.readline()
            outputFile.write(newLine)
            
        outputFile.close()
        return True
        
    except:
        errorPrint("Unable to open file \""+outputFilename+"\" for writing as text file.")
        return False
    
    
def update_copyright_msg(inputFilename, outputFilename, createYear, modificationYear, owner = CopyRight_Owner, updateLicence = None):
    """
    Update the copyright years of input file.

    @param inputFilename (filename string): Name of the file to read
    @param outputFilename (filename string): Name of the file to output updated text to
    @param createYear (integer): Year the file was created (first copyright year)
    @param modificationYear (integer): Year the file was last modified (last copyright year)
    @param owner (string): Copyright owner, default = CopyRight_Owner
    @param updateLicence (string list): New licence text to put in copyright comment block or 
                                        None if only the copyright should be updated, default = None
        
    @retval True - Copyright message updated, else False
    """
    try:
        inputFile = open(inputFilename, mode='rt')

        if (createYear == modificationYear): newCopyRightMsg = CopyRight_Marker+" "+str(createYear)+" "+owner
        else: newCopyRightMsg = CopyRight_Marker+" "+str(createYear)+"-"+str(modificationYear)+" "+owner

        if (updateLicence is None):
                copyrightPosition, copyrightFound = find_copyright_line(inputFile)
                if (copyrightFound): 
                    debugPrint(DBG_MSG_VERBOSE, "Copyright Message found at %d." % (copyrightPosition))
                    returnVal = insert_new_copyright_line(inputFile, outputFilename, newCopyRightMsg)
        else:
            # Use the file name extension to determine the block 
            name_ext = os.path.splitext(inputFilename)
            extension = name_ext[1]
            
            try:
                # Find the comment block
                commentBlockData = CommentBlockDelim[extension]
                commentStart, commentEnd, copyrightPosition, copyrightFound = find_copyright_block(inputFile, commentBlockData)
                debugPrint(DBG_MSG_VERBOSE, "Copyright Comment block found, start %d, end %d." % (commentStart, commentEnd))
                if (copyrightFound): returnVal = insert_new_copyright_block(inputFile, outputFilename, commentStart, commentEnd, 
                                                                            commentBlockData, newCopyRightMsg, updateLicence)

            except KeyError:
                errorPrint("Unknown file type \""+extension+"\" no copyright block added.")
                returnVal = False
            
        inputFile.close()
        return returnVal

    except:
        errorPrint("Unable to open file \""+inputFilename+"\" for reading as text file.")
        return False

"""
Main
"""        
testfile = "./envparse/inc/envparse.h"
creationYear, modificationYear = getFileYears(testfile)

if(creationYear is None):
    print ("None returned from getFileYears() for creation year")
    creationYear = datetime.now().year
if(modificationYear is None):
    print ("None returned from getFileYears() for modification year")
    modificationYear = datetime.now().year

debugPrint(DBG_MSG_MINIMAL, "Creation Year:     "+str(creationYear))
debugPrint(DBG_MSG_MINIMAL, "Modification Year: "+str(modificationYear))
update_copyright_msg(testfile, "./updated-envparse.h", creationYear, modificationYear, CopyRight_Owner, MIT_LicenceText)            