reset
set terminal png truecolor size 480,480
n=70 #number of intervals
max=300 #max value
min=0 #min value
width=(max-min)/n #interval width
#function used to map a value to the intervals
hist(x,width)=width*floor(x/width)+width/2.0
set term png #output terminal and file
set output "plot\\speedHistogram.png"
set xrange [min:max]
set yrange [0:]
#to put an empty boundary around the
#data inside an autoscaled graph.
set offset graph 0.05,0.05,0.05,0.0
set xtics min,(max-min)/10,max
set boxwidth width*0.9
set style fill solid 0.5 #fillstyle
set tics out nomirror
set xlabel "speed in km/h"
set ylabel "Frequency"
#count and plot
plot "log\\output.txt" u (hist($3,width)):(1.0) smooth freq w boxes lc rgb"green" notitle

#stats 'log\\output.txt'
#binwidth=10
#bin(x,width)=width*floor(x/width)
#plot 'log\\output.txt' using (bin($3,binwidth) + binwidth/2):(100.0/STATS_records) smooth freq with boxes notitle