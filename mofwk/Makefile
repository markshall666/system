INC=-Iinc -I../itc/inc -I../common/inc
SDIR=src/libCaller.cpp src/libFactory.cpp ../common/src/messages.pb.cc
CC=g++
CCARM=arm-linux-gnueabihf-g++
CFLAGS=-fpic -Wall -Werror
OBJECTS=libCaller.o libFactory.o messages.pb.o
OUT=libmofwk.so
LINKER=-shared -pthread -L../itc/ -litc -lprotobuf

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
