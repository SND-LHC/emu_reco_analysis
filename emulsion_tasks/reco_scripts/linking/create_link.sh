#!/bin/bash
#usage: source create_link_miccern.sh brickID platelast platefirst 
# (es. source create_link_miccern.sh 44 3 2) 

BRICKID=51
NWALL=(${BRICKID:0:1})
NBRICK=(${BRICKID:1:2})
BRICKFOLDER="$(printf "b%0*d" 6 $BRICKID)"
RAWDATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND_mic2/RUN3/RUN3_W$NWALL\_B$NBRICK
RECODATAPATH=/eos/user/o/onur/playground/$BRICKFOLDER
TEMP=$(pwd)

# Basic if statemen
for iplate in $(seq $2 $1)
  do
  RAWPLATEFOLDER="$(printf "P%03d" $(( 10#$iplate )))"
  PLATEFOLDER="$(printf "p%03d" $(( 10#$iplate )))"
  xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/$PLATEFOLDER
  ln -s $RAWDATAPATH/$RAWPLATEFOLDER/tracks.raw.root $RECODATAPATH/$PLATEFOLDER/$BRICKID.$iplate.0.0.raw.root
  echo created link $PLATEFOLDER to folder $RAWDATAPATH/$RAWPLATEFOLDER
done

xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/plot_link
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/plot_prelink
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/plot_first_align
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/plot_second_align
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/plot_local_align
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/goodcouples
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/trackfiles
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/trackfiles/rootfiles
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/trackfiles/rootfiles/4_1
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/trackfiles/parfiles
xrdfs root://eosuser.cern.ch mkdir $RECODATAPATH/trackfiles/parfiles/4_1
