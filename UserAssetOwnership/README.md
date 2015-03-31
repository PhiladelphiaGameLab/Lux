# User Asset Ownership API $_2^0$
The User Ownership API is designed to track what Asset's a user is permitted to update. This API is designed to be hooked into the User's Document, and should automatically be checked from the Assets API to see if the user has proper permissions to change that Asset. 

Assets need to be 0,1,2 in order to be altered, and 0,1,2,3 in order to be viewed. If the Asset is level 4, then even if it is global, the user will be denied both viewing and editing rights

Some Assets will be globally alterable, and should thus be marked as such in their lux\_info. Some assets will be alterable by multiple users, and will therefore need multiple users to reference the Assset's Id.

The script "userAssetOwnership" is called from within the Assets API in order to determine if the User has the correct permissions to edit the file. 

## API
### Adjust Permission level
Sets the permission level for a given asset by \_id. Revoke can be used to remove permissions from that \_id.

* asset
* level
* revoke? 

### Fetch Permission level [not yet implemented]
Gets the permission level for a given asset
* asset

### Fetch Assets at permission level
Gets the list of assets at a given permission level 
* level

### Get User's Assets
Gets the assets owned by a particular user at a given level
* user\_id

## API Elaborations
Get User's Assets refers to getting assets that another user owns (public assets only). Fetch Permission level is the permission level that the User has on the Asset/


## Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Users | 
| 3rd Party: | x |
