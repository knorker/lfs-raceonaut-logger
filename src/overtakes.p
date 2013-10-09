reset
set size ratio 1
# set autoscale fix
set key above autotitle columnhead

#set yrange [-1300:700]
#set xrange [-1000:1000]
set cbrange [-9:9]



set palette defined (-9 "green", -8 "green", -7 "green", -6 "green", -5 "green", -4 "green", -3 "green", -2 "green", -1 "green", 0 "black", 1 "red", 2 "red", 3 "red", 4 "red", 5 "red", 6 "red", 7 "red", 8 "red", 9 "red") 
plot 'AS.jpg' binary filetype=jpg center=(0,0) with rgbimage notitle,\
    	'output.txt' using 1:2:4:($4 == 0 ? 1/0 : $4) with points palette pt 7 ps 0.5 title columnheader(1)
	
	 

