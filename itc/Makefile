INC=-Iinc -I../inc -I../common/inc
SDIR=src/itc.c src/itc_internal.c
CC=gcc
CCARM=/opt/cross/bin/arm-unknown-linux-gnueabi-gcc
CFLAGS=-std=c99 -fpic -Wall -Werror
OUT=libitc.so
OBJECTS=itc.o itc_internal.o
LINKER=-shared -pthread

default:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) $(LINKER) -o $(OUT) $(OBJECTS)
	rm -f $(OBJECTS)
arm:
	$(CCARM) -c $(SDIR) $(INC) $(CFLAGS)
	$(CCARM) $(LINKER) -o $(OUT) $(OBJECTS)
	rm -f $(OBJECTS)
clean:
	rm -f $(OBJECTS) $(OUT)
