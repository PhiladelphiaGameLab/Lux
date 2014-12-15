//open a Mongo Connection
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
mongoclient.open(function(err, mc){
	var db = mc.db("Lux2");
	db.collection("Users").findOne({access_token:"abc"}, function(err, userDoc){
		console.log(userDoc);
		mongoclient.close();
	});
});
