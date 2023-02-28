#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

makescanset -set=11.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2 -reset

echo "Starting pre-linking"

cp firstlink.rootrc link.rootrc

makescanset -set=11.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=11.0.0.0 -new -v=2

cp b000011.0.0.0.link.ps plot_prelink/b000011.0.0.0.prelink_$1_$2.ps

#copying cp.root from first linking, to check shrinkage distributions later on
for iplate in $(seq $2 $1)
 do
  cp p00$iplate/11.$iplate.0.0.cp.root p00$iplate/11.$iplate.0.0.firstlinkcp.root
 done

echo "Starting true linking"

cp secondlink.rootrc link.rootrc

makescanset -set=11.0.0.0 -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emlink -set=11.0.0.0 -new -v=2

cp b000011.0.0.0.link.ps plot_link/b000011.0.0.0.link_$1_$2.ps
