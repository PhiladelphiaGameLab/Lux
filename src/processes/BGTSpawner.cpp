#include "BGTSpawner.h"
#include "BattleGround.h"
#include "SendUpdate.h"
#include "DBWriter.h"
#include "SendNewRelevant.h"

bool spawnNewBgt(int bgtID){
    //connect to the database
    mongo::DBClientConnection c;
    c.connect("localhost");
    
    //Create a BSON object
    BSONobj object = BSON(GENOID<<"IP"<<IP<<"PORT"<<0 << "BGT_ID" << bgtID);
    
    //Create a new document
    BSONObj bgtDoc = c.insert(DATABASE_NAME.COLLECTION_NAME,object);
    
    s_bgt_params_in bgt_params_in;
    
    // get the newly created _id
    bgt_params_in.bgtID = bgtDoc["_id"].String();
    
    //string error = c.getLastError();
    
    
    // these paths have to be unique names (just have to be unique)
    const char *bgt_sut_pipeLocation = "/temp/lux_pipe0"; // bgt->sut
    const char *sut_db_pipeLocation = "/temp/lux_pipe1"; // sut->dbwriter
    const char *hmbl_snr_pipeLocation = "/temp/lux_pipe2"; // HMBL->SNR
    
    if(mkfifo(bgt_sut_pipeLocation, 0666) == 0){
        bgt_params_in.pipe_w = bgt_sut_pipeLocation;
    }
    
    if(mkfifo(hmbl_snr_pipeLocation, 0666) == 0){
        bgt_params_in.pipe_hmbl = hmbl_snr_pipeLocation;
    }
    
    //Spawn a BGT thread
    pthread_create(&BGT_ID,NULL,(void *) &BattleGround:spawn, (void *) &bgt_params_in);
    
    
    s_sut_params_in sut_params_in;
    sut_params_in.pipe_r = bgt_sut_pipeLocation;
    
    if(mkfifo(sut_db_pipeLocation, 0666) == 0){
        sut_params_in.pipe_w = sut_db_pipeLocation;
    }
    
    //Spawn a SUT thread
    pthread_create(&SUT_ID,NULL,(void *) &SendUpdate:spawn, (void *) &sut_params_in);
    
    s_dbWriter_params_in dbWriter_params_in;
    dbWriter_params_in.pipe_r = sut_db_pipeLocation;
    
    //Spawn  DBWriter thread
    pthread_create(&DBW_ID,NULL,(void *) &DBWriter:spawn, (void *) &dbWriter_params_in);
    
    
    s_snr_params_in snr_params_in;
    snr_params_in.pipe_r = hmbl_snr_pipeLocation;
    //Spawn a SNR thread
    pthread_create(&SNR_ID,NULL,(void *) &SendNewRelevant:spawn, (void *) &snr_params_in);   
    
    return 1;
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
