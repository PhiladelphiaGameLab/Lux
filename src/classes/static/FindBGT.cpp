#include "FindBGT.h"
#include "HMBL.h"

s_BGTInfo FindBGT::find(BSONObj clientDocument) {

  // get accessToken from BSONObj clientDocument
  std::string accessToken = clientDocument["sender"]["accessToken"].String();
  // get EUID from BSONObj clientDocument
  std::string EUID = clientDocument["sender"]["EUID"].String();
  
  // create a s_BGTInfo struct 
  s_BGTInfo BGTInfo;
      
  if (Authenticate::authenticateAccessToken(accessToken, EUID)) {
    
    // pull x-location and y-location from clientDocument
    int location[0] = atoi(clientDocument["object"]["location"]["x"].String().c_str());
    int location[1] = atoi(clientDocument["object"]["location"]["y"].String().c_str());
    
    // assign proper value to BGTInfo.bgt_id
    BGTInfo.bgt_id = HMBL::findBucket(location[0], location[1]);
    // assign proper value to BGTInfo.buckets
    BGTInfo.buckets = FindBGT::findSurroundings(clientDocument, BGTInfo.bgt_id);
    
  }

  return BGTInfo;
  
}

//-----------------------------------HELPER FUNCTION-----------------------------------

std::vector<int> FindBGT::findSurroundings(BSONObj clientDocument, int bgt_id) {
  
    // connect to the database
    mongo::DBClientConnection c;
    c.connect("localhost")
    
    // find the BGT Document
    BSONObj BGTDoc = c.findOne(DATABASE_NAME, Query("Type" << "BGT" << "BGT_ID" << bgt_id));
       
    // pull x-location and y-location from clientDocument
    int location[0] = atoi(clientDocument["object"]["location"]["x"].String().c_str());
    int location[1] = atoi(clientDocument["object"]["location"]["y"].String().c_str());
    
    // pull radius, mapX, mapY, columns, and rows from BGTDocument to use as parameters in HMBL::surroundings
    int radius = atoi(BGTDoc["object"]["radius"].String().c_str());
    int mapX = atoi(BGTDoc["object"]["mapX"].String().c_str());
    int mapY = atoi(BGTDoc["object"]["mapY"].String().c_str());
    int columns = atoi(BGTDoc["object"]["columns"].String().c_str());
    int rows = atoi(BGTDoc["object"]["rows"].String().c_str())
    
    return HMBL::surroundings(location[0], location[1], radius, mapX, mapY, columns, rows);

}



