<?php
include_once('../core/upsert.php');
// Format a rating into a usable number for the system

$assetID = $_POST["assetID"];
$userID = $_POST["userID"];
$rating = $_POST["rating"];

$results = [
    "assetID" => $assetID,
    "userID" => $userID,
    "rating" => $rating
];

$doc = json_encode($results);

upsert(
        array(
                "collectionName" => "AssetVotingAndRating"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
                ,"doc" => $doc
        )
);

/*
 So many questions...

1. I added an entry to the array passed to upsert called doc. Can upsert take more than the four initial arguments?
2. How are we getting the identity of the user? I assumed it was posted because I couldn't find session code anywhere in the core directory. Is it post?

 */
?>



