# Assets 
The assets should be able to altered without any checks, since almost anything can be stored as an asset in this collection. That means that it should have: 

* Upsert
* Copy
* Query

The Query should Subscribe the user to a call for use in a later script so that real time updates can be sent. 

The Upsert should Publish that Change to the Publishes database in order for it to be sent out to the user's who are Subscribed. 


