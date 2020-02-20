#Find fcl files corresponding to jobs which failed silently

import os

fileName = input("Enter the filename of the dig.*.art files which were successfully produced:\n")


completedJobsFile = open(fileName, "r")

numbers = []
for line in completedJobsFile.readlines():
    numbers.append(int(line.split("/")[10])) #Extract the directory number from the path


pathToFcl = input("Enter a path to the the directory containing fcl files:\n")

fclFiles = os.listdir(pathToFcl)

outDir = input("Enter an output directory name :")
editPath = "/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/Fcl_files/" + outDir + "/"
finalPath = "/pnfs/mu2e/scratch/users/bbarton/workflow/" + outDir + "/"

os.mkdir(editPath)
os.mkdir(finalPath)

print("Made edit directory: " + editPath)
print("Made final directory: " + finalPath)

appendLine = "physics.analyzers.digiCompressionCheck.checkTrackerDuplicateSteps : false"

print("Copying files from original location, appending duplicate error fix, and copying back to pnfs...")


for num in range(len(fclFiles)):
    if num not in numbers:
        os.system("cp " + pathToFcl + fclFiles[num] + " " + editPath) #Copy the desired fcl files to local dir for edit
    
        #Add the modification to the fcl file
        fclFile = open(editPath + fclFiles[num], "a")
        fclFile.write("\n ##Line added when reprocessing\n")
        fclFile.write(appendLine)
        fclFile.close()

        #Copy the file to pnfs
        os.system("cp " + editPath + fclFiles[num] + " " + finalPath)
