reset
unset multiplot
set terminal png truecolor  size 480*4,480*4
set output "plot\\startSpeed.png"

set multiplot layout 4, 4 title "Start reaction and acceleration"

set xrange [0:]
set yrange [0:250]


set key font ",15"

#green light at 20s = 20*4 = 80 messwerte. 90-68 = 22
green = 0
set tics out nomirror
set grid

set tics scale 0.2

set ytics offset 1,0
set y2tics offset -1,0

set xtics format " " 
set xtics 0,4,4*5
set xtics add ("green" green)
set xtics add ("1" green+4)
set xtics add ("2" green+8)
set xtics add ("3" green+12)
set xtics add ("4" green+16)
set xtics add ("5" green+20)
set xtics add ("6" green+24)
set xtics add ("7" green+28)

set ylabel "speed in km/h" offset 3
set xlabel "time in s"

set y2label "divergence in °" offset -3
set y2range [0:90]
set y2tics nomirror

set key left top
set key left top autotitle columnhead
#set title "fuck"

do for [i=0:11] {
set key center top
set key center top autotitle columnhead

plot  "log\\playerlogs\\log" .i. ".txt" using 3 every ::80::108 with lines lw 2, \
		"log\\playerlogs\\log" .i. ".txt" using 6 every ::80::108 axis x1y2  with lines lw 2 notitle,

}
#stats 'log\\output.txt'
#binwidth=10
#bin(x,width)=width*floor(x/width)
#plot 'log\\output.txt' using (bin($3,binwidth) + binwidth/2):(100.0/STATS_records) smooth freq with boxes notitle

unset multiplot