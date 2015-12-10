#!/bin/bash

URL=$1
VIDEO=${URL##*/}
HEIGHT=$(cut -d _ -f 2 <<< $VIDEO | sed "s/^.*x//")
WIDTH=$(cut -d _ -f 2 <<< $VIDEO | sed "s/x.*$//")
FPS=$(cut -d _ -f 3 <<< $VIDEO | sed "s/Hz//")
FORMAT=$(cut -d _ -f 4 <<< $VIDEO | tr [:upper:] [:lower:])
echo Video path: $URL
echo Video name: $VIDEO
echo Height: $HEIGHT
echo Width: $WIDTH
echo FPS: $FPS
echo Format: $FORMAT

mplayer -noborder -vo gl -nosound -loop 1 -demuxer rawvideo -rawvideo w=$WIDTH:h=$HEIGHT:format=$FORMAT:fps=$FPS $URL
#mplayer -nosound -fps 120 -loop 1 -demuxer rawvideo -rawvideo w=1920:h=1080:format=uyvy:size=4147200 Chevaux-90_1920x1080_120Hz_UYVY_8bits.yuv

