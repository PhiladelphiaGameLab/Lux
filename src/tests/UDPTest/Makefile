CC = g++
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -std=c++11 -stdlib=libc++ -I../../../lib/luxsocket
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: client server socklib

client: 
	$(CC) client.cpp -o client.out

server: socklib server.cpp
	$(CC) $(CFLAGS) $(OBJ_FILES) server.cpp -o server.out $(LIB)

socklib:
	(cd ../../../lib/luxsocket; make)

clean:
	rm -f *.o server.out client.out *~
	(cd ../../../lib/luxsocket; make clean)
