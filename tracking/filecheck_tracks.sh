#!/bin/bash


#iplate=$1
minimumsize=400000000

counter=0
for var in $(seq 0 59)
 do
  xbin=$((var / 6))
  ybin=$((var % 6))
  xname=$((xbin + 1))
  yname=$((ybin + 1))
  file=b000044.0.$xname.$yname.trk.root
  if [ -f "$file" ]
    then
   actualsize=$(wc -c <"$file")
   let counter++
   if [ $actualsize -lt $minimumsize ]; then
      echo $file is under $minimumsize bytes $var
   fi
  else
   #echo $file does not exists $var
   echo $var
  fi
 done
echo total $counter
