LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: test cgi

cgi:
	(cd ../../classes/instanceable/; g++ -c CGI.cpp)

test: cgi
	g++ cgi_test.cpp ../../classes/instanceable/CGI.o -o test.out $(LIB)

clean:
	(cd ../../classes/instanceable/; rm -f *.o)
	rm -f *.o test.out *~
