#!/bin/sh

########################
## using “convert” from ImageMagick to do ps convert into PNG
#########################

echo PS convert to PNG, please wait the process
for INP in *.ps
 do
 newname=`basename $INP .ps`
 if [ ! -f $newname-00.png ]; then
  #convert -density 150 -geometry 100% $INP $newname%02d.png
  convert $INP -rotate 90 $newname-%02d.png
  echo ” convert $INP to $newname-.png completely”
 fi
 done
echo ” process ended, please check your graphical files”
