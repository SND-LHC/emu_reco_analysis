#!/bin/bash
#usage: source create_link_miccern.sh brickID platelast platefirst 
# (es. source create_link_miccern.sh 44 3 2) 

BRICKID=$1
NWALL=(${BRICKID:0:1})
NBRICK=(${BRICKID:1:2})
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RAWDATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN1/RUN1_W$NWALL\_B$NBRICK
RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/$BRICKFOLDER/

# Basic if statemen
for iplate in $(seq -w $3 $2)

  do

  RAWPLATEFOLDER="$(printf "P%0*d" 3 $iplate)"
  PLATEFOLDER="$(printf "p%0*d" 3 $iplate)"

  mkdir $RECODATAPATH/$PLATEFOLDER
  ln -s $RAWDATAPATH/$RAWPLATEFOLDER/tracks.raw.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.raw.root
  echo link creato per la cartella $PLATEFOLDER alla cartella $RAWDATAPATH/$RAWPLATEFOLDER/
  
done
