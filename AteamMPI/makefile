CC = mpicc
CFLAGS = -g
CLIBS = -lm -lpthread

ECHO = echo

all: project-main
	
project-main: begin main end
	
begin:
	@$(ECHO) ""
	@$(ECHO) "Building Project Ateam-MPI:"
	@$(ECHO) "================================"
	
	
main: ./bin/initMD ./bin/serverMD ./bin/initMP ./bin/ag_3opt ./bin/stopATEAM ./bin/stopSERVER ./bin/stopFINAL ./bin/serverMP ./bin/ag_consen ./bin/ag_adjust ./bin/ag_search ./bin/ag_subgr ./bin/ag_beasley ./bin/ag_dual ./bin/ag_primal ./bin/dumpMEM
	
./bin/initMD: ./obj/grdual.o ./obj/agentrot.o ./obj/readsc.o ./obj/readparam.o ./obj/reduction.o ./src/initMD.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/grdual.o ./obj/agentrot.o ./obj/readsc.o ./obj/readparam.o ./obj/reduction.o ./src/initMD.c -o ./bin/initMD
	
./obj/grdual.o: ./src/grdual.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/grdual.c -o ./obj/grdual.o
	
./obj/agentrot.o: ./src/grdual.c ./src/agentrot.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/agentrot.c -o ./obj/agentrot.o
	
./obj/readsc.o: ./src/grdual.c ./src/readsc.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/readsc.c -o ./obj/readsc.o
	
./obj/readparam.o: ./src/grdual.c ./src/readparam.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/readparam.c -o ./obj/readparam.o
	
./obj/reduction.o: ./src/grdual.c ./src/reduction.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/reduction.c -o ./obj/reduction.o
	
./bin/serverMD: ./obj/readparam.o ./obj/dualrot.o ./src/serverMD.c ./src/serverMD.h ./src/globais.h
	$(CC) $(CFLAGS) ./obj/readparam.o ./obj/dualrot.o ./src/serverMD.c -o ./bin/serverMD $(CLIBS)
	
./obj/dualrot.o: ./src/dualrot.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/dualrot.c -o ./obj/dualrot.o
	
./bin/ag_3opt: ./obj/agentrot.o ./obj/threeopt.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_3opt.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/threeopt.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_3opt.c -o ./bin/ag_3opt
	
./obj/threeopt.o: ./src/threeopt.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/threeopt.c -o ./obj/threeopt.o
	
./bin/stopATEAM: ./src/stopATEAM.c ./src/globais.h
	$(CC) $(CFLAGS) ./src/stopATEAM.c -o ./bin/stopATEAM
	
./bin/stopSERVER: ./src/stopSERVER.c ./src/globais.h
	$(CC) $(CFLAGS) ./src/stopSERVER.c -o ./bin/stopSERVER
	
./bin/stopFINAL: ./src/stopFINAL.c
	$(CC) $(CFLAGS) ./src/stopFINAL.c -o ./bin/stopFINAL
	
./obj/grprimal.o: ./src/grprimal.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/grprimal.c -o ./obj/grprimal.o
	
./obj/frgreedy.o: ./src/frgreedy.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/frgreedy.c -o ./obj/frgreedy.o
	
./bin/initMP: ./obj/frgreedy.o ./obj/grprimal.o ./obj/agentrot.o ./obj/readsc.o ./obj/readparam.o ./obj/reduction.o ./src/initMP.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/frgreedy.o ./obj/grprimal.o ./obj/agentrot.o ./obj/readsc.o ./obj/readparam.o ./obj/reduction.o ./src/initMP.c -o ./bin/initMP $(CLIBS)
	
./obj/primalrot.o: ./src/primalrot.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/primalrot.c -o ./obj/primalrot.o
	
./bin/serverMP: ./obj/readparam.o ./obj/primalrot.o ./src/serverMP.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/readparam.o ./obj/primalrot.o ./src/serverMP.c -o ./bin/serverMP $(CLIBS)
	
./bin/ag_consen: ./obj/agentrot.o ./obj/grprimal.o ./obj/frgreedy.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_consen.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/grprimal.o ./obj/frgreedy.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_consen.c -o ./bin/ag_consen $(CLIBS)
	
./obj/adjust.o: ./src/adjust.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/adjust.c -o ./obj/adjust.o
	
./bin/ag_adjust: ./obj/agentrot.o ./obj/adjust.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_adjust.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/adjust.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_adjust.c -o ./bin/ag_adjust $(CLIBS)
	
./obj/lsearch.o: ./src/lsearch.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/lsearch.c -o ./obj/lsearch.o
	
./obj/frsearch.o: ./src/frsearch.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/frsearch.c -o ./obj/frsearch.o
	
./bin/ag_search: ./obj/agentrot.o ./obj/frsearch.o ./obj/lsearch.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_search.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/frsearch.o ./obj/lsearch.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_search.c -o ./bin/ag_search $(CLIBS)
	
./obj/subgrad.o: ./src/subgrad.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/subgrad.c -o ./obj/subgrad.o
	
./bin/ag_subgr: ./obj/agentrot.o ./obj/subgrad.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_subgr.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/subgrad.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_subgr.c -o ./bin/ag_subgr $(CLIBS)
	
./obj/beasley.o: ./src/beasley.c ./src/globais.h
	$(CC) $(CFLAGS) -c ./src/beasley.c -o ./obj/beasley.o
	
./bin/ag_beasley: ./obj/agentrot.o ./obj/beasley.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_beasley.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/beasley.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_beasley.c -o ./bin/ag_beasley $(CLIBS)
	
./bin/ag_dual: ./obj/agentrot.o ./obj/grdual.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_dual.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/grdual.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_dual.c -o ./bin/ag_dual $(CLIBS)
	
./bin/ag_primal: ./obj/agentrot.o ./obj/grprimal.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_primal.c ./src/globais.h ./src/agentes.h
	$(CC) $(CFLAGS) ./obj/agentrot.o ./obj/grprimal.o ./obj/readparam.o ./obj/reduction.o ./obj/readsc.o ./src/ag_primal.c -o ./bin/ag_primal $(CLIBS)
	
./bin/dumpMEM: ./src/dumpMEM.c
	$(CC) $(CFLAGS) ./src/dumpMEM.c -o ./bin/dumpMEM
	
end:
	@$(ECHO) ""
	@$(ECHO) "Ready!"
	@$(ECHO) ""
	
	
clean:
	rm ./obj/*
	rm ./bin/*
	
