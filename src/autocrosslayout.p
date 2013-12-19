reset
#load "config.p"



set lmargin 0
set rmargin 0
set tmargin 0
set bmargin 0
set bmargin 0

set terminal png truecolor size 2560,2560
set output "plot\\autocrosslayout2.png"

set size ratio 1
set autoscale fix

set key above autotitle columnhead
set key font ",20"



#plot "AU.jpg" binary filetype=jpg center=(0,0) with rgbimage notitle,\
#     'log\\autocrosslayout.txt' using 1:2:(($3!=2 ? 2.0:1/0)) with circles lc rgb "blue" fs transparent solid 0.5 noborder,\
#	 'log\\autocrosslayout.txt' using 1:2:(($3!=3 ? 2.0:1/0)) with circles lc rgb "red" fs transparent solid 0.5 noborder,\


set angles degrees
set style arrow 1 nohead back nofilled linetype 1 linewidth 0.25 linecolor rgb "black"
set style arrow 2 nohead back nofilled linetype 1 linewidth 0.25 linecolor rgb "yellow"
set style arrow 3 nohead back nofilled linetype 1 linewidth 0.1 linecolor rgb "grey"
set style arrow 4 nohead back nofilled linetype 1 linewidth 0.1 linecolor rgb "grey"

#background
#speedbumps part 1
#speedbumps part 2
#tires
#chalk
plot "AU.jpg" binary filetype=jpg center=(0,0) with rgbimage notitle, \
     'log\\autocrosslayout.txt' using ($4==3 ? $1:1/0):2:(-sin($3)*3):(cos($3)*3) with vectors arrowstyle 1,\
	 'log\\autocrosslayout.txt' using ($4==3 ? $1:1/0):2:(sin($3)*3):(-cos($3)*3) with vectors arrowstyle 2,\
	'log\\autocrosslayout.txt' using 1:2:($4==2?0.65:0) with circles lc rgb "white",\
	'log\\autocrosslayout.txt' using ($4==4 ? $1:1/0):2:(-sin($3)*3):(cos($3)*3) with vectors arrowstyle 3 ,\
	'log\\autocrosslayout.txt' using ($4==4 ? $1:1/0):2:(sin($3)*3):(-cos($3)*3) with vectors arrowstyle 4,\

#plot "AU.jpg" binary filetype=jpg center=(0,0) with rgbimage notitle, \
#	'log\\autocrosslayout.txt' using 1:2:($4==2?5.0:0) with circles lc rgb "green"
										 
	#(($4!=2 ? 2.0:1/0))
#($4==2?$5.0:1/0)	 
#	 'log\\autocrosslayout.txt' using 1:2:(($3!=3 ? 2.0:1/0)) with circles lc rgb "red" fs transparent solid 0.5 noborder,\

