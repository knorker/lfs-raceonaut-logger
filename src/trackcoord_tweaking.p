reset
#load "config.p"
trackimage="AS.jpg"
set key left top
set title "blab blub blib" 

set terminal png truecolor size 640,640
set output "plot\\AStweak_coords.png"
set tics scale 1
set xtics 50
set ytics 50

set size ratio 1
set autoscale fix
set key above autotitle columnhead

set style fill transparent solid 0.5 noborder

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")

#only the track image (use this to get x, y ranges)
plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle,\