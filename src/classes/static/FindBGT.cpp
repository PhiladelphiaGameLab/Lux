#include "FindBGT.h"

s_BGTInfo FindBGT::find(BSONObj clientDocument, BSONObj BGTDocument) {

  // get accessToken from BSONObj clientDocument
  std::string accessToken = clientDocument["sender"]["accessToken"].String();
  // get EUID from BSONObj clientDocument
  std::string EUID = clientDocument["sender"]["EUID"].String();
  
  // get accessToken from BSONObj BGTDocument
  std::string BGTAcessToken = BGTDocument["sender"]["accessToken"].String();
  // get EUID from BSONObj BGTDocument
  std::string EUID = BGTDocument["sender"]["EUID"].String();
  
  //create structure 
  s_BGTInfo BGTInfo;
      
  if (Authenticate::authenticateAccessToken(accessToken, EUID)) {
    
    //pulls x-location and y-location from clientDocument
    int location[0] = atoi(clientDocument["object"]["location"]["x"].String().c_str());
    int location[1] = atoi(clientDocument["object"]["location"]["y"].String().c_str());
    
    //assigns proper value to BGTInfo
    BGTInfo.bgt_id = HMBL.findBucket(location[0], location[1]);
    
    //pulls radius, mapX, mapY, columns, and rows from BGTDocument to use as parameters in HMBL.surroundings
    int radius = atoi(BGTDocument["object"]["radius"].String().c_str());
    int mapX = atoi(BGTDocument["object"]["mapX"].String().c_str());
    int mapY = atoi(BGTDocument["object"]["mapY"].String().c_str());
    int columns = atoi(BGTDocument["object"]["columns"].String().c_str());
    int rows = atoi(BGTDocument["object"]["rows"].String().c_str());
    
    BGTInfo.buckets = HMBL.surroundings(location[0], location[1], radius, mapX, mapY, columns, rows);
    
  }

  return BGTInfo;
  
}
