#include "BGTSpawner.h"

void BGTSpawner::spawn(struct pipe params_in){
    // this class will find if BGTs need to be spawned or something

    // Check enviorment variables to see what the game type is

    // game types include:
        // Battle Arena
        // Static persistant world (in which case this should spawn them and then go into "Make sure they are still running Mode"
        // Dynamic persistant world
        // according to Alex there are like 50 ways to do this


    // query Find BGT class to see the number of BGTs online

    // check number of clients on each BGT

    // decide if a new one needs to be spawned
    
    //connect to the database
    mongo::DBClientConnection c;
    c.connect("localhost");
    
    //Create a BSON object
    BSONobj object = BSON(GENOID<<"IP"<<IP<<"PORT"<<0);
    
    //Create a new document
    c.insert(DATABASE_NAME.COLLECTION_NAME,object);
    
    string error = c.getLastError();
    
    //Spawn a BGT thread
    pthread_create(&BGT_ID,NULL,(void *) &BattleGround:spawn, (void *) &params_in);
    
    //Spawn a SUT thread
    pthread_create(&SUT_ID,NULL,(void *) &SendUpdate:spawn, (void *) &params_in);
    
    //Spawn  DBWriter thread
    pthread_create(&DBW_ID,NULL,(void *) &DBWriter:spawn, (void *) &params_in);
    
    //Spawn a SNR thread
    pthread_create(&SNR_ID,NULL,(void *) &SendNewRelevant:spawn, (void *) &params_in);
        // spawn a new thread, and pass in what port it should be on
        // save a new document to mongo with the port & basic info on the BGT
        
        // duplicate the respective base collection from mongo
        // update the findBGT class to include this BGT
            // will need to store in mongo so that it can be accessed staticly across multiple processes
}
