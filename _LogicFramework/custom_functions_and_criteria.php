<?php

/* Custom criteria */
function createAndInsertCriteria($db) {
    $criteria_collection = new MongoCollection($db, 'criteria');
    
    /* Create criteria */
    $criteria = array();
    array_push($criteria,   
        /* Testing of criteria for $gt */
        array(
            'update' => array(
                'pop' => array('$gt' => 20000) 
                ),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'function_name' => 'popChanged',
            'name' => 'updatePhillyPopGreaterThan20k'
            ),
        /* Testing of criteria for $lt */
        array(
            'update' => array(
                'pop' => array('$lt' => 31000)
                ),
            'query' => array('city' => 'PHILADELPHIA'),
            'collection_name' => 'zips',
            'function_name' => 'popChanged',
            'name' => 'updatePhillyPopLessThan31k'
            )
        /* Testing of criteria for $lte */
        array(
            'update' => array(
                'pop' => array('$lte' => 24000)
                ),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zips',
            'function_name' => 'popChanged',
            'name' => 'updateNewJerseyPopLessOrEqual24k'
        )
        /* Testing of criteria for $exists*/
        array(
            'update' => array(
                'pop' => array('$lte' => 24000,
                    '$exists'=>true)
                ),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zips',
            'function_name' => 'foundValue',
            'name' => 'foundRequestedValues'
        )
        /* Testing of criteria $and */
           'update' => array(
                'pop' => array('$and' =>)
                ),
            'query' => array('city' => 'NEW JERSEY'),
            'collection_name' => 'zips',
            'function_name' => 'popChanged',
            'name' => 'blah blah'
         // TODO: Ceate other criteria here for other 
        // MongoDB special cases e.g. $gt, $lt, $lte
        // $exists, $and, $or, etc.
    );

/* Insert criteria */
foreach($criteria as $criterion) {
    $criteria_collection->insert($criterion);
}
}

/* Custom functions */
function popChanged($document, $upsert) {
    echo 'Population has been changed!' . PHP_EOL;
}

function anyUpsert($document, $upsert) {
    echo 'Upsert!' . PHP_EOL;
}

function foundValue($document, $upsert) {
    echo 'The value requested has been found' . PHP_EOL;
}

// TODO: Create other functions here as needed by added criteria

?>
  
