CC = g++
Warnings = 
#-Wall -pedantic -W -Wextra -v
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -m64 -std=c++11 -c -std=c++11 -I./lib/luxsocket -I./output -I./cgi_bin -I./src/cgi_bin -I./src/classes/static -I./src/classes/instanceable -I./src/processes $(Warnings) 
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp


Auth = ./src/cgi_bin/AuthorizationServer.cpp -o ./cgi_bin/AuthorizationServer
Init = ./src/cgi_bin/Initialize.cpp -o ./cgi_bin/Initialize
Authen = ./src/classes/static/Authenticate.cpp -o ./output/Authenticate
FindBGT = ./src/classes/static/FindBGT.cpp -o ./output/FindBGT
MD5 = ./src/classes/static/MD5.cpp -o ./output/MD5
CGI = ./src/classes/instanceable/CGI.cpp -o ./output/CGI
HMBL= ./src/classes/instanceable/HMBL.cpp -o ./output/HMBL
socket = ./lib/luxsocket/socket.cpp -o ./output/socket
socketB = ./lib/luxsocket/luxSocket.cpp -o ./output/luxSocket
BGTSpawner = ./src/processes/BGTSpawner.cpp -o ./output/BGTSpawner
DBWriter = ./src/processes/DBWriter.cpp -o ./output/DBWriter
SendNewRelevant =  ./src/processes/SendNewRelevant.cpp -o ./output/SendNewRelevant
battleground = ./src/processes/battleground.cpp -o ./output/battleground
sendupdate = ./src/processes/sendupdate.cpp -o ./output/sendupdate

all: clean build run

mkdir:
	mkdir ./output
	mkdir ./cgi_bin

# tell apache where the cgi_bin is
apache: 

# Build All the Files!
build: mkdir MD5 CGI socketB socket Authen Auth HMBL FindBGT DBWriter sendupdate battleground BGTSpawner SendNewRelevant Init

# Working:
MD5:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(MD5) $(LIB) 

CGI:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(CGI) $(LIB)

socketB:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(socketB) $(LIB)

socket:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(socket) $(LIB)

Authen:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Authen) $(LIB)

Auth:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Auth) $(LIB)

HMBL:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(HMBL) $(LIB)

FindBGT:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(FindBGT) $(LIB)

DBWriter:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(DBWriter) $(LIB)

sendupdate:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(sendupdate) $(LIB)

battleground:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(battleground) $(LIB)
	
BGTSpawner:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(BGTSpawner) $(LIB) 
	
SendNewRelevant:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(SendNewRelevant) $(LIB)

# Jake	
Init:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Init) $(LIB)


# spawn BGT
run:
	chmod -R 777 ./output
	chmod -R 777 ./cgi_bin
	./output/BGTSpawner

# deletes all of the ./cgi_bin/ & ./output/
clean:
	rm -rf ./output
	rm -rf ./cgi_bin




