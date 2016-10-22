INC=-Iinc -I../mofwk/inc -I../common/inc
SDIR=src/main.cpp src/myCallbackObj.cpp
OBJECTS=main.o myCallbackObj.o
OUT=app
EXTLIB=-L. ../mofwk/libMofwk.so.1.0.1
CC=g++
CFLAGS=-Wall -pthread


default:
	$(CC) -c $(SDIR) $(INC) $(CFLAGS)
	$(CC) -o $(OUT) $(OBJECTS) $(EXTLIB) $(CFLAGS)
	cp $(OUT) ../

clean:
	rm -f *.o $(OUT)
