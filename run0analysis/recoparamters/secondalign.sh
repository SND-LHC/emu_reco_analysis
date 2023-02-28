#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the second alignment. Usage is: '
    echo ' '
    echo 'source secondalign.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

cp second_align.rootrc align.rootrc

makescanset -set=31.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 

echo "Starting second align"

emalign -set=31.0.0.0 -new -v=2

cp b000031.0.0.0.align.ps plot_second_align/b000031.0.0.0.secondalign_$1_$2.ps


