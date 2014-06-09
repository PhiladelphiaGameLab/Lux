CC = g++
Warnings = 
#-Wall -pedantic -W -Wextra -v
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -std=c++11 -c  $(Warnings) 
INC = -std=c++11 -I../../../lib/luxsocket -I./output -I.cgi_bin $(Warnings) 
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp


Auth = ./src/cgi_bin/AuthorizationServer.cpp -o ./cgi_bin/Auth
Init = ./src/cgi_bin/Initialize.cpp -o ./cgi_bin/Init
Authen = ./src/classes/static/Authenticate.cpp -o ./output/Authen
FindBGT = ./src/classes/static/FindBGT.cpp -o ./output/FindBGT
MD5 = ./src/classes/static/MD5.cpp -o ./output/MD5
CGI = ./src/classes/instanceable/CGI.cpp -o ./output/CGI
HMBL= ./src/classes/instanceable/HMBL.cpp -o ./output/HMBL
socket = ./lib/luxsocket/socket.cpp -o ./output/socket
socketB = ./lib/luxsocket/luxSocket.cpp -o ./output/socketB
BGTSpawner = ./src/processes/BGTSpawner.cpp -o ./output/BGTSpawner
DBWriter = ./src/processes/DBWriter.cpp -o ./output/DBWriter
SendNewRelevant =  ./src/processes/SendNewRelevant.cpp -o ./output/SendNewRelevant
battleground = ./src/processes/battleground.cpp -o ./output/battleground
sendupdate = ./src/processes/sendupdate.cpp -o ./output/sendupdate

all: clean build run

# tell apache where the cgi_bin is
apache: 

# Build All the Files!
build:
	mkdir ./output
	mkdir ./cgi_bin
	$(CC) $(CFLAGS) $(OBJ_FILES) $(MD5) $(LIB) 
	$(CC) $(CFLAGS) $(OBJ_FILES) $(CGI) $(LIB)
	

	$(CC) $(CFLAGS) $(OBJ_FILES) $(socketB) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(socket) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(HMBL) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Auth) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(FindBGT) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Init) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(Authen) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(DBWriter) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(SendNewRelevant) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(battleground) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(sendupdate) $(LIB)
	$(CC) $(INC) $(OBJ_FILES) $(BGTSpawner) $(LIB) 

# spawn BGT
run:
	./output/BGTSpawner

# deletes all of the ./cgi_bin/ & ./output/
clean:
	rm -rf ./output
	rm -rf ./cgi_bin




