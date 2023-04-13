#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

if [[ $3 -le 9 ]] 
then

   brickID=0$3
   workdir=b00000$3
elif  [ $3 -gt 9 ] && [ $3 -le 99 ] 
then

   brickID=$3
   workdir=b0000$3
elif [[ $3 -gt 99 ]] 
then

   brickID=$3
   workdir=b000$3

fi

if [ ! -d "$workdir" ]
then 
   echo "The directory does not exist!"
   return 0 
fi

cd ./$workdir
eval 'echo we are in $(pwd)'

makescanset -set=$brickID.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 -reset

echo "Starting pre-linking"

cp firstlink.rootrc link.rootrc

makescanset -set=$brickID.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=$brickID.0.0.0 -new -v=2

cp $workdir.0.0.0.link.ps plot_prelink/$workdir.0.0.0.prelink_$1_$2.ps


#copying cp.root from first linking, to check shrinkage distributions later on
for iplate in $(eval echo {$1..$2}) 
 do

  if [ $iplate -le 9 ]
   then
   cp p00$iplate/$brickID.$iplate.0.0.cp.root p00$iplate/$brickID.$iplate.0.0.firstlinkcp.root
   echo "written p00$iplate/$brickID.$iplate.0.0.cp.root p00$iplate/$brickID.$iplate.0.0.firstlinkcp.root"

  elif [ $iplate -le 99 ]
   then 
   cp p0$iplate/$brickID.$iplate.0.0.cp.root p0$iplate/$brickID.$iplate.0.0.firstlinkcp.root
   echo " written p0$iplate/$brickID.$iplate.0.0.cp.root p0$iplate/$brickID.$iplate.0.0.firstlinkcp.root "

  else
   cp p$iplate/$brickID.$iplate.0.0.cp.root p$iplate/$brickID.$iplate.0.0.firstlinkcp.root
   echo " written p$iplate/$brickID.$iplate.0.0.cp.root p$iplate/$brickID.$iplate.0.0.firstlinkcp.root "
  fi
 done

echo "Starting true linking"

cp secondlink.rootrc link.rootrc

makescanset -set=$brickID.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=$brickID.0.0.0 -new -v=2

cp $workdir.0.0.0.link.ps plot_link/$workdir.0.0.0.link_$1_$2.ps
