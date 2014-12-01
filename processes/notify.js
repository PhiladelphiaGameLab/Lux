// Connect to the user database to access tokens and _ids
// Connect to other collections for more info

// Open server and listen for new connections
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('../..')(server);
var port = process.env.PORT || 3000;

var Db = require('mongodb').Db,
    MongoClient = require('mongodb').MongoClient,
    Server = require('mongodb').Server,
    ReplSetServers = require('mongodb').ReplSetServers,
    ObjectID = require('mongodb').ObjectID,
    Binary = require('mongodb').Binary,
    GridStore = require('mongodb').GridStore,
    Grid = require('mongodb').Grid,
    Code = require('mongodb').Code,
    BSON = require('mongodb').pure().BSON,
    assert = require('assert');

	// Set up the connection to the local db
    var mongoclient = new MongoClient(new Server("localhost", 27017), {native_parser: true});
	


function getClientId(acc_tok){
	mongoclient.open(function(err, mongoclient) {
		var db = mongoclient.db("Lux");
		db.collection('Users').findOne({"access_token": acc_tok}, 
		function(err, results){
			if(results != null){
				return results._id;
			}else{
				return "loggonError";
			}
			mongoclient.close();
		});
	});
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
			}
			if(message.hasOwnProperty("id")){
				params.id = message.id;	
				params.query = {"_id": message.id};
			}else if(message.hasOwnProperty("query")){
				params.query = message.query;
			}
		}else{ // insert the document
			params.insert = true;
		}
		if(params.hasOwnProperty("remove")){
		        mongoclient.open(function(err, mongoclient) {
		                var db = mongoclient.db("Lux");
        		        db.collection('Assets').remove(params.query, {w:1}
                		function(err, results){
					socket.emit("upsert", results);
				});
        		});

		}else if(params.hasOwnProperty("insert")){
                        mongoclient.open(function(err, mongoclient) {
                                var db = mongoclient.db("Lux");
                                db.collection('Assets').insert(message.doc, {w:1}
                                function(err, results){
					socket.emit("upsert", results);
                                });
                         });
		}else{
			mongoclient.open(function(err, mongoclient) {
                                var db = mongoclient.db("Lux");
                                db.collection('Assets').update(message.query, message.doc, 
				{upsert: true, w:1},
                                function(err, results){
					socket.emit("upsert", results);
				});
                         });
		}
		// Add upsert to Published with references resolved
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
                	var db = mongoclient.db("Lux");
                        db.collection('Assets').findOne({"_id" : message.id},
                        function(err, results){
         	               socket.emit("query", results);
                        });
		}else if(message.hasOwnProperty("query"){
			var db = mongoclient.db("Lux");
                        db.collection('Assets').findOne(message.query,
                        function(err, results){
                               socket.emit("query", results);
                        });
		}

		// Add Query to Subscribed
		var cursor = //// find all by query
		if(cursor != null){
			//// update subscribed
		}else{
			//// insert subscription document
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





