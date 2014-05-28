LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: test mongo

mongo:
	(cd ../../mongodb; g++ -c mongoWrapper.cpp)

test: mongo
	g++ mongo_test.cpp ../../mongodb/mongoWrapper.o -o test.out $(LIB)

clean:
	(cd ../../mongodb/; rm -f *.o *~)
	rm -f *.o test.out *~
