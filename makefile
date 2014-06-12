CC = g++
Warnings = 
#-Wall -pedantic -W -Wextra -v
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -m64 -std=c++11 -std=c++11 -I./lib/luxsocket -I./output -I./cgi_bin -I./src/cgi_bin -I./src/classes/static -I./src/classes/instanceable -I./src/processes $(Warnings) 
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp
AuthLink= ./CGI.o ./MD5.o ./Authenticate.o
InitLink= $(AuthLink) ./FindBGT.o ./HMBL.o
BGTSpawnerLink= $(AuthLink) ./DBWriter.o ./SendNewRelevant.o ./HMBL.o ./battleground.o ./sendupdate.o ./socket.o ./luxSocket.o

Auth = ./src/cgi_bin/AuthorizationServer.cpp $(AuthLink) -o ./cgi_bin/AuthorizationServer 
Init = ./src/cgi_bin/Initialize.cpp $(InitLink) -o ./cgi_bin/Initialize
BGTSpawner = ./src/processes/BGTSpawner.cpp $(BGTSpawnerLink) -o ./output/BGTSpawner


Authen = -c ./src/classes/static/Authenticate.cpp
FindBGT = -c ./src/classes/static/FindBGT.cpp
MD5 = -c ./src/classes/static/MD5.cpp
CGI = -c ./src/classes/instanceable/CGI.cpp
HMBL= -c ./src/classes/instanceable/HMBL.cpp
socket = -c ./lib/luxsocket/socket.cpp
socketB = -c ./lib/luxsocket/luxSocket.cpp
DBWriter = -c ./src/processes/DBWriter.cpp
SendNewRelevant =  -c ./src/processes/SendNewRelevant.cpp
battleground = -c ./src/processes/battleground.cpp
sendupdate = -c ./src/processes/sendupdate.cpp

all: clean build run

mkdir:
	mkdir ./output
	mkdir ./cgi_bin

# tell apache where the cgi_bin is
apache: 

# Build All the Files!
build: mkdir MD5 CGI socketB socket Authen HMBL FindBGT DBWriter sendupdate battleground SendNewRelevant Init BGTSpawner Auth

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
	
SendNewRelevant:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(SendNewRelevant) $(LIB)

Auth:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Auth) $(LIB)

Init:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Init) $(LIB)

BGTSpawner:
	$(CC) $(CFLAGS) $(OBJ_FILES) $(BGTSpawner) $(LIB) 

# spawn BGT
run:
	chmod -R 777 ./output
	chmod -R 777 ./cgi_bin
	./output/BGTSpawner

# deletes all of the ./cgi_bin/ & ./output/
clean:
	rm -rf ./output
	rm -rf ./cgi_bin




