#!/bin/bash

brickID=51

#finally doing cell by cell tracking with realign
#edit track.rootrc before launching

var1=$3
xbin=$((var1 / 19))
ybin=$((var1 % 19))

#starting from 1 to 19, not 0 to 18
xname=$((xbin + 1))
yname=$((ybin + 1))


echo "Starting tracking without realign"

makescanset -copyset -A=$brickID.0.0.0 -suff=cp.root -set=$brickID.0.$xname.$yname -from_plate=$1 -to_plate=$2 -v=2

emtra -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin

#mkdir -p ./trackfiles/57_1/rootfiles/cell_${xname}_${yname}
#mkdir -p ./trackfiles/57_1/parfiles/cell_${xname}_${yname}

mv -v b0000$brickID.0.$xname.$yname.trk.root trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root 

mv -v b0000$brickID.0.$xname.$yname.set.root trackfiles/${1}_${2}/parfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root

cd ./trackfiles/57_1/rootfiles/cell_${xname}_${yname}/
FILE=./linked_tracks.root
if [ ! -f "$FILE" ]; then
    ln -s /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root linked_tracks.root
fi

root -l -q /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/vertexing.C  
