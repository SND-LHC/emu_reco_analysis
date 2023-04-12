#!/bin/bash

for i in {1..60}
do 
	dirname=p0$i

	if [[ $i -le 9 ]]
	then
		dirname=p00$i
	fi
	
	echo linking file in $dirname
	eval 'mkdir $dirname'
	cd $dirname
	ln -s /eos/experiment/sndlhc/emulsionData/2022/Bologna/RUN1_W5_B2/P$i/tracks.raw.root 52.$i.0.0.raw.root
	cd ..
done
