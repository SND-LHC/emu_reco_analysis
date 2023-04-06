#!/bin/bash

echo "Set up sndsw"
SNDBUILD_DIR=/afs/cern.ch/user/g/gpsndlhc/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2022/July14/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest`

echo "Set up FEDRA"
source /afs/cern.ch/user/g/gpsndlhc/fedra/setup_new.sh

echo  "go into reconstruction folder "
cd /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051

echo "do alignment"

source firstalign.sh 1 48

source secondalign.sh 1 48
