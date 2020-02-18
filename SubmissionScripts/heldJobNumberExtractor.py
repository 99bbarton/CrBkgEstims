
import sys
from os import listdir

######## Extract job numbers of held files
fileName = input("Enter a filename: ")

inFile = open(fileName, "r")

lines = inFile.readlines()
numbers = []

for line in lines:
    fullID = line.split("@")[0]
    
    numbers.append(int(fullID.split(".")[1]))

#Make a list of the fcl files used in the job

genPath = "/pnfs/mu2e/scratch/users/bbarton/workflow/"

dirName = input("Input the directory (not path) of fcl files: ")

fcls = listdir(genPath + dirName)


#Make a fcllist of files that need to be reprocessed
outFileName = input("Enter a filename for the output fcllist: ")

outFile = open(outFileName,"w")

for num in numbers:
    outFile.write(genPath + dirName + fcls[num] + "\n")


outFile.close()



