#!/bin/bash
#usage: source create_link_miccern.sh brickID platelast platefirst 
# (es. source create_link_miccern.sh 44 3 2) 

BRICKID=44
NWALL=(${BRICKID:0:1})
NBRICK=(${BRICKID:1:2})
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RAWDATAPATH=/eos/experiment/sndlhc/emulsionData/2022/Napoli/SND/mic3/RUN3_W$NWALL\_B$NBRICK
RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/emureco_Napoli/RUN3/$BRICKFOLDER/

# Basic if statemen
for iplate in $(seq $2 $1)
  do

  RAWPLATEFOLDER="$(printf "P%02d" $(( 10#$iplate )))"
  PLATEFOLDER="$(printf "p%03d" $(( 10#$iplate )))"

  mkdir $RECODATAPATH/$PLATEFOLDER
  ln -s $RAWDATAPATH/$RAWPLATEFOLDER/tracks.raw.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.raw.root
  echo created link of $PLATEFOLDER to folder $RAWDATAPATH/$RAWPLATEFOLDER
  
done
