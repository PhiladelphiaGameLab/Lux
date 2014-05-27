#include "../../classes/instanceable/mongoWrapper.h"


int main(int argc, char *argv[]){
    
    MongoWrapper db("localhost");
    
    std::cout << "Insert {name:\"a\", age:20} into aa.person." 
	      << std::endl << std::endl;
    db.insert("aa.person", "{name:\"a\", age:20}");
    std::cout << "Insert {name:\"b\", age:30} into aa.person." 
	      << std::endl << std::endl;
    db.insert("aa.person", "{name:\"b\", age:30}");

    std::cout << "Find all in aa.person:" << std::endl;
    std::cout << db.query("aa.person", "") << std::endl << std::endl;
    std::cout << "Find {age:30} in aa.person:" << std::endl;
    std::cout << db.query("aa.person", "{age:30}") << std::endl << std::endl;
    std::cout << "Find {age:20} in aa.person:" << std::endl;
    std::cout << db.query("aa.person", "{age:20}") << std::endl << std::endl;

    std::cout << "Update {age:20} to 50" << std::endl << std::endl;
    db.update("aa.person", "{age:20}", "{'$set' : {'age' : 50}}");

    std::cout << "Find all in aa.person:" << std::endl;
    std::cout << db.query("aa.person", "") << std::endl << std::endl;

    std::cout << "Remove all in aa.person" << std::endl;
    db.remove("aa.person", "");
    return 0;
}
