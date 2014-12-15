# Lux 2.0

This version of Lux is designed to still function as a multi-player communication system, while also encompassing all back-end functionality necessary for a single player game, a website, or a multi-person interactive system. 

**These are in the relative order That They should be tackled- or at least each of the tasks should have an outline, and then move onto the next, and then move back to improve each section**

## Authorization
Authorization is an important aspect of any online system- verifying a user's authenticity, and ensuring continuity throughout the system. In order to facilitate this, we will need to provide both an OAuth 2.0 system, and custom authorization (for authorizing when a secondary authorization provider is not ideal). 

* OAuth 2.0
	* Start with Google and provide a custom link that can be clicked to enable simple OAuth functionality by only providing the basic keys necessary on the client side. Once they have filled in a simple form, the OAuth will take care of everything else, including saving their entire user profile that is available from the scopes. 
	* This can be a PHP system based off the current time tracker- which we will provide a custom link for the developer to use- which will redirect to a specific page (or serve a different home page if they are logged in)
	* The Log-in system should also create a user session so that user variables can be referenced.
	* If the user does not have an account, one should be created for them, which includes both a user info account- linked to a social network account- linked to a transaction account- linked to a  leaderboard ect.
		* Account linking should be done separately from all those other things, so that each user identifier is stored in a single unified location
* Authorization should be handled the same way as OAuth, except instead of jumping through all the OAuth hoops, it is handled on the server. 
	* Pass all the login info to a single script which verifies the user. 

* Save OAuth info for use in posting later?
* API to post to the Users facebook/twitter page without worrying as long as they are logged in via API

##### Priority : 1
##### Time : 4 Days
##### Language: php, node.js
##### Database: Mongo

## Asset Storage System & Uploading
This system refers to assets of several kinds and is at the core of the Lux system, holding anything from Game elements, items for sale, elements of a reference database, or images being voted on. The use of a Mongo database is essential for this portion, allowing self organizing, and self documenting JSON objects to be dynamically uploaded and stored, or statically stored for use in a game or reference database.

* Web form for uploading
* Static elements can be copied but not altered by the system
	* Can be copied to Dynamic Elements though
* Dynamic elements can be altered by system
	* All Dynamics start at static until a system copy
	* Global assets are dynamic and changes are reflected for all users
	* User assets are dynamic and changes are reflected for only 1 user
	* Grouped assets are dynamic and changes are reflected for a SN group, or users matching a specific criteria. 
* In order to accommodate different global levels, copies of documents will need to be created for each level. 
	* *This is going to lead to a ton of documents and maybe we need a better method*
* Dynamic assets will need to be updated by both an HTTP, websocket and a UDP method for real time updates in collaborative systems
* Assets will need to be retrieved by both HTTP, websocket and UDP methods
	* Assets can be retrieved by both querying, or _id
* The websocket and UDP methods will need to track who has what assets and update clients about updates to assets. 
* WS and UDP will also need to be able to update clients based on a query- updating all team members of a target or updating all players of a clock extension... ect.
* Users can also subscribe to information matching a query and then have any updates to messages matching that query passed to them directly
	* Query to match any documents in a game, any documents in the vicinity, or any documents on the current page (bids on ebay or something)

### Who Knows What?
Tracking document knowledge is key to this system, from knowing that a user is looking at a page and that page has 10 elements that all are being constantly updated, to knowing that a user is in the middle of a game with 10 other clients- all of whom need to be made aware of any changes or updates in the game. It is important not only to know who as made a HTTP request, but also a websocket/UDP request, or who has updated has updated a document recently. Figuring this out, and allowing users to update the document while still allowing for every other relevant user to see these updates is key. However, some users may be subscribed to certain information, while others are subscribed to different information- so the system is not based on constantly knowing who is updated on what. 

* For clients Listening on a query or multiple queries or specific documents:
	* Pub/Sub model
		* User's add themselves to a list of Subscribers 
		* System checks each message against a set of Publisher rules
		* If a user sets up a new Publisher rule then it is added and people can Subscribe to it.
		* Publish rules can be set for individuals, groups, or global
		* Same system can be used for Logic Framework.
* For systems where only a small group needs to know the updates, you can subscribe to a Graph Node and send updates to that Graph Node
* For Large persistent Map we can reuse the HMBL, but you subscribe to updates on a bucket.

##### Priority : 1
##### Time : 11 Days
##### Language: php, node.js, python
##### Database: mongo

## Logic Framework
The logic Framework is based on a query system, where any document updated in the Asset Database is tested against the logic framework queries and it is decided which documents need to be run through which logic system. This is done as soon as a document arrives, and the logic framework has access to the Asset Database that the current Asset belongs to (Global, user, ect). This is useful for determining if a grenade killed someone, or if a team gets to acquire a flag (idk). Outside of games, it could be used to see if an item has sold out, or if a user is matched with another user on a dating site. 

* Logic framework is called anytime that the Asset Database API is accessed
* Can be Python, Node.js, php, java, C++.... idk.
*  Passes in a reference to the database, and the Asset being updated that matched the Query
* Logic framework rules are set through the Online interface

##### Priority : 2
##### Time : 7 Days
##### Language: php, node.js, python
##### Database: N/A

## Social Network 
The system should be integrated with a custom social network that functions via HTTP requests and is integrated with a Graph Database such as Neo4j

* This can all be PHP since it is all HTTP requests.
	* Can be based off Justin's exisiting API
	
##### Priority : 1
##### Time : 5 Days
##### Language: php
##### Database: Neo4j

## Auto-File Upload handling
File upload is an important aspect of the internet and should not be overlooked or underused. File uploads can be single files, multiple files, large files, synchronous, or asynchronous. Lux should be able to handle all of these and more with a built in file handling system that saves files on the server when they are uploaded from the clients application. 

* Built into all other APIs
* synchronous and asynchronous
* large file splitting
* save to designated path

##### Priority : 2
##### Time : 14 Days
##### Language: php, node.js
##### Database: N/A

## User Achievements
Achievements help to gamify anything, and as such, are an important part of the system. User Achievements should be stored in separate section then user information, to prevent the user information document from growing uncontrollably. 
* Tracking progress towards achievement
* Automatically granting an achievement when it is earned
* Config file that can be changed dynamically from the web interface 

##### Priority : 2
##### Time : 7 Days
##### Language: php
##### Database: mongo

## Facilitated Communication
Users can either receive messages in real-time by continually listening on a message port dedicated to them, or they can use an HTTP request to retrieve all messages. 

* Text
* Video
* Audio
* System Code (for sending gifts)

In order to facilitate certain newer functionality- messages can optionally be sent with temporary nature so that certain applications do not need to account for this functionality:

* Self-destruct after being opened
* Expire a certain period of time after being sent
* Locked unless either sender or receiver account matches certain criteria

##### Priority : 3
##### Time : 19 Days
##### Language: php, node.js
##### Database: mongo most likely

## Transaction Database
Transaction database is the bank of the system- ensuring that all transactions and exchanges are performed without glitch. This can be heavily based of a combination of banking systems, western union and the existing Lux transaction server. The Transaction server should be built to be as complete as possible, and also as un-hackable as possible- since all transactions, and trades will be going through this system. The System is good for anything the user wishes to use it for, but will not interfere with any other systems.

* Full API for trading, acquiring, selling, transferring 
* Optionally require Pin or password for transaction verification
* Stores all pending, and previous transactions for reference
* Accounts and Subaccounts for all users
* Linked to Social Network graph with ability for any node on Social Network Graph to create an account
* HTTP only for now.

##### Priority : 3
##### Time : 10 Days
##### Language: php->python or something later for security
##### Database: mongo as a hierarchical database

## Asset Voting and Ranking Systems
The asset voting system may not be used in games, but is certainly a common model on the internet- and possibly in games. Any Asset should be linked to a ranking, which includes either votes or stars. Assets can be commented on, and comments are then treated as assets that can be voted or stared. 

* Linked to Social Network
* Identity not required for vote/star/ect, but is recommended
* Users can rank an asset personally and that is stored, but so is the global ranking
* Comments are stored by reference and treated as assets
* Querying by ranking, age descending, age ascending, ect... 
	* Querying by Social Network
* Updates should be by Websocket, HTTP, and UDP
	* Same update model as the Asset Storage System
		* Querying, subscription, update if you have asset
* Rankings and Stars can be stored in mysql database 

##### Priority : 4
##### Time : 10 Days
##### Language: php, node.js, python
##### Database: mongo, mysql

## Match Making API 
The match making API is important not just for games, but also for random chat applications, and various other instances where being paired with a random person is good

* Can be completely HTTP
* Several methods including:
	* Simple Queue
	* Social Media Based
	* Social Network Based
	* ~Social Media Based
	*  Limited repeat
	* One sided
	* ect

The match making API should provide both a blocking and non-blocking method which can wait for a match to be returned, or you can make several queries to find if a match has been made. This should be a GET variable

##### Priority : 4
##### Time : 6 Days
##### Language: php
##### Database: mysql

## Leader Board
This can be HTTP based, and should be a simple API for updating and retrieving the user's scores, top scores, and ect. as well as the Global top scores and leader board.

* Should have social media posting 
* Sorting by Social Network Graph
* High score and such
* whatever other features this needs

##### Priority : 4
##### Time : 6 Days
##### Language: php
##### Database: mysql

## Email & Newsletter system
Email one user or all users, send a generic message- or a specific one, at the end of the day, every system needs to email someone about something. 

* HTTP Request to send email with specific message
* Add links with code to verify it is from an email and you are the recipient
* Newsletter to send to all users or some based on query
* Automail users on a click
* User's can be auto subscribed to an event matching a query being triggered
* User's can be emailed after certain periods of inactivity (1 day, 3 weeks, 6 months, ect..)
* Web interface to manage emails schedualed

##### Priority : 6
##### Time : 7 Days
##### Language: php
##### Database: Mongo


## Webserver and CPanel
This is relatively low priority at this point, but will be very time consuming and annoying- so... there is that. 

example:
http://x3demoa.cpx3demo.com:2082/cpsess265324844/frontend/x3/index.html?post_login=65980757386108
User ID: x3demoa | Password: x3demoa

* FTP or Git file update manager (including github)

##### Priority : 5
##### Time : 13 Days
##### Language: php
##### Database: mysql


## Code Generation System
With the increased need for offline interaction between users, the generation of Codes and QR Codes for interaction has become very important. 

* Codes are generated and can be linked to accounts or assets
* Codes can be used as a URL for pointing to a certain page or something
* I haven't really put much thought into this yet.
	* Think Uber and Scavunt- two seperate uses for the same idea
* Generate String Code "8avft" or QR Code- which translates to the string
* Code can be attached to link or an action that applied (lowering the price of something in the users pending transactions)

##### Priority : 5
##### Time : 4 Days
##### Language: php
##### Database: mysql

## Events and Triggers

## Push Notifications

## in-app purchases

## Nessicary Plugins/Wrappers/Libraries
* Unity
* Javascript
* Mobile
* Console
* E-commerce


# API (HTTP)
**Access Token is always required as a parameter**
## System Calls
#### Upsert (and delete)
* Parameters (POST)
	* Document Id or Query String
	* New Document or Document Updates
		* New Documents must contain a group id
		* If Document is left out, then document of that Id will be deleted
	* Priority (Optional)
* Functionality
	* Inserts new document into Asset Collection or updates the existing document
	* Inserts Updated Document into Queue
*  Output
	*	Success Message, Document

#### Query
* Parameters
	* Query string
	* Primary (optional)
* Functionality
	* Queries the Database for matching documents
	* If Primary, then find users previous primary and delete it
	* Returns all matching documents as an array
	* Add this query to the Pub/Sub database
*  Output
	* Matching Documents as a JSON array

### Copy
* Parameters
	* Document Id for old document or Query criteria for old documents
* Functionality
	* Copies the Documents and assigns them new _ids
*  Output
	* The New Documents including Ids. 

## Assets
The Assets section is used for tracking and maintaining Assets in the database, assets could be dynamic game objects, assets in a store, lessons on a learning website, or anything else. 

#### Upsert (and delete)
* Calls System Call
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
	* Puts document in Queue to be sent out to subscribers
	* Can take specific fields and update for all documents matching query
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query

#### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query
	* Subscribes the Querier to the query they are requesting

### Copy
* Calls System Call
* Functionality:
	* Copies a single document, or a query of documents and accepts an update string to edit the document(s)
	* Puts the Copy of the document into the Queue (optionally)

## Group
#### Upsert (and delete)
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
	* Puts document in Queue to be sent out to subscribers
	* Creates a Node in the social Network if the Document is being inserted
		* Adds the Node to the Document Description
	* Can take specific fields and update for all documents matching query
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query/Update
	
#### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query
	* Adds the Queries to the Associated Group if they are not already

### Copy
* Calls System Call
* Functionality:
	* Copies a single document, or a query of documents and accepts an update string to edit the document(s)
	* Puts the Copy of the document into the Queue (optionally)
	* Copies all of the documents in the Asset Database and re-associates them with this group
	* Puts new Group into Queue, does not enQueue Assets
	* If Original copy is associated with SN Group then create a new SN Group accordingly

## Pub/sub

### Upsert
* Calls System Call
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
		* The document given is the Query being updated or an extra subscriber/remove a subscriber
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query
	* Appends a Timestamp and all that stuff that is nessicary

### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query

### Unsubscribe
* Call System Upsert Function
	* Format a Query to remove the client from the Query they have specified
### Subscribe
* Call System Upsert Function
	* Format a Query to Push the client into the Query they have specified with the timestamp

## Queue
### Upsert
* Calls System Call
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
		* Puts the Document update in the data section of the Queue Document
	* Can take specific fields and update for all documents matching query
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query

### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query

## Authorization
### OAuth (google, facebook, twitter, etc)
* Parameters
* Functionality
*  Output
### System Account
* Parameters
* Functionality
*  Output
### Lux Account
* Parameters
* Functionality
*  Output
## User
### Upsert (or Delete)
* Calls System Call
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
	* Can take specific fields and update for all documents matching query
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query

### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query

## SocialNetwork (Neo4j)
### Upsert Node (or delete)
* Parameters
* Functionality
*  Output
### Upsert Edge (or delete)
* Parameters
* Functionality
*  Output
### Query
* Parameters
* Functionality
*  Output
## Transaction
### Make Trade
* Parameters
* Functionality
*  Output
### Make Pending
* Parameters
* Functionality
*  Output
### Add Asset
* Parameters
* Functionality
*  Output
### Remove Asset
* Parameters
* Functionality
*  Output
### Cancel Trade
* Parameters
* Functionality
*  Output
### Trade w/ NPC
* Parameters
* Functionality
*  Output
### History
* Parameters
* Functionality
*  Output
### Current
* Parameters
* Functionality
*  Output


----------


### Sub accounts
#### Upsert (or delete)
* Calls System Call

#### Query
* Calls System Call

### Copy
* Calls System Call

## Achievements
### Upsert (or delete)
* Calls System Call
* Functionality:
	* Updates, deletes, or inserts a document that is given as a parameter
	* Can take specific fields and update for all documents matching query
	* Achievements need to have both a reward and a Query as to how they are Acquired
	* Achievements should be automatically rewarded when a user updates a document in the same way a certain number of times. 
	* Returns the Updated/inserted document(s)
	* Input should not need to be formatted as a JSON Query/Update for a short Query

### Query
* Calls System Call
* Functionality:
	* Queries the Collection for the specified document or documents
	* Uses either _id or a query call
	* Can query on a single parameter, and will format the query into a Mongo Query and return all matching documents
	* Can specify the Limit of the number of documents returned
	* Can Specify Ordering, and system will format into a query

### Award
* Functionality:
	* Calls the User Upsert function to add a Ref to this achievement to their Achievement's Array (or creates an achievements array).

### Revoke
* Functionality:
	* Calls the User Upsert function to remove a Ref to this achievement to their Achievement's Array

### Retrieve
* Functionality 
	* Calls the User Query function and then just returns from it their "achievements array"
	* Does this for either a user _id that is passed in, or assumes you want the current user who is making the request

## Leader board (MySql)
### Upsert (or delete)
* Parameters
* Functionality
*  Output
### Query
* Parameters
* Functionality
*  Output
### Top
* Parameters
* Functionality
*  Output
### Bottom
* Parameters
* Functionality
*  Output

## Match Maker
### Request
* Parameters
* Functionality
*  Output
### Check Pending
* Parameters
* Functionality
*  Output
### Query
* Parameters
* Functionality
*  Output
### Cancel Request
* Parameters
* Functionality
*  Output

## Code Generator
### Get Code
* Parameters
* Functionality
*  Output
### Attach Code to group
* Parameters
* Functionality
*  Output

## Asset Voting and Ranking
### Rate
* Parameters
* Functionality
*  Output
### rm Rating
* Parameters
* Functionality
*  Output
### Up-vote
* Parameters
* Functionality
*  Output
### Down-vote
* Parameters
* Functionality
*  Output
### Comment
* Parameters
* Functionality
*  Output
### Query
* Parameters
* Functionality
*  Output

## Email
### Send To
* Parameters
* Functionality
*  Output
### Contact Form
* Parameters
* Functionality
*  Output
## Facilitated Communication
### Send To
* Parameters
* Functionality
*  Output
### Retrieve From
* Parameters
* Functionality
*  Output
### Retrieve Inbox
* Parameters
* Functionality
*  Output
## Newsletter
### Subscribe
* Parameters
* Functionality
*  Output
### Unsubscribe
* Parameters
* Functionality
*  Output
### Query
* Parameters
* Functionality
*  Output
### Send
* Parameters
* Functionality
*  Output
### Upsert
* Parameters
* Functionality
*  Output
### Cron
* Parameters
* Functionality
*  Output

## Auto File Uploading

## Node.js/Python scripts