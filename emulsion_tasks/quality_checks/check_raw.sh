#!/bin/bash
#source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
#source /afs/cern.ch/user/o/onur/SNDLHCSOFT/config.sh
#source /afs/cern.ch/user/o/onur/fedra/setup_new.sh

##echo "Set up SND environment"
##SNDLHCSOFT=/afs/cern.ch/user/o/onur/SNDLHCSOFT
##export ALIBUILD_WORK_DIR=$SNDLHCSOFT/sw
##source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
#eval `alienv load --no-refresh sndsw/latest`
#source /afs/cern.ch/user/o/onur/fedra/setup_new.sh


echo "Set up SND environment"
SNDBUILD_DIR=/afs/cern.ch/user/o/onur/SNDLHCSOFT_BASE/sw
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
eval `alienv load -w $SNDBUILD_DIR --no-refresh sndsw/latest`
source /afs/cern.ch/user/o/onur/fedra/setup_new.sh	


platefolder="$(printf "P%0*d" 3 $1)"
path=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND_mic2/RUN3/RUN3_W5_B1/$platefolder/tracks.raw.root
root -b -q -l $path '/afs/cern.ch/user/o/onur/emulsion_tasks/quality_checks/check_raw.C(1)'
root -b -q -l $path '/afs/cern.ch/user/o/onur/emulsion_tasks/quality_checks/thickness.C'

for plot in *.png
do 
    b=$(basename $plot .png)
    if [[ "$b" == *thickness* ]]
    then
        xrdcp $plot /eos/user/s/snd2cern/emu_reco_plots/RUN3/b000051/plots/thicknesses/"$b"_plate"$1".png
    else
        xrdcp $plot /eos/user/s/snd2cern/emu_reco_plots/RUN3/b000051/plots/raws/"$b"_plate"$1".png
    fi 
done
echo "Finished task"
