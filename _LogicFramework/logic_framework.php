<?php

set_include_path(getcwd());
include_once('ArrayQuery/src/ArrayQuery.class.php');
include_once('custom_functions_and_criteria.php');

function combineAndQuery($query1, $query2, $collection) {
	$results = $collection->find(array('$and' => array($query1, $query2)));
	return $results;
}

function doesMatchQuery($document, $query) {
    $document_db = new ArrayQuery(array($document));
    print_r($document);
    print_r($query);
	$results = $document_db->find($query);
	if (!empty($results)) {
		return true;
	}
	return false;
}

/*
 * Functionally equivalent to doesMatchQuery but uses an actual MongoDB
 * Instead of the ArrayQuery class that implements MongoDB-like queries on
 * single documents. Used as a point of comparison to see if the performance
 * costs of inserting into a DB warrant relying on the little-tested ArrayQuery
 * class.
 */
function doesMatchQueryWithDatabase($document, $query, $collection) {
    
}
/*
 *
 * @param [in] functions	an array of functions, each function corresponding
 * 							to a criteria, to be executed when that criteria
 *							is met
 *
 * @param [in] criteria		an array of criteria that an upsert must meet
 * 							in order for its corresponding function to be
 * 							executed. criteria have 3 members:
 *							
 *							criteria[i] => {
 *								'update" => <JSON-object>,
 *								"query" => <JSON-object>,
 *							}
 *							
 *							"query" is a JSON-object in the form of a MongoDB
 *							query, that describes the "what"--what files does
 *							this criteria care about?
 *							
 *							"update" is a JSON-object in the form of a MongoDB
 *							query, that describes the "how"--how must those
 *							files be updated to meet the criteria?
 *
 *							"collection_name" is the name of the MongoDB
 *							collection the relevant documents are stored in
 * 
 * @param [in] upsert		an array representation of an upsert operation.
 *							stores three key-value pairs:
 *								
 *								"update" => <JSON-object>
 *								"query" => <JSON-object>
 *								"collection_name" => <string>
 *
 *								"update" is a JSON-object that represents
 *								the changes to be made by the upsert.
 *								
 *								"query" is a JSON-object, in the form of a
 *								MongoDB query, that describes which documents
 *								will be upserted.
 *								
 *								"collection_name" is the name of the collection
 *								in which the documents to be upserted are
 *								found
 */
function preProcessUpsert($upsert, $db) {
	$criteria_collection = new MongoCollection($db, 'criteria');
	# TODO: should criteria be able to specify more than one collection?		
	$criteria = $criteria_collection->find(array('collection_name' => $upsert['collection_name']));
	
	if (!empty($criteria)) {
		$upsert_collection = new MongoCollection($db, $upsert['collection_name']);
	    echo "Found criteria for the given collection..." . PHP_EOL;	
	    foreach ($criteria as $criterion) {
		    $upsert_query = $upsert['query'];
		    $criteria_query = $criterion['query'];

			if (!empty($upsert_query)) {
                echo "Upsert is modifying existing documents..." . PHP_EOL;
				$matched_docs_to_modify = combineAndQuery(
											$upsert_query, 
											$criteria_query,
											$upsert_collection	
										);
				if (!empty($matched_docs_to_modify)) {
                    echo "Upsert is modifying existing documents for the given criteria..." . PHP_EOL;
                    if (doesMatchQuery(	$upsert['update'],
										$criterion['update'])) {
						echo "Criteria tripped" . PHP_EOL;
						foreach ($matched_docs_to_modify as $matched_doc) {
							call_user_func(	$criterion['function_name'], 
											$matched_doc, 
                                            $upsert );
						} 
					}
				}	
			} else {
				// upsert is inserting a new document, so it's not modifying
				// any existing files 
                echo "Upsert is inserting a new document..." . PHP_EOL;
				if (doesMatchQuery($upsert['update'], $criterion['query']) and
					doesMatchQuery($upsert['update'], $criterion['update'])) {
					echo "Criteria tripped" . PHP_EOL;
                    // TODO: mention in docs that criteria functions are called
                    // once per document update, not once per upsert
                    foreach ($matched_docs_to_modify as $matched_doc) {
						call_user_func(	$criterion['function_name'], 
										$matched_doc, 
										$upsert);
					}
				}
			}
		}
	}
}

?>
  
