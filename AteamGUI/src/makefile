CC = mpicc
CFLAGS = -g -DMPICH_IGNORE_CXX_SEEK
CLIBS = -lm -lpthread

all: main

clean:  
	rm *.o
	rm initMD
	rm serverMD
	rm ag_3opt
	rm stopATEAM
	rm stopServer
    
main: initMD serverMD stopATEAM stopSERVER stopFINAL

initMD: grdual.o agentrot.o readsc.o readparam.o reduction.o initMD.o
	

grdual.o: grdual.c
	$(CC) $(CFLAGS) -c grdual.c

agentrot.o: agentrot.c
	$(CC) $(CFLAGS) -c agentrot.c

readsc.o: readsc.c
	$(CC) $(CFLAGS) -c readsc.c

readparam.o: readparam.c
	$(CC) $(CFLAGS) -c readparam.c

reduction.o: reduction.c
	$(CC) $(CFLAGS) -c reduction.c

initMD.o: initMD.c
	$(CC) $(CFLAGS) -c initMD.c

serverMD: readparam.o dualrot.o serverMD.c
	$(CC) $(CFLAGS) readparam.o dualrot.o serverMD.c -o serverMD $(CLIBS)

dualrot.o:
	$(CC) $(CFLAGS) -c dualrot.c


stopATEAM: stopATEAM.c
	$(CC) $(CFLAGS) stopATEAM.c -o stopATEAM

stopSERVER: stopSERVER.c
	$(CC) $(CFLAGS) stopSERVER.c -o stopSERVER
	
stopFINAL: stopFINAL.c
	$(CC) $(CFLAGS) stopFINAL.c -o stopFINAL
