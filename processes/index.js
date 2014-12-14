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

var mongoclient = new MongoClient(new MongoServer("localhost", 27017), {native_parser:true});

server.listen(port, function(){
	console.log('Server Listening on port %d', port);
});

// Point to the Game Files
app.use(express.static(__dirname + '/public'));

// Functions to be utilized
function getClientId(acc_tok){
	return acc_tok;
}
function query(query, userId){ // yes
	console.log(userId + " Queried for a doc " + query);
	mongoclient.open(function(err, mongoclient){
		var db = mongoclient.db("Lux2");
		var cursor =  db.collection("Assets").find(query);
		mongoclient.close();
		return cursor.toArray();
	});
}
function upsert(data, userId){ 
	console.log(userId + " Upserted a doc" + data);
	mongoclient.open(function(err, mongoclient){
		var db = mongoclient.db("Lux2");
		var removed = false;
		if(data.hasOwnProperty("update") && data.hasOwnProperty("query")){
			// upsert
			if(data.update.hasOwnProperty('$set')){
				var options = {upsert:true, multiple:true};
			}else{
				var options = {upsert:true};
			}
			db.collection("Assets").update(data.query, data.update, options)
			documents = db.collection("Assets").find(data.query);
		}else if(data.hasOwnProperty("query")){
			// remove
			documents = db.collection("Assets").find(data.query);
			db.collection("Assets").remove(data.query);
			removed = true;
		}else{
			// insert	
			db.collection("Assets").insert(data.update);
			documents = db.collection("Assets").find(data.update);
		}
		documents.each(function(err, doc){
			if(err == null && doc != null){
				if(removed == true){ doc["removed"] = true; }
				publish(doc, socket.userId);
			}
		});
		mongoclient.close();
	});
}
function publish(update, userId){ // yes
	console.log(userId + " Published " + update);
	mongoclient.open(function(err, mongoclient){
		var db = mongoclient.db("Lux2");
		udpate["info"] = {sender: userId, checked_by:{python:false, node:false}};
		db.collection("Published").insert(update);
		mongoclient.close();
	});
	
}
function subscribe(query, userId){ // yes
	console.log(userId + " Subscribed " + query);
	mongoclient.open(function(err, mongoclient){
		var db = mongoclient.db("Lux2");
		db.collection("Subscribers").update({query:query}, 
				{'$addToSet':{'subscribers': {id: userId}}},
				{upsert:true});
		mongoclient.close();
	});
}
function unsubscribeAll(userId){// yes
	console.log(userId + " Unsubscribed to all");
	mongoclient.open(function(err, mongoclient){
		var db = mongoclient.db("Lux2");
		db.collection("Subscribers").update({}, 
				{'$pull':{'subscribers': {id: userId}}},
				{upsert:true, multiple:true});
		mongoclient.close();
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
				var userId = getClientId(data.access_token);
				if(userId != false && userId != null){
					socket.userId = userId;
					socket.connected = true;
					sockets[userId] = socket;
					socket.emit('joined', {status: 'connected'});
					console.log("A user has connected " + userId);
				}else{
					socket.emit('error', {error: "access Token Invalid"});
				}
			}else{
				socket.emit('error', {error: "access_token is missing"});
			}
		}else{
			socket.emit('error', {error: "you are already connected"});
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

function emitMessages(){
mongoclient.open(function(err, mongoclient){
	var db = mongoclient.db('Lux2');
	db.collection('Subscribers').find({'subscribers' : {$not: {$size:0}}}, function(err, subscriptions){
		subscriptions.each(function(err, subscription){
			if(subscription != null){
				var query = subscription.query;
				query["info.checked_by.node"] = false;
				db.collection('Published').find(query, function(err, publish){
					publish.each(function(err, doc){
						if(doc != null){
							db.collection("Published").update({"_id":doc._id}, {$set:{"info.checked_by.node":true}}, function(err, doc){});
							subscription.subscribers.forEach(function(err, subscriber){
								console.log("subscribers notified");
								if(sockets.hasOwnProperty(subscriber.id)){
									sockets[subscriber.id].emit('updated', doc);
								} 
								//mongoclient.close();
							});
						}else{
							mongoclient.close();
						}
					});
					if(publish == null){ mongoclient.close();}
				});
			}else{
				mongoclient.close();
			}
		});
		if(subscriptions == null){ mongoclient.close();}
	});
});
}


setInterval(emitMessages, 1000);

