<?php

set_include_path(getcwd());
include_once('logic_framework.php');

/*
* Builds and returns test cases
*/
function getUpserts() {
    $upserts = array();
    array_push($upserts,  

        /* This should meet the criteria for $gt - greater than */
        array(
            'update' => array('pop' => 25000),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'name' => 'changePhillyPopsTo25K'
            ),
        /* Test to see if function is not called because criteria for $gt is not met */
        array(
            'update' => array('pop' => 19999),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'name' => 'changePhillyPopsTo19k'
            ),
        /* This should meet the criteria for $lt - less than */
        array(
            'update' => array('pop' => 28000),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'name' => 'changePhillyPopsTo28k'
            ),
        /* Test to see if function is not called because criteria for $lt is not met */
        array(
            'update' => array('pop' => 32000),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'name' => 'changePhillyPopsTo32k'
            ),
        /* This should meet the criteria for $lte - less than or equal to */
        array(
            'update' => array('pop' => 13000),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zip',
            'name' => 'changeNewJerseyPopTo13k'
            ),
        /* Test to see if function is not called because criteria for $lte is not met */
        array(
            'update' => array('pop' => 25000),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zip',
            'name' => 'changeNewJerseyPop25k'
            ),
        
        /* This should meet the criteria for $exists using $lte */
        array(
            'update' => array('pop' => 23000),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zip',
            'name' => 'updatePopto23KIfExists'
            ),
        /* This should not meed the criteria for $exists using $lte */
        array(
            'update' => array('pop' => 25000),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zip',
            'name' => 'updatePopto25kIfExists'
            ),
        )
        // TODO: Create upserts for tripping
        // other criteria here
    );    
    return $upserts;
}

/*
* Runs an array of test cases on a database
*/
function testLogicFramework($upserts, $db) {
    foreach ($upserts as $upsert) {
        preProcessUpsert($upsert, $db);
    }
}

/*
* Get a db cursor for the test database and perform each test case on it
*/
$mongo = new MongoClient();
$db = $mongo->selectDB('scratch');
createAndInsertCriteria($db);
testLogicFramework(getUpserts(), $db);

?>
  
