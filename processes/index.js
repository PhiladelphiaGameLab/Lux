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

// Functions to be utilized
function getClientId(acc_tok, callback){
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	mongoclient.open(function(err, mc){
		var db = mc.db("Lux2");
		db.collection("Users").findOne({access_token:acc_tok}, function(err, userDoc){
			if(userDoc != null){
				var userId = userDoc["_id"];
				mongoclient.close();
				callback(userId);
			}else{
				callback(false);
			}
		});
	});
}
function query(query, userId){ // yes
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Queried for a doc " + query);
	mongoclient.open(function(err, mc){
		var db = mc.db("Lux2");
		db.collection("Assets").find(query, function(err, cursor){
			cursor.toArray(function(err, array){
				mongoclient.close();
				return array;
			});
	});
});
}
function upsert(data, userId){ 
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Upserted a doc" + data);
	mongoclient.open(function(err, mc){
		var db = mc.db("Lux2");
		var removed = false;
		if(data.hasOwnProperty("update") && data.hasOwnProperty("query")){
			// upsert
			if(data.update.hasOwnProperty('$set')){
				var options = {upsert:true, multiple:true};
			}else{
				var options = {upsert:true};
			}
			db.collection("Assets").update(data.query, data.update, options, function(err, results){})
			documents = db.collection("Assets").find(data.query);
		}else if(data.hasOwnProperty("query")){
			// remove
			documents = db.collection("Assets").find(data.query);
			db.collection("Assets").remove(data.query);
			removed = true;
		}else{
			// insert	
			db.collection("Assets").insert(data.update, function(){});
			documents = db.collection("Assets").find(data.update);
		}
		documents.each(function(err, doc){
			if(err == null && doc != null){
				if(removed == true){ doc["removed"] = true; }
				doc["info"] = {sender: userId, checked_by:{python:false, node:false}};
				db.collection("Published").insert(doc, function(){});
			}
		});
		mongoclient.close();
	});
}
function subscribe(query, userId){ // yes
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Subscribed " + query);
	mongoclient.open(function(err, mc){
		var db = mc.db("Lux2");
		db.collection("Subscribers").update({query:query}, 
				{'$addToSet':{'subscribers': {id: userId}}},
				{upsert:true}, function(err, results){
					mongoclient.close();
				});
	});
}
function unsubscribeAll(userId){// yes
var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
	//console.log(userId + " Unsubscribed to all");
	mongoclient.open(function(err, mc){
		var db = mc.db("Lux2");
		db.collection("Subscribers").update({}, 
				{'$pull':{'subscribers': {id: userId}}},
				{upsert:true, multiple:true}, function(){
					mongoclient.close();
				});
	});
}
var sockets = {};

// Socket Logic
io.on('connection', function(socket){
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
						console.log("you fucked up bro");
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
		upsert(data, socket.id);
	});
	socket.on('query', function(data){	
		socket.emit('query', query(data, socket.id));
		subscribe(data.query, socket.userId);
	});
	socket.on('disconnect', function(){
		delete sockets[socket.id];
		unsubscribeAll(socket.id);
	});

});

function emitUpdates(){
var mongoclient1 = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
var mongoclient2 = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
var mongoclient3 = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
var mongoclient4 = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});

mongoclient1.open(function(err, mc1){
	if(err != null){console.log("Connection : " +err);}
	var db1 = mc1.db('Lux2');
	db1.collection('Subscribers').find({'subscribers' : {$not: {$size:0}}}, 
		function(err, subscriptions){
			if(err != null){
				console.log("finding Subscription Documents: " + err);	
			}else if(subscriptions != null){
				subscriptions.each(function(err, subscription){
					if(err != null){
						console.log("iterating Subscription Documents: " + err);
					}else if(subscription != null && subscription.hasOwnProperty("query")){
						var query = subscription.query;
						var subscribers = subscription.subscribers;
						query["info.checked_by.node"] = false;
						//mongoclient2.open(function(err, mc2){
						//	if(err != null){console.log(err);}
						//	var db2 = mc2.db('Lux2');
							db1.collection('Published').find(query,
								function(err, published){
									if(err != null){
										console.log("Finding Published Documents" + err);
									}else if(published != null){
										published.each(function(err, publish){
											if(err != null){
												console.log("Iterating Published Documents " + err);
											}else if(publish != null){
												subscribers.forEach(function(err, subscriber){
													if(err != null){
														console.log("Iterating Subscribers: " + JSON.stringify(err));
													}else if(subscriber != null){
														if(sockets.hasOwnProperty(subscriber)){
															sockets[subscriber].emit('updated', publish);
															console.log("Subscriber Notified");
														}
													}
												});
												//mongoclient3.open(function(err, mc3){
												//	var db3 = mc3.db('Lux2');
													db1.collection('Published').update({"_id":publish._id}, {$set:{"info.checked_by.node":true}}
														,function(err, doc){
															if(err != null){
																console.log("Updating Published: " + err);
															}else if(doc != null){
																console.log("Updated Published " + doc);
															}
															//mongoclient3.close();
														});
												//});
											}
										});			
									}
									//mongoclient2.close();
								});
						//});	
					}
				});
			//mongoclient1.close();
			}
		});

});
}


function emitMessages(){
var mongoclient2 = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});
mongoclient2.open(function(err, mc){
	var db = mc.db('Lux2');
	db.collection('Subscribers').find({'subscribers' : {$not: {$size:0}}}, function(err, subscriptions){
		//console.log("Subscription Queried");
		subscriptions.each(function(err, subscription){
			if(subscription != null){
			//console.log("Subscription Found");
				var query = subscription.query;
				query["info.checked_by.node"] = false;
				db.collection('Published').find(query, function(err, publish){
					if(publish != null){
				//	console.log("Published Queried");
					publish.each(function(err, doc){
						console.log(doc);
						if(doc != null){
						console.log("Published Found" + doc);
							db.collection("Published").update({"_id":doc._id}, {$set:{"info.checked_by.node":true}}, function(err, doc){});
							console.log("Published Updated");
							subscription.subscribers.forEach(function(err, subscriber){
								if(sockets.hasOwnProperty(subscriber.id)){
									sockets[subscriber.id].emit('updated', doc);
									console.log("Subscriber Notified");
								} 
								mongoclient2.close();
							});
						}else{
							//mongoclient2.close();
						}
					});
					}
					//if(publish == null){ mongoclient2.close();}
				});
			}else{
				//mongoclient2.close();
			}
		});
		//if(subscriptions == null){ mongoclient2.close();}
	});
});
}


setInterval(emitUpdates, 100);

