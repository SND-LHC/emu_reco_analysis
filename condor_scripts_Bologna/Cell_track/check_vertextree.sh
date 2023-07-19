#!/bin/bash

counter=0
for var1 in {0..360}
do
	xbin=$((var1 / 19))
	ybin=$((var1 % 19))

	#starting from 1 to 19, not 0 to 18
	xname=$((xbin + 1))
	yname=$((ybin + 1))

	if [ ! -f "/eos/experiment/sndlhc/emulsionData/2022/emureco_Bologna/RUN1/b000051/trackfiles/57_1/rootfiles/cell_${xname}_${yname}/vertextree.root" ]; then
	        echo "The vertexing is still missing in cell_${xname}_${yname}"
		((counter +=1))
	fi
done
echo "In total missing ${counter} cells"
unset counter 

