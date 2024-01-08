#!/bin/bash

ProcId=$2
PLATENUMBER=$3

echo "Set up SND environment"
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
source /afs/cern.ch/user/o/onur/SNDLHCSOFT_BASE/config.sh
source /afs/cern.ch/user/o/onur/fedra/setup_new.sh	

echo  "merge cells for plate ",$PLATENUMBER

cd /eos/user/o/onur/playground/b000051

root -l -q merge_coupleshistos.C\($PLATENUMBER\)

