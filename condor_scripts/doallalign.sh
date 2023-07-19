#!/bin/bash

FROMPLATE=2
TOPLATE=1

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/user/s/snd2cern/public/SNDBUILD/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Jan22/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest-master-release`
source /afs/cern.ch/user/s/snd2cern/public/fedra/setup_new.sh	

echo  "go into reconstruction folder "
cd /eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/

echo "do alignment"

source firstalign.sh $FROMPLATE $TOPLATE

source secondalign.sh $FROMPLATE $TOPLATE

source localalign.sh $FROMPLATE $TOPLATE
