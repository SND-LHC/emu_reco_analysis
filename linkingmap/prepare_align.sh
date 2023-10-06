#!/bin/bash

BRICKID=44
NWALL=4
NBRICK=4
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN3/$BRICKFOLDER

# Basic if statemen
for iplate in $(seq $2 $1)
  do
  PLATEFOLDER="$(printf "p%03d" $(( 10#$iplate )))"
  ln -s -f $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0_merged.cp.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.cp.root
  echo link creato per $BRICKID.$iplate.0.0.cp.root
done
