INC=-Iinc -I../inc
SDIR=src/itc.c src/itc_internal.c
CC=gcc
CFLAGS=std=c99 -fPIC -Wall

default:
	$(CC) -c $(SDIR) $(INC) -$(CFLAGS)
	$(CC) -shared -pthread -Wl,-soname,itc.so.1 -o itc.so.1.0.1 itc.o itc_internal.o -Wall -std=c99

links: 
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:`pwd`"
	ln -s itc.so.1.0.1 itc.so.1
	ln -s itc.so.1.0.1 itc.so

clean:
	rm -f *.o
