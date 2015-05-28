# Assets 
The Asset Management API allows users to create Assets and store them in a specific Asset Database. This allows access to the Database via the standard commands, which gives users full access to the collections. The Asset Database can be broken down by collections which are defined by the developer. This Database (and set of collections) are the only ones with arbitrary structure defined by the Developer. 

## API
### Upsert
GET or POST
* coll?: the Asset collection which the user wishes to update
* options?: the options array that the user wishes to use (options can be any mongo write option). 
* update? : the update you wish to perform, if no update is present then remove is envoked
* query? : The query that should be used to find documents, id or query must be present
* id? : The id of the document you wish you wish to update, id or query must be present

	/Lux/Assets/upsert.php?coll=items&id=13123124342
		{update:}
	/Lux/Assets/upsert.php?coll=items
		{update: }
	/Lux/Assets/upsert.php?coll=items
		{update:{}, query:{}}

### Query
GET or POST 
* query? : The query that should be used to find documents, id or query must be present
* id? : The id of the document you wish you wish to retrieve, id or query must be present
* distinct? : Runs a Mongo "distinct" call instead of a find call 
* aggregate? : Runs a Mongo "Aggregate" call instead of a find call
* resolve? [not implemeneted] : Resolves any Document references
* ~accessi\_token? : This is optional, and will change behavior in some way if it is included.

### Adjust- Key:Value
GET or POST
* coll?: the Asset collection which the user wishes to update
* key : The key that you wish to change, dot operator is permitted here
* value : The new value of that key
* query? : The query that should be used to find documents, id or query must be present
* id? : The id of the document you wish you wish to update, id or query must be present


### API Elaborations
A Query with no access\_token will only return documents who have global access specified. All queries however should check access before returning a document. APIs should strip out lux\_info

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database: | Assets |
| Collection(s): | * | 
| 3rd Party: | x |
		

