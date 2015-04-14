# Achievements
## A Lux function
The Achievements section is based on the idea that an Achievements collection could be set-up by the developer, and any Asset "upsert" that comes in, should be compared to all the Achievements in that database to determine if an "Achievement" has been accomplished, or progress towards an achievement has been made. 

### Achievement Collection
In order to achieve the creation of the Achiements collection, there will need to be functions for:

* Upsert
* Copy
* Query 

of the Achievements database. Each of these Achievemenets should consist of:
 
* Criteria 
* Quantity
* Repeatable
* Description
* Name
* Reward - optional

As well as any other criteria that a developer feels is needed. The Upsert function should check to make sure that all of those have been added to the document, which will assure that the achievement can be used. 

### Rewarded Collection
The Set of achievements to be awarded should have the same basic set of functions of:

* Upsert
* Copy
* Query

and the Rewarded Collection should contain the criteria:

* Criteria/Name
* Progress
* Quantity (if repeatable)

#### Logic:
The Achievements Logic is that on each upsert, it should be a function that is called (or a class that is initialized) which should look at the Achievement at the Upsert and see if it matches any of the Criteria in the Achivement Collection. 

In order to make that check, you will have to query the Asset's Collection to see if the upsert is in reference to any Asset's that might be affected by one of the Critia in the Achievements Collection. 




