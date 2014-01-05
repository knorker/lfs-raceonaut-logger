reset
load "config.p"

filename = "log\\playerlogs\\log".i.".txt"
plotfile = "plot\\plotBestLap".i.".png"
set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set bmargin 0

#plotsize = 200
#set terminal png truecolor size plotsize,plotsize  *((yrangeMax-yrangeMin)/(xrangeMax-xrangeMin))

set terminal png truecolor size 2560,2560
set output plotfile

#set size ratio 1
#set autoscale fix
#set key inside left autotitle columnhead
set key above autotitle columnhead
#set key font ",20"
set key font ",15"
set key textcolor rgb "white"
set cblabel "speed in km/h"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02

filename2 = "log\\playerlogs\\log11.txt"

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 0.95 "green", 1 "#00FFFF")
set format z "%t"

plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle ,\
     filename using 1:2:($5 != 38 ? 1/0 : $3) with linespoints palette linewidth 1 pt 6 ps 1 title columnheader(1) , \
#	 filename2 using 1:2:($5 != 31 ? 1/0 : $3) with linespoints palette linewidth 1 pt 4 ps 1 title columnheader(1)
	 filename using 1:2:(0.5):floor($5 != 38 ? 1/0 : $3) every 4 with labels
	 
#     filename using 1:2:3 with points palette pt 7 ps 0.5 title columnheader(1)
#	 'layout.txt' using 1:2 with points lc 5 pt 9 ps 0.7

#set output
#i=i+1
#if (i <= n) reread