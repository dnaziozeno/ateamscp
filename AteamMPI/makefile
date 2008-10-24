CC = mpicc
CFLAGS = -g
CLIBS = -lm -lpthread

all: main

clean:  
	rm *.o
	rm initMD
	rm serverMD
	rm ag_3opt
	rm stopATEAM
	rm stopSERVER
	rm stopFINAL
	rm initMP 
	rm serverMP 
	rm ag_consen 
	rm ag_adjust 
	rm ag_search   
	rm ag_subgr
	rm ag_beasley
	rm ag_primal
	
main: initMD serverMD ag_3opt stopATEAM stopSERVER stopFINAL initMP serverMP ag_consen ag_adjust ag_search ag_subgr ag_beasley ag_dual ag_primal dumpMEM

initMD: grdual.o agentrot.o readsc.o readparam.o reduction.o initMD.c
	$(CC) $(CFLAGS) grdual.o agentrot.o readsc.o readparam.o reduction.o initMD.c -o initMD

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


serverMD: readparam.o dualrot.o serverMD.c
	$(CC) $(CFLAGS) readparam.o dualrot.o serverMD.c -o serverMD $(CLIBS)

dualrot.o:
	$(CC) $(CFLAGS) -c dualrot.c

ag_3opt: agentrot.o threeopt.o readparam.o reduction.o readsc.o ag_3opt.c
	$(CC) $(CFLAGS) agentrot.o threeopt.o readparam.o reduction.o readsc.o ag_3opt.c -o ag_3opt

threeopt.o: 
	$(CC) $(CFLAGS) -c threeopt.c

stopATEAM: stopATEAM.c
	$(CC) $(CFLAGS) stopATEAM.c -o stopATEAM

stopSERVER: stopSERVER.c
	$(CC) $(CFLAGS) stopSERVER.c -o stopSERVER
	
stopFINAL: stopFINAL.c
	$(CC) $(CFLAGS) stopFINAL.c -o stopFINAL	

grprimal: grprimal.c
	$(CC) $(CFLAGS) -c grprimal.c
	
frgreedy: frgreedy.c	
	$(CC) $(CFLAGS) -c frgreedy.c

initMP: frgreedy.o grprimal.o agentrot.o readsc.o readparam.o reduction.o initMP.c
	$(CC) $(CFLAGS) frgreedy.o grprimal.o agentrot.o readsc.o readparam.o reduction.o initMP.c -o initMP $(CLIBS)

primalrot: primalrot.c
	$(CC) $(CFLAGS) -c primalrot.c

serverMP: readparam.o primalrot.o serverMP.c
	$(CC) $(CFLAGS) readparam.o primalrot.o serverMP.c -o serverMP $(CLIBS)
	
ag_consen: agentrot.o grprimal.o frgreedy.o readparam.o reduction.o readsc.o ag_consen.c
	$(CC) $(CFLAGS) agentrot.o grprimal.o frgreedy.o readparam.o reduction.o readsc.o ag_consen.c -o ag_consen $(CLIBS)

adjust: adjust.c
	$(CC) $(CFLAGS) -c adjust.c

ag_adjust: agentrot.o adjust.o readparam.o reduction.o readsc.o ag_adjust.c
	$(CC) $(CFLAGS) agentrot.o adjust.o readparam.o reduction.o readsc.o ag_adjust.c -o ag_adjust $(CLIBS)

lsearch: lsearch.c
	$(CC) $(CFLAGS) -c lsearch.c

frsearch: frsearch.c
	$(CC) $(CFLAGS) -c frsearch.c

ag_search: agentrot.o frsearch.o lsearch.o readparam.o reduction.o readsc.o ag_search.c
	$(CC) $(CFLAGS) agentrot.o frsearch.o lsearch.o readparam.o reduction.o readsc.o ag_search.c -o ag_search $(CLIBS)
	
subgrad: subgrad.c
	$(CC) $(CFLAGS) -c subgrad.c

ag_subgr: agentrot.o subgrad.o readparam.o reduction.o readsc.o ag_subgr.c
	$(CC) $(CFLAGS) agentrot.o subgrad.o readparam.o reduction.o readsc.o ag_subgr.c -o ag_subgr $(CLIBS)

beasley: beasley.c
	$(CC) $(CFLAGS) -c beasley.c

ag_beasley: agentrot.o beasley.o readparam.o reduction.o readsc.o ag_beasley.c
	$(CC) $(CFLAGS) agentrot.o beasley.o readparam.o reduction.o readsc.o ag_beasley.c -o ag_beasley $(CLIBS)

ag_dual: agentrot.o grdual.o readparam.o reduction.o readsc.o ag_dual.c
	$(CC) $(CFLAGS) agentrot.o grdual.o readparam.o reduction.o readsc.o ag_dual.c -o ag_dual $(CLIBS)

ag_primal: agentrot.o grprimal.o readparam.o reduction.o readsc.o ag_primal.c
	$(CC) $(CFLAGS) agentrot.o grprimal.o readparam.o reduction.o readsc.o ag_primal.c -o ag_primal $(CLIBS)

dumpMEM: dumpMEM.c
	$(CC) $(CFLAGS) dumpMEM.c -o dumpMEM
