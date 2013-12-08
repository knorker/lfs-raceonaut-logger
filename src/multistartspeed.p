reset
unset multiplot
set terminal png truecolor  size 480*5,480*5
set output "plot\\startSpeed.png"

set multiplot layout 5, 5 title "Start reaction and acceleration"

set xrange [0:]
set yrange [0:250]


set key font ",15"

#green light at 24s = 24*4 = 96 messwerte. 96-88 = 8

set tics out nomirror
set grid

set tics scale 0.2

set ytics offset 1,0

set xtics format " " 
set xtics 0,4,60
set xtics add ("green" 8)
set xtics add ("1" 8+4)
set xtics add ("2" 8+8)
set xtics add ("3" 8+12)
set xtics add ("4" 8+16)
set xtics add ("5" 8+20)
set xtics add ("6" 8+24)
set xtics add ("7" 8+28)

set ylabel "speed in km/h" offset 3
set xlabel "time in s"
#count and plot


set key left top
set key left top autotitle columnhead
#set title "fuck"

do for [i=0:23] {
set key center top
set key center top autotitle columnhead

plot  "log\\playerlogs\\log" .i. ".txt" using 3 every ::88::140 with lines lw 2

}
#stats 'log\\output.txt'
#binwidth=10
#bin(x,width)=width*floor(x/width)
#plot 'log\\output.txt' using (bin($3,binwidth) + binwidth/2):(100.0/STATS_records) smooth freq with boxes notitle

unset multiplot