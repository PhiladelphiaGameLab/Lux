# API Proxy Service $_1^1$
This allows any Proxy that is set-up to the queried from the JS by forwarding the API call to the Proxy and returning the direct results of that API call to the JS.

The API Proxy service includes a way to make OAuth calls by looking for the User's access\_token and making calls with that.

## API
### Get
* Params?
* Call
* Service

In the database must already be a service with key\_name and key specified.

### Fetch [not yet implemented] 
* Params?
* Call
* Service
* key\_name

The difference between the get and fetch calls is that fetch will use the appropriate user specific key that was obtained during OAuth

### Adjust service
* base\_url?
* key?
* key\_name?
* service

This call can be used to update the parameters that are stored in the database, this can be particularly useful if an API updates the base\_url or a new key is required, without needing to update the values across a variety of platforms

## Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | API | 
| 3rd Party: | x |

