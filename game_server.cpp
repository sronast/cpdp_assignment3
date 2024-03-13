#include "game_server.hpp"

// GameServer

bool GameServer::start(){
    // 1. Create a server socket here
    // 2. Use select to handle multiple use cases like, accepting new connection, communication between clients
    // 3.
}

vector<string> GameServer::displayAllUsers() {} // who

vector<string> GameServer::displayAllGames() {}

User GameServer::getUser() {} // stats [name]

User GameServer::getUser(string username) {} // stats [name]

User GameServer::registerUser(string username, string password, bool isGuest)
{
    User usr = User();
    if (isGuest == false)
    {
        cout << "Only guest can register as new user" << endl;
        return usr;
    }
    return usr.registerUser(username, password);
}

bool GameServer::loginUser(string username, string password) {}