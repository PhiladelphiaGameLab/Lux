CC = g++
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -std=c++11 -stdlib=libc++ -I../../../lib/luxsocket -I../../chat/
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: socklib test

test:
	$(CC) $(CFLAGS) $(OBJ_FILES) test.cpp -o test.out $(LIB)

client: socklib 
	$(CC) $(CFLAGS) $(OBJ_FILES) client.cpp -o client.out $(LIB)

socklib:
	(cd ../../../lib/luxsocket; make)

clean:
	rm -f *.o *.out *~
	(cd ../../../lib/luxsocket; make clean)
