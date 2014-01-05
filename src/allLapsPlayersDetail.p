reset
load "config.p"
set terminal png truecolor size 640*4,640*3
set output "plot\\multiplotAllLaps.png"

set size ratio 1
set autoscale fix

set key above autotitle columnhead
set key font ",20"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02
unset colorbox


set cbrange [0:topspeed]
set yrange [-780:-630]
set xrange [-640:-490]

set tics out nomirror
set grid
set tics scale 0.2
set ytics offset 1,0
set y2tics offset -1,0

set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set bmargin 0

set style fill transparent solid 0.5 noborder

#order as on startgrid NOT ordered by fastet lap!

set multiplot layout 3, 4 title "all laps"

do for [i=0:11] {
#set colorbox vertical user origin 0.1, 0.1 size 0.02, 0.15
set cblabel "speed in km/h"
set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 0.95 "green", 1 "#00FFFF")
plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle,\
		"log\\playerlogs\\log" .i. ".txt" using 1:2:3 with lines palette linewidth 2 title columnheader(1)

}
unset multiplot