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


server.listen(port, function(){
	console.log('Server Listening on port %d', port);
});

// Point to the Game Files
app.use(express.static(__dirname + '/public'));

var mongoConnection;
var db;

var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
mongoclient.open(function(err, mc){
	if(err != null){ 
		console.log("getClientId open" + err); 
	}else{
		mongoConnection = mc;
		db = mc.db("Lux2");		
	}
});

// Functions to be utilized
function getClientId(acc_tok, callback){ // done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//	mongoclient.open(function(err, mc){
	if(db == null){ console.log("getClientId open" + err); }else{
		//var db = mc.db("Lux2");
		db.collection("Users").findOne({access_token:acc_tok}, function(err, userDoc){
			if(userDoc != null){
				var userId = userDoc["_id"];
		//		mongoclient.close();
				callback(userId);
			}else{
				if(err == null){console.log("query: " + err);}
				callback(false);
			}
		});
	}
//	});
}
function query(query, userId){ // done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Queried for a doc " + query);
//	mongoclient.open(function(err, mc){
	if( db == null){ console.log("query open " + err); }else{
		//var db = mc.db("Lux2");
		db.collection("Assets").find(query, function(err, cursor){
			cursor.toArray(function(err, array){
				if(err != null){console.log("query: " + err);}else{
					console.log(userId);
					//console.log(sockets[userId]);
					if(sockets[userId] != undefined && sockets[userId] != null){
						sockets[userId].emit('query', array);
						console.log("sent query out");
					}
				}
				//mongoclient.close();
			});
		});
	}
//	});
}

function update(query, update, options, userId){ // done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//	mongoclient.open(function(err, mc){
	if(db == null){ console.log("update open " + err); }else{
	//	var db = mc.db("Lux2");
		db.collection("Assets").update(query, update, options
			,function(err, results){
			if(err != null){console.log("update: " + err);}else{
				// needs a loop
				publish(query, userId, true, false);
				console.log("updated");		
			}
	//		mongoclient.close();
			});
	}
//	});
}
function insert(doc, userId){ // done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//	mongoclient.open(function(err, mc){
	if(db  == null){ console.log("insert open " + err); }else{
		//var db = mc.db("Lux2");
		db.collection("Assets").insert(doc
			,function(err, results){
			if(err != null){console.log("insert: " + err);}else{
				publish(doc, userId, false, false);
				console.log("inserted");
			}
		//	mongoclient.close();
			});
	}
//	});
}
function remove(query, userId){ //done 
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//	mongoclient.open(function(err, mc){
	if(db == null){ console.log("remove open " + err); }else{
	//	var db = mc.db("Lux2");
		db.collection("Assets").remove(query
			,function(err, results){
			if(err != null){console.log("remove: " + err);}else{
				console.log("removed");
			}
	//		mongoclient.close();
			});
	}
//	});
}
function publish(doc, userId, multi, removed){ // done
	if(multi){
		//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//		mongoclient.open(function(err, mc){
		if(db == null){ console.log("ERROR: publish Multi open " +err);}else{
		//	var db = mc.db("Lux2");
			db.collection("Assets").find(doc, function(err, cursor){
				if(err != null){console.log("Published find: " + err);}else{
					cursor.each(function(err, docsy){
						if(err != null){console.log("ERROR: Published find doc: " + err);}else if(docsy != null){
							//console.log("Published Find Doc");
							publish(docsy, userId, false, false); 
						}else if(docsy == null){
		//					mongoclient.close();
						}	
					});
					if(removed){
						remove(doc, userId);
					}
				}
			});	
		}
//		});	
	}else{
		//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//		mongoclient.open(function(err, mc){
		if(db == null){ console.log("ERROR: publish open connection" + err); }else{
		//	var db = mc.db("Lux2");
			if(removed){doc["removed"] = true}
			doc["info"] = {sender: userId, checked_by:{python:false, node:false}};
			db.collection("Published").update({"_id":doc["_id"]}, doc, {upsert:true}
				,function(err, results){
				if(err != null){console.log("ERROR: Published: " + err);}else{
					//console.log("Published");
				}
		//		mongoclient.close();
				});
		}
//		});
	}
}

function upsert(data, userId){ // done
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
}
function subscribe(query, userId){ // done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Subscribed " + query);
//	mongoclient.open(function(err, mc){
		if(db == null){console.log("ERROR: subscribe Opening: " + err);}else{
		//var db = mc.db("Lux2");
		db.collection("Subscribers").update({query:query},
				{'$addToSet':{'subscribers': {id: userId}}},
				{upsert:true}, function(err, results){
					if(err != null){ console.log("ERROR: sub update " + err); }else{
		//				mongoclient.close();
					}
				});
		}
//	});
}
function unsubscribeAll(userId){// done
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Unsubscribed to all");
//	mongoclient.open(function(err, mc){
		if(db == null){console.log("ERROR: unsubAll: " + err);}else{
		//var db = mc.db("Lux2");
		db.collection("Subscribers").update({}, 
				{'$pull':{'subscribers': {id: userId}}},
				{upsert:true, multiple:true}, function(err, results){
					if(err != null){ console.log("ERROR: unsub update " + err); }else{
		//				mongoclient.close();
					}
				});
		}
//	});
}
var sockets = {};

// Socket Logic
io.on('connection', function(socket){ // done
	socket.emit('connected', {status: "successfully connected"});
	// for when you join!
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
						console.log("A user has connected " + userId);
					}else{
						console.log("you messed up bro");
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
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//mongoclient.open(function(err, mc){
	if(db == null){ console.log("ERROR: SU Opening DB" + err); }else{
	//	var db = mc.db("Lux2");
		db.collection("Subscribers").find({'subscribers' : {$not: {$size:0}}},
		function(err, subscriptions){
			if(err != null){ console.log("ERROR: SU Found Documents" + err); }else{
				//console.log("SU Found Documents");
				subscriptions.each(function(err, subscription){
					if(err != null){ console.log("ERROR: SU Iterating Documents " + err);
					}else if(subscription != null){
						var query = subscription.query;
						var subscribers = subscription.subscribers;
						query["info.checked_by.node"] = false;
						//console.log("SU Iterating Documents Found");
						findPublishedDocs(query, subscribers);
						//removeUpdated();
					}else{
	//					mongoclient.close();
					}
				});
			}
		});
	}
//});
}
// Step 2:
// Successfully Finds Docs and Iterates them
function findPublishedDocs(query, subscribers){
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//mongoclient.open(function(err, mc){
	if(db == null){ console.log("ERROR: FPD Opening DB" + err); }else{
	//	var db = mc.db("Lux2");
		db.collection("Published").find(query,
		function(err, publishedDocs){
			if(err != null){ console.log("ERROR: FPD Found Docs" + err); }else{
				//console.log("FPD Found Docs");
				publishedDocs.each(function(err, published){
					if(err != null){ console.log("ERROR: FPD Iterating Found Docs" + err);
					}else if(published != null){
						// emit each doc to subscribers
						//console.log("FPD Iterating Found Docs");
						emitUpdates(published, subscribers);
						updatePubDoc(published);
					}else{
	//					mongoclient.close();
					}
				});
			}
		});
	}
//});
}
// Step 3:
//
function emitUpdates(published, subscribers){
	subscribers.forEach(function(subscriber){
		if(sockets.hasOwnProperty(subscriber.id)){
		//console.log("Subscriber id: " + subscriber.id);
			if(subscriber != null && sockets[subscriber.id] != null){
				sockets[subscriber.id].emit('updated', published);
				//console.log("EU Subscriber Notified " + subscriber.id);
			}
		}
	});
}


// Step 3:
// Documents are successfully Updated
function updatePubDoc(published){
//var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
//mongoclient.open(function(err, mc){
	if(db == null){ console.log("ERROR: UPD DB Exists" + err); }else{
	//	var db = mc.db("Lux2");
		db.collection("Published").update({"_id":published._id}, {$set:{"info.checked_by.node":true}}
		,function(err, results){
			if(err != null){ console.log("ERROR: UPD Updating Doc" +err); }else{
				//console.log("Updated Pub Doc");
	//			mongoclient.close();
			}
		});
	}
//});
}

function removeUpdated(){
	if(db == null){ console.log("ERROR: RU open" + err); }else{
		db.collection("Published").remove({"info.checked_by.node":true, "info.checked_by.python":true}
		,function(err, results){
			if(err != null){ console.log("ERROR: RU removing the documents" +err); }else{
				console.log("Removed Documents");
			}
		});
	}
}

setInterval(sendUpdates, 1000);

