var mongo require('mongodb');
 
// Connect to the user database to access tokens and _ids
// Connect to other collections for more info

// Open server and listen for new connections
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('../..')(server);
var port = process.env.PORT || 3000;

function getClientId(access_token){
	// look in user database for the access_token
	// query Users collection on the access_token
	
	// get the id from the returned document	

	// return the user's id if one exists, or an error
	if(id != null){
		return id;
	}else{
		return "loggonError";
	}
}


app.use(express.static(__dirname + '/public'));

server.listen(port, function(){
	console.log("Server is now on at port %d', port);
});

var sockets = {};
var numUsers = 0;

io.on('connection', function(socket){
	// If someone connects, look at their access code and get their _id
	socket.on('add user', function(username){
		socket.access_token = access_token;
		// figure out the user's _id
		var id = getClientId(access_token);
		if(id != "loggonError"){
			socket.id = id;
			// Save to a data structure the socket and the _id
			sockets[id] = socket;
			++numUsers;
		}else{
			socket.send('error', {
				error : "Please log on, it's rude to be anon."
			});
		}
	});
	// listen for messages labled as "upsert, or query"
	socket.on('upsert', function(message){
		// upsert the message
		var params = {};
		if(message.hasOwnProperty("doc")){
			params.update = message.doc;
			params.query = message.doc;	
		}
		if(message.hasOwnProperty("id") || message.hasOwnProperty("query")){
			if(message.hasOwnProperty("doc")){
				params.upsert = true
			}else{
				params.remove = true;
				params.newItem = false;
				params.update = null;
			}
			if(message.hasOwnProperty("id")){
				params.id = message.id;	
				//// make this into a query
			}else if(message.hasOwnProperty("query")){
				params.query = message.query;
			}
		}else{ // remove
			params.insert = true;
		}
		var result = "";
		if(params.hasOwnProperty("remove")){
			result = //// remove the document
		}else if(params.hasOwnProperty("insert")){
			result = //// insert the document
		}else{
			result = //// update the document
		}
		// Add upsert to Published with references resolved
		socket.emit("upsert", results);
		// put into the queue
		if(params.hasOwnProperty("remove")){
			//// enqueue as a remove
		}else if(params.hasOwnProperty("insert")){
			//// enqueue as an insert
		}else{
			//// enqueue as an update
		}
	});
	socket.on('query', function(message){
		// make a query and return the data 
		var result;
		if(message.hasOwnProperty("id")){
			result = //// find one by id
		}else if(message.hasOwnProperty("query"){
			result = //// find all by query
		}

		if(true){ // should subscribe
			// Add Query to Subscribed
			var cursor = //// find all by query
			if(cursor != null){
				//// update subscribed
			}else{
				//// insert subscription document
			}
		}
		socket.emit("upsert", results);
	});
	socket.on('disconnect', function(){
		delete ids[socket.id];
		--numUsers;
	}
});

function emitMessages(){
	while(1){
		// Meanwhile- cycle through the subscribers criterium
		// query the Published collection for the subscribers criteria
		// look at subscribers list from subscribers collection
		// send message to subscribers if [id] exists in "sockets" structure
		// Mark Published document as "sent to nodejs"
	}
}

emitMessages();





