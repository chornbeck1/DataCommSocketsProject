server=TigerS.c
client=TigerC.c
cc=gcc
cflags = -Wall -std=c11 -pedantic -ggdb
serverRun=TigerS
clientRun=TigerC

.PHONY: all
all: $(serverRun) $(clientRun)
clean: 
	rm *.o $(serverRun) $(clientRun)

$(serverRun):$(server)
	$(cc) $(cflags) $(server) -o $(serverRun)
$(clientRun):$(client)
	$(cc) $(cflags) $(client) -o $(clientRun)