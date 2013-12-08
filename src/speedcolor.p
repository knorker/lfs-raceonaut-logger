reset
load "config.p"

set terminal png truecolor size 2560,2560
set output "plot\\output.png"

set size ratio 1
set autoscale fix

set key above autotitle columnhead
set key font ",20"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02

set cblabel "speed in km/h"

#set style fill transparent solid 0.5 noborder

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 0.95 "green", 1 "#00FFFF")

#only the track image (use this to get x, y ranges)
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\

#auch ganz cool aber lines können keine transparenz
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'log12.txt' using 1:2:3 with lines palette


#unsorted data as circles   (1)=radius fuer full track
plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle,\
     'log\\output.txt' using 1:2:(0.4):3 with circles fillstyle transparent solid 0.5 palette title columnheader(1)


#  the third value in (  ) (1.5) is the radius of the circles
#plot 'output.txt' using 1:2:(1.5):3 with circles fillstyle transparent solid 0.5 palette title columnheader(1)


#plot 'SO.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'output.txt' using 1:2:3 with points  palette pt 7 ps 0.5 title columnheader(1)