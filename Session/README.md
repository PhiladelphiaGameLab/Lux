# Session Management $_1^1$
The Sesssion management system allows specific devices to save session variables (mobile, web, ect) on the server-side, so that when moving between pages/sections, or if there is a disconnect- then variables can be saved. Developers can also save access\_tokens in the session so that user's who leave the page for extended periods do not need to loose temporary data. Session is the only thing that is validated by the requesting device and not the user's access token. Devices that can hold cookies can also use a cookie as the identifier for a session. All session information is written into Mongo on each call- in order to save sessions for extended periods (without adjusting php attributes).

\$LF should also be able to access session info automatically in any other API.

Sessions are held in both the Mongo DB and the PHP Session.

## API
### Adjust attribute 
* Session id?
* key
* Value

### Fetch session
* Session id?

### Renew Session
* Session id?

### Annihilate Session
* Session id?

### Adjust attribute timeout
### Adjust Session Timeout
### Mimic User Session (admin)

### API Elaborations
Adjust Session Attribute should create a new session if none exists. Sessions are accessible via an "id". The Session tool can be used to store anything and if a session id is passed into another API then $LF should access the session variables associated with it. 

Mimic User Session is accessible only via the admin, and returns the session variables for a particular user.  Lux info should be stripped out when the whole session is requested by the javascipt

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Sessions | 
| 3rd Party: | PHP Sessions |
