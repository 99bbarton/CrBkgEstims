import os


dirName = "digi_2030_rerunDuplicateFails"

os.chdir("Fcl_files/digis_2030_rerunDuplicateFails/")

fileList = os.listdir()

appendLine = "physics.analyzers.digiCompressionCheck.checkTrackerDuplicateSteps : false"
outDirPath = "/pnfs/mu2e/scratch/users/bbarton/workflow/" + dirName +"/"
os.system("mkdir " + outDirPath)

for fNum in range(len(fileList)):
    f = open(fileList[fNum], "a")
    f.write("\n#Line added during reprocessing\n")
    f.write(appendLine)
    f.close()

    os.system("cp " + fileList[fNum] + " " + outDirPath)
