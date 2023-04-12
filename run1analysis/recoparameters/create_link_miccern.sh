#!/bin/bash
#usage: source create_link_mic2.sh platelast platefirst 
# (es. source create_link_mic2.sh 3 2) 
BRICKID=44
RAWDATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/SND/RUN1/RUN1_W4_B4
RECODATAPATH=/eos/experiment/sndlhc/emulsionData/2022/CERN/emu_reco/RUN1/b000044/
# Basic if statemen
for i in $(seq -w $2 $1)

  do

  if [ $i -le 9 ]
      
      then

      mkdir $RECODATAPATH/p00$i
      ln -s $RAWDATAPATH/P00$i/tracks.raw.root $RECODATAPATH/p00$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p00$i
      
  elif [ $i -le 99 ]

      then

      mkdir $RECODATAPATH/p0$i
      ln -s $RAWDATAPATH/P0$i/tracks.raw.root $RECODATAPATH/p0$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p0$i

  else
      
      mkdir $RECODATAPATH/p$i
      ln -s $RAWDATAPATH/P$i/tracks.raw.root $RECODATAPATH/p$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p$i
      
  fi
  
done
