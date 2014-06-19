#include "mongoWrapper.h"

MongoWrapper::MongoWrapper(const std::string &hostname) {
    connect(hostname);
}

void MongoWrapper::connect(const std::string &hostname) {
    try {
	_c.connect(hostname);
    } catch(const mongo::DBException &e) {
	error("Connection error.");
    }
}

void MongoWrapper::error(const std::string &msg) {
    std::cout << msg << std::endl;
}

void MongoWrapper::insert(const std::string &ns, const mongo::BSONObj &obj) {
    _c.insert(ns, obj);
}


void MongoWrapper::insert(const std::string &ns, const std::string &obj) {
    insert(ns, mongo::fromjson(obj));
}

std::string MongoWrapper::query(const std::string &ns, const mongo::BSONObj &q) {
    std::auto_ptr<mongo::DBClientCursor> cursor = _c.query(ns, q);
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

#ifdef MONGO_DRIVER_26
void MongoWrapper::update(const std::string &ns, const mongo::BSONObj &q,
			  const mongo::BSONObj &obj) {
    _c.update(ns, q, obj);
}

void MongoWrapper::update(const std::string &ns, const std::string &q, 
			  const std::string &obj) {
    update(ns, mongo::fromjson(q), mongo::fromjson(obj));
}

void MongoWrapper::remove(const std::string &ns, const mongo::BSONObj &q) {
    _c.remove(ns, q);
}

void MongoWrapper::remove(const std::string &ns, const std::string &q) {
    remove(ns, mongo::fromjson(q));
}
#else
void MongoWrapper::update(const std::string &ns, const mongo::BSONObj &q,
			  const mongo::BSONObj &obj, bool upsert, 
			  bool multi, const WriteConcern* wc) {
    _c.update(ns, q, obj, upsert, multi, wc);
}

void MongoWrapper::update(const std::string &ns, const std::string &q, 
			  const std::string &obj, bool upsert, 
			  bool multi, const WriteConcern* wc) {
    update(ns, mongo::fromjson(q), mongo::fromjson(obj),
	   upsert, multi, wc);
}

void MongoWrapper::remove(const std::string &ns, const mongo::BSONObj &q,
			  bool justOne, const WriteConcern* wc) {
    _c.remove(ns, q, justOne, wc);
}

void MongoWrapper::remove(const std::string &ns, const std::string &q,
			  bool justOne, const WriteConcern* wc) {
    remove(ns, mongo::fromjson(q), justOne, wc);
}
#endif

mongo::BSONObj MongoWrapper::findOne(const std::string &ns,
				     const mongo::BSONObj &q) {
    return _c.findOne(ns, q);    
}

mongo::BSONObj MongoWrapper::findOne(const std::string &ns,
				     const std::string &q) {
    return findOne(ns, mongo::fromjson(q));
}

void MongoWrapper::arrayPush(const std::string &ns, const mongo::BSONObj &q,
			     const std::string &array_field_name,
			     const std::string &elements,
			     bool pushAll) {
    std::string method("$push");
    if (pushAll) {
	method = "$pushAll";
    }
    arrayUpdate(ns, q, array_field_name, elements, method);
}
    
void MongoWrapper::arrayPush(const std::string &ns, const std::string &q,
			     const std::string &array_field_name,
			     const std::string &elements,
			     bool pushAll) {
    arrayPush(ns, mongo::fromjson(q), array_field_name, elements, pushAll);
}

void MongoWrapper::arrayPull(const std::string &ns, const mongo::BSONObj &q,
			     const std::string &array_field_name,
			     const std::string &elements,
			     bool pullAll) {
    std::string method("$pull");
    if (pullAll) {
	method = "$pullAll";
    }
    arrayUpdate(ns, q, array_field_name, elements, method);
}

void MongoWrapper::arrayPull(const std::string &ns, const std::string &q,
			     const std::string &array_field_name,
			     const std::string &elements,
			     bool pullAll) {
    arrayPull(ns, mongo::fromjson(q), array_field_name, elements, pullAll);
}


void MongoWrapper::arrayUpdate(const std::string &ns, const mongo::BSONObj &q,
			       const std::string &array_field_name,
			       const std::string &elements,
			       const std::string &method) {
    mongo::BSONObjBuilder a;
    a.append(array_field_name, elements);
    mongo::BSONObjBuilder b;
    b.append(method, a.obj());
    update(ns, q, b.obj());
}
