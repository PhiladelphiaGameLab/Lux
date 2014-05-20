/*
 * ChatTest.cpp: Author, Justin MacIntosh
 * Description: A test of BSON, Mongo, Curlpp, Neo4j and Boost 
 * (all of which will be integral to the chat client).
 * To compile, use:
 * g++ ChatTest.cpp -pthread -lmongoclient -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lcurlpp -o chat
 */

#include <curlpp/cURLpp.hpp>
#include <cstdlib>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

using namespace curlpp;
using namespace std;
using namespace mongo;

//This is the url of the graph database when the Neo4j server is running
#define HOST "http://localhost:7474/"

int main(int argc, char * argv[]) {
	//Takes in a function (ex. ./chat CRE Tom or ./chat DELR Tom Tina)
	if (argc < 2) {
		cout << "ERROR: At least one argument is needed." << endl;
		return 0;
	}
	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;
	//a header file to be used with myRequest
	std::list<std::string> header;
	//format and attach the header
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));

	//fucntion CRE: Creates a node in the graph
	if (!strcmp(argv[1], "CRE")) {
		//ex: ./chat CRE Tom
		if (argc < 3) {
			cout << "ERROR: CRE takes an argument." << endl;
			return 0;
		}
		//This is the one function that doesn't use the Neo4j language Cypher
		//instead we are connected directly to the url
		string url = HOST;
		url += "db/data/index/node/user?uniqueness=get_or_create";
		myRequest.setOpt<options::Url>(url);

	    //use the BSON object to create the json needed
	    //ex. { "key" : "name", "value" : "Tom", "properties" : {"name" : "Tom"}}
		bson = BSON("key" << "name" << "value" << argv[2] << "properties" << BSON("name" << argv[2]));

		//add this json to myRequest
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    //set the size to something that will not be easily exceeded
	    myRequest.setOpt(new curlpp::options::PostFieldSize(100));

	    //set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os));
	    //perform the action, supposedly creating a node 
	    myRequest.perform();

	    cout << "Attempt: Created the node, " << argv[2] << ", if it does not yet exist." << endl;
	}

	//function DEL: Deletes a node in the graph
	else if (!strcmp(argv[1], "DEL")) {
		//ex: ./chat DEL Tom
		if (argc < 3) {
			cout << "ERROR: DEL takes an argument." << endl;
			return 0;
		}
		//connect to the url that handles cypher
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

	    //this cypher code finds a node n with the name {name} (to be defined later in params),
	    //and then deletes this node and all of it's relationships (edges)
		string cypherString = "MATCH (n { name: {name}}) OPTIONAL MATCH (n)-[r]-() DELETE n, r;";

		//set the params to the given name
		bson = BSON("query" << cypherString << "params" << BSON( "name" << argv[2]));

		//add this json to myRequest
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    //set the size to something that will not be easily exceeded
	    myRequest.setOpt(new curlpp::options::PostFieldSize(150));
 
 		//set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os)); 
	    //perform the action, supposedly deleting a node 
	    myRequest.perform();

	    cout << "Attempt: Deleted the node, " << argv[2] << ", if it existed." << endl;
	}

	//function PR_N: Prints the name of every node in the graph
	else if (!strcmp(argv[1], "PR_N")) {
		//ex: ./chat PR_N

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

	    //this cypher code returns the name of every node in the graph
		string cypherString = "MATCH (n) RETURN (n.name)";

		//no params needed, so just send the query
		bson = BSON("query" << cypherString);

		//add this json to myRequest
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    //set the size to something that will not be easily exceeded
	    myRequest.setOpt(new curlpp::options::PostFieldSize(150));
 
 		//set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os)); 
	    //perform the action, supposedly retunring all of the names of the nodes
	    myRequest.perform();

	    //create a new BSONObj from the return value
	    BSONObj ret = fromjson(os.str());
	    //the data will be given in the following way
	    //ex. ..."data" : [ [ Tom ] [ Tina ] ]
	    vector<BSONElement> elements = ret["data"].Array();
	    //create a vecotr of vectors to store the names
	    vector<vector <BSONElement> > data(elements.size());
	    for (int i = 0; i < elements.size(); i++) {
	    	//store the names
	    	data[i] = elements[i].Array();
	    }
	    for (int j = 0; j < data.size(); j++) {
	    	//print the names
	    	cout << data[j][0].str() << endl;
		}
	}

	//function PR_R: Prints the nodes engaged in relationships in the graph
	else if (!strcmp(argv[1], "PR_R")) {
		//ex: ./chat PR_N

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

	    //this cypher code finds every relationship and then returns the name of the two people in this relationship
		string cypherString = "MATCH (n)-[r]->(m) RETURN n.name, m.name";
		bson = BSON("query" << cypherString);

		//set the json of myRequest
		myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));

	    //set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os)); 
	    //perform the action, supposedly returning all of the relationships 
	    myRequest.perform();

	     //create a new BSONObj from the return value
	    BSONObj ret = fromjson(os.str());
	    //the data will be given in the following way
	    //ex. ..."data" : [ [ Tom ] [ Tina ] ]...
	    vector<BSONElement> elements = ret["data"].Array();
	    //create a vecotr of vectors to store the names
	    vector<vector <BSONElement> > data(elements.size());

	    for (int i = 0; i < elements.size(); i++) {
	    	//store the names
	    	data[i] = elements[i].Array();
	    }
	    for (int j = 0; j < data.size(); j++) {
	    	//print the relationships as names
	    	cout << data[j][0].str() << " likes " << data[j][1].str() << endl;
		}
	}

	//function REL: Creates a relationship between two nodes
	else if (!strcmp(argv[1], "REL")) {
		//ex: ./chat REL Tom Tina
		if (argc < 4) {
			cout << "ERROR: REL takes 2 arguments." << endl;
			return 0;
		}

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

		//these lines of cypher code create a relationship from name1 to name 2 and from name2 to name1
		//these tow names will be defined in the params
		string cypherString = "MATCH (n { name: {name1}}), (m { name :{name2}}) CREATE (n)-[r:Likes]->(m) RETURN r";
		string cypherString2 = "MATCH (n { name: {name2}}), (m { name :{name1}}) CREATE (n)-[r:Likes]->(m) RETURN r";

		//params
		bson = BSON("query" << cypherString << "params" << BSON("name1" << argv[2] << "name2" << argv[3]));

		//set the WriteStream to os
		myRequest.setOpt(options::WriteStream(&os)); 
		myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
	    //perform the action, supposedly creating a relationship 
	    myRequest.perform();

	    //switch the params ot create the opposite edge
	    bson = BSON("query" << cypherString2 << "params" << BSON("name1" << argv[2] << "name2" << argv[3]));
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
	    //perform the action, supposedly creating a relationship 
	    myRequest.perform();
	    cout << "Attempt: Relationships created between " << argv[2] << " and " << argv[3] << "." << endl;
	}

	//function DELR: Deletes a relationship between two nodes
	else if (!strcmp(argv[1], "DELR")) {
		//ex: ./chat DELR Tom Tina
		if (argc < 4) {
			cout << "ERROR: DELR takes 2 arguments." << endl;
			return 0;
		}

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

		//these lines of cyper code delete the two relationships between name1 and name2 (defined in params)
		string cypherString = "MATCH n-[rel:Likes]->m WHERE n.name={name1} AND m.name={name2} DELETE rel";
		string cypherString2 = "MATCH n-[rel:Likes]->m WHERE n.name={name2} AND m.name={name1} DELETE rel";

		//params
		bson = BSON("query" << cypherString << "params" << BSON("name1" << argv[2] << "name2" << argv[3]));

		//set the WriteStream to os
		myRequest.setOpt(options::WriteStream(&os)); 
		myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));

	    //perform the action, supposedly deleting a relationship 
	    myRequest.perform();

	    //same params, different cypherString
	    bson = BSON("query" << cypherString2 << "params" << BSON("name1" << argv[2] << "name2" << argv[3]));
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
	    //perform the action, supposedly deleting a relationship 
	    myRequest.perform();

	    //print the deleted relationships
		cout << "Attempt: Relationships deleted between " << argv[2] << " and " << argv[3] << "." << endl;
	}

	//function CHANGE: Changes the name of a node
	else if (!strcmp(argv[1], "CHANGE")) {
		//ex: ./chat CHANGE Tom Tim
		if (argc < 4) {
			cout << "ERROR: DELR takes 2 arguments." << endl;
			return 0;
		}

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

		//this cypher code changes the name of a node named name1 to name2 (defiend in params)
		string cypherString = "MATCH (n { name: {name1} }) SET n.name = {name2} RETURN n";

		//params
		bson = BSON("query" << cypherString << "params" << BSON("name1" << argv[2] << "name2" << argv[3]));

		//set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os)); 
	    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
	    //perform the action, supposedly changing the name of a node
	    myRequest.perform();
		cout << "Attempt: Name changed from " << argv[2] << " to " << argv[3] << "." << endl;
	}

	//function CLEANSE: Compeltely deletes the graph and every relationship in the node
	else if (!strcmp(argv[1], "CLEANSE")) {
		//ex: ./chat CLEANSE

		//connect to the url that handles cypher commands
		string url = HOST;
		url += "db/data/cypher";
		myRequest.setOpt<options::Url>(url);

		//this line of cypher code finds every relationship and node and deletes them completely
		string cypherString = "MATCH (n) OPTIONAL MATCH (n)-[r]-() DELETE n,r";

		//no params needed
		bson = BSON("query" << cypherString);

		//set the WriteStream to os
	    myRequest.setOpt(options::WriteStream(&os)); 
		myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
	    myRequest.setOpt(new curlpp::options::PostFieldSize(150));

	    //perform the action, supposedly destroying the entire graph
	    myRequest.perform();
	    cout << "Graph was deleted completely." << endl;
	}
	else
		cout << "Function not recognized." << endl;
	return 0;
}