#include "game_server.hpp"
#include "classes.hpp"
#include <vector>
#include <string>

vector<User> allUsers = {};

// GameServer

GameServer::GameServer(int port=50001)
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


void initializeUser() {
    // Clear existing user data if needed
    allUsers.clear();
    cout << "initializing users" << endl;

    std::string usersFolder = "users";

    // Iterate through files in the "users" directory
    for (const auto& entry : std::filesystem::directory_iterator(usersFolder)) {
        // Check if the entry is a regular file
        if (entry.is_regular_file()) {
            // cout << "is a regular file" << endl;
            std::string filename = entry.path().filename().string();
            // Extract username from filename
            std::string username = filename.substr(0, filename.find(".txt"));

            // Open the user file
            std::ifstream userFile(entry.path());
            if (userFile.is_open()) {
                // Read user data from file
                std::string line;
                User user;

                // Parse user data from each line
                while (std::getline(userFile, line)) {
                    std::istringstream iss(line);
                    std::string key, value;
                    if (std::getline(iss, key, ':') && std::getline(iss, value)) {
                        // Trim whitespace from key and value
                        key.erase(0, key.find_first_not_of(" \t\r\n"));
                        key.erase(key.find_last_not_of(" \t\r\n") + 1);
                        value.erase(0, value.find_first_not_of(" \t\r\n"));
                        value.erase(value.find_last_not_of(" \t\r\n") + 1);

                        // Parse specific fields
                        if (key == "password") {
                            user.setPassword(value);
                        } else if (key == "wins") {
                            user.setWins(std::stoi(value));
                        } else if (key == "loss") {
                            user.setLoss(std::stoi(value));
                        } else if (key == "draw") {
                            user.setDraw(std::stoi(value));
                        } else if (key == "isPlaying") {
                            user.setIsPlaying(value == "true");
                        } else if (key == "messages") {
                            // Parse messages
                            std::vector<Message> messages = user.parseMessage(value);
                            user.setMessages(messages);
                        } else if (key == "mail") {
                            // Parse mail
                            std::vector<std::string> mail = user.parseMail(value);
                            user.setMail(mail);
                        } else if (key == "quietMode") {
                            user.setQuietMode(value == "true");
                        } else if (key == "blockList") {
                            // Parse blockList
                            std::vector<std::string> blockList = user.parseBlockList(value);
                            user.setBlockList(blockList);
                        } else if (key == "rank") {
                            user.setRank(std::stoi(value));
                        } else if (key == "points") {
                            user.setPoints(std::stoi(value));
                        } else if (key == "totalGames") {
                            user.setTotalGames(std::stoi(value));
                        }
                        // Add more fields as needed
                    }
                }
                user.setUsername(username);
                userFile.close();

                // Add user to allUsers
                allUsers.push_back(user);
            } else {
                std::cerr << "Failed to open user file: " << filename << std::endl;
            }
        }

        // Print out all users
    cout << "Printing all users:" << endl;
    for (const auto& user : allUsers) {
        cout << "Username: " << user.getUsername() << endl;
        // cout << "Password: " << user.getPassword() << endl;
        // cout << "win: " << user.getWins() << endl;
        // cout << "loss: " << user.getLoss() << endl;
        // cout << "draw: " << user.getDraw() << endl;
        // cout << "points: " << user.getPoints() << endl;
        // cout << "rank: " << user.getRank() << endl;
        // cout << "Messages:" << endl;
        const std::vector<Message>& messages = user.getMessages();
        // for (const auto& message : messages) {
        //     cout << "From: " << message.getFrom() << endl;
        //     cout << "Time: " << message.getTime() << endl;
        //     cout << "Status: " << message.getStatus() << endl;
        //     cout << "Message: " << message.getMsg() << endl;
        //     cout << endl;
        // }

        // cout << "mail" << endl;
        // for (const auto&mail: user.getMail()) {
        //     cout << mail << endl;
        // }
        // cout << "block list" << endl;
        // for (const auto&block: user.getBlockList()) {
        //     cout << block << endl;
        // }
        // cout << "quietMode: " << user.getQuietMode() << endl;
        // cout << "isPLaying: " << user.getIsPlaying() << endl;
        // cout << "totalGames: " << user.getTotalGames() << endl;
        // Print other user information as needed
    }
    }
}

void GameServer::start()
{
    setupServer();
    cout << "server setup complete...\n"
         << endl;
    initializeUser();
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
    maxfd = server_socket;
    // cout << "Server socket: " << server_socket << endl;
    // cout << "Server started at IP: " << inet_ntoa(server_address.sin_addr) << " Port: " << htons(server_address.sin_port) << endl;
}

void GameServer::handleConnections()
{
    while (true)
    {
        cout<<"again in the server\n";
        // select is destructive, so make a copy
        ready_sockets = all_sockets;
        int socket_count;
        if ((socket_count = select(maxfd + 1, &ready_sockets, NULL, NULL, NULL)) < 0)
        {
            handleConnectionError("Select error!!");
        }

        // Make a temporary vector for iterating over the current fds
        const vector<int> temp_fds = {active_connections.begin(), active_connections.end()};
        cout << "set size: " << active_connections.size() << endl;
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
    maxfd = max(maxfd, client_socket);
    not_logged_in[client_socket] = "";
    // cout<<"client_socket: "<<client_socket<<endl;
    // cout<<"Max fd: "<<maxfd<<endl;

    // Try sending init message to the newly connected client
    // socket_user_map[client_socket] = "guest";

    sendMsg(client_socket, init_message);
    return true;
    // 1. Check if maximum number of connections is already active
    // 2. Handle connection request
}

bool GameServer::handleClient(int client) // For handling different client inputs
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
    cout << "Command is: " << command<<endl;
    cout << "=============== " << endl;

    bool is_empty_msg = (received_data[0] == '\n' || received_data[0] == '\r');

    // Client tries to log in
    if (not_logged_in.find(client) != not_logged_in.end())
    {
        cout<<"In login"<<endl;
        handleLogin(client, is_empty_msg, tokens, command, received_data);
    }
    // The client is guest
    else if (active_guests.find(client) != active_guests.end())
    {
        handleGuest(client, is_empty_msg, tokens, command, received_data);
    }
    // The client is registered user
    else
    {
        handleRegisteredUser(client, is_empty_msg, tokens, command, received_data);
    }

    // handle the cases where the client is the registererd user
    return true;
}

void GameServer::handleLogin(int &client, bool &is_empty_msg, vector<string> &tokens,
                             string &command, string &received_data)
{
    string user = not_logged_in[client];
    // if empty, then expect username else add client as guest
    if (user.empty())
    {

        if (is_empty_msg)
        {
            not_logged_in.erase(client);
            active_guests.insert(client);
            handleEmptyMsg(client);
        }
        else if ((all_users.find(command) != all_users.end()) && (tokens.size() == 1))
        {
            not_logged_in[client] = command;
            string msg = "Password: ";
            sendMsg(client, msg);
        }
        else
        {
            string msg = "Username not found";
            handleClientExit(client, msg);
        }
    }
    else
    {
        not_logged_in.erase(client);
        string saved_password = "rama";
        if (saved_password == received_data)
        {
            user_socket_map[user] = client;
            socket_user_map[client] = user;
            string msg = "Successfully logged in\n";

            // Get user
            // Find the number of unread messages
            // send to client.
        }
        else
        {
            string msg = "Password does not match...";
            handleClientExit(client, msg);
        }
   
    }
}

void GameServer::handleGuest(int &client, bool &is_empty_msg, vector<string> &tokens,
                             string &command, string &received_data)
{
    if (is_empty_msg)
    {
        handleEmptyMsg(client);
    }
    else if (command == "exit" || command == "quit")
    {
        string msg = "Bye Bye...\n";
        handleClientExit(client, msg);
    }

    else if (command == "register")
    {
        string username = tokens[1];
        string password = tokens[2];
        User user = registerUser(username, password, true);
        if (user.getUsername().empty()){
            cout << "User registration failed \n";
        }
        else{
            cout << "User registered \n";
        }
        // handle user regsitration
    }
    else
    {
        string message = "\tYou are logged in as a guest.\n"
                         "\tClient can only register new user\n"
                         "\tCommand: register username password\n";
                         "<guest: >";

        sendMsg(client, message);
    }
}

void GameServer::handleRegisteredUser(int &client, bool &is_empty_msg, vector<string> &tokens,
                                      string &command, string &received_data)
{
    if (command == "who")
    {
        // list all online users
        string allOnlineUsers = getOnlineUsers();
        sendMsg(client, allOnlineUsers);
    }
    else if (command == "stats")
    {
    }
    else if (command == "game")
    {
    }
    else if (command == "observe")
    {
    }
    else if (command == "unobserve")
    {
    }
    else if (command == "match")
    {
    }
    else if (command == "resign")
    {
    }
    else if (command == "refresh")
    {
    }
    else if (command == "shout")
    {
    }
    else if (command == "tell")
    {
    }
    else if (command == "kibitz")
    {
    }
    else if (command == "\"")
    {
    }
    else if (command == "quiet")
    {
    }
    else if (command == "nonquiet")
    {
    }
    else if (command == "block")
    {
    }
    else if (command == "unblock")
    {
    }
    else if (command == "listmail")
    {
    }
    else if (command == "readmail")
    {
    }
    else if (command == "deletemail")
    {
    }
    else if (command == "mail")
    {
    }
    else if (command == "info")
    {
    }
    else if (command == "passwd")
    {
    }
    else if (command == "help")
    {
    }
    else if (command == "?")
    {
    }
    else if (command == "register")
    {
    }
    else
    {
        string msg = "Command not suppported!!";
    }
}

void GameServer::handleEmptyMsg(int &client)
{

    // string user = socket_user_map[client];
    auto it = active_guests.find(client);

    string manual_msg = manual;

    // if the request is from guest
    if (it != active_guests.end())
    {
        string msg = "\tYou are logged in as a guest.\n"
                     "\tThe only command that you can use is 'register username password' and 'quit/exit'.\n"
                     "<guest: >";
        manual_msg = string(manual_msg) + msg;
    }
    else
    {
        // if not
    }

    sendMsg(client, manual_msg);
}

void GameServer::handleClientExit(int &client, string &msg)
{
    // string user = socket_user_map[client];

    // Remove socket from active connections

    if (!msg.empty())
    {
        sendMsg(client, msg);
    }

    active_connections.erase(client);

    if (active_guests.find(client) != active_guests.end()){
        active_guests.erase(client);
    }

    else if(not_logged_in.find(client) != not_logged_in.end()){
        not_logged_in.erase(client);
    }

    else{

    }



    close(client);
    // Clear it from the socket
    FD_CLR(client, &all_sockets);
    cout << "Closed, AC size: " << active_connections.size() << endl;
    maxfd = getMaxSet(active_connections);
    cout << "successfully exited: "<< endl;

}

void GameServer::handleConnectionError(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void GameServer::sendMsg(int &client, string &msg)
{
    if (send(client, msg.c_str(), msg.length(), 0) < 0)
    {
        std::cerr << "Send failed" << std::endl;
    }
    cout << "sent.." << endl;
}

string GameServer::getOnlineUsers()
{
    string res = "";
    for (const auto &pair : user_socket_map)
    {
        std::cout << "Key: " << pair.first << std::endl;
        res = res + pair.first + " ";
    }
    return res;
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
    std::string filename = "users/" + username + ".txt";
    std::ofstream userFile(filename);
    if (!userFile.is_open()) {
        std::cerr << "Error creating user file." << std::endl;
        return usr;
    }

    // Write user data to file
    userFile << "username: " << username << "\n";
    userFile << "password: " << password << "\n";
    userFile << "isPlaying: false\n";
    userFile << "wins: 0\n";
    userFile << "loss: 0\n";
    userFile << "draw: 0\n";
    userFile << "message: {}\n";
    userFile << "mail: {}\n";
    userFile << "quietMode: false\n";
    userFile << "blockList: {}\n";
    userFile << "rank: 0\n";
    userFile << "points: 0\n";
    userFile << "totalGames: 0\n";
    userFile.close();

    usr.setUsername(username);
    usr.setPassword(password);
    usr.setIsPlaying(false);
    usr.setWins(0);
    usr.setDraw(0);
    usr.setLoss(0);
    usr.setRank(0);
    usr.setPoints(0);
    usr.setPoints(0);
    usr.setMail({});
    usr.setMessages({});
    usr.setBlockList({});
    usr.setQuietMode(false);

    allUsers.push_back(usr);
    return usr;
    // return usr.registerUser(username, password);
}




bool GameServer::loginUser(string username, string password) {}