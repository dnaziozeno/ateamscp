PREFIX=/usr/local
CONFDIR=${PREFIX}/etc
BINDIR=${PREFIX}/bin

ARCH = linux
# BSD systems: ARCH=bsd
# Solaris systems: ARCH=sun

CC = mpicc
CFLAGS = -g -Wall -Wno-write-strings -D$(ARCH) -DMPICH_IGNORE_CXX_SEEK

WXFLAGS = `wx-config --cxxflags`
WXLIBS = `wx-config --libs`
WXPLOTLIB = -lplot

ECHO = echo
REMOVE = rm -rf

module = 

ifneq ($(module), )
all: clear-dep begin project-all end
else
all: begin project-main end
endif
	
clear-dep:
	@$(ECHO) ""
	@$(ECHO) "Removing files:"
	@$(ECHO) "================================"
	
	$(REMOVE) ./bin/ateam-scp
	
begin:
	@$(ECHO) ""
	@$(ECHO) "Building Project Ateam-SCP:"
	@$(ECHO) "================================"
	
	
project-all: ./bin/ateam-scp
project-main: ./obj/MainFrame.o ./obj/IOParam.o ./obj/Graph.o ./obj/AteamScp.o
	
	
./bin/ateam-scp: ./modules/$(module)/obj/AteamParam.o ./modules/$(module)/obj/InitMemories.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/Graph.o ./obj/AteamScp.o
	$(CC) $(CFLAGS) ./modules/$(module)/obj/AteamParam.o ./modules/$(module)/obj/InitMemories.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/Graph.o ./obj/AteamScp.o $(WXLIBS) $(WXPLOTLIB) -o ./bin/ateam-scp -lwxchart
	
./obj/Graph.o: ./src/Graph.cpp ./src/Graph.h
	$(CC) $(CFLAGS) ./src/Graph.cpp $(WXFLAGS) -c -o ./obj/Graph.o
	
./modules/$(module)/obj/AteamParam.o: ./modules/$(module)/src/AteamParam.cpp ./modules/$(module)/src/AteamParam.h
	$(CC) $(CFLAGS) ./modules/$(module)/src/AteamParam.cpp $(WXFLAGS) -c -o ./modules/$(module)/obj/AteamParam.o
	
./modules/$(module)/obj/InitMemories.o: ./modules/$(module)/src/InitMemories.cpp ./modules/$(module)/src/InitMemories.h
	$(CC) $(CFLAGS) ./modules/$(module)/src/InitMemories.cpp $(WXFLAGS) -c -o ./modules/$(module)/obj/InitMemories.o
	
./obj/MainFrame.o: ./src/MainFrame.cpp ./src/MainFrame.h
	$(CC) $(CFLAGS) ./src/MainFrame.cpp $(WXFLAGS) -c -o ./obj/MainFrame.o
	
./obj/IOParam.o: ./src/IOParam.cpp ./src/IOParam.h
	$(CC) $(CFLAGS) ./src/IOParam.cpp $(WXFLAGS) -c -o ./obj/IOParam.o
	
./obj/AteamScp.o: ./src/AteamScp.cpp
	$(CC) $(CFLAGS) ./src/AteamScp.cpp $(WXFLAGS) -c -o ./obj/AteamScp.o
	
	
end:
	@$(ECHO) ""
	@$(ECHO) "Ready!"
	@$(ECHO) ""
	
	
clear:
	@$(ECHO) ""
	@$(ECHO) "Removing files:"
	@$(ECHO) "================================"
	
ifneq ($(module), )
	$(REMOVE) ./modules/$(module)/obj/AteamParam.o
	$(REMOVE) ./modules/$(module)/obj/InitMemories.o
endif
	
	$(REMOVE) ./obj/Graph.o
	$(REMOVE) ./obj/MainFrame.o
	$(REMOVE) ./obj/IOParam.o
	$(REMOVE) ./obj/AteamScp.o
	$(REMOVE) ./bin/ateam-scp
	
	@$(ECHO) ""
	@$(ECHO) "Ready!"
	@$(ECHO) ""
	
	
install:
	
	
	
uninstall:
	
	
	
help:
	@$(ECHO) ""
	@$(ECHO) "Ateam-SCP Make Help"
	@$(ECHO) "================================"
	@$(ECHO) "make all"
	@$(ECHO) "make clean"
	@$(ECHO) "make install"
	@$(ECHO) "make unistall"
	@$(ECHO) ""
	
