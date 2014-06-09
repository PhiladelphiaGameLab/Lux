#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

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
    protected:
    private:
	static string createNewEUID(const string uniqueID);
        static string createAccessToken(string EUID, string timeBucket);
        static string getTimeBucket();
        static string getPreviousTimeBucket();

};

#endif // AUTHENTICATE_H

