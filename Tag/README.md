# Tag API $_1^2$
The Tag system allows tags to be added to an Asset and should be stored directly in the Asset. The Tag API allows developers to query based on tag, add tags, and get the most popular tags without writing custom queries for each operation. Tags are short (1 word or a short sentence) describing a user or Asset. 

Later this should include tags on Users too- but for now, just on Assets

## API
### Adjust Tag to Asset Profile
* coll?
* id
* tag(s) 

### Get Assets By Tag
* coll?
* tag(s)

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | Assets |
| Collection(s): | * | 
| 3rd Party: | x |

