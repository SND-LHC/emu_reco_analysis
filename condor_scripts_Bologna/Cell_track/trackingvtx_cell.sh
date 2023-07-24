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

echo "Studying cell ${xname}_${yname}"
if [ -f "trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/vertextree.root" ]; then 
	echo "The vertexing was already done"
	return 0
fi

if [ ! "linked_tracks.root" ]; then
	echo "Starting tracking without realign"

	makescanset -copyset -A=$brickID.0.0.0 -suff=cp.root -set=$brickID.0.$xname.$yname -from_plate=$1 -to_plate=$2 -v=2

	emtra -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin

	if [ ! -d "./trackfiles/57_1/rootfiles/cell_${xname}_${yname}" ]; then
	    mkdir -p ./trackfiles/57_1/rootfiles/cell_${xname}_${yname}
	fi

	if [ ! -d "./trackfiles/57_1/parfiles/cell_${xname}_${yname}" ]; then
	    mkdir -p ./trackfiles/57_1/parfiles/cell_${xname}_${yname}
	fi
	mv -v b0000$brickID.0.$xname.$yname.trk.root trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root 

	mv -v b0000$brickID.0.$xname.$yname.set.root trackfiles/${1}_${2}/parfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root

	cd ./trackfiles/57_1/rootfiles/cell_${xname}_${yname}/

	ln -s /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root linked_tracks.root

else
	echo "Linked_tracks.root is already in the folder"
	if [ ! -f "trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root" ]; then
 		echo "Copying .trk.root file"
		cp -v b0000$brickID.0.$xname.$yname.trk.root trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.trk.root
	fi
	
	if [ ! -f "trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root" ]; then
		echo "Copying .set.root file" 
		cp -v trackfiles/${1}_${2}/parfiles/cell_${xname}_${yname}/b0000$brickID.0.$xname.$yname.set.root trackfiles/${1}_${2}/rootfiles/cell_${xname}_${yname}/b0000$brickID.0.0.0.set.root
	fi
	
	cd ./trackfiles/57_1/rootfiles/cell_${xname}_${yname}/

fi

if [ ! -f "b0000$brickID.0.0.0.set.root" ]; then
	echo "no set file in this folder"
	return 0
fi
root -l -q /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/vertexing.C 

cd /eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/
