CC = g++
CFLAGS = -std=c++11 -stdlib=libc++
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: luxSocket.o socket.o

luxSocekt.o:
	$(CC) $(CFLAGS) -c luxSocket.cpp $(LIB)

socket.o:
	$(CC) -c socket.cpp

clean:
	rm -f *.o *~
