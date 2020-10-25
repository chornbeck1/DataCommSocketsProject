# DataCommSocketsProject
# Cole Hornbeck

Working iterative server so far. No concurrency support.

Server should be able to handle 1 client at a time with
no major problems. Default username and password is
	user
	pass
Additional logins can be added in serverFiles/usersList.txt
in the form of <username> <password> on a new line.


OVERVIEW: Run commands in this order
make all
./CreateDataFiles.sh
./TigerS
./bashScript.sh


PROGRAM COMMANDS	
tconnect <IP> <user> <password>
	Will connect to the server with the given IP.
	Must be using a valid username and password
	Has protection for invalid login
tget <fileName>
	Will attempt to download the given file from the server
	Will provide small amount of data to user on download
	Has protection for non-existant file
tput <fileName>
	Will attempt to upload the file to the server
	Will provide small amount of data to user on upload
	Has protection for non-existant file
exit
	Will close the client socket and end the client program
	Server will check the socket and close itself when it notices
		the client is no longer connected
	

RANDOM DATA FILES:
Test execution of this program requires ~200 1MB and above
files in /clientFiles and /serverFiles. Before running any
test programs, user must first run CreateDataFiles.sh to
populate these directories. The script can be modified to
change the file sizes. Final size will be blockSize*blockCount.
Default size is 1024*1024=1MiB.
Similarly, running RemoveDataFiles.sh will clean up and
delete all extra data files in these directories, to prevent
a ton of space from being used.


TEST
The script bashScript.sh was provided to use as a test bench
for testing the client/server. It hasn't been modified
and should work correctly straight off.
Before running, make sure you launch TigerS, preferably
in its own terminal window, using ./TigerS in this directory.


MAKEFILE
Two Make commands exist:
make all - builds and compiles TigerS.c and TigerC.c into
	TigerS and TigerC - MUST BE RUN PRIOR TO PROGRAM TEST
make clean - Removes all .o files and TigerS and TigerC to
	kepp directory clean