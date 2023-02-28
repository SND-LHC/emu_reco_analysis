#!/bin/bash

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/work/a/aiulian/public/SNDLHCBuild/sw/
source /cvmfs/sndlhc.cern.ch/SNDLHC-2022/July14/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest-master-release`
source /afs/cern.ch/work/a/aiulian/public/fedra/setup_new.sh

echo  "go into reconstruction folder "
cd /eos/experiment/sndlhc/emulsionData/2022/emureco_CERN/b000331
echo "do linking"
source linkingloop.sh 15 15

