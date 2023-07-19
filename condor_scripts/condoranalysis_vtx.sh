#!/bin/bash

ProcId=$2
CELL=$3

fromplate=57
toplate=1

echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/work/f/falicant/public/SNDBUILD/sw/
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Jan22/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest-master-release`
source /afs/cern.ch/work/f/falicant/public/fedra/setup_new.sh

echo "starting vertices analysis for cell "$CELL
python /afs/cern.ch/work/f/falicant/public/RUN1/condor_submissions/brick44/23_06_28_vtxanalysis/vtx_cell_analysis.py --cell $CELL


#to avoid crashes put these in sub
#requirements = Machine =!= LastRemoteHost
#on_exit_remove          = (ExitBySignal == False) && ((ExitCode == 1) || (ExitCode == 0))
#max_retries             = 3
