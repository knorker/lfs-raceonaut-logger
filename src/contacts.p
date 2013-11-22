reset
set size ratio 1
# set autoscale fix
set key above autotitle columnhead

#set yrange [-800:750]
#set xrange [-800:750]
#set cbrange [0:200]

set yrange [-200:500]
set xrange [-450:250]


set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
plot 'AS.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
     'log\\scollisionlog.txt' using 1:2 pt 5 ps 0.5 title columnheader(1)