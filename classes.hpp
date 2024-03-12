#ifndef CLASSES_HPP
#define CLASSES_HPP

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
    Message createNewMessage(string from, string to, string msg);

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

    string toJson(User usr);
    User registerUser(string username, string password);
    
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
    void updateRanking(string userId);
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
    vector<string> displayAllUsers();  // who

    vector<string> displayAllGames();

    User getUser();  // stats [name]

    User getUser(string username);  // stats [name]

    User registerUser(string username, string password, bool isGuest);

    bool loginUser(string username, string password);
};


#endif