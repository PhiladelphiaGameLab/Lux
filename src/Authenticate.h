#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H


class Authenticate{
    public:
        static std::string createAccessToken(std::string EUID);
        static bool authenticateJWT(std::string JWT, std::string Client_API_KEY);
        static bool authenticateAccessToken(std::string AccessToken, std::string EUID);
        static std::string refreshAccessToken(std::string AccessToken, std::string EUID);
    protected:
    private:
};

#endif // AUTHENTICATE_H

