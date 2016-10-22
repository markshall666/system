INC=-Iinc -I../itc/inc -I../common/inc
SDIR=src/libCaller.cpp src/libFactory.cpp
CC=g++
CFLAGS=-fPIC -pthread -Wall
CFLAGS2=-shared -pthread -Wall
OBJECTS=libCaller.o libFactory.o
EXTLIB=-L. ../itc/itc.so.1.0.1
OUT=libMofwk.so.1.0.1
LINKER=-Wl,-soname,libMofwk.so.1

default:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) $(CFLAGS2) $(LINKER) -o $(OUT) $(OBJECTS) $(EXTLIB)
links: 
	ln -s libMofwk.so.1.0.1 libMofwk.so.1
	ln -s libMofwk.so.1.0.1 libMofwk.so

clean:
	rm -f *.o $(OUT)
