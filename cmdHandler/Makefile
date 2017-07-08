SHELLINC=-Iinc -I../common/inc -I../itc/inc
SHELLSDIR=src/cmdShell.cpp
SHELLOBJECTS=cmdShell.o
SHELLOUT=cmdShell
SHELLOUTARM=cmdShellArm
SHELLLINKER=-L../itc/ -litc -pthread
SHELLLINKERARM=-L../itc/ -litcArm -pthread

CLIENTINC=-Iinc -I../common/inc -I../itc/inc
CLIENTSDIR=src/cmdAppClient.cpp
CLIENTOBJECTS=cmdAppClient.o
CLIENTOUT=cmdAppClient.a
CLIENTOUTARM=cmdAppClientArm.a

AR=ar rvs
ARARN=/opt/cross/bin/arm-unknown-linux-gnueabi-ar
CC=g++
CCARM=/opt/cross/bin/arm-unknown-linux-gnueabi-g++
CFLAGS=-Wall -Werror -g

default:
	$(CC) -c $(SHELLSDIR) $(SHELLINC) $(CFLAGS)
	$(CC) -o $(SHELLOUT) $(SHELLOBJECTS) $(SHELLLINKER)
	$(CC) -c $(CLIENTSDIR) $(CLIENTINC) $(CFLAGS)
	$(AR) -o $(CLIENTOUT) $(CLIENTOBJECTS)
	rm -f $(SHELLOBJECTS) $(CLIENTOBJECTS)
arm:
	$(CCARM) -c $(SHELLSDIR) $(SHELLINC) $(CFLAGS)
	$(CCARM) -o $(SHELLOUTARM) $(SHELLOBJECTS) $(SHELLLINKERARM)
	$(CCARM) -c $(CLIENTSDIR) $(CLIENTINC) $(CFLAGS)
	$(AR) -o $(CLIENTOUT) $(CLIENTOBJECTS)
	rm -f $(OBJECTS) $(CLIENTOUTARM)
clean:
	rm -f $(SHELLOBJECTS) $(CLIENTOBJECTS) $(SHELLOUT) $(SHELLOUTARM) $(CLIENTOUT) $(CLIENTOUTARM)
