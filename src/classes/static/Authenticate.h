#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

using namespace std;
// Static class
// allows for basic authentication stuff

class Authenticate{
    public:
        static string createAccessToken(string EUID);
        static bool authenticateJWT(string JWT, string Client_API_KEY);
        static bool authenticateAccessToken(string AccessToken, string EUID);
        static string refreshAccessToken(string AccessToken, string EUID);
    protected:
    private:
        static string createAccessToken(string EUID, string timeBucket);
        static string getTimeBucket();
        static string getPreviousTimeBucket();

};

#endif // AUTHENTICATE_H

