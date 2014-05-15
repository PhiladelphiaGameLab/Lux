#DEFINE SERVER_SECRET "LUXisAwesome!"


std::string Authenticate::createAccessToken(std::string EUID, std::string timeBucket){
    // NEEDS TO BE DONE
    return "afalsdasda";
}

std::string Authenticate::authenticateJWT(std::string JWT, std::string Client_API_KEY){

    // NEEDS TO BE DONE
    return "12013443"; // return unique ID
}

std::string Authenticate::createNewEUID(std::string JWT, std::string Client_API_KEY)
{
	//NEEDS TO BE DONE
	return "new ID";

}



std::string Authenticate::createAccessToken(std::string EUID){
    return Authenticate::createAccessToken(EUID, Authenticate::getTimeBucket());
}

bool Authenticate::authenticateAccessToken(std::string AccessToken, std::string EUID){
    std::string newAccessToken = Authenticate::createAccessToken(EUID);
    if(AccessToken.compare(newAccessToken)){
        return true;
    }else{
        return false;
    }
}

std::string Authenticate::refreshAccessToken(std::string AccessToken, std::string EUID){
    std::string newHash = Authenticate::createAccessToken(EUID, Authenticate::getTimeBucket());
    if(AccessToken.compare(newHash)){
        return AccessToken;
    }else if(AccessToken.compare(Authenticate::createAccessToken(EUID, Authenticate::getPreviousTimeBucket()))){
        return newHash;
    }else{
        return "";
    }
}

std::string Authenticate::getTimeBucket(){
        time_t timer;
        struct tm y2k;
        double seconds;

        y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
        y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

        time(&timer);  /* get current time; same as: timer = time(NULL)  */

        seconds = difftime(timer,mktime(&y2k));

        int timeBucket = (int)seconds/14336;

    return std::to_string(timeBucket);
}

std::string Authenticate::getPreviousTimeBucket(){
    return std::to_string(atoi(Authenticate::getTimeBucket())-1);
}



