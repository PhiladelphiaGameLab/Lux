# Scoreboard
----------

Scoreboard is a collection on the database that store metrics relating to the score of an individual user. Metrics can be integers or pointers to assets held by the user. Note that the assets stored in the collection are NOT the actual objects, simply pointers. Additionally, a user's complete profile is held in a different collection on the database.

Overall structure of a user's scoreboard is as follows:

{

    '_id' : ~ // this is the mongoID assigned to a user's score document upon creation
    'userID' : ~ // this is the mongoID assigned to a user's complete information in a separate collection
    'score' : { // the score array holds key:value pairs of types and integers that are not level specific
        'raw' : 0
    }
    'levels' : { // the levels array is an array of subdocuments that contain information about a level a user has earned
        {
            '_id' : ~ // a mongoID for this subdocument, needed for querying specific levels
            'levelID' : ~ // a custom levelID for development ease and querying
            'levelname' : ~ // the name of the level
            'levelposition' : ~ // an integer representing the order of the levels
        }
    }
    'metrics' : { // the metrics array is an array of subdocuments that hold metrics regarding a particular level
        {
            '_id' : ~ // a mongoID for this subdocument, needed for querying specific metrics
            'levelID' : ~ // a custom levelID for development ease and querying
            // add in whatever is needed here
        }
    }
    'assets' : { // the assets array holds key:value pairs of assets and quantity; assets are not level specific
        'GoldenBoots' : 1
    }
    
}

Functions required for Scoreboard are:

* Add asset
* Remove asset
* Adjust asset
* Get assets
* Add score
* Remove score
* Adjust score
* Add level
* Remove level
* Adjust level
* Add metric
* Remove metric
* Adjust metric
* Get metric
* Get user level
* Get user scores
* Get user scoreboard
