IDIR=../inc
SDIR=../src/itc.c ../src/itc_internal.c src/itcTest.c mock/linuxMock.c
CC=gcc
CFLAGS=std=c99 -Wall -lcmocka -g
OUT=itcTest

default:
	$(CC) $(SDIR) -I$(IDIR) -$(CFLAGS) -o $(OUT) 

clean:
	rm -f *.o