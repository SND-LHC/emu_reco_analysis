#!/bin/bash
source /afs/cern.ch/user/o/onur/setUp.sh
source  /afs/cern.ch/user/o/onur/SNDLHC_BETA/config.sh
root -b -q $1 /afs/cern.ch/user/o/onur/emulsion_tasks/check_tr.C
xrdcp *.pdf /eos/user/o/onur/emuqua/
echo "Finished task"
