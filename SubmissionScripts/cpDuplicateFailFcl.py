

import os

fileName = input("Enter a filename: ")

inFile = open(fileName, "r")

dirName = input("Enter a name for the output directory: ")

pathToTempDir = "/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/Fcl_files/" + dirName + "/"

os.system("mkdir " + pathToTempDir)

for line in inFile.readlines():
    os.system("cp " + line[:-42] + "/" + line[-42:-1] + " " + pathToTempDir)

os.system("cd " + pathToTempDir)

fileList = os.listdir()

print(fileList)


#appendLine = "physics.analyzers.digiCompressionCheck.checkTrackerDuplicateSteps : false"
#outDirPath = "/pnfs/mu2e/scratch/users/bbarton/workflow/" + dirName +"/"
#for fNum in range(len(fileList)):
#    f = open(fileList[fNum], "a")
#    f.write(appendLine)
#    f.close()

#    os.system("cp " + fileList[fNum] + " " + outDirPath)



    
