reset
set size ratio 1
# set autoscale fix
set key above autotitle columnhead

set yrange [-1300:700]
set xrange [-1000:1000]
#set cbrange [0:200]

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
plot 'AS.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
     'collisionlog.txt' using 1:2 pt 6 ps 0.5 title columnheader(1)