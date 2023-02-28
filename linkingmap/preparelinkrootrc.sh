#!/bin/bash

#finally doing cell by cell linking

var1=$1
xbin=$((var1 / 19))
ybin=$((var1 % 19))

cp -av p015/331.15.0.0.raw.root p015/331.15.$xbin.$ybin.raw.root
python cellprinting.py $xbin $ybin

#makescanset -set=331.0.$xbin.$ybin -copyset -A=331.0.0.0 #copying previous set
makescanset -set=331.0.$xbin.$ybin -dzbase=195 -dz=-1350 -from_plate=15 -to_plate=15 -reset #from scratch

echo "Start linking for cells "$xbin " and "$ybin
cat firstlink_noheadercut.rootrc linkheadercut$xbin\_$ybin.rootrc > link$xbin\_$ybin.rootrc
emlinkmap -set=331.0.$xbin.$ybin -new -v=2 -ix=$xbin -iy=$ybin
