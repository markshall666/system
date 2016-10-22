INC=-I../inc -I../../inc
SDIR=../src/itc.c ../src/itc_internal.c src/itcTest.c mock/linuxMock.c
CC=gcc
CFLAGS=std=c99 -Wall -lcmocka -g -ftest-coverage -fprofile-arcs
OUT=itcTest

default:
	$(CC) $(SDIR) $(INC) -$(CFLAGS) -o $(OUT)

clean:
	rm -f *.o
