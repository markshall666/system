INC=-Iinc -I../common/inc -I../itc/inc
SDIR=src/cmdShell.cpp
OBJECTS=cmdShell.o
OUT=cmdShell
LINKER=-L../itc/ -litc -pthread
CC=g++
CCARM=/opt/cross/bin/arm-unknown-linux-gnueabi-g++
CFLAGS=-Wall -Werror -g

default:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) -o $(OUT) $(OBJECTS) $(LINKER)
	rm -f $(OBJECTS)
	cp $(OUT) ../
arm:
	$(CCARM) -c $(SDIR) $(INC) $(CFLAGS)
	$(CCARM) -o $(OUT) $(OBJECTS) $(LINKER)
	rm -f $(OBJECTS)
	cp $(OUT) ../
clean:
	rm -f $(OBJECTS) $(OUT)
