CC = cd ./output; g++
PROJ_DIR = /home/ec2-user/Alpha
Warnings =
#-Wall -pedantic -W -Wextra -v
OBJ_FILES = $(patsubst %.cpp,%.o, $(wildcard ../../../lib/luxsocket/*.cpp))
CFLAGS = -m64 -std=c++11 -std=c++11 -I$(PROJ_DIR)/lib/luxsocket -I$(PROJ_DIR)/output -I$(PROJ_DIR)/cgi_bin -I$(PROJ_DIR)/src/cgi_bin -I$(PROJ_DIR)/src/classes/static -I$(PROJ_DIR)/src/classes/instanceable -I$(PROJ_DIR)/src/processes $(Warnings)
LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp
AuthLink= $(PROJ_DIR)/output/CGI.o $(PROJ_DIR)/output/MD5.o $(PROJ_DIR)/output/Authenticate.o
InitLink= $(AuthLink) $(PROJ_DIR)/output/FindBGT.o $(PROJ_DIR)/output/HMBL.o
BGTSpawnerLink= $(AuthLink) $(PROJ_DIR)/output/DBWriter.o $(PROJ_DIR)/output/SendNewRelevant.o $(PROJ_DIR)/output/HMBL.o $(PROJ_DIR)/output/battleground.o $(PROJ_DIR)/output/sendupdate.o $(PROJ_DIR)/output/socket.o $(PROJ_DIR)/output/luxSocket.o 
#$(PROJ_DIR)/output/temp_battleground.o

Auth = ../src/cgi_bin/AuthorizationServer.cpp $(AuthLink) -o $(PROJ_DIR)/cgi_bin/AuthorizationServer.cgi -g
Init = ../src/cgi_bin/Initialize.cpp $(InitLink) -o $(PROJ_DIR)/cgi_bin/Initialize.cgi -g
BGTSpawner = ../src/processes/BGTSpawner.cpp $(BGTSpawnerLink) -o $(PROJ_DIR)/output/BGTSpawner -g

Authen = -c ../src/classes/static/Authenticate.cpp
FindBGT = -c ../src/classes/static/FindBGT.cpp
MD5 = -c ../src/classes/static/MD5.cpp -g
CGI = -c ../src/classes/instanceable/CGI.cpp -g
HMBL= -c ../src/classes/instanceable/HMBL.cpp -g
socket = -c ../lib/luxsocket/socket.cpp -g
socketB = -c ../lib/luxsocket/luxSocket.cpp -g
DBWriter = -c ../src/processes/DBWriter.cpp -g
SendNewRelevant = -c ../src/processes/SendNewRelevant.cpp -g
battleground = -c ../src/processes/battleground.cpp -g
#temp_bg = -c ../src/processes/temp_battleground.cpp -g
sendupdate = -c ../src/processes/sendupdate.cpp -g

all: clean build run

Mongo:
	/home/ec2-user/mongodb/mongodb-linux-x86_64-2.6.1/bin/mongod --dbpath /home/ec2-user/data/db &

mkdir:
	mkdir $(PROJ_DIR)/output
	mkdir $(PROJ_DIR)/cgi_bin

# tell apache where the cgi_bin is
apache:

# Build All the Files!
build: mkdir MD5 CGI socketB socket Authen HMBL FindBGT DBWriter sendupdate battleground SendNewRelevant Init BGTSpawner Auth

cgi-exec: Init BGTSpawner Auth

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

#temp_bg:
	 #$(CC) $(CFLAGS) $(OBJ_FILES) $(temp_bg) $(LIB)
# spawn BGT
run:
	chmod -R 777 $(PROJ_DIR)/output
	chmod -R 777 $(PROJ_DIR)/cgi_bin
	rm -f $(PROJ_DIR)/lux_pipe*
	$(PROJ_DIR)/output/BGTSpawner

# deletes all of the ./cgi_bin/ & ./output/
clean:
	rm -rf ./output
	rm -rf ./cgi_bin
	rm -f lux_pipe*

debug:
	#/home/ec2-user/mongodb/mongodb-linux-x86_64-2.6.1/bin/mongod --dbpath /home/ec2-user/data/db & > run.mongo
	rm run.txt;
	for ((a=1; a <= 1000000 ; a++)); do  echo " "; echo $$a; echo " " >>run.txt; echo $$a >> run.txt; make run | tail -n 5 | tee -a run.txt; done; 
	#cat run.txt
	#for ((a=1; a <= 1000000 ; a++)); do  echo " "; echo $$a; echo " " >>run.txt; echo $$a >> run.txt; timeout 280s make run | tail -n 5 | tee -a run.txt; done; 
	cat run.txt | awk '{if($$0 ~ /ObjectId/) print $$2 " "  $$3 " "  $$4 " "  $$5 " " $$6; else for (i=2; i<NF; i++) printf $$i " "; print $$NF} ;' | sort | uniq -c -d

debugLook:
	cat run.txt | awk '{if($$0 ~ /ObjectId/ && $$0 != /[0-9]*/ && NF != 1) print $$2 " "  $$3 " "  $$4 " "  $$5 " " $$6; else for (i=2; i<NF; i++) printf $$i " "; print $$NF } ;' | sort | uniq -c -d

