#include <iostream>
#include <cstdlib>
#include <string>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

#define MONGO_DRIVER_26

class MongoWrapper {
    public:
    MongoWrapper(const std::string &hostname = "localhost");
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
    
    #ifdef MONGO_DRIVER_26 
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

    #else
    // Update
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    void update(const std::string &ns, const mongo::BSONObj &q,
		const mongo::BSONObj &obj, bool upsert = false, 
		bool multi = false, const WriteConcern* wc=NULL);
    void update(const std::string &ns, const std::string &q,
		const std::string &obj, bool upsert = false, 
		bool multi = false, const WriteConcern* wc=NULL);

    // Remove
    // ns namespace to query, format is <dbname>.<collectname>[.<collectname>]*
    void remove(const std::string &ns, const mongo::BSONObj &q, 
		bool justOne = 0, const WriteConcern* wc=NULL);
    void remove(const std::string &ns, const std::string &q,
		bool justOne = 0, const WriteConcern* wc=NULL);    
    #endif
            
    // Find a single document that satisfies the query criteria
    mongo::BSONObj findOne(const std::string &ns, 
			   const mongo::BSONObj &q);
    mongo::BSONObj findOne(const std::string &ns,
			   const std::string &q);

    // Insert elements into an array of a document
    void arrayPush(const std::string &ns, const mongo::BSONObj &q,
		   const std::string &array_field_name,
		   const std::string &elements,
		   bool pushAll = false);

    void arrayPush(const std::string &ns, const mongo::BSONObj &q,
		   const std::string &array_field_name,
		   const mongo::BSONObj &elements,
		   bool pushAll = false);

    // Delete elements from an array of a document
    void arrayPull(const std::string &ns, const mongo::BSONObj &q,
		   const std::string &array_field_name,
		   const std::string &elements,
		   bool pullAll = false);

    void arrayPull(const std::string &ns, const mongo::BSONObj &q,
		   const std::string &array_field_name,
		   const mongo::BSONObj &elements,
		   bool pullAll = false);

    
    private:
    mongo::DBClientConnection _c;
    
    // Print error message
    void error(const std::string &msg);
    
    void arrayUpdate(const std::string &ns, const mongo::BSONObj &q,
		     const std::string &array_field_name,
		     const std::string &element,
		     const std::string &method);

    void arrayUpdate(const std::string &ns, const mongo::BSONObj &q,
		     const std::string &array_field_name,
		     const mongo::BSONObj &element,
		     const std::string &method);
};

