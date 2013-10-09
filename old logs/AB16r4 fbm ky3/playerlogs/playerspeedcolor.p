reset



filename = "log".i.".txt"
plotfile = "plot".i.".png"

set terminal png size 2560,2560
set output plotfile

set size ratio 1
# set autoscale fix
set key above autotitle columnhead



set yrange [-1000:1100]
set xrange [-1050:1050]


set cbrange [0:220]

set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
plot 'KY.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle ,\
     filename using 1:2:3 with points palette pt 7 ps 0.5 title columnheader(1) ,\
	 'layout.txt' using 1:2 with points lc 5 pt 9 ps 0.7
set output
i=i+1
if (i <= n) reread