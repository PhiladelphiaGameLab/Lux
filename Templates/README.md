# Template Engine Render (Jade, Markdown, JMU [JSON MarkUp]) $_3^1$
This Service allows developers to query for a document and have it rendered into HTML, pdf, or another format before it is delivered to the End-User. This service will also connect to the CMS, to pull content items. The Template render should also include routing options to tell HTTP requests which file should be accessed. 

The initial implementation only needs to handle Rendering HTML, and needs to be able to handle routing problems. Hooks are specified by {{ field }}

## API
### Fetch Page
* Template

### Change Render Settings


## API Elaborations
Content From the hooks are pulled from the CMS service. Multiple Pages can be concatenated in order to allow headers and footers to be used. 

The flow is: fetch page, switch out content, render as html, echo to page. Mustache style loops can be implemented for information that may be variable (a list or items or time entries). 

### Document Structure

| Property | Value |
|----------|-------|
| DB Provider: | Mongo |
| Database(s): | System |
| Collection(s): | Settings && Templates| 
| 3rd Party: | x |
