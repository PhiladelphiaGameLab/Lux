#DEFINE SERVER_SECRET "LUXisAwesome!"


static std::string Authenticate::createAccessToken(std::string EUID){
    return "afalsdasda";
}
static bool Authenticate::authenticateJWT(std::string JWT, std::string Client_API_KEY){
    return true;
}
static bool Authenticate::authenticateAccessToken(std::string AccessToken, std::string EUID){
    return true;
}
static std::string Authenticate::refreshAccessToken(std::string AccessToken, std::string EUID){
    return AccessToken;
}
