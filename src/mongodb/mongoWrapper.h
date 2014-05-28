#include <iostream>
#include <cstdlib>
#include <string>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

class MongoWrapper {
    public:
    MongoWrapper(const std::string &hostname);
    ~MongoWrapper() {};

    // Connect to database
    void connect(const std::string &hostname);

    // Insert
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    void insert(const std::string &ns, const mongo::BSONObj &obj);
    void insert(const std::string &ns, const std::string &obj);

    // Query
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    std::string query(const std::string &ns, const mongo::BSONObj &q);    
    std::string query(const std::string &ns, const std::string &q);

    // Update
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    void update(const std::string &ns, const mongo::BSONObj &q,
		const mongo::BSONObj &obj);
    void update(const std::string &ns, const std::string &q,
		const std::string &obj);

    // Remove
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    void remove(const std::string &ns, const mongo::BSONObj &q);
    void remove(const std::string &ns, const std::string &q);
        
    private:
    mongo::DBClientConnection c;
    
    // Print error message
    void error(const std::string &msg);
    
};
