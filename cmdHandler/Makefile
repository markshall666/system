INC=-Iinc -I../common/inc -I../itc/inc
LINKER=-L../itc/ -litc -pthread
LINKERARM=-L../itc/ -litcArm -pthread
AR=ar rvs
ARARM=arm-linux-gnueabihf-ar rvs
CC=g++
CCARM=arm-linux-gnueabihf-g++
CFLAGS=-Wall -Werror -g

SHELLSDIR=src/cmdShell.cpp
SHELLOBJECTS=cmdShell.o
SHELLOUT=cmdShell
SHELLOUTARM=cmdShellArm

DAEMONSDIR=src/cmdDaemon.cpp
DAEMONOBJECTS=cmdDaemon.o
DAEMONOUT=cmdDaemon
DAEMONOUTARM=cmdDaemonArm

CLIENTSDIR=src/cmdAppClient.cpp
CLIENTOBJECTS=cmdAppClient.o
CLIENTOUT=cmdAppClient.a
CLIENTOUTARM=cmdAppClientArm.a


default:
	$(CC) -c $(SHELLSDIR) $(INC) $(CFLAGS)
	$(CC) -o $(SHELLOUT) $(SHELLOBJECTS) $(LINKER)
	$(CC) -c $(DAEMONSDIR) $(INC) $(CFLAGS)
	$(CC) -o $(DAEMONOUT) $(DAEMONOBJECTS) $(LINKER)
	$(CC) -c $(CLIENTSDIR) $(INC) $(CFLAGS)
	$(AR) -o $(CLIENTOUT) $(CLIENTOBJECTS)
	rm -f $(SHELLOBJECTS) $(CLIENTOBJECTS) $(DAEMONOBJECTS)
arm:
	$(CCARM) -c $(SHELLSDIR) $(INC) $(CFLAGS)
	$(CCARM) -o $(SHELLOUTARM) $(SHELLOBJECTS) $(LINKERARM)
	$(CCARM) -c $(DAEMONSDIR) $(INC) $(CFLAGS)
	$(CCARM) -o $(DAEMONOUTARM) $(DAEMONOBJECTS) $(LINKERARM)
	$(CCARM) -c $(CLIENTSDIR) $(INC) $(CFLAGS)
	$(ARARM) -o $(CLIENTOUTARM) $(CLIENTOBJECTS)
	rm -f $(SHELLOBJECTS) $(CLIENTOBJECTS) $(DAEMONOBJECTS)
clean:
	rm -f $(SHELLOBJECTS) $(CLIENTOBJECTS) $(DAEMONOBJECTS) $(SHELLOUT) $(SHELLOUTARM) $(CLIENTOUT) $(CLIENTOUTARM) $(DAEMONOUT) $(DAEMONOUTARM) 
