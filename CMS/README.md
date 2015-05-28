# CMS API 
This API allows developers to add content to a CMS which can be edited from within the Lux Intranet CMS Tool. The CMS API allows content to be uploaded and edited. Content can also be queried from the CMS API so that it can be rendered for the End-User.

## API
### Adjust Field
The Adjust field is used to create and edit the content in a given field on the page. This is very simple but powerful functionality. 
* field
* content\_full?
* content\_medium?
* content\_short?
* header.text?
* header.url\_safe?
* header.sub?
* picture.banner?
* picture.other[]?
* picture.slideshow[]?

### Get Field
* field - this can be an array to get multiple fields
* ~access\_token

### Adjust Field Render Options [not yet implemeneted]
* field
* setting
* option

## API Elaborations
Adjust Field Render options refers to access level required to get a field.

## Linked UI Element
* CMS UI

## Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Content | 
| 3rd Party: | x |
		

