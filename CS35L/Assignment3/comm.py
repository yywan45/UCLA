#! /usr/bin/python 

import random, sys, locale, string
from optparse import OptionParser

class comm:
    def __init__(self, args, onlyFile1, onlyFile2, bothFiles, unsortedFiles):

        # take into account if file1 or file2 is stdin
        
        if args[0] == "-":
            self.lines1 = sys.stdin.readlines()
        else:
            f1 = open(args[0], 'r')
            self.lines1 = f1.readlines()
            f1.close()

        if args[1] == "-":
            self.lines2 = sys.stdin.readlines()
        else:
            f2 = open(args[1], 'r')
            self.lines2 = f2.readlines()
            f2.close()

        self.onlyFile1 = onlyFile1
        self.onlyFile2 = onlyFile2
        self.bothFiles = bothFiles
        self.unsorted = unsortedFiles
        self.lines1 = self.processFileFunction(self.lines1)
        self.lines2 = self.processFileFunction(self.lines2)
        self.file1Name = args[0]
        self.file2Name = args[1]

    # if the last line of the file is not a new line then append one
    def processFileFunction(self, file):
        if not len(file): # since file is a list
            file.append("\n")
        else:
            lastLine = file[-1]
            if "\n" not in lastLine:
                lastLine += "\n"
            file[-1] = lastLine
        return file

    # return whether the files are sorted
    def sortFunction(self):
        filesSorted = True

        if sorted(self.lines1) != self.lines1:
            filesSorted = False
            sys.stderr.write(self.file1Name + " is not sorted.\n")

        if sorted(self.lines2) != self.lines2:
            filesSorted = False
            sys.stderr.write(self.file2Name + " is not sorted.\n")

        return filesSorted

    # print each line of the output table
    def printLineFunction(self, line, columnNumber):
        stream = ""
        
        if columnNumber == 1:
            if not self.onlyFile1:
                return
        if columnNumber == 2:
            if not self.onlyFile2:
                return
            if self.onlyFile1:
                stream += "        "
        if columnNumber == 3:
            if not self.bothFiles:
                return
            if self.onlyFile1:
                stream += "        "
            if self.onlyFile2:
                stream += "        "
                
        sys.stdout.write(stream + line)

    # compares both files and prints output table    
    def compareFunction(self):
        if self.unsorted:
            uniqueFile2 = self.lines2
            for word1 in self.lines1:
                if word1 not in self.lines2:
                    self.printLineFunction(word1, int(1))
                else:
                    self.printLineFunction(word1, int(3))
                    uniqueFile2.remove(word1)

            for word2 in uniqueFile2:
                self.printLineFunction(word2, int(2))
            return

        if not self.sortFunction():
            return

        i, j = 0, 0
        wordLength1, wordLength2 = len(self.lines1), len(self.lines2)

        while i in range(wordLength1) or j in range(wordLength2):
            word1, word2 = "", ""

            if i < wordLength1:
                word1 = self.lines1[i]
            if j < wordLength2:
                word2 = self.lines2[j]

            if word1 == "" or (word2 != "" and word1 > word2):
                self.printLineFunction(word2, 2)
                j += 1
            elif word2 == "" or word2 > word1:
                self.printLineFunction(word1, 1)
                i += 1
            else:
                self.printLineFunction(word2, 3)
                i += 1
                j += 1
                
        return

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
    Compare files FILE1 and FILE2 line by line.
    When FILE1 or FILE2 (not bothFiles) is -, read standard input.
    -1 suppress column 1 (lines unique to FILE1)
    -2 suppress column 2 (lines unique to FILE2)
    -3 suppress column 3 (lines that appear in both files)
    -u Allows sorting of unsorted files
    """

    # comm options
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action = "store_false", dest = "onlyFile1",
                      default = True,
                      help = "Suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", action = "store_false", dest = "onlyFile2",
                      default = True,
                      help = "Suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", action = "store_false", dest = "bothFiles",
                      default = True,
                      help = "Suppress column 3 (lines in both files)")
    parser.add_option("-u", action = "store_true", dest = "unsortedFiles",
                      default = False,
                      help = "Compatible with unsorted files")
    options, args = parser.parse_args(sys.argv[1:])

    # conditions for error messages
    if len(args) != 2:
        parser.error("Invalid number of arguments")

    if args[0] == "-" and args[1] == "-":
        parser.error("Both files cannot be read from stdin")
    
    try:
        onlyFile1 = bool(options.onlyFile1)
    except:
        parser.error("Invalid argument for -1".format(options.onlyFile1))
    try:
        onlyFile2 = bool(options.onlyFile2)
    except:
        parser.error("Invalid argument for -2".format(options.onlyFile2))
    try:
        bothFiles = bool(options.bothFiles)
    except:
        parser.error("Invalid argument for -3".format(options.bothFiles))
    try:
        unsortedFiles = bool(options.unsortedFiles)
    except:
        parser.error("Invalid argument for -u".format(options.unsortedFiles))

    try:
        generator = comm(args, onlyFile1, onlyFile2, bothFiles, unsortedFiles)
    except OSError as err:
        parser.error("OS Error: {0}".format(err))
    try:
        generator.compareFunction()
    except:
        parser.error("error")

if __name__ == "__main__":
    main()
