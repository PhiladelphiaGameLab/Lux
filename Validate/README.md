# Form Validation 
This can be used to access specific form element types and validate their input serverside. This can be as simple as checking that an email is properly formatted to sending an email to check that their email is valid or verify a phone number is legitimate. 

## API
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
* phone number (checks that it is real)

## API Clarifications
This API 1 file which can be passed many different values as an array of objects, each object is:

{
	 key: <string>
	,value:	<string>
	,validate: <type>
	,required: <bool> ?
	,deep:<bool> ?
}

Deep will specify if a field needs to be verified (phone number, email, url, ect) tested for existance- normally with a two step verification processes. 
Once a value is checked once, it does not need to be checked again if it has not changed (for deep, no harm in double checking for simple). 


There is only 1 high level variable, and that is the array "values";

