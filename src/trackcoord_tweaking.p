reset
set key left top
set title "blab blub blib" 

set terminal png truecolor size 2560,2560
set output "plot\\output.png"

set size ratio 1
set autoscale fix
set key above autotitle columnhead

set yrange [-650:650]
set xrange [-800:500]
#set xrange [-700:-100]
set cbrange [0:300]

set style fill transparent solid 0.5 noborder

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")

#only the track image (use this to get x, y ranges)
plot 'SO.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\