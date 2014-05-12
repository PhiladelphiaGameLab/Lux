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
        // spawn a new thread, and pass in what port it should be on
        // save a new document to mongo with the port & basic info on the BGT
        // duplicate the respective base collection from mongo
        // update the findBGT class to include this BGT
            // will need to store in mongo so that it can be accessed staticly across multiple processes
}
