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

#temporary copy of link to raw data

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
makescanset -set=$brickID.0.$xbin.$ybin  -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 -reset

echo "Starting pre-linking"

makescanset -set=$brickID.0.$xbin.$ybin -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

#cp firstlink.rootrc link.rootrc
emlink -set=$brickID.0.$xbin.$ybin -new -v=2 -ix=$xbin -iy=$ybin

cp b0000$brickID.0.$xbin.$ybin.link.ps plot_prelink/b0000$brickID.0.$xbin.$ybin.prelink_$1_$2.ps

#copying cp.root from first linking, to check shrinkage distributions later on
for iplate in $(seq $2 $1)
 do

  if [ $iplate -le 9 ]
   then
   cp p00$iplate/$brickID.$iplate.$xbin.$ybin.cp.root p00$iplate/$brickID.$iplate.$xbin.$ybin.firstlinkcp.root
   rm p00$iplate/$brickID.$iplate.$xbin.$ybin.cp.root

  elif [ $iplate -le 99 ]
   then 
   cp p0$iplate/$brickID.$iplate.$xbin.$ybin.cp.root p0$iplate/$brickID.$iplate.$xbin.$ybin.firstlinkcp.root
   rm p0$iplate/$brickID.$iplate.$xbin.$ybin.cp.root

  else
   cp p$iplate/$brickID.$iplate.$xbin.$ybin.cp.root p$iplate/$brickID.$iplate.$xbin.$ybin.firstlinkcp.root
   rm p$iplate/$brickID.$iplate.$xbin.$ybin.cp.root
  fi
 done
