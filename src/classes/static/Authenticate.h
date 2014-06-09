#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include "MD5.h"

using namespace std;
// Static class
// allows for basic authentication stuff

class Authenticate{
    public:
        static string createAccessToken(string EUID);
        static bool authenticateJWT(const string JWT, const string Client_API_KEY);
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

