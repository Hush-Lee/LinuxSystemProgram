clien:src/util.o src/client.o
	g++ src/client.o src/util.o -o client

server:src/util.o src/server.o
	g++ src/server.o src/util.o -o server

src/server.o:src/server.cpp 
	g++ stc/server.cpp -c -Wall -g -o src server.o

src/client.o:src/client.cpp
	g++ src/client.cpp -c -Wall -g -o src/client.o
src/util.o:src/util.cpp
	g++ src/util.cpp -c -Wall -g -o src/util.o

clean:
	rm *.o client -rf
	rm *.o server -rf 
