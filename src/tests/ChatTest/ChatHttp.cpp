#include "ChatHttp.h"

using namespace curlpp;
using namespace std;
using namespace mongo;

#define HOST "http://localhost:7474/"

void AddUser(std::string uName, std::string givenIP, std::string givenPort) {
    std::string name = uName;
    std::string iP = givenIP;
    std::string port = givenPort;

	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;

	//a header file to be used with myRequest
	std::list<std::string> header;
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));

	//connect to the url that handles cypher
	string url = HOST;
	url += "db/data/cypher";
	myRequest.setOpt<options::Url>(url);
	string cypherString = "CREATE (n { name : {name}, type : {type} })";
	//set the params to the given name
	bson = BSON("query" << cypherString << "params" << BSON("name" << name << "type" << "user"));

	//add this json to myRequest
    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    //set the size to something that will not be easily exceeded
    myRequest.setOpt(new curlpp::options::PostFieldSize(150));
	//set the WriteStream to os
    myRequest.setOpt(options::WriteStream(&os));
    //perform the action, supposedly creating a user node 
    myRequest.perform();

    cout << "Attempt: Created the node, " << name << ", if it didn't already exist." << endl;
	return;
}

void AddGroup(std::string grName) {
    std::string name = grName;
	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;

	//a header file to be used with myRequest
	std::list<std::string> header;
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));

	//connect to the url that handles cypher
	string url = HOST;
	url += "db/data/cypher";
	myRequest.setOpt<options::Url>(url);
	string cypherString = "	CREATE (n { name : {name}, type : {type} })";
	//set the params to the given name
	bson = BSON("query" << cypherString << "params" << BSON("name" << name << "type" << "group"));

	//add this json to myRequest
    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    //set the size to something that will not be easily exceeded
    myRequest.setOpt(new curlpp::options::PostFieldSize(150));
	//set the WriteStream to os
    myRequest.setOpt(options::WriteStream(&os));
    //perform the action, supposedly creating a group node 
    myRequest.perform();

    cout << "Attempt: Created the node, " << name << ", if it didn't already exist." << endl;
	return;
}

void DeleteUser(std::string uName) {
    std::string name = uName;
	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;

	//a header file to be used with myRequest
	std::list<std::string> header;
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));
	//connect to the url that handles cypher
	string url = HOST;
	url += "db/data/cypher";
	myRequest.setOpt<options::Url>(url);
    //this cypher code finds a node n with the name {name} (to be defined later in params),
    //and then deletes this node and all of it's relationships (edges)
	string cypherString = "MATCH (n { name: {name}}) OPTIONAL MATCH (n)-[r]-() DELETE n, r;";

	//set the params to the given name
	bson = BSON("query" << cypherString << "params" << BSON("name" << name));
	//add this json to myRequest
    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    //set the size to something that will not be easily exceeded
    myRequest.setOpt(new curlpp::options::PostFieldSize(150));
	//set the WriteStream to os
    myRequest.setOpt(options::WriteStream(&os));
    //perform the action, supposedly deleting a node 
    myRequest.perform();
    cout << "Attempt: Deleted the node, " << name << ", if it existed." << endl;

	return;
}

void AddEdge(std::string uName1, std::string uName2) {
    std::string name1 = uName1;
    std::string name2 = uName2;
	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;

	//a header file to be used with myRequest
	std::list<std::string> header;
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));
	//connect to the url that handles cypher commands
	string url = HOST;
	url += "db/data/cypher";
	myRequest.setOpt<options::Url>(url);
	//these lines of cypher code create a relationship from name1 to name 2 and from name2 to name1
	//these tow names will be defined in the params
	string cypherString = "MATCH (n { name: {name1}}), (m { name :{name2}}) CREATE (n)-[r:Likes]->(m) RETURN r";
	string cypherString2 = "MATCH (n { name: {name2}}), (m { name :{name1}}) CREATE (n)-[r:Likes]->(m) RETURN r";

	//params
	bson = BSON("query" << cypherString << "params" << BSON("name1" << name1 << "name2" << name2));
	//set the WriteStream to os
	myRequest.setOpt(options::WriteStream(&os)); 
	myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
    //perform the action, supposedly creating a relationship 
    myRequest.perform();

    //switch the params ot create the opposite edge
    bson = BSON("query" << cypherString2 << "params" << BSON("name1" << name1 << "name2" << name2));
    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
    //perform the action, supposedly creating a relationship 
    myRequest.perform();

    cout << "Attempt: Relationships created between " << name1 << " and " << name2 << "." << endl;
	return;
}

void DeleteEdge(std::string uName1, std::string uName2) {
    std::string name1 = uName1;
    std::string name2 = uName2;
	//curlpp Easy class, used to comminicate with servers
	Easy myRequest;
	//BSON Object for use with myRequest
	BSONObj bson;
	//an output string stream used as an output for the Easy myRequest
	//(rather than having it print to cout)
	std::ostringstream os;

	//a header file to be used with myRequest
	std::list<std::string> header;
	header.push_back("Content-Type: application/json");
	myRequest.setOpt(new curlpp::options::HttpHeader(header));

	//connect to the url that handles cypher commands
	string url = HOST;
	url += "db/data/cypher";
	myRequest.setOpt<options::Url>(url);
	//these lines of cyper code delete the two relationships between name1 and name2 (defined in params)
	string cypherString = "MATCH n-[rel:Likes]->m WHERE n.name={name1} AND m.name={name2} DELETE rel";
	string cypherString2 = "MATCH n-[rel:Likes]->m WHERE n.name={name2} AND m.name={name1} DELETE rel";

	//params
	bson = BSON("query" << cypherString << "params" << BSON("name1" << name1 << "name2" << name2));
	//set the WriteStream to os
	myRequest.setOpt(options::WriteStream(&os)); 
	myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
    //perform the action, supposedly deleting a relationship 
    myRequest.perform();

    //same params, different cypherString
    bson = BSON("query" << cypherString2 << "params" << BSON("name1" << name1 << "name2" << name2));
    myRequest.setOpt(new curlpp::options::PostFields(bson.jsonString()));
    myRequest.setOpt(new curlpp::options::PostFieldSize(190));
    //perform the action, supposedly deleting a relationship 
    myRequest.perform();

    //print the deleted relationships
	cout << "Attempt: Relationships deleted between " << name1 << " and " << name2 << "." << endl;
	return;
}

int main() {
	CGI environment;

    std::string EUID = environment.get("EUID");
    std::string accessToken = environment.get("accessToken");
	std::string name1;
    std::string name2;

     if(!(EUID != "") && !(accessToken != "")){
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
        	std::string method = environment.get("method");
    		if (method =="AddUser") {
    			name1 = environment.get("name1");
    			std::string iP = environment.get("IP");
    			std::string port = environment.get("port");
    			AddUser(name1, iP, port);
    		}
    		else if (method == "AddGroup") {
    		    name1 = environment.get("name1");
    			AddGroup(name1);
    		}
    		else if (method == "DeleteUser") {
    			name1 = environment.get("name1");
    			DeleteUser(name1);
    		}
    		else if (method == "AddEdge") {
    			name1 = environment.get("name1");
    			name2 = environment.get("name2");
    			AddEdge(name1, name2);
    		}
    		else if (method == "DeleteEdge") {
    			name1 = environment.get("name1");
    			name2 = environment.get("name2");
    			DeleteEdge(name1, name2);
    		}
         }
        else {
            environment.error("Invalid Access Token. Please try Rehashing", 2);
        }
    }
    else {
        environment.error("Missing Access Token or End-User ID", 3);
    }
	return 0;
}