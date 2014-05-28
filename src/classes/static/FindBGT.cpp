#include "FindBGT.h"

s_BGTInfo FindBGT::find(BSONObj clientDocument, BSONObj BGTDocument) {

  // get accessToken from BSONObj clientDocument
  std::string accessToken = clientDocument["sender"]["accessToken"].String();
  // get EUID from BSONObj clientDocument
  std::string EUID = clientDocument["sender"]["EUID"].String();
  
  //create structure 
  s_BGTInfo BGTInfo;
      
  if (Authenticate::authenticateAccessToken(accessToken, EUID)) {
    //pulls x-location and y-location from clientDocument
    int location[0] = atoi(clientDocument["object"]["location"]["x"].String().c_str());
    int location[1] = atoi(clientDocument["object"]["location"]["y"].String().c_str());
    
    BGTInfo.bgt_id = HMBL.findBucket(location[0], location[1]);
    
    //THIS IS FAR FROM DONE -- need to see how Jeff implements this function in locbasedhashmap.cpp
    BGTInfo.buckets = HMBL.surroundings(location[0], location[1], bucketX, bucketY, radius);
  }

  return BGTInfo;
  
}
