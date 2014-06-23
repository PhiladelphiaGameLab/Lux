#include "BGTSpawner.h"
#include <string>

int BGTSpawner::counter = 0;

bool spawnNewBgt(int bgtID) {
    std::cout << sizeof(mongo::BSONObj) << std::endl;
    //connect to the database
    mongo::DBClientConnection c;
    c.connect("localhost");
    
    string IP = "54.209.204.41";
    std::cout << "HELP! BGTSpawner : 1" << std::endl;

    //Create a BSON object
    BSONObj object = BSON(GENOID<<"IP"<<IP<<"PORT"<<0 << "BGT_ID" << bgtID);
    
    //Create a new document
    c.insert(DATABASE_NAME,object);
    BSONObj bgt_doc = c.findOne(DATABASE_NAME,QUERY("BGT_ID"<<bgtID));

    s_bgt_params_in *bgt_params_in = new s_bgt_params_in;
    
    // get the newly created _id
    bgt_params_in->bgtID = bgt_doc["_id"].toString(false);
   
    std::cout << bgt_params_in->bgtID << std::endl; 
    //string error = c.getLastError();
   
    pthread_t BGT_ID;
    pthread_t SNR_ID;
    pthread_t SUT_ID;
    pthread_t DBW_ID;
    
    // these paths have to be unique names (just have to be unique)
    //const char *bgt_sut_pipeLocation = "./lux_pipe0"; // bgt->sut
    //const char *sut_db_pipeLocation = "./lux_pipe1"; // sut->dbwriter
    //const char *hmbl_snr_pipeLocation = "./lux_pipe2"; // HMBL->SNR
    
    // just for test
    std::string xx = "./lux_pipe" + std::to_string(BGTSpawner::counter+10);//.c_str();
    std::string yy = "./lux_pipe" + std::to_string(BGTSpawner::counter+11);//.c_str();
    std::string zz = "./lux_pipe" + std::to_string(BGTSpawner::counter+12);//.c_str();

   // BGTSpawner::counter++;


    std::cout<<xx<<std::endl;
    std::cout<<yy<<std::endl;
    std::cout<<zz<<std::endl;

    const char *bgt_sut_pipeLocation = xx.c_str(); // bgt->sut
    const char *sut_db_pipeLocation = yy.c_str(); //"./lux_pipe1"; // sut->dbwriter
    const char *hmbl_snr_pipeLocation = zz.c_str(); //"./lux_pipe2"; // HMBL->SNR
    
    if(mkfifo(bgt_sut_pipeLocation, 0666) >= 0){
        bgt_params_in->pipe_w = bgt_sut_pipeLocation;
    }
    
    if(mkfifo(hmbl_snr_pipeLocation, 0666) >= 0){
        bgt_params_in->pipe_hmbl = hmbl_snr_pipeLocation;
    }
    std::cout << bgt_params_in->pipe_hmbl << std::endl; 
    //Spawn a Battleground thread
    pthread_create(&BGT_ID,NULL,BattleGround::spawn, bgt_params_in);
    
    
    s_sut_params_in *sut_params_in = new s_sut_params_in;
    sut_params_in->pipe_r = bgt_sut_pipeLocation;
    
    if(mkfifo(sut_db_pipeLocation, 0666) >= 0){
        sut_params_in->pipe_w = sut_db_pipeLocation;
    }
    
   //Spawn a SUT thread
    pthread_create(&SUT_ID,NULL,SendUpdate::spawn, sut_params_in);
    
    s_dbWriter_params_in *dbWriter_params_in = new s_dbWriter_params_in;
    dbWriter_params_in->pipe_r = sut_db_pipeLocation;
    
    //Spawn  DBWriter thread
    // pthread_create(&DBW_ID,NULL,DBWriter::spawn, dbWriter_params_in);
    
 
    s_snr_params_in *snr_params_in = new s_snr_params_in;
    snr_params_in->pipe_r = hmbl_snr_pipeLocation;
    //Spawn a SNR thread
    pthread_create(&SNR_ID,NULL,SendNewRelevant::spawn, snr_params_in);   
    
    return true;
}



int main(){
    bool spawnOne = false;
    while(true){
        if(!spawnOne){
            spawnNewBgt(0);
            spawnOne = true;
        }
    }
    return EXIT_SUCCESS; 
}
