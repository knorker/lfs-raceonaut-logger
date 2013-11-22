reset

filename = "log\\playerlogs\\log".i.".txt"
plotfile = "plot\\plot".i.".png"

set terminal png truecolor size 2560,2560
set output plotfile

set size ratio 1
set autoscale fix
#set key inside left autotitle columnhead
set key above autotitle columnhead
set key font ",20"
set cblabel "speed in km/h"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02

set colorbox vertical user origin 0.21, 0.75 size 0.02, 0.2

#CHANGE RANGES HERE

set yrange [-200:500]
set xrange [-450:250]
set cbrange [0:200]

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
#CHANGE TRACK HERE
plot 'AS.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle ,\
     filename using 1:2:3 with lines palette linewidth 2 title columnheader(1)
#     filename using 1:2:3 with points palette pt 7 ps 0.5 title columnheader(1)
#	 'layout.txt' using 1:2 with points lc 5 pt 9 ps 0.7
set output
i=i+1
if (i <= n) reread