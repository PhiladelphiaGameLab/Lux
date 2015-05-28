# lux Software Development Toolkit
## Intro

Lux is focused on creating a more friendly market for start-ups and front-end developers to work in which allows them to be more self-reliant and more focused on developing their technology at their speed. Lux is not just a back-end as a service, but a full suite of products that are integrated into a single package that allows for creation of products, services, and business tools with a strong online presence. Lux is built to be built to grow, every aspect of lux can be easily tied together, while still being treated as a separate entity. This ability to grow and change with it's needs allows lux to be a powerful tool, and  big reason to make open-source a staple in that tool.

### Resources needed
The resources needed to finish lux are a combination of people and time. The time listed below is by no means a 100% accurate estimate, but only an estimation given current knowledge and best guesses on the subject. If priorities 0-4 are met before June 15th, then the lux will be useable as a stand alone product by this summer. My preference would be to complete up through level 5 by this summer, which is a stretch- but might be possible. 


| Priority Level | Description | Jake's Est | Team Mi Exp | Team Max Exp |
|----------------|-------------|------------|-------------|--------------|
|  0 | lux should be use-able to build any simple application with or without client logins with limited end-user specific functionality. These would be primarily static websites or application's who focus on information distribution rather than on functionality (A database website, a registration, or corporate webpage).  | 17 | 34 | 66.5 |
|  1 | lux should be use-able to build any single player game or non-realtime multiplayer game. This would include turn based strategy games, longer term games, and any single player game. | 31.5 | 63 | 123 |
|  2 | lux should be use-able to build any multi-player game with realtime game play functionality, including MMO games and real-time strategy games. This section may need to be revisited to improve functionality- but relatively large sets of user's (on the scale of 5,000) should be able to play this game without server-side interruption *on a ec2 t2.micro instance*. A larger AWS Instance would scale this number appropriately with respect to the number of processors and processor speed. | 41 | 82 | 154.5 |
|  3 | lux should be capable of handling most games or applications, given the correct knowledge and usage.  This also concludes the FOSS portion of lux, and some functionality is possible through this section, but not as simple or as well handled as it would be with the proprietary portions of lux. | 55.5 | 111 | 218.5 |
|  4 | This is the basis for the lux proprietary model and offers the ability to launch and pay for a lux instance through a web interface, so that internal and external developers can begin using lux to create their own applications. | 81.5 | 163 | 293 |
|  5 | This level is meant to add value to lux proprietary on top of the FOSS and the ease of launching a FOSS instance in the form of more APIs that are still generalized for use in all applications.  | 99.5 | 199 | 356.5 |
|  6 | Priority 6 is meant to make launching and creating a non-technology business easier and faster in order to give incentive to use lux proprietary. These are APIs and tools more targeted at solving complex technology problems for non-technology business models. | 125 | 250 | 442.5 |
|  7 | This is meant to make rapid technology development more feasible on lux for less experienced front-end developers, who wish to build an application. The same tools can be used by more experienced developers in order to  more quickly build and deploy games and applications on top of lux. | 155.5 | 311 | 531 |
|  8 | This level is designed to target application's towards more developed companies, making it possible for them to deploy games and application's with an automatically generated API and automated testing procedures, making lux more appealing to large companies. | 185.5 | 371 | 616 |
|  9 | This level is meant to help developers monetize their applications through lux by providing easier access to integrated adverts and advert networks. This means that developers can build application's with passive revenue streams using only lux. | 224.5 | 449 | 724|
| 10 | This level is meant to help small businesses rapidly deploy products on top of lux with a minimal number employees, focusing efforts on functional game or application development. | 258.5 | 517 | 812 |
| 11 | These are only extra functionality to be added to make lux a full and complete package, as well as to increae the number of rapid development uses. This is the lowest level priority because most of these act as (almost) stand alone products which would need full development and are not really within the scope of this project. | $\infty$ | $\infty$ | $\infty$ |

**All of the above Estimations are in 1 person 8 hour days and are cumulative*
**Descriptions are what lux should be capable of doing after this level is complete* 


The specialized skills that may be worth hiring for (All short-stay hires) in the near future are listed below, although some skills may have been overlooked:

* Advanced Multi-Threading and distributed computing
* Advanced HTML/CSS/JS for front-end development
* PayPal/Stripe/ Any Online Payment Gateway system experiance
* Advanced Shell/CLI Programming ability
* UDP/WebSocket optimization
* Real-time Streaming protocols (RTSP) building experience (not just interfacing with an existing)
* Other languages (for implementation's of SDK libraries)

Other skills that are always helpful or **necessary** for lux developers:

* AWS SNS 
* AWS CLI 
* MultiThreading 
* Image Processing 
* **HTTP** 
* **HTML/CSS/JS** 
* UDP 
* RTSP 
* Google 
* AWS S3 
* **php**
* **WebSockets** 
* neo4j 
* MySQL 
* RegEx 
* GitHub 
* **Python** 
* **Mongo** 
* other languages 

### Open-Source
The lux project is an open-source project designed to grow and expand with the needs of the community. Contribution should be limited and monitored through GitHub, and will need a dedicated team to approve pull-requests from non-employee contributors. Affiliated contributors are in-house developers volunteering their time towards the lux project and maintaining the lux repository. The open source area of lux consists of the Back-end library on which applications are built, in this document they fall under the category of "API Calls For Lux".

The Open Source analytics platform is built into the API Calls For Lux, and is attached to it's own server-side processes. The Redux sections will be primarily open source, and given a marketplace around them- giving the ability to sell and profit off of redux functions. The data analytics framework is closed source, and not publicly available for modification. 

The Open Source Community behind Lux will help to ensure that all possible features are developed and included in the lux library. Although any technology could theoretically be built on lux open source alone, the community is encouraged to use the closed source software as well. 

### A note on Overarching goals
Many Backend as a Service providers are focused on singular platforms or application types- hence Game Backend as a Service, and Mobile Backend as a Service. lux is designed to be applied to any sector or industry that it needs to be. Another theme in Backend as a service providers has always been to solve problems by increasing the amount of server-side hardware being utilized. Although this solution is unavoidable- lux should be able to scale well on a small server, and should not need an increase in hardware as it's only solution.

lux, unlike other backend services, should also have a heavy connection to it's analytics that creates an environment which not only services users, but also provides useful feedback about them and their interactions with the system.  
 
### Definitions, Acronyms, Abbreviations, and Terms 
* Developer: A developer working on top of a Lux instance to create an application
* Lux Developer: A developer working to develop Lux
* End-user: An application user who is using a third party application built on Lux by a developer
* lux Based products: Applications and Games Built on Lux.

### Scheme
Basic operations include:

* Upsert
	* Create
	* Edit
	* Remove
* Query

However for most APIs these should be broken down into finer detail, allowing only a few parameters to be altered with any given call. For almost all calls, the user should specify specific key->value changes to be made, and the php should handle those changes. In general, "Add" or "Create" and "Delete" should not be used, instead a function for "Adjust" should be used for editing an existing, and will create if one does not exist, "Adjust" can also remove whatever property with either a "remove" tag, or leaving the update field blank. Unless Specified as `~access_token`, all queries require an access\_token. If access\_token is optional (normally this changes behavior) then `~access_token?` will be used. Any GET request can also be a POST request, however POST requests can not be GET requests in all cases. 

Instead of Query, we should use get and fetch- where fetch is used when return value is dependent on user, and get is deterministic. Check should return a Boolean regarding the query (normally checking to see if a query is true or false)

The major exception to this, is Assets- which should be alterable via a direct Upsert so that they can remain as flexible as possible. For API wrappers (Social), the API should reflect the Social API as well as the users. The goal is to allow front-end developers to only use the lux API to create an entire application, and not need to interface with the server, or any other server, in any other way. 

### This document
This document is for the exclusive use of the lux team. Any alterations should be noted in the comments, and then after discussion the changes can be made. APIs in the first section are ordered by priority, however some leeway is permitted for ease of development purposes.  Please wait until you have discussed a section with the project lead before proceeding to build any aspect. 

Priority is listed in the title of every API, and currently range from 0-8. Priorities 0-3 are considered to be part of the open source project, and 4-8 are considered to be part of the proprietary lux software. Each API is also marked with a difficulty level:

1. Requires Only HTTP & Mongo Writes
2. Requires a class which can be added to other APIs
3. Requires Interaction with a non-Mongo database/third party service 
4. Requires Interaction with non-HTTP protocols to communicate with the End-User
5. Requires server-side scripts to be running which will need to be multi-threaded and optimized 

Priorities are superscripts $^x$, and difficulties are subscripts $_x$. Difficulty levels can be considered as the maximum number of days required to complete the section. 

----------

[TOC]


----------

# API Calls For Lux
## Asset Management API $_1^0$ [done]
The Asset Management API allows users to create Assets and store them in a specific Asset Database. This allows access to the Database via the standard commands, which gives users full access to the collections. The Asset Database can be broken down by collections which are defined by the developer. This Database (and set of collections) are the only ones with arbitrary structure defined by the Developer. 

### API
* Upsert
	* Query || _id
	* Update
	* Remove
* Adjust- Key:Value
	* Query || _id
	* Key
	* Value
* Query
	* Query || _id
	* Resolve (boolean)
	* ~access_token?

### API Elaborations
A Query with no access\_token will only return documents who have global access specified. All queries however should check access before returning a document. APIs should strip out lux\_info

### Related Website/Application
* Any game or website would find this useful
* Anything where user content is key
	* Wikipedia
	* Amazon
	* ect

### Relevant Project 
* Liberty
* Jason Anderson
* Any Game

### Linked UI Element
* Asset Management Software
* Any Graphic/3D creation tool

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database: | Assets |
| Collection(s): | * | 
| 3rd Party: | x |
		

### References

## User Management API $_1^0$ [done]
This API allows storage and creation of user documents, updating user values, and fetching the User's document. User documents should not be accessible via Upsert- but only Upsert- Key:Value and Fetch Me. Only a user should be able to access their own document, other user's should not have direct access to this document. If a User's profile information is to be made public to end-users, they should have a separate document for that in the User Profile API. This should contain log-in information, access_token, general system related info like settings. This is where their social access tokens are stored, however those are not exposed to the front-end. They should however be consistently refreshed so that they do not expire (unless they are given a lux expiration date). 

The difference between this and the User Profile API is that this information is protected/private information, so it should not be accessible outside of the server. Some information can be duplicated between this and the Users Profile API. 

### API
* Adjust (key->value)
	* email
	* name
	* phone number
	* Account Type
	* ect
* Fetch Me
* Adjust Profile Access Level
	* User _id
	* Level
* Adjust Account Suspension(User/Admin)

### API Elaborations
The key->value pairs are meant to be edited in user profile information. Change Profile Access level is an Admin functionality, and requires either dev access or the highest access level in the application. 

### Related Website/Application
* Any website where people log-in

### Relevant Project 
* Any project where users log-in

### Linked UI Element
* User management 

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | x |

### References
[Stormpath](http://docs.stormpath.com/rest/product-guide/#accounts)
[userapp](https://www.userapp.io/)
[apigee](http://apigee.com/docs/app-services/content/working-user-data)



## User Profile API $_3^0$
The User Profile API is part of the Social network and stores public information about the user that can be accessed by any user who is authorized in the system. Information availability can be limited further by setting permissions on certain parts of a profile. This information is stored in the social network (neo4j), and not in mongo. 

### API
* Adjust (key->value):
	* About
	* age_range
	* bio
	* birthday
	* context
	* photo
	* education
	* email
	* favorites
	* first_name
	* gender
	* hometown
	* inspirations
	* website
	* ect
* Fetch Me
* Get User

### API Elaborations
Fetch User can be restricted by user preferences (must be in network, must be a direct edge), but is otherwise public.

### Related Website/Application
* Any Game or application that has social elements

### Relevant Project 
* Smokr

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | neo4j |
| Database(s): | SocialNetwork |
| Collection(s): | x | 
| 3rd Party: | x |
		
### References

[Facebook](https://developers.facebook.com/docs/graph-api/reference)

## User Asset Ownership API $_2^0$
The User Ownership API is designed to track what Asset's a user is permitted to update. This API is designed to be hooked into the User Profile Document, and should automatically be checked from the Assets API to see if the user has proper permissions to change that Asset. 

Some Assets will be globally alterable, and should thus be stored under a Global User. Some assets will be alterable by multiple users, and will therefore need multiple users to reference the Assset's Id.

### API
* Adjust Permission level
* Fetch Permission level
* Fetch Owned Assets
* Get Asset's Owner
* Get User's Assets


### API Elaborations
Get User's Assets refers to getting assets that another user owns (public assets only). Fetch Permission level is the permission level that the User has on the Asset/

### Related Website/Application
* Flickr
* GitHub
* Youtube
* Anything were only a certain group of users can make changes to any given Asset

### Relevant Project 
* Phonics

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | x |

### References

## Scoreboard $_1^0$
Scoreboard allows developers to easily keep track of user's score and Game state on a user By User bases. This is what is used by Leaderboard to asses user scores. 

### API
* Adjust Level Array 
* Adjust Level
* Adjust Metric
* Adjust item
* fetch User Scoreboard
* fetch user statistics
* fetch user items


### API Elaborations
Adjust item should include functionality to "Use" an item. Some items can be used multiple times- so creating an item should require a value of how often an item can be used, with a default of 1. 

Adjust Level Array is to create an array that can be used, like: levels, checkpoints, ect. 

### Related Website/Application
* Any Game

### Relevant Project 
* Phonics
* Liberty

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Stats |
| Collection(s): | Scoreboard | 
| 3rd Party: | x |

### References

[Reference](https://api.playfab.com/Documentation/)

## Real-Time Updates (Websockets and UDP)  $_4^0$
Real-time updates allow users to receive all Updates to the Asset Collection which match queries they are subscribed to over WebSockets or UDP. This will follow the existing architecture, but with improved threading ability and no-sticking or faulting. 

### API
* Query
* Upsert
* Subscribe
* Unsubscribe

### API Elaborations
All of the Upserts and queries are in reference to the Asset Database, other parameters need to be changed via HTTP requests for now. It is important that this functionality work well- and on a large scale, even with a small (single core) server.

### Related Website/Application
* Any Real-time application or Realtime game

### Relevant Project 
* Liberty
* Games

### Linked UI Element
* Asset Statistics Manager

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System && Assets |
| Collection(s): | Pub && Sub | 
| 3rd Party: | x |


### References

[Reference](https://www.pubnub.com/docs/javascript/api/reference.html#)

## Template Engine Render (Jade, Markdown, JMU [JSON MarkUp]) $_3^1$
This Service allows developers to query for a document and have it rendered into HTML, pdf, or another format before it is delivered to the End-User. This service will also connect to the CMS, to pull content items. The Template render should also include routing options to tell HTTP requests which file should be accessed. 

The initial implementation only needs to handle Rendering HTML, and needs to be able to handle routing problems. 

### API
* Fetch Page
* Change Render Settings

### API Elaborations
Content From the hooks are pulled from the CMS service. Multiple Pages can be concatenated in order to allow headers and footers to be used. 

The flow is: fetch page, switch out content, render as html, echo to page. Mustache style loops can be implemented for information that may be variable (a list or items or time entries). 

### Related Website/Application
* Any website with static content (PGL website, most corporate websites)

### Relevant Project
* PGL website, Lux Website

### Linked UI Element
Online IDE/Text Editor for Renderable markup languages

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Settings && Templates| 
| 3rd Party: | x |
		
### References

## Groups API $_1^1$
The Groups API is tied to the Social Network, and gives all the functionality described in the User Profile API. The Groups API also allows subgroups to exist for Admins or other functionality. 

### API
* Upserts (key->value)
	* Photo
	* description
	* email
	* icon
	* link
	* name
	* owner
	* parent
	* privacy
	* last_updated
* Get Group

### API Elaborations

### Related Website/Application
* Anything with cohorts:
	* Games
	* Education tools
	* Social tools
	* ect

### Relevant Project 
* Phonics

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | neo4j |
| Database(s): | Social Network |
| 3rd Party: | x |

### References

## Groups Profile API $_3^1$
See the User Profile API section

## Groups Asset Ownership API $_2^1$
This API functions much the same as the User Equivalent- however any permissions given to groups are able to be propagated down into the group members, making it possible to limit who can query for or alter an Asset, and limiting not only to groups but group admins or subgroups. 

### API
* Adjust Permission level
* Adjust Permission level on Asset to Group
* Check Permission level
* Get Owned Assets
* Check Editing rights
* Get Owners


### API Elaborations

### Related Website/Application
* A community site
* NOBE

### Relevant Project 
* None

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Groups | 
| 3rd Party: | x |

### References



## GeoLocation $_1^1$
This function is specifically for logging the location of a user in a system so it can be used for the POI API and the GeoFencing API

### API
* Adjust User Location
* Fetch User Location
* Get Other User Location
* Adjust Location Sharing Settings

### API Elaborations
Retrieve other use location can have parameters adjusted to limit who can find you and how accurate that is.

### Related Website/Application
* Tinder
* Google Maps
* Any Location based app

### Relevant Project 
* Sonic
* Scavunt

### Linked UI Element
* Admin Map

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | x |

### References
[Mongo](http://docs.mongodb.org/manual/reference/operator/query-geospatial/)

## CMS API $_1^1$
This API allows developers to add content to a CMS which can be edited from within the Lux Intranet CMS Tool. The CMS API allows content to be uploaded and edited. Content can also be queried from the CMS API so that it can be rendered for the End-User.

### API
* Adjust Field
* Get Field
* Adjust Field Render Options

### API Elaborations
Adjust Field Render options refers to access level required to get a field.

### Related Website/Application
* Wordpress 
* Drupal

### Relevant Project 
* PGL Website
* Anything with any text

### Linked UI Element
* CMS UI

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Content | 
| 3rd Party: | x |
		
### References
[Wordpress](http://codex.wordpress.org/XML-RPC_WordPress_API)
[Drupal](https://www.drupal.org/)

## Session Management $_1^1$
The Sesssion management system allows specific devices to save session variables (mobile, web, ect) on the server-side, so that when moving between pages/sections, or if there is a disconnect- then variables can be saved. Developers can also save access_tokens in the session so that user's who leave the page for extended periods do not need to loose temporary data. Session is the only thing that is validated by the requesting device and not the user's access token. Devices that can hold cookies can also use a cookie as the identifier for a session. All session information is written into Mongo on each call- in order to save sessions for extended periods (without adjusting php attributes).

\$LF should also be able to access session info automatically in any other API.

Sessions are held in both the Mongo DB and the PHP Session.

### API
* Adjust attribute 
* Fetch session
* Fetch attribute
* Renew Session
* Adjust attribute timeout
* Adjust Session Timeout
* Fetch attribute
* Annihilate Session
* Mimic User Session (admin)

### API Elaborations
Adjust Session Attribute should create a new session if none exists. Sessions are accessible via an "id". The Session tool can be used to store anything and if a session id is passed into another API then $LF should access the session variables associated with it. 

Mimic User Session is accessible only via the admin, and returns the session variables for a particular user.  Lux info should be stripped out when the whole session is requested by the javascipt

### Related Website/Application
* Any Non-SPA

### Relevant Project 
* Jason Anderson 
 
### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Sessions | 
| 3rd Party: | PHP Sessions |


		$_SESSION[] = ...;



### References
[php session](http://php.net/manual/en/ref.session.php)
[php SESSION](http://php.net/manual/en/reserved.variables.session.php)


## File Management API $_3^1$
The file management API allows users to upload or access files that are stored on S3 or glacier- as well as access the elastic transcoder, and other server side file functionality. Assets can also be linked to Files, and Files can be given permissions and Access Rights from within the File Management API

### API
* Upload File
* Get/Fetch All files
* Get/Fetch File Count
* Get/Fetch File by name
* Get/Fetch Files by Type
* Remove File
* Remove All Files
* Upload File linked to user
* Upload File linked to group
* Grant Access
* Revoke Access
* Archive File
* Un-Archive File

### API Elaborations
All Uploads are to S3, but management and access is via mongo

### Related Website/Application
* Most websites and development environments
* Dropbox/drive

### Relevant Project 
* Phonics
* CMS

### Linked UI Element
* File Management System

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Files |
| Collection(s): | * | 
| 3rd Party: | S3 |


### References

[Reference](http://docs.aws.amazon.com/AmazonS3/latest/API/RESTObjectOps.html)

## Transaction API $_1^1$
The transaction server acts as the banking system within a Lux instance, and can be used to hold virtual currency or credits which are traded between players. Certain actions may be 

### API
* Make Request
* Encode Item Array
* Transaction
* Add User Account
* Remove User Account
* add Sub account
* remove sub account
* get Transaction History
* get transaction detail
* get account info
* get subaccount info
* add item to subaccount
* remove item from sub account

### API Elaborations
This system needs to be distinguished by it's ability to handle any transaction, and it's ability to function as an in-game banking system. Security is also paramount to this system. 

### Related Website/Application
* Most Games and some websites that might have a tradeable item

### Relevant Project 
* Smokr

### Linked UI Element
* Bank Management System

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Transactions |
| Collection(s): | * | 
| 3rd Party: | x |


### References

[Reference](https://www.plaid.com/) ?

## API Proxy Service $_1^1$
This allows any Proxy that is set-up to the queried from the JS by forwarding the API call to the Proxy and returning the direct results of that API call to the JS.

The API Proxy service includes a way to make OAuth calls by looking for the User's access\_token and making calls with that.

### API
* Make Call
* Update API keys
* Update a call's parameters

### API Elaborations

### Related Website/Application
* Any website that cross references (Netflix:Rotten Tomatos)

### Relevant Project 
* Apollo

### Linked UI Element
* 3rd Party API Manager 

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | API | 
| 3rd Party: | x |

### References

------

## Social Network $_3^2$
The social network Allows users to be connected within this application/Lux instance specifically- and allows for user's to add friends, join, groups, ect. The functionality of a social network is limitless. This combined with the POI API could be very useful in finding nearby friends. 

### API
* Add Friend
* Add to group
* Add Tag Category
* Add Tag to Category
* Add Media
* Add Activity
* Add Blog Post
* Comment On Blog Post
* Comment on Activity
* Comment on Comment

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |


### References
[Reference](http://developer.ning.com/docs/ningapi/1.0/index.html)
[Reference](https://developers.facebook.com/docs/graph-api/reference)

## Asset Voting And Rating System $_1^2$
This API is designed to allow assets to be rated, voted, and commented on. 

### API
* Upvote Asset
* Downvote Asset
* Upvote Comment
* Downvote Comment
* Create Comment To Asset
* Edit Comment To Asset
* Remove Comment To Asset
* Create Subcomment
* Edit Subcomment
* Remove Subcomment
* Create Rating to Asset
* Edit Rating to Asset
* Remove Rating to Asset
* Create Rating to Comment
* Edit Rating to Comment
* Remove Rating to Comment

### Related Website/Application
* Facebook
* Twitter
* Netflix 
* Everything

### Relevant Project 
* Apollo

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Stats |
| Collection(s): | AVR | 
| 3rd Party: | x |

### References

## Tag API $_1^2$
The Tag system allows tags to be added to an Asset and should be stored directly in the Asset. The Tag API allows developers to query based on tag, add tags, and get the most popular tags without writing custom queries for each operation. Tags are short (1 word or a short sentence) describing a user or Asset. 

### API
* Add Tag to Asset/User Profile
* Remove Tag From Asset/User Profile
* Edit Tag From Asset/User Profile
* Fetch Asset/User Tags
* Get Assets/Users By Tag
* Get Similar Tags

### Related Website/Application
* Twitter
* Tumblr
* ect

### Relevant Project 
* Chinese Kids thing
* Apollo

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Stats |
| Collection(s): | AVR | 
| 3rd Party: | x |


			// Part of AVR
			,tags:[]
		
### References

## Leaderboard $_1^2$
Leaderboard allows you to query for leaderboard results based on a variety of criteria. This API has no alteration functionality- only a series of queries. All of the Fetches should have parameters to define how the leaderboard is calculated

### API
* Fetch friend Leaderboard
* Fetch Leaderboard
* Fetch Facebook Friend leaderboard
* Fetch Leaderboard around user


### API Elaborations

### Related Website/Application
* Any Game

### Relevant Project 
* Liberty
* Phonics

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo or MySQL |
| Database(s): | Stats |
| Collection(s): | Scoreboard | 
| 3rd Party: | Facebook |

### References

[Reference](https://api.playfab.com/Documentation/)


## Massive Asset Voting And Rating System
This API is designed to allow assets to be rated, voted, and commented on. The only difference is that this one is geared towards extremely comment heavy applications and will use MySQL instead of Mongo.

### API
* Upvote Asset
* Downvote Asset
* Upvote Comment
* Downvote Comment
* Create Comment To Asset
* Edit Comment To Asset
* Remove Comment To Asset
* Create Subcomment
* Edit Subcomment
* Remove Subcomment
* Create Rating to Asset
* Edit Rating to Asset
* Remove Rating to Asset
* Create Rating to Comment
* Edit Rating to Comment
* Remove Rating to Comment

### Related Website/Application
* Imgur
* Reddit

### Relevant Project 
* Apollo

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | MySQL |
| Database(s): | AVR |
| Collection(s): | * | 
| 3rd Party: | x |

### References

## Achievements/Badges
Achievements/Badges are criteria that are automatically met based on Elements on the scoreboard. Users can attach actions to Achievements and track the number of actions that have been taken. Achievements can then be queried to find updates or new Achievements. Badges work in the same way as achievements, but there may be some subtle differences?

### API
* Upsert Achievement
* Earn achievement
* Fetch all achievements
* Check for achievement
* Fetch user achievements
* Upsert Achievement criteria

### API Elaborations

### Related Website/Application
* Any Game

### Relevant Project 
* Liberty 
* Phonics

### Linked UI Element
* Achievement Creator

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Stats |
| Collection(s): | Scoreboard && Achievements | 
| 3rd Party: | x |


		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}

### References
[Reference](https://api.playfab.com/Documentation/)

## Social 
The Social API allows users to post to a variety of social media outlets from the Front-end, and allows Lux to track those social media postings. A developer should only need to send a single request to Lux in order to Post... whatever... and then the post should be processed By Lux- including access tokens, refresh tokens, ect.

The API should open up any functionality of the Social Media outlets API that is necessary- posting to their wall, getting their likes, ect... Developers should not need to connect to the Social Media API at all, but instead run all functions via Lux.  

### Facebook API
### Twitter API
### Google API
### GitHub API
### Ect

### API Elaborations

### Related Website/Application
* Any Website/Game that posts to facebook

### Relevant Project 
* Sticky Pigs
* Liberty

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | Facebook, Twitter, ect |

			// Part of Users doc
			,permissions:{
				facebook:{

				},
				twitter:{
	
				},
				ect.
			}

### References
[Reference](https://api.playfab.com/Documentation/)
[Reference](https://docs.oneall.com/services/implementation-guide/)

## Wishlist API
The Wishlist API allows users to save specific items into several Wishlists. Wishlists can be moved into the shopping cart with an API call. 

### API
* Move to Cart
* Create wishlist
* Upsert item
* Fetch Items
* Fetch item
* isEmpty
* Checkout
* Set Quantity
* Increase Quantity
* Decrease Quantity
* Clear All items


### API Elaborations

### Related Website/Application
* Amazon or any E-commerce website

### Relevant Project 
* Apollo 



### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Stats |
| Collection(s): | Cart | 
| 3rd Party: | x |


		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}
		
### References
[Reference](http://docs.shopify.com/manual)

## GeoFencing $_1^2$
This API allows for certain functions to be executed when a User is within a given area. Allows you to add a published document to the set when a Geo Fence is triggered. This will need a script running specifically for geofencing "cron" jobs.

### API
* Create Geo Fence
* Add Member to Fence
* Remove Member
* Update Callback
* Check Triggers
* Remove Triggers
* Set Fence Expiration
* Get All Geo Fences
* Get Active GeoFences

### API Elaborations

### Related Website/Application
* Find Friends
* Starbucks App
* Location Based Games

### Relevant Project 
* Sonic
* Scavunt

### Linked UI Element
* Admin Map
* GeoFence Creator

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Geo |
| Collection(s): | Fences | 
| 3rd Party: | x |


		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}
		
### References
[Google](https://developers.google.com/maps/documentation/tracks/geofences)
[Mongo](http://docs.mongodb.org/manual/reference/operator/query-geospatial/)

## Auth/Custom Auth $_3^2$
The OAuth should provide access to any OAuth provider that the developer chooses by adding only a single link. Custom Auth should allow users to create a profile and log-in using only the API. Users can have multiple accounts linked (Facebook, Google, Custom) to a single Lux Account. Things like account reset, and Reset Password are included in the Custom Auth functionality. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 

### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References
[Reference](http://oauth.net/articles/authentication/)
[Reference](http://lepture.com/en/2013/create-oauth-server)
[Reference](http://docs.stormpath.com/rest/product-guide/)

## Roles & Rule Set
Roles and Rule sets can be arbitrarily applied to users and must be controlled on the front-end by adding calls to check the user's permissions on a given page. A User's roles and rules can dictate editing rights, page access rights, or ability to alter Rules & Roles.  

### API
* Create Rule Set
* Create Access Level
* Attach Rule to User
* Attach Rule to Asset
* Attach Rule to Page
* Attach Rule to xxxx

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Form Validation 
This can be used to access specific form element types and validate their input serverside. This can be as simple as checking that an email is properly formatted to sending an email to check that their email is valid or verify a phone number is legitimate. 

### API
* required
* min length
* max length
* exists (in assets/user)
* range length
* min value
* max value
* email (tests that it is real)
* url (tests that it is real)
* date
* date ISO
* number
* digits
* credit card (checks that it is real)
* equalTo
* file Type
* phone number

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References
[Reference](http://www.html5rocks.com/en/tutorials/forms/constraintvalidation/)

## Timer Service
The timer Service creates an instance of a clock on a server which can be referenced as a "game time" or other use of standard timing (auction, ect) in order to determine when something expires according to server time. Timer callbacks are messages that can be published to the real-time script.

### API
* Create timer
	* w/ callback
	* periodic timer (infinite or finite)
* update timer
* start/stop timer
* delete timer
* clear periodic
* reset periodic
* check time

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References
[Reference](https://api.dartlang.org/apidocs/channels/stable/dartdoc-viewer/dart:async.Timer)


## Facilitated Communication
Facilitated communication is meant to add a social element to a website, allowing users to communicate either via Video, Audio or Text. 

### Chat

#### API 
* Send Message 
* Send Typing 
* Send Read Reciept
* Check messages
* Set Status

[Reference](http://developers.livechatinc.com/rest-api/#!introduction)

### Inbox (PM)

#### API
* Get messages
* create message
* send message
* view messages with user
* delete message
* delete message thread
* mark thread as read
* mark thread as unread
* mark thread as important
* mark message as read
* mark message as unread
* mark message as important
* ect

[Reference](https://documentation.mailgun.com/api-sending.html#sending)

### Video Chat
[Reference](https://tokbox.com/opentok/libraries/client/js/)

### Audio Chat & VOIP
[Reference](https://www.twilio.com/docs/api/rest)

### Feed tool
This provides a Social network based feed that users can post content to, and can query for the latest contacts from their network.

## Match Maker API
The match maker is built on both the Social network, leaderboard, and achievements (as well as First come first serve matching), allowing you to query for a match- create a group- or create an instance of a game in order to play. The match Maker API can also be used for matching individuals on a dating site to give recommended matches. 

### API
* Get current Games
* Get Game Server
* Find match
* Start Game
* check for matches
* Join Match
* Leave Match
* Fetch Match User's Info
* Update Match info
* Fetch Match Info


### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://api.playfab.com/Documentation/Matchmaker)


## Survey Tool
This is a tool designed to allow the simple creation of survey's which can be viewed and edited via the intranet tool. Surveys are available as either anonymous or sign-in, and can be deployed internally or externally. Survey answers can be viewed in the intranet tool.

### API
* Create survey
* add question
* remove question
* update order
* get survey
* submit survey answers
* fetch survey answers

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Feedback And Helpdesk API
This API allows developers to add buttons and input boxes to the page which allow users to either 


----------


 the business, or request help. It can be linked to a chat system, or just an inbox system. The API should allow for messages to be both input to the system and Pulled out (on both the customer and business side). This will be linked to the Lux Intranet tool- which will provide an interface for businesses to view any feedback or help tickets that have been logged. 

### API
* Create ticket
	* email
	* name
	* requester_id
	* subject
	* descritption
	* status
	* priority
	* source
	* deleted
	* spam
	* responder_id
	* group_id
	* ticket_type
	* cc_email
	* is_escalated
	* attachments
	* due_by
	* cc_emails
* Create ticket w/ attachment
* View ticket
* View Ticket List
* Update Ticket
* Pick a ticket
* Delete a ticket
* Restore a ticket
* Assign a ticket
* get All ticket fields
* add note to a ticket
* add note with attachment
* view user tickets
* view users on ticket
* make agent
* view agent
* view all agents

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](http://freshdesk.com/api#introduction)

## Ad Exchange/Network/Cross Promotion API
The Differences between these 3 boils down to where the ads are sourced from/how much is being paid for them, but the API would be wrapped into one- allowing you to pull ads based on the user, the page, ect. The API would determine (based on your criteria, and your account) which ad should be displayed. The ad Exchange/Network/Cross Promotion Portal would be located on the Lux Intranet Tools, and allows users to both post ads and use them. The Ad Portal should also be available outside of the Lux system, and linked to any Ad networks that we can link it to. The Ad Portal should be API based as well. 

### API
* Create ad object
* Create segment
* Create ad unit (object, segment, geo, type [video, graphic, image], size)
* Create GeoTag
* Fetch ad Unit (by segment, geo, ect)
* Fetch ad Object
* Report Ad 

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References
[Reference](http://openx.com/)

## POI API
This API does not provide POI, but allows assets to be given "Locations" and allows querying to happen in reference to those locations. This means that you can query for assets and sort the results by distance, or some other criteria within a given distance. Developers are still expected to provide their own Assets/POI, the API just makes it easier to query for them. 

### API
* Get all points
* create geo point
* get nearby points
* get points in circle
* remove geo point
* alter-geo point

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://www.yelp.com/developers/documentation)

## Forum Tool 
This API acts as expected, allowing you to post to, or query a forum, as well as access elements of the forum like "reply" and creating threads. This may very well be integrated into the AVR set if necessary. 

### API
* Create Forum category
* update category
* view category
* view all categories
* delete category
* create forum
* update forum
* view forum
* delete forum
* create topic
* update topic
* view topic
* delete topic
* create post
* update post
* delete post

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](http://blog.vanillaforums.com/api/)

## IMAP access
Access an IMAP (or gmail) server and do all the normal functions (mark as read, read, send, ect) according to the user's account and via the API. 

### API
* Append Message
* Get Body
* check mailbox
* clear flags
* create folder
* delete message
* fetch body
* fetch header
* fetch mime header
* fetch structure
* get quota
* get subscribed
* compose mail
* move mail
* send mail
* num messages
* num recent messages
* num unread messages
* search mailbox
* get thread
* subscribe to folder
* get status of folder
* undelete message
* unsubscribe from folder

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](http://php.net/manual/en/book.imap.php)

## Gift Management API
This API is focused on giving and recieving gifts within the social network- and just adds specific edges that are formatted as "gifts". It also contains a Mongo portion that allows for the creation and deletion of gift definitions, and can be used in parallel to either the payment API or the transaction server.  

### API
* Create Gift
* Distribute Gifts
* Send Gift
* Request Gift
* Get Gift Requests
* Accept Gift Request
* Reject Gift Request
* Remove Gifts
* Get All gifts
* Get Gift Count
* Get Gift By Name
* Get Gift By Tag
* Delete Gift by Name


### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References


## Payment API
The Payment API is a method of integrating credit card payments into the site so they are naturally processed on the server- including setting up reoccurring payments, setting up future payments, processing credit cards, and automatically disabling features/access if payment is not processed. The Payment system can originally be a wrapper for the PayPal API, but later should be developed as a custom Direct Deposit payment system. The Payment system makes security crucial. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://stripe.com/docs/checkout)
[Reference](https://developer.paypal.com/docs/api/)

## Inventory API
The inventory API allows developers to add and remove items from an inventory in order to track inventory. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Shopping Cart API
The Shopping cart acts as it should with links to Assets and allows Checking out which ties directly to the Payment system

### API
* Create cart
* Add item
* Get Items
* Get item
* is Empty
* Checkout
* Set Quantity
* Increase Quantity
* Decrease Quantity
* Remove Item
* Remove All items
### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](http://docs.shopify.com/manual)

## Order API
The Order API allows users to place orders into an order system, and allows a second API to "fulfill" orders. Order Fulfillment is tied to the inventory system as well, and inventory and Shopping cart system as well. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## AJAX Search API
The AJAX Search API acts as a querying functionality for Assets- but is tailored to search tags, multiple collections, and return a projected set of information to the database. Results can also be limited to a certain number- however a cursor object is created which allows them to view more results by querying just that cursor object. 

### API
* Search for 
* Add to selected

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Collection Querying
Collection Querying allows for any arbitrary collection in any database to be queried, but not upserted. 

### API
* Query

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Newsletter API
The newsletter API allow you to add or remove members from a mailing list- add or remove newsletters to be sent out, and is linked to the User's document, so that suspending their account will remove them from the list, and updating the email on their account can update the email on the list. Non-account emails can also be added to the list. The newsletter API automatically appends an unsubscribe link, and allows you to send HTML based newsletters or mass emails out to Users and Subscribers. This will use the Amazon Message Sending Service (the one for mass no-reply emails). 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://apidocs.mailchimp.com/api/downloads/)
 
## Image Processing API
This API Allows you to run server side image processing on a file that has been uploaded to S3, and save that file back to S3 as well as return the original file to the Developer query. 

### API
* Create Editing Object
* Resize
* Thumbnail
* Scale
* Crop
* Convert Format
* Add Filter
* Add Layer
* Tile
* Rotate
* Slice image 
* Add Object to Layer
* Transform/Move

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://github.com/6px-io/6px-api-docs)

## Short Code Generator
This API can be used to generate Short Codes and link them to a user or an Asset. It also allows developers to verify the Short code and return the Document that is linked to it. When a short code is 

### API 
* Generate Code
* Attach to user
* attach to reward
* attach to location
* attach to discount
* redeem code
* get code owner

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Reward Service API
Rewards, which are often mapped to short codes (or have short codes mapped to them) can work as a discount when using the payment system- and payment will be partially paid via a corporate account opposed to the user's account. Rewards can be given and tracked in a variety of ways. Rewards can also just be counted as a discount opposed to actually subtracting from the corporate account. 

### API
* Create reward
* Get reward description
* Redeem reward
* add reward to account
* remove reward from account
* use reward
* Pass reward into payment

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Push Messaging
This is to be built on top of Amazons push notification service, but we will manage the user's account preferences and stuff?

### API
* Add device
* Send Notification to User
* Send Notification to Group
* Publish Notification for subscribers
* Create notification
* Change notification settings
* Subscribe user to notifications query
* Unsubscriber user to notifications query

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://pushover.net/api)

## Auction API
This provides auction selling and buying functionality for unique Assets. 

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](https://go.developer.ebay.com/api-documentation)

## Recommendations Engine 
This needs more planning- but originally will just recommend assets based on the current asset. Eventually it should track user activity, user searches, previous purchases, ect to come up with exactly which asset is desired

### API
* Add to User preference
* Add to User history
* Get user Recommendations
* Get Similar
* Get by .... 

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Virtual Server API
The virtual server API creates a temporary server box that users can access using API calls and file uploads, these boxes can be saved and destroyed via API, and output from terminal execution can be returned via API

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Server Code Execution System
This API Allows users to simply execute a script that has been loaded via FTP onto the server, either by an HTTP Call or as a Cron Job. Server Code can be tested using the Virtual Server API. The Server code Execution system can be used to add custom functionality or add custom logic to a game which can (or should not) be executed on the client machine. 

### API
* Get Server Script Id
* Run Server Script
* Get Revision History
* Set Published Revision
* Update Cloud Script
* Test Cloud Script

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Avatar Service
Create an avatar by plugging in certain variables and getting a full Model/image returned (which can be saved for quick generation). This is a silly add-on type feature and not a necessary element. 

### API
* Create Avatar
* Get All Avatars
* Get Avatar by Name
* Update Avatar
* Remove Avata
* Avatar From image
* Adjust Feature
	* Clothes
	* Hair
	* ect

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

## Logging API
The Logging API is primarily used for Analytics/Crash testing, and allows developers to log User Actions or Application Level Errors into a Lux Collection. The Timestamp, IP, and Other information will be automatically added as well. 

### API
* Log Event
* Fetch Logs By error
* Fetch Logs by ...

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: |  |
| Database(s): |  |
| Collection(s): |  | 
| 3rd Party: |  |

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
		}


### References

[Reference](http://api.shephertz.com/app42-docs/app-logging-service/)


----------------------------------

# Non-lux APIs
## Simulated Heavy Traffic Testing
The Simulated Heavy Traffic Testing is designed to test a site from multiple devices to see how it reacts under heavy load- seeing where the breaking point of users is at which upgrading to a larger instance becomes necessary 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## API Generator
The API Generator should pull from the User's collection and Assets Database (and any other relevant collections) to automatically generate an API.

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## SDK Generator
This Will take a generated API and create a set of Language specific libraries which wrap the HTTP calls for that API.

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## API Documentation Framework
The API Documentation Framework takes the API generated and generates documentation. It then allows users to edit the text of that documentation and update it. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## API Access Gateway
The API Access Gateway manages the Access Tokens for an API, provides OAuth Access for users, and limits 3rd Party Developer Connections. It also allows developers to limit API access by account type.

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## App Store Submission
App store submission uses the remote server to submit applications to various app stores from the server, and updates submissions from the server at the click of a button. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## Automated App Testing
Automated App testing allows you to set up a series of tests on a series of devices which can be executed on remote servers as a user interaction and report any bugs/crashes which result. It can also randomly interface with the application to find bugs which cause it to crash. Any test can be played back to see the error, but a file is generated and sent to the developer with the test results.

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References


## AI/Neural Network Interface
The AI/Neural Network is a machine learning interface that plays simple games and is matched using the Match Maker. It requires no programming to interface, but accesses the Game as a user- and randomly uses controls until it's feedback registers a result. 

### API

### API Elaborations

### Related Website/Application

### Relevant Project 



### Linked UI Element

### References



# Analytics

The analytics functions for lux are built on a few key concepts.  The first of these concepts is the Mongo+Hadoop architecture, and the use of Hadoop's MapReduce functions for processing Analytics data that is saved into Mongo. This allows for scalability within the lux, which means that as the application scales to multiple servers, hadoop can be used to process analytics on these servers. 

The Lux team would however prefer to replace the use of Hadoop with a similar architecture which is controlled by lux's built in scalability model. This new architecture would still use Mongo and the MapReduce model- but can either be a customized version of Hadoop or a new system all together. 

The second concept is the combination of event/trigger and integrated passive analytics. The passive nature of both analytics is that user's need not know they are being analyized (unless the developer wishes for them to be aware), and just their interactions in the system is enough to update the system. 

Analytics can be customized by creating new MapReduce functions, or triggering existing MapReduce functions with new variables. All analytics information can be accessed back into the application for adaptive environments, pulling back in user personality profiles, or even recommendations without developers needing to create custom algorithms.  

All analytics have an optional access\_token included in the request, which allows tracking based on the user's lux identity. If the access\_token is not included, then a temporary identity will be created for the user. This temporary identity is less accurate and may include the same user under multiple identities, but should not include multiple users in one identity. This may lead to skews in results. This identity is similar to a server-side Session. 

### Event/trigger Analytics
The analytics contained in this section need to be integrated into your application by the developer, but will be automatically processed by the scripts that run in the MapReduce architecture. This entire section is contained in a single call to `record.php` and we primarily provide standard codes for developers to follow. 

When an event is triggered in the developer's application, they should call record.php to notify the lux application that this has happened. For Event/Trigger Analytics, access to data is more manual and may need to be pulled through a standard call such as `fetch.php` or `get.php`. 

Events can be given a severity level which is measured from -100 to 100. For some calls this will always be 100, and for others (User Personality) the measured level becomes very important. 

| Event Object | Event Code | Trigger | Trigger Code | Desc |
|--------------|------------|---------|--------------|------|
| Application Loaded | 000 | Mobile | 0000 | Application was opened in a native Mobile wrapper | 
| Application Error | 001 | Error Caught in Code | 0000-0500 | An error was caught with a try:catch statement within the application code |
| Application Error | 001 | Error Caused Application Crash | 0501-1000 | A Crash Report was present upon Application start-up | 
| Application Error | 001 | Non-Fatal Error | 1001-1500 | Application does not handle the current state, or an unexpected state reached |
| User Personality | 005 | Extroversion | 0000-0999 | User demonstrated Extroverted personality traits |
| User Personality | 005 | Agreeableness | 1000-1999 | User demonstrated Agreeableness personality traits |
| User Personality | 005 | Contentiousness | 2000-2999 | User demonstrated Contentiousness personality traits |
| User Personality | 005 | Neurotic | 3000-3999 | User demonstrated Neurotic personality traits |
| User Personality | 005 | Openness | 4000-4999 | User demonstrated Openness personality traits |
| Custom Event Object | 050-999 | Custom Triggers | 0-1000 | Developer Defined Analytic's objects | 

#### Custom Event Objects
The custom event objects are anything that a developer wishes to track in the system, whether this be  the number of times that a user walks through a door in a game, or the number of times a user clicks a given button the page. These Custom Event Objects can still take full advantage of the MapReduce functions for whatever information they would like to know. 


#### App Promotion Campaigning Analytics
Analaytics that are tied to App downloads from Promotions, App opens due to promotions, and app engagement due to promotions. All of these tied in via an analytics Rest Call which allows an update to the analytics when an App is opened or a Promotional code is input. Promotional code analytics should be directly monitored via the back-end services. App views and app opens can be triggered automatically from front-end functionality. 

#### Crash Analytics 
Crash analytics allows you to view crashes and how they can be avoided. Works by sending an update when App is reloaded after a crash. 

#### User Analytics 
User analytics focus on user engagement, and game-play to see how the app is being used. This is the core of the Lux Analytics engine, and focuses on promoting a better App experience- as well as providing insights about the Application's user base. 

#### User Character Tree Analysis

#### Cohort/User interaction Analysis

### Integrated Analytics
Integrated Analytics happen without any developer interaction. 
They are based on existing lux features that can be altered and changed according to developer needs. An example of an integrated Analytics application is the use of Scoreboard, which developers use in their application, and need to update on a regular basis. When a developer does update the user scoreboard, this change will be reflected in their Analytics without needing to add any additional code.  

For integrated analytics access to information is available through pre-made APIs. 

#### Buy Tracking
This tracks the number of purchases and the number of downloads for an App, as well as the in-app purchases to see how to best monetize your application. 


#### E-Commerce Analytics
Similar to buy tracking, but is focused on the purchase of physical goods and is more closely tied to the shopping cart, wishlist, and order system.


#### Location Analytics
Location Analytics focus on using user data to find where the primary user-base is and how it grows, changes, and develops- as well as Where people are using the application (doctor's office, bars, at home). These can also be applied to in-game locations, and can be applied to both physical world and in-game locations at the same time.


#### App Store Analytics
Automatically pulls all app store information related to your app, including competition and more. Allows developers to see how their app does in different environments- and how it compares to similar competitor products. 



## Split Testing


## Info-Graphic Generator

--------------------
--------------------
--------------------
--------------------
# Lux Intranet Tool

The Lux intranet tool is geared to provide all intranet, business, and other tools that a company should need. The Business development tool is a subset of this. Any employee should be able to go to the intranet tool and perform all business tasks/job tasks that are necessary for the company. 

## Newsletter Tool
MailChimp or dada rip off. 

## Internal Chat tool
A tool to chat internally with other employees- anyone else who is on the intranet. Although this should be integrated with the intranet (inbox, chat stuff, video conferencing ect), it should also be available as an external application. 

## Programming Test
The programming test should be targeted at hiring specific programmers and is accessible outside of the intranet. Programmers should be able to take the test and share their results across multiple companies. Companies should be able to request a test directly for specific applicants. This acts as both a job board and a hiring tools built into the intranet. 

## Time Tracker
An integrated version of the WWTW tracker, this should log hours, vacation days, manage pay, request advances, ect, and should be linked with project management

## HR Tools
This should give people an interface with HR and hiring, as well as allow HR to manage payroll, time-off, and anything else. This tool should handle both complaints (including anonymous complaints), and Employee management. 

### Manage Corporate Access
Who gets access to what, who can change what, and who get's access to what in the Intranet Tool. Allows enabling and disabling of tools, emails, ect. 

## Accounting Tools
The Accounting Tools built into the Intranet should handle all employee 

### Receipt Tracking
Standard Feature

### Payroll
Standard Feature

### Contracts/Invoice Manager (Allow online paying)
Standard Feature

### Auto Reports/Tax Forms
Generate all necessary forms, pre-filled out with only a few questions. User's should also be able to fill out tax forms through the system, or export to various formats (Quickbooks, whatever people use to file taxes). 

### Billing 
Billing is just how people view and pay their lux bill- as well as where they can see a break-down of what they are being charged for. 

## Order Management System
The Order Management system should be hooked into the shopping cart and allow for users to see and manage order and order shipping. Ideally a partner entity could handle warehousing and shipping- but that is in the future

## Inventory Management System
The inventory management system should allow users to maintain inventory, view inventory and see inventory at 3rd party systems (amazon, ect), in order to manage production and supply chain resources. This is closely tied with the Order Management System

## Helpdesk and Feedback tools
The help desk tools are too allow site and application users to give feedback and help desk tickets, and the intranet tools are built in to help manage those requests. Some minimal database access is given- but needs manager approval for modifying. Database access is only available for a single user who is identified by a security code of some sort. 

## Internal Game System
This is just for fun, and provides a small mini-game system for inter office competition and games. Games are both Long term turn based, and short term realtime games, but are all geared towards creating a community within the company. Games can be added from an app store, and can be enabled on a user by user bases. HR has the ability to block user's from a game, and see how much game play is taking place. Credits can be earned and used for rewards which are managed by HR. 

## User Management Tools 
Suspend Users, look at flagged users

## Asset Management Tools
Look at flagged assets

## CMS
The Content Management system is designed to allow employees (with permission) to update the external page quickly and efficiently. 

## Lux Management Console 

### Manage Lux Instances
This allows users to create, destroy, and change settings of their lux instances. It also allows users to control who has access to their lux instances. Changes that require significant financial changes should need to be approved by accounting. 

#### Update Lux From Github
This is simply a button within the lux instance that allows you to update and or manage updates of the open source core technology- or choose to select a different branch/fork of the open source. 

#### DNS Manager/Routing Manager
This is to manage the Route53 and EC2 Routing. 

#### CPanel
The CPanel gives many options to manage the features of apache and control how aspects of a webserver are handled.

#### API Service Management
This allows you to register for and sign up for APIs. I would like to partner with various APIs to allow you to get an API key through Lux, starting with lux based products automatically generating API keys for new lux based products. 

# Lux XDE

## Hardware Design Suite
The Hardware design suite should be geared towards users to develop hardware, PCBs, and layouts. It is meant for product prototyping from within the lux system, and should include easy access to a variety of tools and services. http://123d.circuits.io/

### Hardware Component Creator
The Component creator allows users to add components with real values to a system- and allows components to simulate code execution. 

### Hardware simulator & Code simulator (Arduino/Edison)
The hardware simulator should test both Code and circuits so that an entire first stage prototype can be built for any product before any parts are ordered. 

## Pixlr Editor
This provides both vector and pixel art to be performed from within the web browser/intranet tool and allows users to save their work to the cloud, as well as make it available to other system users. 

### Favicon Generator
The same as the pixlr editor, but geared towards Favicons. 

### Graphic Design Generator
The Same as the pixlr editor but geared towards UI/UX design and vector art. 

## 3D CAD/Animation
A full CAD and animation software available online. 

## GistBox
The GistBox allows users to share code within their organization or externally directly from the the IDE. 

## Component Marketplace
The Component Marketplace allows users to share any Components that are not proprietary for use outside the company, as well as to grab any internal components that exist. 

## Ad Exchange/Network/Cross Promotion Marketplace
This provides a network for users to access the marketplace and find ads to either sell or display. Ads can be added to a cart for display, or uploaded for marketing. 

## Tools/Libraries

### Sonic
Sonic Libraries should be included for device applications, and should have a JS wrapper that allows developers to easily and quickly use sonic within an application. 

### GitHub Integration
Elements should be syncable with github and have a local git repository available. You should be able to perform Pull Requests and Merges from within the application, as well as be able to fetch new code. This means that each users code base should be connected to a fork of a master. 

### App Store/Submission
The should be a deploy application to send applications to the app store, and an update feature that can update applications when necessary. 

### OS/Browser Emulator
The editors should have OS and Browser emulators available which render the elements and you can interact with.  

### CMS Hooks
Libraries should be avalable to easily create CMS hooks in the format 

		{{content}}
		{{about}}
		{{link}}[href]

or something similar.

These will be interpreted server side to be converted into CMS content- either to be replaced on the server side, or replaced on the client-side. This can be used in conjunction with the template renderer. 

### Cross Platform Tools
This should allow code to be written in JS/HTML and rendered on the appropriate device in a browser wrapper, and as a stand alone webpage. CSS should be rendered separately for each device type- and targeted at working across all devices. 

### Hardware integration Tools
JS wrappers for device functionality like: camera, gps, ect.

### Localization Toolkit 
A few lines of code to set-up how often the device location should be updated on the server  

### Ad Integration Library
A few lines of code to include an ad into a space- optimized for the device dimensions, pulled from lux, and fit to the container. Ads that are not shown on the page will not be fetched- and fetching ads can be set to happen after the page loads (to avoid ad block). Ads can also be triggered before videos, or full page ads after a button is pressed.

### Mobile App Management Library
MAM is the ability to control who can use an application and who is allowed to open an application based on their credentials. 

### Mobile Device Management 
MDM is a library that allows a company to control a devices functionality because this app was downloaded. This can be tied to Mobile App Management in that some applications are unusable if the MDM application is not installed. 

### App Monetization/in-App Sales Library
Similar to device hardware libraries, this allows developers to redirect users to the app store and manage what purchases have been made in the app store.  

### Automatic Offline Handling 
Saves data locally when connection is not available and forwards it later- prevents users from doing anything that requires connection if one is not available 

## App Store Optimization
App Store optimization uses the SEO tags on a page to generate ASO tags for automatic app submission. 

## Automated App Testing
Once a test are loaded programatically and can be executed on the server at given intervals- which will generate reports and send them to developers. 

## Beta Testing Distribution Set
Beta Testing distribution allows users to deploy an app via a web link opposed to via the app store. The user can also create Beta Testing prompts which can be deployed as a user interacts with the application as a prompt. 

# IDE

## Program Language Converter

## Virtual Program Execution Environments (c9.io)

# ADE 

## Prototyping and Mock-up
Allows users to drag elements onto the page and convert them into HTML/JS code once the Mockup is done. 

## Story-boarding 
Like Mock-up, story-boarding allows users to specify which clicks will lead to new interfaces, and the "compile" will add those links and the necessary JS code to implement that functionality 

## UI Framework (Bootstrap)
A CSS/JS UI framework that users are given access to through the system- these are the standard elements that are available in the Prototyping/Mock-up and Story-boarding tools.

## Native JS Framework (jQuery)
The JS Framework will primarily provide libraries to be used within the application

## App Factory
The App factory allows users to grab pre-built templates and make alterations to them quickly and easily for a rapid release cycle.


# GDE 

## Scripting and VPL 
Multiple skill levels and progressions

## Asset Workflow

## Scene Building

## Game Engine Tools

-------

# Lux Business Development Toolkit

## Create Company
Legal Stuff, Tax stuff, Investor Stuff

## Manage Existing Company
Legal Stuff, Tax Stuff, Investor Stuff, this could also be under HR or Accounting. 

# Appendix
# Database structures

## Mongo
Mongo is a document database that utilizes collections of documents. 

### Assets.*
		
		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
			// This is the area that developers are able to 
			// upsert via the **Asset Management API**, they are not given upsert access 
			// to the lux_info
			,lux_info:{
					// Information being stored by other APIs
			}
		}	

### System.User

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
			values:{ // set by **User Management API**
						email:
						name:
						phone_num:
						ect.
			},
			profile_access_level: // set by **User Management API**
			permissions:{
					,assets:{
						0: [] // creator
						1: [] // owner
						2: [] // editor
						3: [] // viewer
						4: [] // denied
					},
					social_network:{
						chat:
						inbox:
						join:
						// other things that can be restricted
					}
			},
			suspension_status:{
						user_level:{
								bool:
								reason:
								time:
								last_suspension:{
									time:
									reason:
								}
						},
						system_level:{
								bool:
								reason:
								time:
								last_suspension:{
									time:
									reason:
								}
						},
			},
			loc:{
					type: "Point"
					, coordinates: [ 40, 5 ] 
			},
			lux_info{
				// stuff like the user's access_token
			}
		}

### Stats.Scoreboard

		{
			"_id" : ObjectId("54eb780f883e4ae10378ec6b"),
			"user_id" : ObjectId("54ae0b30e43ca55e86db0eb3"),
			"levels1" : [ ],
			"levels2" : [
				{
					checkpoint_id: ObjectId("54ae0b30e43ca55e86db0eb3"),
					metric1:
					metric2:
					metric3:
				}
			],
			items: [ ], 
			info:{
				// key->values 
			}
			"time_created" : "2015-02-23T18:57:18.975Z",
			"last_time_modified" : "2015-02-23T18:57:18.975Z"
		}


### RTU.Pub

		{
			// This portion is a direct copy of the document in the Asset's database
			//
			//
			"publication_info" : {
				"sender" : ObjectId("54ae0b30e43ca55e86db0eb3"),
				"timestamp" : "0.97971300 1422309133",
				"priority" : NumberLong(0),
				"checked_by" : {
					"python" : false,
					"node" : false
				}
			}
		}


### RTU.Sub

		{
			"_id" : ObjectId("54b6bc6f98cac76ee552f548"),
			subscription:{
				collection:
				,"query" : {
					// A Query that can be used by the Asset's Database
				},
			}
			"subscribers" : [
				{
					"id" : ObjectId("54b3f81898cac76ee552e27f")
					// meta data
				}
			],
			// more meta data
			"timestamp" : "0.47691600 1421261964"
		}


### RTU.Sockets

		{
			"_id" : ObjectId("54b6bc6f98cac76ee552f548"),
			subscriber:{
				"id" : ObjectId("54b3f81898cac76ee552e27f")
				// meta data
			}
			sockets:{
				websocket:{}
				udp_socket:{}
				ect:{}
			}
		}

### Sessions.php

		{
			"_id" : ObjectId("54b6bc6f98cac76ee552f548"),
			// This portion of the document is dedicated to session info
			// and can be accessed via a key->value pair system
			lux_info:{
				user_id:ObjectId("54b6bc6f98cac76ee552f548"), // this is optional for restoring session in the future
				,expiration:
				,session_id:
				,cookie_id:
				// other meta data
			}
		}

### Files.*
		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
			url: // url to access the document on S3
			access_rights:
			owner:
			uploader:
			time_uploaded:
			file_info:{
				type:
				size:
				name:
				ect.
			}
			lux_info:{
				// whatever meta data you want, just don't return it to the client
			}
		}


### System.Groups

		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
			values:{ // set by **User Management API**
						email:
						name:
						phone_num:
						ect.
			},
			profile_access_level: // set by **User Management API**
			permissions:{
					,assets:{
						owner: []
						creator:[]
						editor:[]
						viewer:[]
						denied:[]
					}
					social_network:{
						chat:
						inbox:
						join:
						// other things that can be restricted
					}
			},
			suspension_status:{
						user_level:{
								bool:
								reason:
								time:
								last_suspension:{
									time:
									reason:
								}
						},
						system_level:{
								bool:
								reason:
								time:
								last_suspension:{
									time:
									reason:
								}
						},
			},
			{ 
				loc:{
						type: "Point"
						, coordinates: [ 40, 5 ] 
				}
			}
		}

### System.Templates

		{
			"_id" : ObjectId("54b6bc6f98cac76ee552f548"),
			page_name:
			section_name:
			section_body:
			lux_info:{
				rendering_settings:{
					engine:
					parameters:
				}
				// whatever meta data you want, just don't return it to the client
			}
		}

### System.CMS

		{
			"_id" : ObjectId("54b6bc6f98cac76ee552f548"),
			page_name:
			section_name:
			section_handler:
			section_short:
			... // steal these areas from wordpress
			section_full:
			lux_info:{
				// whatever meta data you want, just don't return it to the client
			}
			timestamp:
		}

### System.Settings

### System.API
		{ 
			_id: ObjectId("507f1f77bcf86cd799439011"),
				service:
				base_url:
				in_OAuth_url:{
					key:value
					access_token:ajdsnsadsa
					client_id:adasfdskjab
					// other key:value pairs for 
				},
				in_Server_url:{
					key:value
					access_token:ajdsnsadsa
					client_id:adasfdskjab
					// other key:value pairs for 
				}
				
		}

### Transactions

## neo4j
Neo4j is a powerful graph database that uses JSON documents on both of the nodes and edges

### Users

		("31232132132131232312", // neo4j id
			{
				values:{
							about:
							age_range:
							birthday:
							emails:[]
							ect.
				}
				,lux_info:{
					// whatever metadata you want. Just don't return this to the client
				}
			}
		)

### Groups

		("31232132132131232312", // neo4j id
			{
				values:{
				}
				,lux_info:{
					// whatever metadata you want. Just don't return this to the client
				}
			}
		)
