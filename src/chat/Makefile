CC = g++
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../lib/luxsocket/*.cpp))
CFLAGS = -std=c++11 -I../../lib/luxsocket
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: sock chat

chat: sock
	$(CC) $(CFLAGS) $(OBJ_FILES) chatServer.cpp -o chatServer.out $(LIB)

sock:
	(cd ../../lib/luxsocket; make)

clean:
	rm -f *.o *.out *~
	(cd ../../lib/luxsocket; make clean)
