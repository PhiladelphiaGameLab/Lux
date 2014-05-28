#include "mongoWrapper.h"

class MongoTransaction {
    public:
    MongoTransaction(const std::string &id0, 
		     const std::string &id1,
		     const std::string &db_hostname
		     )
	:id0(id0), id1(id1) {
	conn.connect(db_hostname);
	initTransaction();
    }

    // Get the corresponding records for id0 and id1 in db
    void initTransaction();
    
    // Check if quantity for an items if enough
    bool checkQuantity(const std::string &id, 
			 const std::string &item, int qty);
    
    // Add an item(in jason format) into a record
    void addItem(const std::string &record,
		 const std::string &item);

    // Remove an item(in jason format) from a record
    void removeItem(const std::string &record,
		    const std::string &item);

    private:
    // The record of two players
    std::string id0, id1;
    mongo::BSONObj p0;
    mongo::BSONObj p1;
    MongoWrapper conn;
};
