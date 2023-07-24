#!/bin/bash

ProcId=$2
CELL=$3

fromplate=$4
toplate=$5

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/work/s/snd2na/public/SNDBuild/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Jan22/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest`
source /afs/cern.ch/work/s/snd2na/public/fedra/setup_new.sh	

echo  "go into reconstruction folder "
cd /eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN1/b000022
echo "do first linking for cell "$CELL
echo "from plate " $fromplate
echo "to plate " $toplate

source secondlinking_cell.sh $fromplate $toplate $CELL


#to avoid crashes put these in sub
#requirements = Machine =!= LastRemoteHost
#on_exit_remove          = (ExitBySignal == False) && ((ExitCode == 1) || (ExitCode == 0))
#max_retries             = 3

