#!/bin/bash
#create symbolic link to merged couples file
#usage: source create_mergedcoupleslink_miccern.sh brickID platelast platefirst
#example: source create_mergedcoupleslink_miccern.sh 44 3 2

BRICKID=$1
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/$BRICKFOLDER/

for iplate in $(seq $3 $2)
 
 do
 PLATEFOLDER="$(printf "p%0*d" 3 $iplate)"
 ln -s $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0_merged.cp.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.cp.root

 done
