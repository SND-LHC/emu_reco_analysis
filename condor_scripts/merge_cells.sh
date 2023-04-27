#!/bin/bash

ProcId=$2
PLATENUMBER=$3

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/user/s/snd2cern/public/SNDBUILD/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Jan22/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest-master-release`
source /afs/cern.ch/user/s/snd2cern/public/fedra/setup_new.sh	

echo  "merge cells for plate ",$PLATENUMBER

root -l /afs/cern.ch/work/s/snd2cern/public/emu_reco_analysis/linkingmap/merge_couplestrees.C\($PLATENUMBER\)

