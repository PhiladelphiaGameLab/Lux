<?php

set_include_path('/Users/alexcannon/Sites');
include_once('logic_framework.php');

/*
 * Builds and returns test cases
 */
function getUpserts() {
    $upserts = array();
    array_push($upserts,  array(
                                'update' => array('pop' => 25000),
							    'query' => array('city' => 'PHILADELPHIA'),
                                'collection_name' => 'zips',
                                'name' => 'changePhillyPopsTo25K'
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
