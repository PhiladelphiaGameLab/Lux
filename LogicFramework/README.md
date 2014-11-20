# Lux Logic Framework

The Lux logic framework is an additional pre-processing layer that all Lux upserts pass through. The framework allows for custom Lux behavior upon certain user defined events. The Lux user defines these events (aka `criteria`), and the corresponding function calls that they trigger, in the `custom_criteria_and_functions.php` file.

### Criteria

Criteria, just like upserts, are represented as key-value arrays and contain the same three keys that an upsert does as well as others:

1. `update`

    A MongoDB-like query object describing the modifications an upsert must make to a file to satisfy the criteria.
    
2. `query`

    A MongoDB-like query object describing the set of files an upsert must modify to satisfy the criteria.
   
3. `collection_name`

    The name of the MongoDB collection
4. `name`

    A string describing the criteria
    
5. `function_name`

    The name of the function that should be called if the criteria is satisfied.

### Function calls

If a criteria is satisfied, its specified function is called and passed the following arguments:

1. The document the upsert will modify that satisfies the criteria.
2. The entire criteria object that was triggered. This is useful in cases where a certain function is called upon several different criteria being satisfied to determine which criteria the function was called from. It is also important for Achievements, which is a particular form of criteria and function calls.
3. The entire upsert object.

It is up to the user to implement these functions, which they should do in `custom_functions_and_criteria.php`. If an upsert modifies multiple files that satisfy the criteria, the function is called for each of these files.

### Testing

A test script is included for testing the Logic Framework. In it, test cases are specified as upsert documents that are then passed through the pre-processing layer. Different criteria should also be implemented to cover as many different cases as possible. The `mongoimport` command line utility can be used to load the included `zips.json` file into a MongoDB that can be used for testing:

```
mongoimport --db scratch --collection zips --file zips.json
```
