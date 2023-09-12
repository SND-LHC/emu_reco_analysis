#!/bin/sh

BRICKPATH="/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044"

read -p "Cleaning files on "$BRICKPATH" continue with y: " -n 1 -r
echo " "
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "launching rm -I $BRICKPATH/p*/*.*.*.*.raw.root"
    rm -I $BRICKPATH/p*/*.*.*.*.raw.root
    echo "launching rm -I $BRICKPATH/p*/*.par"
    rm -I $BRICKPATH/p*/*.par
    #to be fixed: echo "launching rm -I $BRICKPATH/AFF/44.*.{1..19}.{1..19}.*.aff.par"
    #rm -I $BRICKPATH/AFF/44.*.{1..19}.{1..19}.*.aff.par
fi

