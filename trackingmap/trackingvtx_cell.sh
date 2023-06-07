#!/bin/bash

brickID=21

#finally doing cell by cell tracking with realign
#edit track.rootrc before launching

var1=$3
xbin=$((var1 / 19))
ybin=$((var1 % 19))

#starting from 1 to 19, not 0 to 18
xname=$((xbin + 1))
yname=$((ybin + 1))


echo "Starting tracking without realign"

makescanset -set=$brickID.0.$xname.$yname -dzbase=195 -from_plate=$1 -to_plate=$2 -v=2

emtra -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin

mkdir trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}
mkdir trackfiles/parfiles/${1}_${2}/cell_${xname}_${yname}

mv -v b0000$brickID.0.$xname.$yname.trk.root trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root 

mv -v b0000$brickID.0.$xname.$yname.set.root trackfiles/parfiles/${1}_${2}/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root

cd trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}/
ln -s b0000$brickID.0.$xname.$yname.trk.root linked_tracks.root


root -l -q /afs/cern.ch/work/s/snd2cern/public/emu_reco_analysis/vertexing/vertexing.C  
