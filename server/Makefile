INC=-Iinc -I../common/inc -I../itc/inc
SDIR=src/server.cpp src/marekObject.cpp src/database.cpp src/communicationHandler.cpp src/transactionHandler.cpp src/cli.cpp
OBJECTS=server.o marekObject.o database.o communicationHandler.o transactionHandler.o cli.o
OUT=tranServer
EXTLIB=-L. ../itc/itc.so.1.0.1
CC=g++
CFLAGS=-Wall -pthread -l sqlite3

server:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) -o $(OUT) $(OBJECTS) $(EXTLIB) $(CFLAGS)
	cp $(OUT) ../
clean:
	rm -f *.o $(OUT)
