#Script to locate missing output files from a job, identify which fcl they corresponded to, and build a list of those fcls

import os

inFilename = input("Enter the name of the file containg a list of output files: ")
inFile = open(inFilename, "r")
lines = inFile.readlines()
inFile.close()

goodNums = []


for line in lines:
    dirNum = line.split("/")[-2] #The number of the directory is the second to last item
    goodNums.append(int(dirNum))
    
#print(len(goodNums))

fclDir = input("Enter the directory containing the corresponding fcl files: ")
path = "/pnfs/mu2e/scratch/users/bbarton/workflow/" + fclDir + "/"
fclFiles = os.listdir(path)

#print(len(fclFiles))

outFilename = input("Enter a name for the output file: ")
outFile = open(outFilename, "w")

for fclNum in range(len(fclFiles)):
    if fclNum not in goodNums:
        outFile.write(path + fclFiles[fclNum] + "\n")

outFile.close()
        
        
        
