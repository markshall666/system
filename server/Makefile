INC=-Iinc -I../common/inc -I../itc/inc
SDIR=src/server.cpp src/marekObject.cpp src/database.cpp src/communicationHandler.cpp src/transactionHandler.cpp src/cli.cpp src/moTypes.cpp ../common/src/messages.pb.cc
OBJECTS=server.o marekObject.o database.o communicationHandler.o transactionHandler.o cli.o moTypes.o messages.pb.o
OUT=configServer
LINKER=-L../itc/ -litc -pthread -lsqlite3 -lprotobuf
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
