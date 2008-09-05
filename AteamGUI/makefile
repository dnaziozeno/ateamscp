PREFIX=/usr/local
CONFDIR=${PREFIX}/etc
BINDIR=${PREFIX}/bin

ARCH = linux
# BSD systems: ARCH=bsd
# Solaris systems: ARCH=sun

CC = g++
CFLAGS = -g -Wall -D$(ARCH)

WXFLAGS = `wx-config --cxxflags`
WXLIBS = `wx-config --libs`

ECHO = echo
REMOVE = rm -rf

MODULE = 

ifneq ($(MODULE), )
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
project-main: ./obj/MainFrame.o ./obj/IOParam.o ./obj/AteamScp.o
	
	
./bin/ateam-scp: ./plugins/$(MODULE)/obj/AteamParam.o ./plugins/$(MODULE)/obj/InitMemories.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/AteamScp.o
	$(CC) $(CFLAGS) ./plugins/$(MODULE)/obj/AteamParam.o ./plugins/$(MODULE)/obj/InitMemories.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/AteamScp.o $(WXLIBS) -o ./bin/ateam-scp
	
./plugins/$(MODULE)/obj/AteamParam.o: ./plugins/$(MODULE)/src/AteamParam.cpp ./plugins/$(MODULE)/src/AteamParam.h
	$(CC) $(CFLAGS) ./plugins/$(MODULE)/src/AteamParam.cpp $(WXFLAGS) -c -o ./plugins/$(MODULE)/obj/AteamParam.o
	
./plugins/$(MODULE)/obj/InitMemories.o: ./plugins/$(MODULE)/src/InitMemories.cpp ./plugins/$(MODULE)/src/InitMemories.h
	$(CC) $(CFLAGS) ./plugins/$(MODULE)/src/InitMemories.cpp $(WXFLAGS) -c -o ./plugins/$(MODULE)/obj/InitMemories.o
	
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
	
ifneq ($(MODULE), )
	$(REMOVE) ./plugins/$(MODULE)/obj/AteamParam.o
	$(REMOVE) ./plugins/$(MODULE)/obj/InitMemories.o
endif
	
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
	
