<?php

/* Custom criteria */
function createAndInsertCriteria($db) {
    $criteria_collection = new MongoCollection($db, 'criteria');
    
    /* Create criteria */
    $criteria = array();
    array_push($criteria,   array(
                                'update' => array(
								    'pop' => array('$gt' => 20000) 
							    ),
                                'query' => array('city' => 'PHILADELPHIA'),
                                'collection_name' => 'zips',
                                'function_name' => 'popChanged',
                                'name' => 'phillyPopChangedToGt20K'
                            ),
                            array(
                                'update' => array(),
                                'query' => array(),
                                'collection_name' => 'zips',
                                'function_name' => 'anyUpsert',
                                'name' => 'anyUpsert'
                            )
                            
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

// TODO: Create other functions here as needed by added criteria

?>
