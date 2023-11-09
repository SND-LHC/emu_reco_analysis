#!/bin/bash

brickID=44
minimumsize=100000
for iplate in $(seq $2 $1)
 do
 counter=0
 for var in $(seq 0 323)
  do
   xbin=$((var / 18))
   ybin=$((var % 18))
   #starting from 1 to 19
   xname=$((xbin + 1))
   yname=$((ybin + 1))
   platefolder="$(printf "p%0*d" 3 $iplate)"
   file=$platefolder/$brickID.$iplate.$xname.$yname.firstlinkcp.root
   if [ -f "$file" ]
    then
    actualsize=$(wc -c <"$file")
    let counter++
    if [ $actualsize -lt $minimumsize ]; then
       echo $file is under $minimumsize bytes, it has $actualsize bytes $var
    fi
   else
    echo $var
   fi
  done
 echo $iplate total $counter
done
