#!/bin/bash
CELL=$1
fromplate=$2
toplate=$3

if [[ $# -eq 0 ]] ; then
    echo 'Script for performing the two required linking steps. Usage is: '
    echo ' '
    echo 'source linkingloop.sh nfrom_plate nto_plate'
    echo ' '
    echo 'just replace nfrom_plate and nto_plate with corresponding numbers'
    return 0
fi

echo "Set up SND environment"
source /cvmfs/sndlhc.cern.ch/SNDLHC-2023/Aug30/setUp.sh
source /afs/cern.ch/user/o/onur/SNDLHCSOFT_BASE/config.sh
source /afs/cern.ch/user/o/onur/fedra/setup_new.sh
echo "do first linking for cell "$CELL
echo "from plate " $fromplate
echo "to plate " $toplate

AFSHOME=/afs/cern.ch/user/o/onur
EOSPATH=/eos/user/o/onur/playground
brickID=51
BRICKFOLDER="$(printf "b%0*d" 6 $brickID)"
RECODATAPATH=$EOSPATH/$BRICKFOLDER
SENTINEL_FILE=sentinel_2
if [ ! -f $SENTINEL_FILE ]; then
  # Run this part of the script only once
  cp $AFSHOME/emulsion_tasks/reco_scripts/linking/secondlink.rootrc ./link.rootrc  
  touch $SENTINEL_FILE
fi
#finally doing cell by cell linking
var1=$3
xbin=$((var1 / 18))
ybin=$((var1 % 18))

#starting from 1 to 18, not 0 to 17
xname=$((xbin + 1))
yname=$((ybin + 1))

#temporary copy of link to raw data (need to do again)
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  cp -dv $RECODATAPATH/$platefolder/$brickID.$iplate.0.0.raw.root $RECODATAPATH/$platefolder/$brickID.$iplate.$xname.$yname.raw.root
 done 

echo "Starting true linking"

makescanset -set=$brickID.0.$xname.$yname -dzbase=175 -from_plate=$1 -to_plate=$2 -v=2 -o=$EOSPATH

emlink -set=$brickID.0.$xname.$yname -new -v=2 -ix=$xbin -iy=$ybin -o=$EOSPATH

#rm temporary files, not needed anymore
xrdfs root://eosuser.cern.ch/ rm $RECODATAPATH/b0000$brickID.0.$xname.$yname.set.root
for iplate in $(seq $2 $1)
 do
  platefolder="$(printf "p%0*d" 3 $iplate)"
  xrdfs root://eosuser.cern.ch/ rm $RECODATAPATH/$platefolder/$brickID.$iplate.$xname.$yname.raw.root
 done

xrdcp $RECODATAPATH/b0000$brickID.0.$xname.$yname.link.ps $RECODATAPATH/plot_link/b0000$brickID.0.$xname.$yname.link_$1_$2.ps
xrdfs root://eosuser.cern.ch/ rm $RECODATAPATH/b0000$brickID.0.$xname.$yname.link.ps
