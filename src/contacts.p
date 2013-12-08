reset
load "config.p"
set terminal png truecolor size 640,640
set output "plot\\contacts.png"

set size ratio 1
# set autoscale fix
set key above autotitle columnhead

#set yrange [-800:750]
#set xrange [-800:750]
#set cbrange [0:200]


set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
plot trackimage binary filetype=jpg center=(0,0) with rgbimage notitle,\
     'log\\scollisionlog.txt' using 1:2 pt 5 ps 0.25 title columnheader(1)