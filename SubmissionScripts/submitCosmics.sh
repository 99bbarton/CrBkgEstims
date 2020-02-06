#/bin/bash
#setup mu2e
#source Offline/setup.sh
#setup mu2egrid
#setup mu2etools
#setup gridexport
#setup dhtools

DSCONF=digi
MAINDIR=`pwd`
WFPROJ=CR_BKGDS
YEAR=2025
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

    RESOURCE="--disk=20GB --memory=5000MB"
    command="mu2eprodsys --clustername="${JN}" --fcllist=$SF --wfproject=$WFPROJ --dsconf=$DSCONF \
      --dsowner=bbarton --OS=SL7 ${RESOURCE} --expected-lifetime=23h --code=$CODE \
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
    INFCL=/mu2e/app/users/bbarton/CrBkgEstims/Fcl/CRY-offspill.fcl 
    INLIST=/mu2e/app/users/bbarton/CrBkgEstims/SubmissionScripts/SubmissionLists/inputList_digi_both_test.txt
    MERGE=100
elif [ "$DSCONF" == "reco" ]; then
    INFCL=/mu2e/app/users/Offline/JobConfig/reco/CRY-cosmic-general-mix.fcl #st_testBatch.txt
    INLIST=/mu2e/app/users/bbarton/
    MERGE=5
else
    echo "Unknown configuration"
    exit 0
fi

# Generate fcl files
mkdir Fcl_files/$OUTDIR
echo "Generating fcl files in: " Fcl_files/$OUTDIR

(cd Fcl_files/$OUTDIR && generate_fcl --desc=sim --dsowner=oksuzian --dsconf=$DSCONF --inputs=${INLIST} --merge=${MERGE}  --embed ${MAINDIR}/${INFCL})

if [ "$DSCONF" == "digi" ] || [ "$DSCONF" == "reco" ] ; then
    # Copy fcl files to pnfs area
    (for dir in Fcl_files/$OUTDIR/???; do ls $dir/*.fcl; done) | while read FF; do echo "Working on:" $FF; ifdh cp $FF $OUTPNFS; done
    #Make pnfs fcl lists
    (for dir in $OUTPNFS; do ls $dir/*.fcl; done) | while read FF; do echo $FF; done | split -l 10000 -d - Pnfs_fcl/$OUTDIR/fcllist.
    submit_job
fi
