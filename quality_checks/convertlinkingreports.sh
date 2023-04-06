#!/bin/sh

########################
## using “convert” from ImageMagick to do ps convert into PNG
#########################

echo PS convert to PNG, please wait the process
for INP in *.ps
 do
 newname=`basename $INP .ps`
 if [ ! -f $newname-00.png ]; then
    echo " converting $INP to $newname-.png"
    convert $INP $newname-%02d.png
    echo " converted $INP to $newname-.png completely"
 fi
 done
echo " process ended, please check your graphical files"
