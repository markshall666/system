
default:
	g++ -o main_all src/main.cpp src/myCallbackObj.cpp src/libCaller.cpp src/libFactory.cpp src/itc.cpp -Iinc
lib:
	g++ -fPIC -c src/libCaller.cpp src/libFactory.cpp -Iinc -Iitc/inc -pthread -Wall
	g++ -shared -pthread -Wall -Wl,-soname,extLib.so.1 -o extLib.so.1.0.1 libCaller.o libFactory.o -L. itc/itc.so.1.0.1
links: 
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:`pwd`"
	ln -s extLib.so.1.0.1 extLib.so.1
	ln -s extLib.so.1.0.1 extLib.so
app:
	g++ -c src/main.cpp src/myCallbackObj.cpp -Iinc -Wall
	g++ -o main main.o myCallbackObj.o -L. extLib.so.1.0.1 -Wall
server:
	g++ -c src/server.cpp -Iinc -Iitc/inc -pthread -Wall
	g++ -o callServer server.o -L. itc/itc.so.1.0.1 -Wall 
clean:
	rm -f *.o
