// oped,    69448k free,    25712k buffers
// n a server instance
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
server.listen(port, function(err){
	if(err != null){ console.log("Server not established" + err) }else{
		console.log('Server Listening on port %d', port);
	}
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
		console.log("Connecting to db" + " : " + new Date().getTime());
		db = mc.db("Lux2");		
		console.log("Connected to db" + " : " + new Date().getTime());
	}
});
// Delcare the list of Sockets Online
var sockets = {};

// Get the client Id, only need to do this once
// Since if the client disconnects then they will
// Get a new port
function getClientId(acc_tok, callback){ // done
	if(db == null){ console.log("ERROR: getClientId Database Not Open "); }else{
		db.collection("Users").findOne({access_token:acc_tok}, function(err, userDoc){
			if(err != null){ console.log("ERROR: getClientId Query Failed " + err); }else{
				if(userDoc != null && userDoc.hasOwnProperty("_id")){
					var userId = userDoc["_id"];
					console.log("getClientId: Found socket corresponding to '_id'" + " : " + new Date().getTime());
					callback(userId, acc_tok);
				}else{
					console.log("ERROR: getClientId user Not in Users collection" + " : " + new Date().getTime());
					callback(false, acc_tok);
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
							console.log("Query: Emitting Results" + " : " + new Date().getTime());
							sockets[userId].emit('query', array);
						}else{
							console.log("ERROR: query userId is not Registered" + " : " + new Date().getTime());
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
					console.log("Update: Publishing Document" + " : " + new Date().getTime());
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
					console.log("Insert: Publishing Document" + " : " + new Date().getTime());
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
					console.log("Remove: Document Removed" + " : " + new Date().getTime());
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
								console.log("Publish: Publishing Documents" + " : " + new Date().getTime());
							/*	if(singleDocument.hasOwnProperty("attackData")){
									console.log("SINGLE DOCING AN ATTACK!!!! " + singleDocument._id);	
								}else{
									publish(singleDocument, userId, false, false); 
								}*/
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
					console.log("Publish: Document Published" + " : " + new Date().getTime());
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
			console.log("Upsert: Updating Document" + " : " + new Date().getTime());
			if(data.hasOwnProperty("query") && data.hasOwnProperty("update")){
				update(data.query, data.update, options, userId);
			}
		}else if(data.hasOwnProperty("query")){
			// remove
			console.log("Upsert: Publishing Document" + " : " + new Date().getTime());
			if(data.hasOwnProperty("query")){
				publish(data.query, userId, true, true);
			}
		}else{
			// insert	
			console.log("Upsert: Upserting Document" + " : " + new Date().getTime());
			if(data.hasOwnProperty("update")){
				insert(data.update, userId);
			}
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
					console.log("Subscribe: Subscribed" + " : " + new Date().getTime());
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
					console.log("Unsubscribe: Unsubscribing a User from ALl documents" + " : " + new Date().getTime());
					if(err != null){ console.log("ERROR: unsubscribe Query Failed " + err); }
				});
		}
}

// Socket Logic
// Handles A socket Connection and Decides what
// Needs to be executed based on data passed 
var emitting = false;
io.on('connection', function(socket){ 
	socket.emit('connected', {status: "successfully connected"});
	socket.connected = false;
	socket.on('join', function(data){
		if(!socket.connected){
			if(data.hasOwnProperty("access_token")){
				getClientId(data.access_token, function(userId, access_token){
					if(userId != false && userId != null){
						socket.userId = userId;
						console.log("\t\t\t\t\t\t\t\t\t\tJoin: Recieved Access_token: " + access_token + " : " + new Date().getTime());
						socket.access_token = access_token;
						socket.connected = true;
						sockets[userId] = socket;
						socket.emit('joined', {status: 'connected', "id": userId, "time" : new Date().getTime()});
						console.log("\t\t\t\t\t\t\t\t\t\tclient joined " + userId);
					}else{
						console.log("ERROR: s.io join: access_token invalid" + " : " + new Date().getTime());
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
		console.log("Recieved Message From: " + socket.access_token + ", at : " + new Date().getTime());
		/*while(emitting){ 
			; 
		}*/
		upsert(data, socket.userId);
	});
	socket.on('query', function(data){	
		query(data, socket.userId);
		console.log("Recieved Query From: " + socket.access_token + "time : " + new Date().getTime());
		subscribe(data.query, socket.userId);
	});
	socket.on('disconnect', function(){
		delete sockets[socket.userId];
		//unsubscribeAll(socket.userId);
	});

});





/* Real time Updates section */




// Step 1.
// Successfully Finds Documents and iterates them 
function sendUpdates(){
	//removeUpdated();
	// only do this if someone is online
	emitting = true;
	console.log("\t\t\t\t\tRunning SendUpdates function" + " : " + new Date().getTime());
	if(Object.keys(sockets).length != 0){
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
								console.log("\t\t\t\t\tSendUpdates: finding Published Docs" + " : " + new Date().getTime());
								findPublishedDocs(query, subscribers);
							}
						}
					});
				}
			});
		}
	}else{
		console.log("\t\t\t\t\tSendUpdates: Clearing Data, no users online");
		clearPublished();
		removeAssets();
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
							console.log("\t\t\t\t\tFind Published Docs: Emitting Updated Docs " + " : " + new Date().getTime());
							emitUpdates(published, subscribers);
							console.log("\t\t\t\t\tFind Published Docs: Updating Published Docs " + " : " + new Date().getTime());
							updatePubDoc(published);
						}
					}
				});
			}
		});
	}
}
// Step 3: 
// Sends Updates to Users who are Subscribed and Online
function emitUpdates(published, subscribers){
	subscribers.forEach(function(subscriber){
		if(sockets.hasOwnProperty(subscriber["id"])){
			if(subscriber != null && sockets[subscriber["id"]] != null){
				console.log("Emited message to: " + sockets[subscriber["id"]].access_token + " : " + new Date().getTime());
				console.log("\t\t\t\t\tEmited message to: " + sockets[subscriber["id"]].access_token + " : " + new Date().getTime());
				sockets[subscriber["id"]].emit('updated', published);
			}
		}
	});
}


// Step 3:
// Documents are successfully Updated
function updatePubDoc(published){
	if(db == null){ console.log("ERROR: UPD Database Not Open" + err); }else{
		db.collection("Published").remove({"_id":published._id} //{$set:{"info.checked_by.node":true}}
		,function(err, results){
			if(err != null){ console.log("ERROR: Query Failed" +err); }else{
				console.log("\t\t\t\t\tUpdate PubDoc: Removed Published Docs" + " : " + new Date().getTime());
				emitting = false;
			}
		});
	}
}
// Step 1B:
// Removes Published Documents that have been seen by both Parties 
// Should also remove documents more than 3 minutes old
// This will do nothing if the Python Script isn't running
function removeUpdated(){
	if(db == null){ console.log("ERROR: RU Database Not Open" + err); }else{
		db.collection("Published").remove({"info.checked_by.node":true}//, "info.checked_by.python":true}
		,function(err, results){
			if(err != null){ console.log("ERROR: RU Query Failed" +err);}else{
				console.log("\t\t\t\t\tremove Updated: Removed Updated Docs" + " : " + new Date().getTime());
			}
		});
	}
}
function clearPublished(){
	if(db == null){ console.log("ERROR: RU Database Not Open" + err); }else{
		db.collection("Published").remove({}//, "info.checked_by.python":true}
		,function(err, results){
			if(err != null){ console.log("ERROR: RU Query Failed" +err);}else{
				console.log("\t\t\t\t\tremove Updated: Removed Updated Docs" + " : " + new Date().getTime());
			}
		});
	}
}
function removeAssets(){
	if(db == null){ console.log("ERROR: RA Database Not Open" + err); }else{
		db.collection("Assets").remove({'LuxInfoSection':{'$exists':false}}//, "info.checked_by.python":true}
		,function(err, results){
			if(err != null){ console.log("ERROR: RA Query Failed" +err);}else{
				console.log("Remove Assets: Removed Assets" + " : " + new Date().getTime());
				console.log("\t\t\t\t\tRemove Assets: Removed Assets" + " : " + new Date().getTime());
			}
		});
	}
}

setTimeout(function(){
	console.log("\t\t\t\t\tstarting Broadcast" + " : " + new Date().getTime());
	setInterval(sendUpdates, 5);

}, 5000);

