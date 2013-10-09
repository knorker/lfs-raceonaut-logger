Event Control
=============

Version: 0.43
Author: Cristóbal Marco (makakazo)

"Event Control" (formerly Qual Control) is a basic InSim application written in C/C++
using the CInsim library written by Cristóbal Marco. The CInsim library uses WinSock2
for the socket communication, so be sure to link to it.

This has been written highly focused on being a tutorial on programming with InSim
and understanding it.

PThreads-win32 was used for this project, so be sure to link to it when compiling,
and place the pthreads DLL in the working directory of the executable.PThreads-win32
is a library that provides a POSIX pthreads API for WIN32 environments. It is licensed
under the LGPL. You can find info and source @ http://sourceware.org/pthreads-win32/



ABOUT EVENT CONTROL:
===================

Event Control is a tool used to direct races. It is intended to be used by Race Directors
who don't have the admin password to a certain host, so they can use all necessary
commands to get the race going by being appointed as race directors by the host owners
(editing a simple text file).

Basically the Race Directors are able to:

 - Send preformatted Race Director's messages through the chat quickly.
 - Restart soft qualify sessions (telepit and spectating allowed).
 - Restart hardcore qualify sessions (telepit and spectating NOT allowed).
 - Declare the track OPEN: Everybody allowed in.
 - Declare the track CLOSED: Everybody to spectators, nobody allowed to join the track.
 - Restart the race.
 - End race and go to the lobby screen.
 - Automatically reverse the specified number of top grid positions when in lobby screen.
 - Send players to spectators.
 - Use common LFS host commands to configure the race and track (/cars, /track, /laps, etc.)
 
Also, host admins can use the command to completely exit the application, which is not
available to Race Directors. They can also turn Event Control OFF altogether so it
can stay resident not doing any work at all, just waiting to be turned ON again.



USAGE:
=====

An executable is available and ready to use. You can also compile it from the
sources. You must fill all three required fields in the config.cfg file:

/IP=(Host IP address)
/insim_port=(Host insim port)
/admin_password=(admin password)

You can appoint Race Directors who can use all commands except "!eexit". Just
fill the fields in config.cfg file with up to 50 lfs usernames.

You will need the file pthreadGC2.dll to be in the same directory as the executable.

A console application will launch and connect to the specified server. It will send
all currently connected players to spectators and then start its job.

To exit, an admin must type !eexit or !eex in the ingame chat. The rough way of
using Ctrl+C in the console application also works :)



CONTACT:
=======
If you find bugs, or just want to make any comments on the code or the application
itself, look for user "makakazo" at www.lfsforum.net.
