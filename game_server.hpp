#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

#include "utilities.hpp"
#include "classes.hpp"

using namespace std;

class GameServer {
// Load all the saved data from the file when the server starts


private:
    // server should save:
    
    // 1. Socket maintaining connection for each active user
    // 2. List of users
    // 3. List of active games
    // 4. Periodically backup states of users, game 
    // 5. 


public:

    bool start();

    vector<string> displayAllUsers();  // who

    vector<string> displayAllGames();

    User getUser();  // stats [name]

    User getUser(string username);  // stats [name]

    User registerUser(string username, string password, bool isGuest);

    bool loginUser(string username, string password);
};

#endif