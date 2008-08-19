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

all: begin main end
	

begin:
	@$(ECHO) ""
	@$(ECHO) "Building Project Ateam-SCP:"
	@$(ECHO) "================================"
	

main: ./bin/ateam-scp
	
./bin/ateam-scp: ./obj/AteamParam.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/AteamScp.o
	$(CC) $(CFLAGS) ./obj/AteamParam.o ./obj/MainFrame.o ./obj/IOParam.o ./obj/AteamScp.o $(WXLIBS) -o ./bin/ateam-scp
	
./obj/AteamParam.o: ./src/AteamParam.cpp ./src/AteamParam.h
	$(CC) $(CFLAGS) ./src/AteamParam.cpp $(WXFLAGS) -c -o ./obj/AteamParam.o
	
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
	

clean:
	@$(ECHO) ""
	@$(ECHO) "Removing files:"
	@$(ECHO) "================================"
	
	$(REMOVE) ./obj/AteamParam.o
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
	
