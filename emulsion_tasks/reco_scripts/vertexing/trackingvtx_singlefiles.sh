#!/bin/bash

brickID=21

#finally doing cell by cell tracking with realign
#edit track.rootrc before launching

var1=$3
xbin=$((var1 / 18))
ybin=$((var1 % 18))

#starting from 1 to 18, not 0 to 17
xname=$((xbin + 1))
yname=$((ybin + 1))


echo "Starting tracking without realign"

emtra -set=$brickID.0.$xname.$yname -new -v=2

mkdir trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}
mkdir trackfiles/parfiles/${1}_${2}/cell_${xname}_${yname}

mv -v b0000$brickID.0.$xname.$yname.trk.root trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root 

mv -v b0000$brickID.0.$xname.$yname.set.root trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root

cd trackfiles/rootfiles/${1}_${2}/cell_${xname}_${yname}/
ln -s b0000$brickID.0.$xname.$yname.trk.root linked_tracks.root
ln -s b0000$brickID.0.$xname.$yname.set.root b0000$brickID.0.0.0.set.root #to recover missing patterns

root -l -q /afs/cern.ch/work/s/snd2na/public/emu_reco_analysis/vertexing/vertexing.C\($brickID\)
