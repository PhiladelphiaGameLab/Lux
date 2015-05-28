# User Management API $_1^0$
This API allows storage and creation of user documents, updating user values, and fetching the User's document. User documents should not be accessible via Upsert- but only Upsert- Key:Value and Query. Only a user should be able to access their own document, other user's should not have direct access to this document. If a User's profile information is to be made public to end-users, they should have a separate document for that in the User Profile API. This should contain log-in information, access_token, general system related info like settings. This is where their social access tokens are stored, however those are not exposed to the front-end. They should however be consistently refreshed so that they do not expire (unless they are given a lux expiration date). 

The difference between this and the User Profile API is that this information is protected/private information, so it should not be accessible outside of the server. Some information can be duplicated between this and the Users Profile API. 

## API
### Fetch Me

		Lux/Users/fetchMe.php?access_token=

### Adjust (key->value)
* name.full
* email.primary 
* email.other[] 
* name.given 
* name.family 
* pictures.primary
* pictures.other[]
* gender
* locale
* domain  

		Lux/Users/adjust.php
			{name.first:"jake", name.last:"Omega"}

**Obviously a larger set of acceptable key values will need to be provided, I am just not sure what they are yet**

### Adjust Profile Access Level (not yet implemeneted)

### Adjust Account Suspension (not yet implemented)

## API Elaborations
The key->value pairs are meant to be edited in user profile information. Change Profile Access level is an Admin functionality, and requires either dev access or the highest access level in the application. 

## Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | x |

