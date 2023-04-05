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

for iplate in $(seq $2 $1)
 do
  if [ $iplate -le 9 ]
   then
   cp -dv p00$iplate/$brickID.$iplate.0.0.raw.root p00$iplate/$brickID.$iplate.$xbin.$ybin.raw.root

  elif [ $iplate -le 99 ]
   then
   cp -dv p0$iplate/$brickID.$iplate.0.0.raw.root p0$iplate/$brickID.$iplate.$xbin.$ybin.raw.root

  else
   cp -dv p$iplate/$brickID.$iplate.0.0.raw.root p$iplate/$brickID.$iplate.$xbin.$ybin.raw.root
  fi
 done 

echo "Starting true linking"

makescanset -set=$brickID.0.$xbin.$ybin -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

#cp secondlink.rootrc link.rootrc
emlink -set=$brickID.0.$xbin.$ybin -new -v=2 -ix=$xbin -iy=$ybin

#rm temporary files, not needed anymore
rm b0000$brickID.0.$xbin.$ybin.set.root
for iplate in $(seq $2 $1)
 do
  if [ $iplate -le 9 ]
   then
   rm p00$iplate/$brickID.$iplate.$xbin.$ybin.raw.root
   #rm p0$iplate/$brickID.$iplate.$xbin.$ybin.par
  elif [ $iplate -le 99 ]
   then
   rm p0$iplate/$brickID.$iplate.$xbin.$ybin.raw.root
   #rm p0$iplate/$brickID.$iplate.$xbin.$ybin.par
  else
   rm p$iplate/$brickID.$iplate.$xbin.$ybin.raw.root
   #rm p$iplate/$brickID.$iplate.$xbin.$ybin.par
  fi
 done

cp b0000$brickID.0.$xbin.$ybin.link.ps plot_link/b0000$brickID.0.$xbin.$ybin.link_$1_$2.ps
rm b0000$brickID.0.$xbin.$ybin.link.ps
