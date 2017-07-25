INC=-Iinc -I../common/inc -I../itc/inc
SDIR=src/server.cpp src/marekObject.cpp src/database.cpp src/communicationHandler.cpp src/transactionHandler.cpp src/cli.cpp src/moTypes.cpp
OBJECTS=server.o marekObject.o database.o communicationHandler.o transactionHandler.o cli.o moTypes.o
OUT=tranServer
LINKER=-L../itc/ -litc -pthread -lsqlite3
CC=g++
CCARM=arm-linux-gnueabihf-g++
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
