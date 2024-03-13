#include "game_server.hpp"

// GameServer

void GameServer::start()
{
    // 1. Create a server socket here
    // 2. Use select to handle multiple use cases like, accepting new connection, communication between clients
    // 3. Load all the saved states
    // 4.
    // server_socket = setup_server();
    setupServer();
    cout << "setup complete: now handling connection" << endl;
    handleConnections();
}

void GameServer::setupServer()
{
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        handleConnectionError("Error while creating the server socket!!!");
    }

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // htonl(INADDR_ANY);
    server_address.sin_port = htons(58848);      // Server port

    // Bind socket to the address

    if ((bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))) < 0)
    {
        handleConnectionError("Bind Error!!");
    }

    // Listen for connections
    if ((listen(server_socket, 10) < 0))
    {
        handleConnectionError("Listen Error!!");
    }

    // After success
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);
    active_connections.insert(server_socket);
    maxfd = server_socket;
    cout << "Server socket: " << server_socket << endl;
    cout << "Server started at IP: " << inet_ntoa(server_address.sin_addr) << " Port: " << htons(server_address.sin_port) << endl;
}

void GameServer::handleConnections()
{
    while (true)
    {
        cout << "In while" << endl;
        // select is destructive, so make a copy
        ready_sockets = current_sockets;
        int socket_count;
        if ((socket_count = select(maxfd+1, &ready_sockets, NULL, NULL, NULL))<0)
        {
            handleConnectionError("Select error!!");
        }
        // select(maxfd+1, &ready_sockets, NULL, NULL, NULL);
        cout << "now handling connection,, counts: " << socket_count<<endl;
        // Go through all fds,
        for (auto i = active_connections.begin(); i != active_connections.end(); i++)
        {
            // Check which socket is ready
            if (FD_ISSET(*i, &ready_sockets))
            {
                // If it is server socket, connection request received
                if (*i == server_socket)
                {
                    // handle new connection
                    bool res = acceptNewConnection();
                    if(!res){

                    }
                    // Add new client socket to the set of sockets we are watching
                    //  FD_SET(client_socket, &current_sockets);
                }
                // read data from the client
                else
                {
                    handleClient(*i);
                    // FD_CLR(i, &current_sockets);
                }
            }
        }
    }
}

bool GameServer::acceptNewConnection()
{
    cout<<"new client request"<<endl;
    int client_socket;
    struct sockaddr client_address;
    socklen_t addr_len =  sizeof(client_address);
    //Accept the connection
    if(client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len)){
        perror("Accept New Connection Error!!");
        return false;
    }
    cout<<"Max fd: "<<maxfd<<endl;
    FD_SET(client_socket, &current_sockets);
    active_connections.insert(client_socket);
    maxfd = max(maxfd, client_socket);
    cout<<"client_socket: "<<client_socket<<endl;
    cout<<"Max fd: "<<maxfd<<endl;
    
    return true;
    // 1. Check if maximum number of connections is already active
    // 2. Handle connection request
    // 3. Add new client to the active_connection set
}

void GameServer::handleClient(int client)
{
    

}

void GameServer::handleConnectionError(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
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