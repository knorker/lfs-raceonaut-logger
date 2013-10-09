	set multiplot

	set lmargin at screen 0.1
	set rmargin at screen 0.98
	set tmargin at screen 0.98
	set bmargin at screen 0.2

	#set yrange [-1280:1280]
	#set xrange [-1280:1280]
	
set yrange [-850:700]
set xrange [-550:1000]

	
	set size ratio 1
	plot 'BL.jpg' binary filetype=jpg origin = (-1280,-1280) with rgbimage

	set origin 0, 0
	set size ratio 1


	#set yrange [-1280:1280]
	#set xrange [-1280:1280]
set yrange [-850:700]
set xrange [-550:1000]

	set key autotitle columnhead
	set palette defined (0 "black", 0.25 "blue", 0.5 "red", 0.75 "yellow", 1 "green")
	plot 'output.txt' using 1:2:3 with points palette pt 6 ps 0.1

	unset multiplot