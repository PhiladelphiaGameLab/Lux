LIB = -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options

all: test mongo

mongo:
	(cd ../../classes/instanceable/; g++ -c mongoWrapper.cpp)

test: mongo
	g++ mongo_test.cpp ../../classes/instanceable/mongoWrapper.o -o test.out $(LIB)

clean:
	(cd ../../classes/instanceable/; rm -f *.o *~)
	rm -f *.o test.out *~
