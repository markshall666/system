INC=-Iinc -I../inc -I../common/inc
SDIR=src/itc.c src/itc_internal.c
CC=gcc
CCARM=arm-linux-gnueabihf-gcc
CFLAGS=-std=c99 -fpic -Wall -Werror
OUT=libitc.so
OUTARM=libitcArm.so
OBJECTS=itc.o itc_internal.o
LINKER=-shared -pthread

default:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) $(LINKER) -o $(OUT) $(OBJECTS)
	rm -f $(OBJECTS)
arm:
	$(CCARM) -c $(SDIR) $(INC) $(CFLAGS)
	$(CCARM) $(LINKER) -o $(OUTARM) $(OBJECTS)
	rm -f $(OBJECTS)
clean:
	rm -f $(OBJECTS) $(OUT) $(OUTARM)
