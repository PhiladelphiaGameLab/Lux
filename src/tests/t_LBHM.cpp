#include "../classes/instanceable/locbasedhashmap.h"
#include "../classes/static/HashingUtil.h"
#include <iostream>
#include <cstdlib>
#include <string>

bool test1 (HMBL<int>* data);
bool test1a (HMBL<std::string>* data);
bool test2 (HMBL<int>* data);
bool test2a (HMBL<std::string>* data);
bool test3 (HMBL<int>* data);
bool test3a (HMBL<std::string>* data);

int main(int argc, char *argv[]){

    HMBL<int>* tINT = new HMBL<int>();
    HMBL<std::string>* tOBJT = new HMBL<std::string>(1,1,4,5);


    assert(test1(tINT));
    assert(test1a(tOBJT));
    assert(test2(tINT));
    assert(test2a(tOBJT));
    assert(test3(tINT));
    assert(test3a(tOBJT));

    delete tINT;
    delete tOBJT;

	return EXIT_SUCCESS;
}

//TESTS ADDING TO THE LIST
bool test1 (HMBL<int>* data){
	data->add(HashUtil::LBHM2Input(MapLocationDS{0,0,10,10}), 5);
	data->add(HashUtil::LBHM2Input(MapLocationDS{1,5,10,10}), 67403);
	data->add(HashUtil::LBHM2Input(MapLocationDS{5,9,10,10}), 4738);
	data->add(HashUtil::LBHM2Input(MapLocationDS{9,3,10,10}), 167);
	data->add(HashUtil::LBHM2Input(MapLocationDS{8,1,10,10}), 3);
	data->add(HashUtil::LBHM2Input(MapLocationDS{2,7,10,10}), 190);

	return true;
}

//TESTS ADDING TO THE LIST
bool test1a (HMBL<std::string>* data){
	data->add(HashUtil::LBHM2Input(MapLocationDS{0,0,4,5}), "MIKE");
	data->add(HashUtil::LBHM2Input(MapLocationDS{1,5,4,5}), "67403");
	data->add(HashUtil::LBHM2Input(MapLocationDS{5,9,4,5}), "4738");
	data->add(HashUtil::LBHM2Input(MapLocationDS{9,3,4,5}), "167");
	data->add(HashUtil::LBHM2Input(MapLocationDS{8,1,4,5}), "3");
	data->add(HashUtil::LBHM2Input(MapLocationDS{2,7,4,5}), "AWESOME");

	return true;
}


//TESTS GETTING FROM THE LIST
bool test2 (HMBL<int>* data){
	std::list<LocData<int> >* t1 = data->getSocketLists(HashUtil::LBHM2Input(MapLocationDS{2,7,4,5}));
	std::list<LocData<int> > t2 = data->getIndex(HashUtil::LBHM2Input(MapLocationDS{2,7,4,5}));

	return true;
}


//TESTS GETTING FROM THE LIST
bool test2a (HMBL<std::string>* data){
	std::list<LocData<std::string> >* t1 = data->getSocketLists(HashUtil::LBHM2Input(MapLocationDS{2,7,4,5}));
	std::list<LocData<std::string> > t2 = data->getIndex(HashUtil::LBHM2Input(MapLocationDS{2,7,4,5}));

	return true;
}


//TESTS THE REMOVE FUNCTION
bool test3 (HMBL<int>* data){
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();

	return true;
}


//TESTS THE REMOVE FUNCTION
bool test3a (HMBL<std::string>* data){
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();
	data->removeExpiredObjects();

	return true;
}
