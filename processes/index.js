// open a server instance
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.PORT || 3000;


// open a Mongo Connection
var Db = require('mongodb').Db;
var MongoClient = require('mongodb').MongoClient;
var MongoServer = require('mongodb').Server;
var ReplSetServers = require('mongodb').ReplSetServer;
var ObjectID = require('mongodb').ObjectID;
var Binary = require('mongodb').Binary;
var GridStore = require('mongodb').Grid;
var Code = require('mongodb').Code;
var BSON = require('mongodb').pure().BSON;

// Open the server port
server.listen(port, function(){
	console.log('Server Listening on port %d', port);
});

// Point to the Game Files
app.use(express.static(__dirname + '/public'));

// declare the database
var mongoConnection;
var db;

// Only open the database connection Once
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
mongoclient.open(function(err, mc){
	if(err != null){ 
		console.log("getClientId open" + err); 
	}else{
		mongoConnection = mc;
		db = mc.db("Lux2");		
	}
});
// Delcare the list of Sockets Online
var sockets = {};

// Get the client Id, only need to do this once
// Since if the client disconnects then they will
// Get a new port
function getClientId(acc_tok, callback){ // done
	if(db == null){ console.log("ERROR: getClientId Database Not Open " + err); }else{
		db.collection("Users").findOne({access_token:acc_tok}, function(err, userDoc){
			if(err != null){ console.log("ERROR: getClientId Query Failed " + err); }else{
				if(userDoc != null && userDoc.hasOwnProperty("_id")){
					var userId = userDoc["_id"];
					callback(userId);
				}else{
					callback(false);
				}
			}
		});
	}
}

// Query the Database and return the Results
// Should only be utilized if Game has extended Map
// Should not be used for any other reason
function query(query, userId){
	if( db == null){ console.log("ERROR: query Database Not Open" + err); }else{
		db.collection("Assets").find(query, function(err, cursor){
			if(err != null){ console.log("ERROR: query Query Failed " + err); }else{
				cursor.toArray(function(err, array){
					if(err != null){console.log("ERROR: query Conversion to Array " + err);}else{
						if(sockets.hasOwnProperty(userId) && sockets[userId] != undefined && sockets[userId] != null){
							sockets[userId].emit('query', array);
						}else{
							console.log("ERROR: query userId is not Registered");
						}
					}
				});
			}
		});
	}
}

// Part of Upsert
// Update the Database when Asset State Changes 
// No response to the Updating client 
// (The Document should be updated on their side anyway)
function update(query, update, options, userId){
	if(db == null){ console.log("ERROR: update Database Not Open " + err); }else{
		db.collection("Assets").update(query, update, options
			,function(err, results){
				if(err != null){console.log("ERROR: update Query Failed" + err);}else{
					publish(query, userId, true, false);
				}
			});
	}
}

// Part of Upsert
// Insert to Database when Asset is created
// No resposne to the Updating Client
// (The Document should be updated on their side anyway)
function insert(doc, userId){ 
	if(db  == null){ console.log("ERROR: insert Database Not Open " + err); }else{
		db.collection("Assets").insert(doc
			,function(err, results){
				if(err != null){console.log("ERROR: insert Query Failed " + err);}else{
					publish(doc, userId, false, false);
				}
			});
	}
}

// Part of Upsert
// Remove From databse When Asset is created
// No response to the Updating Client
// Should almost never be used, documents should be marked as Deleted 
// Rather then actually deleted
function remove(query){ 
	if(db == null){ console.log("ERROR: remove Database Not Open" + err); }else{
		db.collection("Assets").remove(query
			,function(err, results){
				if(err != null){console.log("ERROR: remove Query Failed" + err);}else{
					// success publish needs to be done prior to removal
					// so there is no operation to call here
				}
			});
	}
}

// Publish the document so it can be updated to other clients
// Used for any Upsert Operation
// Function is recursive on Queries
function publish(doc, userId, multi, removed){ 
	// Need to know if a loop is necissary
	if(multi){
		if(db == null){ console.log("ERROR: publish (multi) Database Not Open " +err);}else{
			db.collection("Assets").find(doc, function(err, cursor){
				if(err != null){console.log("ERROR: publish (multi) Query Failed: " + err);}else{
					cursor.each(function(err, singleDocument){
						if(err != null){console.log("ERROR: publish (multi) Iteration: " + err);}else{
							if(singleDocument != null){
								publish(singleDocument, userId, false, false); 
							}
						}
					});
					if(removed){
						// call removed here since 
						// This is where the document needs to be removed
						remove(doc);
					}
				}
			});	
		}
	// Single document needs to be inserted 
	}else{
		if(db == null){ console.log("ERROR: pulish Database Not Open" + err); }else{
			if(removed){
				doc["removed"] = true
			}
			doc["info"] = {sender: userId, checked_by:{python:false, node:false}};
			db.collection("Published").update({"_id":doc["_id"]}, doc, {upsert:true}
				,function(err, results){
					if(err != null){console.log("ERROR: publish Query Failed: " + err);}
				});
		}
	}
}

// Upsert Handles and redirects Upsert Requests
// Should be constantly Utilized Throughout the Game
// Every Asset that is updated shuold pass through here
function upsert(data, userId){
	if(sockets.hasOwnProperty(userId) && sockets[userId] != undefined && sockets[userId] != null){
		var removed = false;
		if(data.hasOwnProperty("update") && data.hasOwnProperty("query")){
			// upsert
			if(data.update.hasOwnProperty('$set')){
				var options = {upsert:true, multiple:true};
			}else{
				var options = {upsert:true};
			}
			update(data.query, data.update, options, userId);
		}else if(data.hasOwnProperty("query")){
			// remove
			publish(data.query, userId, true, true);
		}else{
			// insert	
			insert(data.update, userId);
		} 
	}else{
		console.log("ERROR: upsert userId is not Registered");
	}
}

// Subscribes the User to the Query 
// Should only be used on Query
// Should only be used if Game has extended Map
function subscribe(query, userId){
		if(db == null){console.log("ERROR: subscribe Database Not Open: " + err);}else{
		db.collection("Subscribers").update({query:query},
				{'$addToSet':{'subscribers': {id: userId}}},
				{upsert:true}, function(err, results){
					if(err != null){ console.log("ERROR: subscribe Query Failed " + err); }
				});
		}
}

// Unsubscriber Users when they Disconnect
// Prevents System from trying to send Ghost Docs
function unsubscribeAll(userId){
		if(db == null){console.log("ERROR: unsubscribe Database Not Open: " + err);}else{
		db.collection("Subscribers").update({}, 
				{'$pull':{'subscribers': {id: userId}}},
				{upsert:true, multiple:true}, function(err, results){
					if(err != null){ console.log("ERROR: unsubscribe Query Failed " + err); }
				});
		}
}

// Socket Logic
// Handles A socket Connection and Decides what
// Needs to be executed based on data passed in
io.on('connection', function(socket){ 
	socket.emit('connected', {status: "successfully connected"});
	socket.connected = false;
	socket.on('join', function(data){
		if(!socket.connected){
			if(data.hasOwnProperty("access_token")){
				getClientId(data.access_token, function(userId){
					if(userId != false && userId != null){
						socket.userId = userId;
						socket.connected = true;
						sockets[userId] = socket;
						socket.emit('joined', {status: 'connected'});
					}else{
						socket.emit('error_lux', {'error_lux': "access Token Invalid"});
					}
				});
			}else{
				socket.emit('error_lux', {error_lux: "access_token is missing"});
			}
		}else{
			socket.emit('error_lux', {error_lux: "you are already connected"});
		}
	});
	socket.on('upsert', function(data){
		upsert(data, socket.userId);
	});
	socket.on('query', function(data){	
		query(data, socket.userId);
		subscribe(data.query, socket.userId);
	});
	socket.on('disconnect', function(){
		delete sockets[socket.userId];
		unsubscribeAll(socket.userId);
	});

});





/* Real time Updates section */




// Step 1.
// Successfully Finds Documents and iterates them 
function sendUpdates(){
	// only do this if someone is online
	if(sockets.length != 0){
		if(db == null){ console.log("ERROR: SU Database Not Open " + err); }else{
			db.collection("Subscribers").find({'subscribers' : {$not: {$size:0}}},
			function(err, subscriptions){
				if(err != null){ console.log("ERROR: SU Query Failed " + err); }else{
					subscriptions.each(function(err, subscription){
						if(err != null){ console.log("ERROR: SU Iterating Documents " + err);}else{
							if(subscription != null){
								var query = subscription.query;
								var subscribers = subscription.subscribers;
								query["info.checked_by.node"] = false;
								findPublishedDocs(query, subscribers);
							}
						}
					});
				}
			});
		}
	}else{
		removeUpdated();
	}
}


// Step 2:
// Successfully Finds Docs and Iterates them
function findPublishedDocs(query, subscribers){
	if(db == null){ console.log("ERROR: FPD Database Not Open " + err); }else{
		db.collection("Published").find(query,
		function(err, publishedDocs){
			if(err != null){ console.log("ERROR: FPD Query Failed " + err); }else{
				publishedDocs.each(function(err, published){
					if(err != null){ console.log("ERROR: FPD Iterating Documents " + err);}else{
						if(published != null){
							emitUpdates(published, subscribers);
							updatePubDoc(published);
						}
					}
				});
			}
		});
	}
//});
}
// Step 3: 
// Sends Updates to Users who are Subscribed and Online
function emitUpdates(published, subscribers){
	subscribers.forEach(function(subscriber){
		if(sockets.hasOwnProperty(subscriber.id)){
			if(subscriber != null && sockets[subscriber.id] != null){
				sockets[subscriber.id].emit('updated', published);
			}
		}
	});
}


// Step 3:
// Documents are successfully Updated
function updatePubDoc(published){
	if(db == null){ console.log("ERROR: UPD Database Not Open" + err); }else{
		db.collection("Published").update({"_id":published._id}, {$set:{"info.checked_by.node":true}}
		,function(err, results){
			if(err != null){ console.log("ERROR: Query Failed" +err); }
		});
	}
}
// Step 1B:
// Removes Published Documents that have been seen by both Parties 
// Should also remove documents more than 3 minutes old
// This will do nothing if the Python Script isn't running
function removeUpdated(){
	if(db == null){ console.log("ERROR: RU Database Not Open" + err); }else{
		db.collection("Published").remove({"info.checked_by.node":true, "info.checked_by.python":true}
		,function(err, results){
			if(err != null){ console.log("ERROR: RU Query Failed" +err); }
		});
	}
}

setInterval(sendUpdates, 100);

