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
private:
    std::string from;
    std::string msg;
    std::string status;  // "init", "sent", "delivered", "read"
    std::string time;

public:
    // Constructor
    Message(const std::string& from, const std::string& msg, const std::string& status, const std::string& time);

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



class User {
private:
    std::string username;
    std::string password;
    int wins;
    int loss;
    int draw;
    bool isPlaying;
    std::vector<Message> messages;
    std::vector<std::string> mail;
    bool quietMode;
    std::vector<std::string> blockList;
    int rank;
    int points;
    int totalGames;

public:
    // Constructor
    User();

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

    void setIsPlaying(bool isPlaying);
    bool getIsPlaying() const;

    void setMessages(const std::vector<Message>& messages);
    std::vector<Message> getMessages() const;

    void setMail(const std::vector<std::string>& mail);
    std::vector<std::string> getMail() const;

    void setQuietMode(bool quietMode);
    bool getQuietMode() const;

    void setBlockList(const std::vector<std::string>& blockList);
    std::vector<std::string> getBlockList() const;

    void setRank(int rank);
    int getRank() const;

    void setPoints(int points);
    int getPoints() const;

    void setTotalGames(int totalGames);
    int getTotalGames() const;

    std::vector<Message> parseMessage(const std::string& messageStr);

    std::vector<std::string> parseMail(const std::string& mailStr);
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