#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the third alignment. Usage is: '
    echo ' '
    echo 'source thirdalign.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

cp local_align.rootrc align.rootrc

makescanset -set=21.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 

echo "Starting test align with very precise parameters"

emalign -set=21.0.0.0 -new -v=2

cp b000021.0.0.0.align.ps plot_local_align/b000021.0.0.0.localalign_$1_$2.ps


