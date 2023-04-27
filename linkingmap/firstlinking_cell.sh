#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

brickID=44

#finally doing cell by cell linking

var1=$3
xbin=$((var1 / 19))
ybin=$((var1 % 19))

#starting from 1 to 19, not 0 to 18
xname=$((xbin + 1))
yname=$((ybin + 1))

#temporary copy of link to raw data
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  cp -dv $platefolder/$brickID.$iplate.0.0.raw.root $platefolder/$brickID.$iplate.$xname.$yname.raw.root
 done 

makescanset -set=$brickID.0.$xname.$yname  -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 -reset

echo "Starting pre-linking"

makescanset -set=$brickID.0.$xname.$yname -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

#cp firstlink.rootrc link.rootrc #REMEMBER TO DO IT BEFORE LAUNCHING THIS CODE!
emlink -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin

cp b0000$brickID.0.$xname.$yname.link.ps plot_prelink/b0000$brickID.0.$xname.$yname.prelink_$1_$2.ps

#copying cp.root from first linking, to check shrinkage distributions later on
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  cp $platefolder/$brickID.$iplate.$xname.$yname.cp.root $platefolder/$brickID.$iplate.$xname.$yname.firstlinkcp.root
  rm $platefolder/$brickID.$iplate.$xname.$yname.cp.root
 done
