#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

int getRandomInt(){
    return rand();
}


class Message {
private:
    string id;
    string from;
    string to;
    string msg;
    string status;  // "init", "sent", "delivered", "read"

public:
    // Constructors, getters, and setters could be added here
    Message createNewMessage(string from, string to, string msg){
        Message message = Message();
        message.id = "m_"+to_string(getRandomInt());
        message.from = from;
        message.to = to;
        message.msg=msg;
        message.status = "init";
        return message;
    }

    // string getMessageStatus(string msgId){
    //     return 
    // }
};

class User {
private:
    string password;
    vector<Message> messages;
    vector<string> blockList;
    bool isGuest;




public:
    string username;
    int win;
    int loss;
    bool quietMode;
    string id;

    string toJson(User usr){
        json jsonObj = {
            {"username", usr.username},
            {"id", usr.id},
            {"win", usr.win},
            {"loss", usr.loss},
            {"isGuest", usr.isGuest},
            {"quietMode", usr.quietMode},
            {"blocklist",accumulate(usr.blockList.begin(), usr.blockList.end(), string())},
            {"password", usr.password}  
        };
        string jsonStr = jsonObj.dump(2);
        return jsonStr;
    }
    User registerUser(string username, string password){
        User usr = User();
        usr.id = "u_"+to_string(getRandomInt());
        usr.username = username;
        usr.password = password;
        usr.win = 0;
        usr.loss = 0;
        usr.isGuest = false;
        usr.blockList = {};
        usr.messages = {};

        // save user to file 
        string usrJson = toJson(usr);
        ofstream userFile("users.txt", std::ios::app);
        if (userFile.is_open()){
            userFile << usrJson;
            userFile << "\n";
        }
        userFile.close();
        return usr;
    }
    
};

class Ratings {
private:
    int matchesPlayed;
    int points;
    int win;
    int loss;
    int draw;
    int rank;
    string userId;

public:
    void updateRanking(string userId){
        // read from the file and update the ranking
    }
};

class Game {
private:
    vector<string> players;
    vector<string> usersWatching;
    bool isRunning;
    bool isDraw;
    vector<string> gameComments;  // "kibitz<msg>"
    vector<string> comments;      // "<msg>"
    int player1Time;
    int player2Time;

public:
    string id;
    string winner;

    // Constructors, getters, and setters could be added here
};

class GameServer {
public:
    vector<string> displayAllUsers() {}  // who

    vector<string> displayAllGames() {}

    User getUser() {}  // stats [name]

    User getUser(string username) {}  // stats [name]

    User registerUser(string username, string password, bool isGuest) {
        User usr = User();
        if (isGuest == false){
            cout << "Only guest can register as new user" << endl;
            return usr;
        }
        return usr.registerUser(username, password);

    }

    bool loginUser(string username, string password) {}
};


