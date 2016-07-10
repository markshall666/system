
default:
	g++ -o main_all src/main.cpp src/myCallbackObj.cpp src/libCaller.cpp src/libFactory.cpp src/itc.cpp -Iinc
lib:
	g++ -fPIC -c src/libCaller.cpp src/libFactory.cpp -Iinc -pthread -Wall
	g++ -shared -pthread -Wall -Wl,-soname,extLib.so.1 -o extLib.so.1.0.1 libCaller.o libFactory.o -L. itc.so.1.0.1
links: 
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:`pwd`"
	ln -s extLib.so.1.0.1 extLib.so.1
	ln -s extLib.so.1.0.1 extLib.so
	ln -s itc.so.1.0.1 itc.so.1
	ln -s itc.so.1.0.1 itc.so
app:
	g++ -c src/main.cpp src/myCallbackObj.cpp -Iinc -Wall
	g++ -o main main.o myCallbackObj.o -L. extLib.so.1.0.1 -Wall
server:
	g++ -c src/server.cpp -Iinc -pthread -Wall
	g++ -o callServer server.o -L. itc.so.1.0.1 -Wall 
itc:
	gcc -fPIC -c src/itc.c -Iinc -Wall -std=c99
	gcc -shared -pthread -Wl,-soname,itc.so.1 -o itc.so.1.0.1 itc.o -Wall -std=c99
clean:
	rm -f *.o
