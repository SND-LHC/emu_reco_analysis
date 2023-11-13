#!/bin/bash

ProcId=$2
PLATENUMBER=$3
BRICKID=23

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/work/s/snd2na/public/SNDBuild/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest`
source /afs/cern.ch/work/s/snd2na/public/fedra/setup_new.sh	

echo  "merge cells for plate ",$PLATENUMBER

root -l /eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN3/b000044/merge_coupleshistos.C\($BRICKID,$PLATENUMBER\)

