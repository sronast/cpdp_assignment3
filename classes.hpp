#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "utilities.hpp"

using namespace std;

class Message {
public:
    std::string from;
    std::string msg;
    std::string status;  // "init", "sent", "delivered", "read"
    std::string time;
    // Constructor
    Message(const std::string& from, const std::string& msg, const std::string& status, const std::string& time);
    ~Message();
    // Getters and setters for member variables
    std::string getFrom() const;
    void setFrom(const std::string& from);
    std::string getMsg() const;
    void setMsg(const std::string& msg);
    std::string getStatus() const;
    void setStatus(const std::string& status);
    std::string getTime() const;
    void setTime(const std::string& time);
   
    
};

class Mail {
public:
    std::string from;
    std::string msg;
    std::string status;  // "init", "sent", "delivered", "read"
    std::string time;
    string header;
    int has_read;
    int id;
    string to;
    // Constructor
    Mail(int id, string from, string msg, string status, string time, string header):has_read(0){}
    Mail():has_read(0){}
    ~Mail();
    // Getters and setters for member variables
    string getFrom() const;
    void setFrom(const string& from);
    string getMsg() const;
    void setMsg(const string& msg);
    string getStatus() const;
    void setStatus(const string& status);
    string getTime() const;
    void setTime(const string& time);
    string getHeaders() const;
    void setHeader(const std::string& header);
    void setId(const int& id);
    int getId() const;
};



class User {
public:
    std::string username;
    std::string password;
    int wins;
    int loss;
    int draw;
    int isPlaying;
    // std::vector<Message> messages;
    // std::vector<Mail> mail;
    int quietMode;
    int rank;
    int points;
    int totalGames;
    int isSendingMessage;
    string opponent;
    int currentGameId;
    string moveName = "";
    int gameObserving;
    string info;
    // Mail draft;
    string draftMessage;
    string draftHeader;
    string mailTo;
    set<string> blockListSet;

    // Constructor
    User();
    User(const User& other);
    ~User();


    // Getters and setters
    void setUsername(const std::string& username);
    std::string getUsername() const;

    void setPassword(const std::string& password);
    std::string getPassword() const;

    void setWins(int wins);
    int getWins() const;

    void setLoss(int loss);
    int getLoss() const;

    void setDraw(int draw);
    int getDraw() const;

    void setIsPlaying(int isPlaying);
    int getIsPlaying() const;
    
    void setIsSendingMsg();
    int getIsSendingMsg() const;

    // void setMessages(const std::vector<Message>& messages);
    // std::vector<Message> getMessages() const;

    // void setMail(const std::vector<Mail>& mail);
    // std::vector<Mail> getMail() const;

    void setQuietMode(int quietMode);
    int getQuietMode() const;


    void setRank(int rank);
    int getRank() const;

    void setPoints(int points);
    int getPoints() const;

    void setTotalGames(int totalGames);
    int getTotalGames() const;

    std::vector<Message> parseMessage(const std::string& messageStr);

    std::vector<Mail> parseMail(const std::string& mailStr);
    std::vector<std::string> parseBlockList(const std::string& blockListStr);

    // std::string getFormattedMessages() const;
    // std::string getFormattedMail() const;
    // std::string getFormattedBlockList() const;

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

#endif