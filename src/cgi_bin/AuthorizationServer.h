#ifndef AUTHORIZATIONSERVER_H
#define AUTHORIZATIONSERVER_H
#define DATABASE_NAME "db.auth"

// Stand Alone CGI script

#include "Authenticate.h"
#include "CGI.h"
#include <iostream>
#include <string>
#include <cstdlib>


#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"


using namespace mongo;
using namespace std;

#endif // AUTHORIZATIONSERVER_H
