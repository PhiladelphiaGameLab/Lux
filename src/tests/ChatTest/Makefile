LIB = -pthread -lcurlpp -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: chat

cgi:
	(cd ../../classes/instanceable/; g++ -c CGI.cpp $(LIB))

auth:
	(cd ../../classes/static/; g++ -std=c++0x Authenticate.cpp -c $(LIB))

md5:
	(cd ../../classes/static/; g++ -std=c++0x MD5.cpp -c $(LIB))

chat: cgi auth md5
	g++ -std=c++0x ChatHttp.cpp ../../classes/instanceable/CGI.o ../../classes/static/MD5.o ../../classes/static/Authenticate.o -o chat.out $(LIB)

clean:
	rm *.o chat.out *~
	(cd ../../classes/instanceable/; rm *.o)
	(cd ../../classes/static/; rm *.o)