CC = g++
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -std=c++11 -stdlib=libc++ -I../../../lib/luxsocket
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp


# ./src/cgi_bin/AuthorizationServer.cpp
# ./src/cgi_bin/AuthorizationServer.h
# ./src/cgi_bin/Initialize.cpp
# ./src/cgi_bin/Initialize.h
# ./src/classes/static/Authenticate.cpp
# ./src/classes/static/Authenticate.h
# ./src/classes/static/FindBGT.cpp
# ./src/classes/static/FindBGT.h
# ./src/classes/static/MD5.cpp
# ./src/classes/static/MD5.h
# ./src/classes/instanceable/CGI.cpp
# ./src/classes/instanceable/CGI.h
# ./src/classes/instanceable/HMBL.cpp
# ./src/classes/instanceable/HMBL.h
# ./src/classes/instanceable/socket.cpp
# ./src/classes/instanceable/socket.h
# ./src/classes/instanceable/socketB.cpp
# ./src/classes/instanceable/socketB.h
# ./src/processes/BGTSpawner.cpp
# ./src/processes/BGTSpawner.h
# ./src/processes/DBWriter.cpp
# ./src/processes/DBWriter.h
# ./src/processes/SendNewRelevant.cpp
# ./src/processes/SendNewRelevant.h
# ./src/processes/battleground.cpp
# ./src/processes/battleground.h
# ./src/processes/sendupdate.cpp
# ./src/processes/sendupdate.h


# ./cgi_bin/
# ./output/


all: clean build run

# tell apache where the cgi_bin is
apache: 


build:
	$(CC) $(CFLAGS) $(OBJ_FILES)   $(LIB)

# spawn BGT
run:


# deletes all of the ./cgi_bin/ & ./output/
clean:




