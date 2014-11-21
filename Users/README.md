# User's API

The Users Api should have the same base functions as all other API's. 

The User's should differ only in that it will add a node to the social network as well as to the Mongo Db when a new user is created, or remove said node when a User is deleted. This is the same functionality as the Groups API, and should store the Neo4j id in the Mongo database, and the Mongo id in the Neo4j Database
