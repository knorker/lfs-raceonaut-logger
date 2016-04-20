Examples:

https://www.lfsforum.net/showthread.php?p=1828231#post1828231

https://www.lfsforum.net/showthread.php?p=1826202#post1826202

https://www.lfsforum.net/showthread.php?p=1825693#post1825693

![http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/raceonaut_logo22mka1.jpg](http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/raceonaut_logo22mka1.jpg)

# Introduction #

Written in a hurry, if wished will improve the text a bit later.
This explains how to log car positions,speed,... into text files and then use gnuplot to create graphics from that.


## Logging ##

1) Start the replay and pause it.

2) In timeline, click the button to jump to start of replay.

Now LFS should show a black screen and blinking "Pause" text.

3) Type into chat:

/insim 29999

4) Start LFS\_Logger.exe

It should now look something like this, the program listing all racers from the start grid:

![http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/how_to_start.png](http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/how_to_start.png)

5) Start playback of replay
The program should spam various messages that can be ignored.

![http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/in_progress.png](http://lfs-raceonaut-logger.googlecode.com/svn/wiki/images/in_progress.png)

The **are just to tell that the connection is still alive and the program is still working.**

Do not set replay speed too high.

6) In log\ and log\playerlogs\ folder the program should start creating .txt files.

7) When race has ended, close the program by clicking the [x](x.md) or CTRL+C

## Plotting ##
1) Now plot the data. For example with free software. gnuplot: http://www.gnuplot.info/

2) Commands:
cd 'c:\blub\bla\'

To change directory to the folder where log\ folder is.

3) load 'speedcolor.p'

Creates the graphic with all cars plotted into one image

4) Indiviual cars:

4.1) Type:

i=0

n=22 (or how many cars there are)

4.1b) NOTE: if you only want to plot one car, set i and n to same number, like:
i=5

n=5

load 'playerspeedcolor.p'

4.2) See gebabbel.txt to find out which plotXY.png belongs to which driver.
(for example plot12.png = Gutholz etc)


### Adjusting plot script for track ###
The plot scripts must be adjusted for each track:

1) The track image must be the correct one, for example KY.png for Kyoto etc.

The ranges must be set so that the track is centered etc.
For example for Kyoto National:

set yrange [-250:950]
set xrange [-850:600]

See file "gnuplot nlotes.txt" for ranges for some other tracks.

Other .p are various other plot files with some examples or hints, maybe.