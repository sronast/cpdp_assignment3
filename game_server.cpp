#include "game_server.hpp"

// GameServer

GameServer::GameServer(int port)
{
    server_port = port;
    init_message = R"(      
        
                    -=-= AUTHORIZED USERS ONLY =-=-
        You are attempting to log into online tic-tac-toe Server.
        Please be advised by continuing that you agree to the terms of the
        Computer Access and Usage Policy of online tic-tac-toe Server.

username (guest):)";

    manual = R"(
        Commands supported:
            who                     # List all online users
            stats [name]            # Display user information
            game                    # list all current games
            observe <game_num>      # Observe a game
            unobserve               # Unobserve a game
            match <name> <b|w> [t]  # Try to start a game
            <A|B|C><1|2|3>          # Make a move in a game
            resign                  # Resign a game
            refresh                 # Refresh a game
            shout <msg>             # shout <msg> to every one online
            tell <name> <msg>       # tell user <name> message
            kibitz <msg>            # Comment on a game when observing
            ' <msg>                 # Comment on a game
            quiet                   # Quiet mode, no broadcast messages
            nonquiet                # Non-quiet mode
            block <id>              # No more communication from <id>
            unblock <id>            # Allow communication from <id>
            listmail                # List the header of the mails
            readmail <msg_num>      # Read the particular mail
            deletemail <msg_num>    # Delete the part02    icular mail
            mail <id> <title>       # Send id a mail
            info <msg>              # change your information to <msg>
            passwd <new>            # change password
            exit                    # quit the system
            quit                    # quit the system
            help                    # print this message
            ?                       # print this message
    )";

    supported_commands = {
        "who", "stats", "game", "observe", "unobserve", "match", "A1", "A2",
        "A3", "B1", "B2", "B3", "C1", "C2", "C3", "resign", "refresh", "shout",
        "tell", "kibitz", "quiet", "nonquiet", "block", "unblock", "listmail",
        "readmail", "deletemail", "mail", "info", "passwd", "exit", "quit", "help", "?"};

    /*
    who, stats [name], game  ,observe <game_num> , unobserve, match <name> <b|w> [t]
            <A|B|C><1|2|3> , resign, refresh, shout <msg>, tell <name> <msg>
            kibitz <msg>, , quiet, nonquiet, block <id>, unblock <id>, listmail, readmail <msg_num>
            deletemail <msg_num> , mail <id> <title>, info <msg>, passwd <new>, exit
            quit, help, ?
    */
}

void GameServer::start()
{
    // 1. Create a server socket here
    // 2. Use select to handle multiple use cases like, accepting new connection, communication between clients
    // 3. Load all the saved states
    // 4.
    // server_socket = setup_server();
    setupServer();
    cout << "server setup complete...\n"
         << endl;
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
    server_address.sin_addr.s_addr = INADDR_ANY;  // htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port); // Server port

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
    // maxfd = server_socket;
    // cout << "Server socket: " << server_socket << endl;
    // cout << "Server started at IP: " << inet_ntoa(server_address.sin_addr) << " Port: " << htons(server_address.sin_port) << endl;
}

void GameServer::handleConnections()
{
    while (true)
    {
        // select is destructive, so make a copy
        ready_sockets = all_sockets;
        int socket_count;
        if ((socket_count = select(maxfd + 1, &ready_sockets, NULL, NULL, NULL)) < 0)
        {
            handleConnectionError("Select error!!");
        }
        // select(maxfd+1, &ready_sockets, NULL, NULL, NULL);
        // cout << "now handling connection, counts: " << active_connections.size()<<endl;
        // Go through all fds,

        // Make a temporary vector for iterating over the current fds
        const vector<int> temp_fds = {active_connections.begin(), active_connections.end()};
        cout<<"set size: "<<active_connections.size()<<endl;
        for (int it : temp_fds)
        {
            // Check which socket is ready
            if (FD_ISSET(it, &ready_sockets))
            {
                // If it is server socket, connection request received
                if (it == server_socket)
                {
                    // handle new connection
                    bool res = acceptNewConnection();
                    if (!res)
                    {
                        cout << "Cannot accept new client's connection ..!!!\n";
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
    }
}

bool GameServer::acceptNewConnection()
{
    cout << "new client request" << endl;
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);
    // Accept the connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len)) < 0)
    {
        if (errno == EINTR)
        {
        }
        else
        {
            perror("Accept New Connection Error!!");
            return false;
        }
    }

    // cout<<"Remote Client info:: IP: "<<inet_ntoa(client_address.sin_addr)<< " Port: "<<ntohs(client_address.sin_port)<<endl;

    // cout<<"Max fd: "<<maxfd<<endl;
    FD_SET(client_socket, &all_sockets);
    active_connections.insert(client_socket);
    // maxfd = max(maxfd, client_socket);
    // cout<<"client_socket: "<<client_socket<<endl;
    // cout<<"Max fd: "<<maxfd<<endl;

    // Try sending init message to the newly connected client
    socket_user_map[client_socket] = "guest";
    if (send(client_socket, init_message.c_str(), init_message.length(), 0) < 0)
    {
        std::cerr << "Send failed" << std::endl;
    }

    return true;
    // 1. Check if maximum number of connections is already active
    // 2. Handle connection request
}

void GameServer::handleClient(int client) // For handling different client inputs
{
    cout << "input from the client: " << client << endl;

    int msg_size;
    char buffer[2048];
    bzero(&buffer, 2048);
    msg_size = read(client, buffer, 2048);

    // Convert buffer to string
    string received_data(buffer, msg_size);


    vector<string> tokens = tokenize(received_data, ' ');
    for (string t : tokens)
    {
        cout << t << endl;
    }

    string command = tokens[0];
    cout << "Command is\n " << command;
    cout << "=============== " << endl;
    // cout << "Tokens is " << tokens;

    if (received_data[0] == '\n' || received_data[0] == '\r')
    {
        handleEmptyMsg(client, received_data, tokens);
    }
    else if(command == "exit" || command=="quit"){
        handleExitMsg(client);
    }
    else if (command == "who"){
        
    }
    else if (command == "stats"){

    }
    else if (command == "game"){

    }
    else if (command == "observe"){

    }
    else if (command == "unobserve"){

    }
    else if (command == "match"){

    }
    else if (command == "resign"){

    }
    else if (command == "refresh"){

    }
    else if (command == "shout"){

    }
    else if (command == "tell"){

    }
    else if (command == "kibitz"){

    }
    else if (command == "\""){

    }
    else if (command == "quiet"){

    }
    else if (command == "nonquiet"){

    }
    else if (command == "block"){

    }
    else if (command == "unblock"){

    }
    else if (command == "listmail"){

    }
    else if (command == "readmail"){

    }
    else if (command == "deletemail"){

    }
    else if (command == "mail"){

    }
    else if (command == "info"){

    }
    else if (command == "passwd"){

    }
    else if (command == "help"){

    }
    else if (command == "?"){

    }
    else if (command == "register"){

    }

}
void GameServer::handleEmptyMsg(int &client, string &msg, vector<string> &tokens)
{
    string user = socket_user_map[client];
    string manual_msg = manual;
    if (user == "guest")
    {
        string msg = "\tYou are logged in as a guest.\n"
        "\tThe only command that you can use is 'register username password' and 'quit/exit'.\n"
        "<guest: >";
        manual_msg = string(manual_msg) + msg;
    }

    if (send(client, manual_msg.c_str(), manual_msg.length(), 0) < 0)
    {
        std::cerr << "Send failed" << std::endl;
    }
    cout << "sent.." << endl;
}

void GameServer::handleExitMsg(int &client){
    string user = socket_user_map[client];

    //Remove socket from active connections
    active_connections.erase(client);
    //Remove socket from socket_user_map
    socket_user_map.erase(client);
    //If user is registered, remove from user_socket_map.
    if(user!="guest"){
        user_socket_map.erase(user);
    }
    //Close the socket
    close(client);
    // Clear it from the socket 
    FD_CLR(client, &all_sockets);
    cout<<"Closed, AC size: "<<active_connections.size()<<endl;
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