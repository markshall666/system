INC=-Iinc -I../itc/inc -I../common/inc
SDIR=src/libCaller.cpp src/libFactory.cpp
CC=g++
CCARM=arm-linux-gnueabihf-g++
CFLAGS=-fpic -Wall -Werror
OBJECTS=libCaller.o libFactory.o
OUT=libmofwk.so
LINKER=-shared -pthread -L../itc/ -litc

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
