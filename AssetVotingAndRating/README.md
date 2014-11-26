# Asset Voting And Rating System

The AssetVoting and Rating System should be able to add Comments and Ratings of arbitrary depth to an asset. You should be able to make a query that specifies an Asset which is being referenced, and a comment or rating on that Asset (or the highest level) that can be rated. 

The Structure should also users to rate the Asset, but also Rate and Comment on Comments or Ratings. 

The functions should be:

* Add Comment
* Remove Comment
* Edit Comment
* Add Rating 
* Remove Rating
* Edit Rating
* Add Upvote
* Remove Upvote
* Add DownVote
* Remove DownVote

Users should be able to leave only 1 rating on the original, or on a comment, but the Comments they can add as many on as many levels as they wish. 

This means that we should also be tracking who left the comment via the User's id. 
The System is good for e-commerce, or for content sharing websites/Applications. 
