#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

brickID=31


#finally doing cell by cell linking

var1=$3
xbin=$((var1 / 18))
ybin=$((var1 % 18))

#starting from 1 to 18, not 0 to 17
xname=$((xbin + 1))
yname=$((ybin + 1))

#temporary copy of link to raw data (need to do again)
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  cp -dv $platefolder/$brickID.$iplate.0.0.raw.root $platefolder/$brickID.$iplate.$xname.$yname.raw.root
 done 

echo "Starting true linking"

makescanset -set=$brickID.0.$xname.$yname -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

#cp secondlink.rootrc link.rootrc #REMEMBER TO DO IT BEFORE LAUNCHING THIS CODE!
emlink -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin

#rm temporary files, not needed anymore
rm b0000$brickID.0.$xname.$yname.set.root
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  rm $platefolder/$brickID.$iplate.$xname.$yname.raw.root
  rm $platefolder/$brickID.$iplate.$xname.$yname.par
 done

cp b0000$brickID.0.$xname.$yname.link.ps plot_link/b0000$brickID.0.$xname.$yname.link_$1_$2.ps
rm b0000$brickID.0.$xname.$yname.link.ps
