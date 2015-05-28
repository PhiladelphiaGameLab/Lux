# Core

## Output
The output class is defined to help create a consistnat output format across all lux scripts. There are two methods within output:
* Success(code, data, results)
* Error(code, message)

where the `results` are the mongodb results, and the `data` is the mongo db documents that are found. 

In error, the `message` is a message developers can pass in to notify users of what they have done wrong.


### Codes:

| success/error | code | meaning |
|---------------|------|---------|
| success | 0 | Update was performed successfully |
| success | 1 | Query was performed successfully |
| success | 2 | Remove was performed successfully |
| error | 0 | Required parameters were not found |


## Other stuff 
The core functions are a set of function that can be used by multiple scripts. This is the Embodiement of DNR. This means that any functions that are used by multiple functions, including (but not limited to)

* Auth
* DB functions
* Upsert
* Query
* Copy
* and HTTP Related Functions

Can be abstracted into this set of functions/files. 
