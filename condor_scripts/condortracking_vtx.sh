#!/bin/bash

ProcId=$2
CELL=$3

fromplate=$4
toplate=$5

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/user/s/snd2cern/public/SNDBUILD/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest`
source /afs/cern.ch/user/s/snd2cern/public/fedra/setup_new.sh

echo  "go into reconstruction folder "
cd /eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN1/b000021/
echo "starting tracking and vertexing for cell "$CELL
source trackingvtx_cell.sh $fromplate $toplate $CELL
echo "from plate " $fromplate
echo "to plate " $toplate



#to avoid crashes put these in sub
#requirements = Machine =!= LastRemoteHost
#on_exit_remove          = (ExitBySignal == False) && ((ExitCode == 1) || (ExitCode == 0))
#max_retries             = 3
