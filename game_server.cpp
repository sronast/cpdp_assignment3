#include "game_server.hpp"

// GameServer

GameServer::GameServer(int port){
    server_port = port;
    init_message = R"(      
        
                    -=-= AUTHORIZED USERS ONLY =-=-
        You are attempting to log into online tic-tac-toe Server.
        Please be advised by continuing that you agree to the terms of the
        Computer Access and Usage Policy of online tic-tac-toe Server.


username (guest):)";
}

void GameServer::start()
{
    // 1. Create a server socket here
    // 2. Use select to handle multiple use cases like, accepting new connection, communication between clients
    // 3. Load all the saved states
    // 4.
    // server_socket = setup_server();
    setupServer();
    cout << "server setup complete..." << endl;
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
    server_address.sin_port = htons(server_port);      // Server port

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
    FD_ZERO(&all_sockets);
    FD_SET(server_socket, &all_sockets);
    active_connections.insert(server_socket);
    maxfd = server_socket;
    cout << "Server socket: " << server_socket << endl;
    cout << "Server started at IP: " << inet_ntoa(server_address.sin_addr) << " Port: " << htons(server_address.sin_port) << endl;
}

void GameServer::handleConnections()
{
    while (true)
    {
        // select is destructive, so make a copy
        ready_sockets = all_sockets;
        int socket_count;
        if ((socket_count = select(maxfd+1, &ready_sockets, NULL, NULL, NULL))<0)
        {
            handleConnectionError("Select error!!");
        }
        // select(maxfd+1, &ready_sockets, NULL, NULL, NULL);
        cout << "now handling connection, counts: " << active_connections.size()<<endl;
        // Go through all fds,

        //Make a temporary vector for iterating over the current fds
        const vector<int> temp_fds = {active_connections.begin(), active_connections.end()};
        cout<<"set size: "<<active_connections.size()<<endl;
        for (int it: temp_fds)
        {
            // Check which socket is ready
            if (FD_ISSET(it, &ready_sockets))
            {
                cout<<"set socket: "<<it<<endl;
                // If it is server socket, connection request received
                if (it == server_socket)
                {
                    // handle new connection
                    bool res = acceptNewConnection();
                    if(!res){
                        cout<<"Cannot create client..!!!\n";
                    }
                    // Add new client socket to the set of sockets we are watching
                    //  FD_SET(client_socket, &all_sockets);
                }
                // read data from the client
                else
                {
                    handleClient(it);
                    // FD_CLR(i, &all_sockets);
                }
            }
        }
        cout<<"First iteration completed..."<<endl;
    }
}

bool GameServer::acceptNewConnection()
{
    cout<<"new client request"<<endl;
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t addr_len =  sizeof(client_address);
    //Accept the connection
    if((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len))<0){
        if (errno == EINTR) {}
        else{
            perror("Accept New Connection Error!!");
            return false;
        }
        
    }

    // cout<<"Remote Client info:: IP: "<<inet_ntoa(client_address.sin_addr)<< " Port: "<<ntohs(client_address.sin_port)<<endl;

    // cout<<"Max fd: "<<maxfd<<endl;
    FD_SET(client_socket, &all_sockets);
    active_connections.insert(client_socket);
    maxfd = max(maxfd, client_socket);
    // cout<<"client_socket: "<<client_socket<<endl;
    // cout<<"Max fd: "<<maxfd<<endl;

    // Try sending init message to the newly connected client 
    if (send(client_socket, init_message.c_str(), init_message.length(), 0) < 0) {
        std::cerr << "Send failed" << std::endl;
    }
    
    return true;
    // 1. Check if maximum number of connections is already active
    // 2. Handle connection request
    // 3. Add new client to the active_connection set
}

void GameServer::handleClient(int client)
{
    cout<<"input from the client: "<<client<<endl;
    int valread;
    char buffer[1024];
    valread = read(client , buffer, 1024); 
    printf("%s\n",buffer );
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