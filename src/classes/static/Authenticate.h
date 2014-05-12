#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H


// Static class
// allows for basic authentication stuff

class Authenticate{
    public:
        static std::string createAccessToken(std::string EUID);
        static bool authenticateJWT(std::string JWT, std::string Client_API_KEY);
        static bool authenticateAccessToken(std::string AccessToken, std::string EUID);
        static std::string refreshAccessToken(std::string AccessToken, std::string EUID);
    protected:
    private:
        static std::string createAccessToken(std::string EUID, std::string timeBucket);
        static std::string getTimeBucket();
        static std::string getPreviousTimeBucket();

};

#endif // AUTHENTICATE_H

