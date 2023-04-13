#!/bin/bash

cd ./b0000$3
eval 'echo $(pwd)'

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate brickID'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate brickID with corresponding numbers e.g. 1 3 51 for plate 1 to 3 of W5B1'
    return 0
fi

makescanset -set=$3.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 -reset

echo "Starting pre-linking"

cp firstlink.rootrc link.rootrc

makescanset -set=$3.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=$3.0.0.0 -new -v=2

cp b0000$3.0.0.0.link.ps plot_prelink/b0000$3.0.0.0.prelink_$1_$2.ps

#copying cp.root from first linking, to check shrinkage distributions later on
for iplate in $(eval echo {$1..$2}) 
 do

  if [ $iplate -le 9 ]
   then
   cp p00$iplate/$3.$iplate.0.0.cp.root p00$iplate/$3.$iplate.0.0.firstlinkcp.root
   echo "written p00$iplate/$3.$iplate.0.0.cp.root p00$iplate/$3.$iplate.0.0.firstlinkcp.root"

  elif [ $iplate -le 99 ]
   then 
   cp p0$iplate/$3.$iplate.0.0.cp.root p0$iplate/$3.$iplate.0.0.firstlinkcp.root
   echo " written p0$iplate/$3.$iplate.0.0.cp.root p0$iplate/$3.$iplate.0.0.firstlinkcp.root "
  else
   cp p$iplate/$3.$iplate.0.0.cp.root p$iplate/$3.$iplate.0.0.firstlinkcp.root
   echo " written p$iplate/$3.$iplate.0.0.cp.root p$iplate/$3.$iplate.0.0.firstlinkcp.root "
  fi
 done

echo "Starting true linking"

cp secondlink.rootrc link.rootrc

makescanset -set=$3.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=$3.0.0.0 -new -v=2

cp b0000$3.0.0.0.link.ps plot_link/b0000$3.0.0.0.link_$1_$2.ps
