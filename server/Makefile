INC=-Iinc -I../common/inc -I../itc/inc
SDIR=src/server.cpp
OBJECTS=server.o
OUT=tranServer
EXTLIB=-L. ../itc/itc.so.1.0.1
CC=g++
CFLAGS=-Wall -pthread

server:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) -o $(OUT) $(OBJECTS) $(EXTLIB) $(CFLAGS)
	cp $(OUT) ../
clean:
	rm -f *.o $(OUT)
