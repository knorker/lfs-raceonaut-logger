reset

set terminal png truecolor size 2560,2560
set output "plot\\output.png"

set size ratio 1
# set autoscale fix
set key above autotitle columnhead

set yrange [-250:950]
set xrange [-850:600]
set cbrange [0:300]

set style fill transparent solid 0.5 noborder

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")

#only the track image (use this to get x, y ranges)
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\

#auch ganz cool aber lines können keine transparenz
#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'log12.txt' using 1:2:3 with lines palette


#unsorted data as circles
plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
     'log\\output.txt' using 1:2:(1):3 with circles fillstyle transparent solid 0.5 palette title columnheader(1)


#  the third value in (  ) (1.5) is the radius of the circles
#plot 'output.txt' using 1:2:(1.5):3 with circles fillstyle transparent solid 0.5 palette title columnheader(1)


#plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'output.txt' using 1:2:3 with points  palette pt 7 ps 0.5 title columnheader(1)