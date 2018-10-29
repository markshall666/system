INC=-Iinc -I../mofwk/inc -I../common/inc
SDIR=src/main.cpp src/myCallbackObj.cpp
OBJECTS=main.o myCallbackObj.o
OUT=app
LINKER=-L../mofwk -lmofwk -pthread -L../itc -litc -lprotobuf
CC=g++
CCARM=arm-linux-gnueabihf-g++
CFLAGS=-Wall -Werror


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
