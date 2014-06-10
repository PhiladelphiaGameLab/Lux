#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <string>
#include <cstdlib>
#include <iostream>

#include "MD5.h"
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

using namespace std;
using namespace mongo;
// Static class
// allows for basic authentication stuff

class Authenticate{
    public:
        static string createAccessToken(string EUID);
        static string authenticateJWT(const string JWT, const string Client_API_KEY);
        static bool authenticateAccessToken(string AccessToken, string EUID);
        static string refreshAccessToken(string AccessToken, string EUID);
	static string createNewEUID(const string uniqueID);
    protected:
    private:
        static string createAccessToken(string EUID, string timeBucket);
        static string getTimeBucket();
        static string getPreviousTimeBucket();

};

#endif // AUTHENTICATE_H

