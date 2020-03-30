#/bin/bash
#setup mu2e
#source Offline/setup.sh
setup mu2egrid
setup mu2etools
setup gridexport
setup dhtools

DSCONF=reproc
MAINDIR=`pwd`
WFPROJ=CR_BKGDS
YEAR=reco_2025_lo
TAG=`date +"%y%m%d%H%M%S"`
OUTDIR=${DSCONF}_${YEAR}_${TAG}
OUTPNFS=/pnfs/mu2e/scratch/users/bbarton/workflow/${OUTDIR}/
SF=Pnfs_fcl/${OUTDIR}/fcllist.00
BN=`basename $SF | cut -d. -f 1`
JN="${BN}_${TAG}"
LN=Logs/submit_${JN}.log
CODE=/pnfs/mu2e/resilient/users/bbarton/gridexport/tmp.i09bLHxJNB/Code.tar.bz

mkdir $OUTPNFS
mkdir Pnfs_fcl/$OUTDIR

submit_job () {

    #Save enviroment variable
    printenv > Pnfs_fcl/$OUTDIR/vars.txt 2>&1

    RESOURCE="--disk=20GB --memory=2500MB" #Was 5000 #########################
    command="mu2eprodsys --clustername="${JN}" --fcllist=$SF --wfproject=$WFPROJ --dsconf=$DSCONF \
      --dsowner=bbarton --OS=SL7 ${RESOURCE} --expected-lifetime=24h --code=$CODE \
      --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC"
    echo "Submitting: " $command
    echo `$command` > $LN 2>&1
    RC=$?

    JID=`grep -oP "cluster\s+\K\w+" $LN`
    echo "RC=$RC JID=$JID"
    if [ $RC -eq 0 ]; then
	DD=`dirname $SF`
    fi

}


echo "Running stage: ${DSCONF}" 

if [ "$DSCONF" == "digi" ]; then
    INFCL=JobConfig/primary/CRY-offspill.fcl 
    INLIST=/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/SubmissionLists/Digis/inputList_digi_lo.txt
    MERGE=50 #Was 100 for original 2025 and 2030 digi processing jobs
elif [ "$DSCONF" == "reco" ]; then
    #INFCL=JobConfig/reco/CRY-cosmic-general-mix.fcl 
    INFCL=JobConfig/reco/CRY-cosmic-general-mix-loweredThresholds.fcl
    INLIST=/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/SubmissionLists/Reco/inputList_2030_reco_lo.txt
    MERGE=2
elif [ "$DSCONF" == "reproc" ]; then
    #SF=/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/SubmissionLists/digi2030_reproFCLs.fcllist
    SF=/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/SubmissionLists/Reco/inputList_reco_2025_lo_reproc2.txt
    MERGE=1
    submit_job
    exit 0
else
    echo "Unknown configuration"
    exit 0
fi

# Generate fcl files
mkdir Fcl_files/$OUTDIR
echo "Generating fcl files in: " Fcl_files/$OUTDIR

(cd Fcl_files/$OUTDIR && generate_fcl --desc=sim --dsowner=bbarton --dsconf=$DSCONF --inputs=${INLIST} --merge=${MERGE}  --embed ${INFCL})

if [ "$DSCONF" == "digi" ] || [ "$DSCONF" == "reco" ] ; then
    # Copy fcl files to pnfs area
    (for dir in Fcl_files/$OUTDIR/???; do ls $dir/*.fcl; done) | while read FF; do echo "Working on:" $FF; ifdh cp $FF $OUTPNFS; done
    #Make pnfs fcl lists
    (for dir in $OUTPNFS; do ls $dir/*.fcl; done) | while read FF; do echo $FF; done | split -l 10000 -d - Pnfs_fcl/$OUTDIR/fcllist.
    submit_job
fi
