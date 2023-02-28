#!/bin/bash
#usage: source create_link_mic2.sh platelast platefirst 
# (es. source create_link_mic2.sh 3 2) 
BRICKID=31
RAWDATAPATH=/mnt/data/SND/mic2/RUN0_W3_B1
# Basic if statemen
for i in $(seq -w $2 $1)

  do

  if [ $i -le 9 ]
      
      then

      mkdir p00$i
      ln -s $RAWDATAPATH/P00$i/tracks.raw.root p00$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p00$i
      
  elif [ $i -le 99 ]

      then

      mkdir p0$i
      ln -s $RAWDATAPATH/P0$i/tracks.raw.root p0$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p0$i

  else
      
      mkdir p$i
      ln -s $RAWDATAPATH/P$i/tracks.raw.root p$i/$BRICKID.$i.0.0.raw.root
      echo link creato per la cartella p$i
      
  fi
  
done
