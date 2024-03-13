#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <numeric>
#include <fstream>
#include <sys/socket.h>

#include "json.hpp"
using json = nlohmann::json;

#include "utilities.hpp"
#include "classes.hpp"

using namespace std;

class GameServer {
// Load all the saved data from the file when the server starts


private:
    int server_socket;
    //current_socket: list of sockets that we are watching
    //ready sockets: for making a copy of current sockets for select()
    fd_set current_sockets, ready_sockets;

    int max_allowed_connections = 4;
    
    set<int> active_connections;

    // server should save:
    
    // 1. Socket maintaining connection for each active user
    // 2. List of users
    // 3. List of active games
    // 4. Periodically backup states of users, game 
    // 5. 


public:

    bool start();

    int setup_server();

    void handleConnections();

    int acceptNewConnection();

    void handleClient(int client);

    vector<string> displayAllUsers();  // who

    vector<string> displayAllGames();

    User getUser();  // stats [name]

    User getUser(string username);  // stats [name]

    User registerUser(string username, string password, bool isGuest);

    bool loginUser(string username, string password);
};

#endif