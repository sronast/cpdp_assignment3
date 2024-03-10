#include <string>
#include <vector>

using namespace std;

class Message{
    string from;
    string to;
    string status; //sent, delivered, read
};

class User{
 public:
    string id;
    string username;
    int win;
    int loss;
    bool isGuest;
    bool quietMode;
    vector<string> blockList;

    private:
        string password;
        vector<string> messages;

};



class Ratings{
    int matchesPlayed;
    int points;
    int win;
    int loss;
    int draw;
    int rank;

};

class Game{
    string id;
    vector<string> players;
    vector<string> usersWatching;
    string winner;
    bool isRunning;
    bool isDraw;
    vector<string> gameComments; //kibitz<msg>
    vector<string> comments; //’<msg>
    int player1Time;
    int player2Time;

};

class GameServer{

    public:
        vector<string> displayAllUsers(){} //who

        vector<string> displayALlGames(){}

        User getUser(){} //stats [name]

        User getUser(string username){} //stats [name]

        User registerUser(string username, string password, bool isGuest){}

        bool loginUser(string username, string password){}



};

