#include "mongoWrapper.h"

MongoWrapper::MongoWrapper(const std::string &hostname) {
    connect(hostname);
}

void MongoWrapper::connect(const std::string &hostname) {
    try {
	c.connect(hostname);
    } catch(const mongo::DBException &e) {
	error("Connection error.");
    }
}

void MongoWrapper::error(const std::string &msg) {
    std::cout << msg << std::endl;
}

void MongoWrapper::insert(const std::string &ns, const mongo::BSONObj &obj) {
    c.insert(ns, obj);
}

void MongoWrapper::insert(const std::string &ns, const std::string &obj) {
    insert(ns, mongo::fromjson(obj));
}

std::string MongoWrapper::query(const std::string &ns, const mongo::BSONObj &q) {
    std::auto_ptr<mongo::DBClientCursor> cursor = c.query(ns, q);
    std::string result("");

    while (cursor->more()) {
	if(result.size() > 0) {
	    result += "\n";
	}
	result += cursor->next().jsonString();
    }
    if (result.compare("") == 0) {
	result += "{}";
    }
    return result;
}

std::string MongoWrapper::query(const std::string &ns, const std::string &q) {
    return query(ns, mongo::fromjson(q));
}

void MongoWrapper::update(const std::string &ns, const mongo::BSONObj &q,
			  const mongo::BSONObj &obj) {
    c.update(ns, q, obj);
}

void MongoWrapper::update(const std::string &ns, const std::string &q, 
			  const std::string &obj) {
    update(ns, mongo::fromjson(q), mongo::fromjson(obj));
}

void MongoWrapper::remove(const std::string &ns, const mongo::BSONObj &q) {
    c.remove(ns, q);
}

void MongoWrapper::remove(const std::string &ns, const std::string &q) {
    remove(ns, mongo::fromjson(q));
}

mongo::BSONObj MongoWrapper::findOne(const std::string &ns,
				     const mongo::BSONObj &q) {
  return c.findOne(ns, q);    
}

mongo::BSONObj MongoWrapper::findOne(const std::string &ns,
				     const std::string &q) {
  return findOne(ns, mongo::fromjson(q));
}
