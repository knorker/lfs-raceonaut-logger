set size ratio 1
set yrange [-850:700]
set xrange [-550:1000]
set key autotitle columnhead
set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
# plot 'output.txt' using 1:2:3 with points palette pt 6 ps 0.1

set style fill transparent solid 0.8 noborder
plot 'output.txt' using 1:2:3 with points palette pt 6 ps 0.1