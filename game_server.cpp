#include "game_server.hpp"

// GameServer

bool GameServer::start(){
    // 1. Create a server socket here
    // 2. Use select to handle multiple use cases like, accepting new connection, communication between clients
    // 3. Load all the saved states
    // 4. 
    server_socket = setup_server();
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);
    handleConnections();
}

int GameServer::setup_server(){

}

void GameServer::handleConnections(){
    while(true){
        // select is destructive, so make a copy
        ready_sockets = current_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL)){
            perror("select error");
            exit(-1);
        }
        // Go through all fds, 
        for(int i=0; i<active_connections.size()+1; i++){
            // Check which socket is ready
            if(FD_ISSET(i, &ready_sockets)){
                // If it is server socket
                if(i == server_socket){
                    //new connection
                    int client_socket = acceptNewConnection();
                    //Add new client socket to the set of sockets we are watching
                    FD_SET(client_socket, &current_sockets);
                }
                //read data from the client
                else{
                    handleClient(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
    }
}

int GameServer::acceptNewConnection(){
    // 1. Check if maximum number of connections is already active
    // 2. Handle connection request
    // 3. Add new client to the active_connection set
}

void GameServer::handleClient(int client){

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