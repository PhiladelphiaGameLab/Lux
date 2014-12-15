<?php

include_once('../core/upsert.php');

class Asset {

    public $assetID = NULL;
    public $assetSource = NULL;
    public $uploadDate = NULL;
    public $upvotes = [];
    public $downvotes = [];
    public $ratings = [];
    public $comments = [];

    function __construct($assetID, $assetSource = NULL) {
        $this->assetID = $assetID;
        $this->assetSource = $assetSource;
        $this->uploadDate = date('mdY');
    }

    function getUpVotes() {
        $count = count($this->upvotes);
        return $count;
    }

    function getDownVotes() {
        $count = count($this->downvotes);
        return $count;
    }

    function getRating() {
        $sum = 0;
        foreach ($this->ratings as $userRating) {
            foreach ($userRating as $userID => $rating) {
                $sum += $rating;
            }
        }
        $averageRating = $sum / count($this->ratings);
        return $averageRating;
    }

}

class Comments {

    public $assetID = NULL;
    public $commentID = NULL;
    public $userID = NULL;
    public $commentText = NULL;
    public $parentCommentID = NULL;
    public $upvotes = [];
    public $downvotes = [];

    function __construct($assetID, $userID, $parentCommentID = NULL) {
        $this->assetID = $assetID;
        $this->userID = $userID;
        $this->parentCommentID = $parentCommentID;
    }

}

class User {

    public $userID = NULL;
    public $upvotes = [];
    public $downvotes = [];

    function addComment() {

    }

    function removeComment() {

    }

    function editComment() {

    }

    function addAssetRating() {

    }

    function removeAssetRating() {

    }

    function editAssetRating() {

    }

    function addAssetUpvote() {

    }

    function removeAssetUpvote() {

    }

    function addAssetDownvote() {

    }

    function removeAssetDownvote() {

    }

}
/* This should all be handled by functions in the directory? */


// Testing below

echo "\n";

/* $thing = new Asset('newPicture');
echo $thing->assetID."\n";
echo $thing->uploadDate."\n";
*/

$a = [['bob'=> 2],['jack' => 5 ]];
$sum = 0;
$avg = 0;
$count = 0;
foreach ($a as $entry) {
    foreach ($entry as $key => $value) {
        echo "Key: $key, Value: $value\n";
        $count += 1;
        $sum += $value;
        $avg = $sum / $count;
        echo "Count: $count, Sum: $sum, Average: $avg\n";
    }
}

echo count($a);

echo "\n";