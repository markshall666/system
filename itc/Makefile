INC=-Iinc -I../inc -I../common/inc
SDIR=src/itc.c src/itc_internal.c
CC=gcc
CFLAGS=std=c99 -fPIC -Wall
CFLAGS2=-shared -pthread -Wall -std=c99
OUT=itc.so.1.0.1
OBJECTS=itc.o itc_internal.o
LINKER=-Wl,-soname,itc.so.1

default:
	$(CC) -c $(SDIR) $(INC) -$(CFLAGS)
	$(CC) $(CFLAGS2) $(LINKER) -o $(OUT) $(OBJECTS)

links: 
	ln -s itc.so.1.0.1 itc.so.1
	ln -s itc.so.1.0.1 itc.so

clean:
	rm -f *.o $(OUT)
