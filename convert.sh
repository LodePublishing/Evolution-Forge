#!/bin/sh
# A simple bash script to screen capture
#
# Supply two arguments, the window id and number of captures

let x=100

# loop until it has captured the number of captures requested
while [ "$x" -le "$1" ]
do
  convert -size 320x200  "shot$x.bmp" -resize 320x200 "sshot$x.bmp"
  let x+=1
done

