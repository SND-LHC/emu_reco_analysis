#!/bin/bash

BRICKID=51
NWALL=5
NBRICK=1
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RECODATAPATH=/eos/user/o/onur/emu_reco/RUN3/$BRICKFOLDER

# Basic if statemen
for iplate in $(seq $2 $1)
  do
  PLATEFOLDER="$(printf "p%03d" $(( 10#$iplate )))"
  ln -s -f $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0_merged.cp.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.cp.root
  echo link created $BRICKID.$iplate.0.0.cp.root
done
