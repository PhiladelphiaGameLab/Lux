<?php

// Turns an array of MongoIDs into an array of the corresponding documents
// Required inputs:
//  $MongoIdArray as an array of MongoIDs
//  $collectionName as the name of the collection you'd like to search

function resolve($MongoIdArray, $collectionName) {
    $array = array();
    $db = new Db();
    $collection = $db->selectCollection($collectionName);
    foreach ($MongoIdArray as $value) {
        if (MongoId::isValid($value)) {
            $doc = $collection->findOne(
                $query = array(
                    '_id' => $value
                )
            );
            array_push($array, $doc);
        } else {
            array_push($array, $value);
        }
    }
    return $array;
}  
