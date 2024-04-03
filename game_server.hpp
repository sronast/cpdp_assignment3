#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <string>
#include <vector>
#include <map>
#include<set>
#include <iostream>
#include <numeric>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <format>
#include <chrono>
#include <random>
#include <ctime>

#include <errno.h>

#include "json.hpp"
using json = nlohmann::json;

#include "utilities.hpp"
#include "classes.hpp"
#include "game.hpp"

using namespace std;

class GameServer {
// Load all the saved data from the file when the server starts
private:
// Server socket
    int server_port;
    int server_socket;

    struct sockaddr_in server_address;
    int maxfd;
    //current_socket: list of sockets that we are watching
    //ready sockets: for making a copy of current sockets for select()
    fd_set all_sockets;
    fd_set ready_sockets;

    int max_allowed_connections = 4;
    
    // Game
    map<int, TicTacToe> all_games;
    // game

    map<string, User> allUsersInfo;
    
    map<string, vector<Message>> allUserMessages;
    map<string, vector<Mail>> allUserMails;

    set<int> active_connections;
    set<string> all_users;
    set<int> active_guests;
    map<int, string> socket_user_map; //{4: User()}
    map<string, int> user_socket_map; //{'leo': 4} do not save guest here
    map<int, string> not_logged_in; //{4: 'user', 5: 'guest'}
    map<string, string> match_requests;
    map<string, string> game_settings;
    map<string , int > user_rank;


    //help
    string init_message;
    string manual;

    //game
    set<string> supported_commands;
    // server should save:
    
    // 1. Socket maintaining connection for each active user
    // 2. List of users
    // 3. List of active games
    // 4. Periodically backup states of users, game 
    // 5. 


public:
    GameServer(int port);
 
    void start();

    void setupServer();

    void handleConnections();
    
    void handleLogin(int &client, bool &is_empty_msg, vector<string> &tokens, string &command, string &received_data);

    void handleGuest(int &client, bool &is_empty_msg, vector<string> &tokens, string &command, string &received_data);

    void handleRegisteredUser(int &client, bool &is_empty_msg, vector<string> &tokens, string &command, string &received_data);

    void gameWon(std::string &user_name, TicTacToe &game, int &client, int &opponentClient, User &usr1, User &usr2);

    bool acceptNewConnection();

    bool handleClient(int client);

    void handleConnectionError(const char *msg);

    // Handling different messsages

    void handleEmptyMsg(int &client);

    void handleClientExit(int &client, string &msg);

    void sendMsg(int &client, string &msg);
    void sendEmptyMsg(int &client);
    //Handling different messsages

    // USers
    bool isUserOnline(string &user);

    bool isUserRegistered(string &user);

    void saveAllData();

    void updateRank();


    // vector<string> displayAllUsers(); 

    string getOnlineUsers();

    // vector<string> displayAllGames();

    // User getUser();  // stats [name]

    // User getUser(string username);  // stats [name]

    int registerUser(string username, string password, bool isGuest);

    // bool loginUser(string username, string password);
    int regHelp(string username, string filename);
    
};

// void handleAlarm(int sig);

#endif