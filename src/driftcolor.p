reset

set terminal png truecolor size 2560,2560
set output "plot\\drift.png"

set size ratio 1
set autoscale fix

set key above autotitle columnhead
set key font ",20"

#at top
#set colorbox horizontal user origin 0.5, 0.94 size 0.4, 0.02

set colorbox vertical user origin 0.26, 0.60 size 0.02, 0.2

set cblabel "drift angle in °"


set yrange [-250:950]
set xrange [-850:600]
set cbrange [0:20]

#set style fill transparent solid 0.5 noborder

set palette defined (0 "white", 0.05 "blue", 0.1 "green", 0.2 "yellow", 1 "red")

#only the track image (use this to get x, y ranges)
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\

#auch ganz cool aber lines können keine transparenz
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'log12.txt' using 1:2:3 with lines palette


#unsorted data as circles   (1)=radius fuer full track
plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
	'log\\output.txt' using 1:2:6:($6 < 2 ? 1/0 : $6) with points palette pt 7 title columnheader(1)

