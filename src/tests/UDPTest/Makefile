CFLAGS=-I/usr/local/lib/mongo-client-install/include/mongo/bson
all: client server

client: 
	g++ client.cpp -o client.out

server: socket.o luxSocket.o server.o
	g++ $(CFLAGS) socket.o luxSocket.o server.o -o server.out

server.o: server.cpp
	g++ -c server.cpp

socket.o: socket.cpp
	g++ $(CFLAGS) -c socket.cpp

luxSocekt.o: luxSocket.cpp 
	g++ $(CFLAGS) -c luxSocket.cpp

clean:
	rm -rf *o server.out client.out
