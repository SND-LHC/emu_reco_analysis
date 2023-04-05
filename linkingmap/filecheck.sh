#!/bin/bash

brickID=44
iplate=1
minimumsize=10000000
counter=0
for var in $(seq 1 360)
 do
  xbin=$((var / 19))
  ybin=$((var % 19))
  file=p00$iplate/$brickID.$iplate.$xbin.$ybin.firstlinkcp.root
  #file=p00$iplate/$brickID.$iplate.$xbin.$ybin.cp.root
  if [ -f "$file" ]
   then
   actualsize=$(wc -c <"$file")
   let counter++
   if [ $actualsize -lt $minimumsize ]; then
      echo $file is under $minimumsize bytes $var
   fi
  else
   echo $file does not exists $var
  fi
 done
echo $counter

