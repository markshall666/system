
default:
	g++ -o main_all src/main.cpp src/myCallbackObj.cpp src/libCaller.cpp src/libFactory.cpp src/itc.cpp -Iinc
lib:
	g++ -fPIC -c src/libCaller.cpp src/libFactory.cpp src/itc.cpp -Iinc    
	g++ -shared -Wl,-soname,extLib.so.1 -o extLib.so.1.0.1 libCaller.o libFactory.o itc.o
links: 
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:`pwd`"
	ln -s extLib.so.1.0.1 extLib.so.1
	ln -s extLib.so.1.0.1 extLib.so
app:
	g++ -c src/main.cpp src/myCallbackObj.cpp -Iinc
	g++ -o main main.o myCallbackObj.o -L. extLib.so.1.0.1
server:
	g++ -o callServer src/server.cpp src/itc.cpp -Iinc
clean:
	rm -f *.o
