#!/bin/bash

echo "Set up SND environment (needed for my FEDRA)"
SNDBUILD_DIR=/afs/cern.ch/work/a/aiulian/public/SNDLHCBuild/sw/
source /cvmfs/sndlhc.cern.ch/SNDLHC-2022/July14/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest-master-release`

echo "Set up FEDRA"
source /afs/cern.ch/work/a/aiulian/public/fedra/setup_new.sh

echo  "go into correction folder "
cd /afs/cern.ch/work/a/aiulian/public/cormtx_Giulia/bot1
echo "do matrix correction"
viewdist -f=clust.raw.root -add=/eos/user/g/gpaggi/SND/cormtx/bot/correction_matrix.txt

