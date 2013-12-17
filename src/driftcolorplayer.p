reset
load "config.p"
set terminal png truecolor size 640*4,640*4
set output "plot\\multidrift2.png"

set size ratio 1
set autoscale fix

set key above autotitle columnhead
set key font ",20"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02



set cbrange [0:20]
set yrange [-400:300]
set xrange [-400:300]

set style fill transparent solid 0.5 noborder

#set palette defined (0 "white", 0.05 "blue", 0.1 "green", 0.15 "yellow", 0.2 "black", 0.3 "red", 0.4 "black")

set palette defined (0 "white", 0.2 "green",  0.6 "red", 1 "black")

#only the track image (use this to get x, y ranges)
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\

#auch ganz cool aber lines können keine transparenz
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'log12.txt' using 1:2:3 with lines palette


set multiplot layout 4, 4 title "driftness"

do for [i=0:15] {
#set output "plot\\playerdriftklein".i.".png"
#set colorbox vertical user origin 0.1, 0.1 size 0.02, 0.15
set cblabel "drift angle in °"
plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle,\
	'log\\playerlogs\\log'.i.'.txt' using 1:2:6:($6 < 5 ? 1/0 : $6) with points palette pt 6 title columnheader(1)

}
unset multiplot