////  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include "httplib.h"
#include "ChatDB.h"
#include "ChatEntry.h"
//#include "TokenGenerator.h"

using namespace httplib;
using namespace std;

const int port = 5005;



std::string generateToken()
{
    std::string token;
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    for (int i = 0; i < 16; ++i) {
        token += chars[dis(gen)];
    }
    return token;
}


void addMessage(string username, string message, map<string,vector<string>> &messageMap) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result += "]";
	messageMap[username].clear();
	return result;
}

string getusersJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = ",\"users\":[";
	for (auto const& message :  messageMap) {
		if (not first) result += ",";
		result+="\"";
		result += message.first;
		result +="\"";
		first = false;;
	}
	result += "]}";
	messageMap[username].clear();
	return result;
}


bool contains(vector<string> vec, const string & elem)
{
    bool result = false;
    if( find(vec.begin(), vec.end(), elem) != vec.end() )
    {
        result = true;
    }
    return result;
}//new contain function to check for previous emails and usernames.prof Skon pls check


bool sixstringtest(string str) {
    bool result = false;
    if( str.length()<6)
    {
        result = true;
    }
    return result;

}
int main(void) {
  Server svr;
 userDB udb;

  int nextUser=0;
  map<string,vector<string>> messageMap;
  map<string,string> TokenMap;
  vector<string> emailvec;
  vector<string> Passwordvec;
  vector<string> Usernamevec;//new vectors???????????????????Pls Check
  bool registered = false;
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string pass = req.matches[2];//new
    string result;
    vector<string> empty;
    cout << username << " joins" << endl;
   if(udb.confirm(username,pass) ){//new
	
	string Token = generateToken();
//	cout << "heelo" << Token;
//	cout << "Test please work" << endl;
	TokenMap[Token]=username;
//gnerate token:
 
//	cout<<"insideif"<<endl;
  // if(contains(Usernamevec,username) && contains(Passwordvec,pass)){//new 
    // Check if user with this name exists
   // if (messageMap.count(username)) {
    //	result = "{\"status\":\"exists\"}";
   // } else {
    	// Add user to messages map
    	messageMap[username]=empty;
    	result = "{\"status\":\"success\",\"token\":\"" + Token +"\",\"user\":\""+username+"\"}";
    //}
    res.set_content(result, "text/json");
	}else{//new
cout<<"not reg"<<endl;
	result = "{\"status\":\"not registered\"}";
        res.set_content(result, "text/json");

    }

  });

   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string token = req.matches[1];
	string message = req.matches[2];
	string result;
	cout<<token<<endl;
	cout<<TokenMap.count(token)<<endl;
	cout<<TokenMap.at(token)<<endl; 
	if(TokenMap.count(token)){

string username = TokenMap.at(token);
    if (!messageMap.count(username)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		addMessage(username,message,messageMap);
		result = "{\"status\":\"success\"}";
	}
}else{
	result = "{\"status\":\"invalid token\"}";
}
    res.set_content(result, "text/json");

  });
  
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string Token = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
	string resultJSON= "";
	if(TokenMap.count(Token)){
	string username = TokenMap.at(Token);
	
    string result1 = getMessagesJSON(username,messageMap);
    string result2 = getusersJSON(username,messageMap);
    resultJSON = result1+result2;
}else{
    resultJSON = "{\"status\":\"invalid token\"}";
}

    res.set_content(resultJSON, "text/json");
  });


   svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string email = req.matches[2];
    string Password = req.matches[3];
	udb.reg(username,email,Password);
    string result;
    bool usernamesuccess = false;//not sure if these will be needed
    bool emailsuccess = false;
    bool passwordsuccess = false;
        // Check if user with this name exists
    if (!contains(Usernamevec,username)) {
    	// Add user to uservect
    	Usernamevec.push_back(username);
	usernamesuccess = true;
    }

    if (!contains(emailvec,email)) {
    
    	// Add email to email vect
    	emailvec.push_back(email);
	emailsuccess = true;
    }

    if (!sixstringtest(Password)) {
    	// Add email to password to  vect
    	Passwordvec.push_back(Password);
	passwordsuccess = true;
    }
	if(passwordsuccess && emailsuccess && usernamesuccess){
	result = "{\"status\":\"success\"}";
	}else{
	result = "{\"status\":\"error\"}";
	}    
    res.set_content(result, "text/json");
  });
  
//Require users to register and 
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
