#include "game_server.hpp"
#include "classes.hpp"
#include <vector>
#include <string>
#include <functional>

// GameServer

string draftMsg = "";

void handleAlarm(int sig)
{
    // Reset the alarm for another 5 minutes (300 seconds)
    alarm(120);
}

GameServer::GameServer(int port)
{

    signal(SIGALRM, handleAlarm);
    alarm(120);

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

    allUsersInfo.clear();
    cout << "initializing users....." << endl;

    std::string usersFolder = "users";

    // Iterate through files in the "users" directory
    for (const auto &entry : std::filesystem::directory_iterator(usersFolder))
    {

        // Check if the entry is a regular file
        if (entry.is_regular_file())
        {

            // cout << "is a regular file" << endl;
            std::string filename = entry.path().filename().string();
            cout << filename << endl;
            // Extract username from filename
            std::string username = filename.substr(0, filename.find(".txt"));

            // Open the user file
            std::ifstream userFile(entry.path());
            if (userFile.is_open())
            {

                // Read user data from file
                std::string line;
                User user;

                // Parse user data from each line
                while (std::getline(userFile, line))
                {
                    cout << "Line:: " << line << endl;

                    std::istringstream iss(line);
                    std::string key, value;
                    if (std::getline(iss, key, ':') && std::getline(iss, value))
                    {
                        // Trim whitespace from key and value
                        key.erase(0, key.find_first_not_of(" \t\r\n"));
                        key.erase(key.find_last_not_of(" \t\r\n") + 1);
                        value.erase(0, value.find_first_not_of(" \t\r\n"));
                        value.erase(value.find_last_not_of(" \t\r\n") + 1);

                        // Parse specific fields
                        if (key == "password")
                        {
                            user.setPassword(value);
                        }
                        else if (key == "wins")
                        {
                            user.setWins(std::stoi(value));
                        }
                        else if (key == "loss")
                        {
                            user.setLoss(std::stoi(value));
                        }
                        else if (key == "draw")
                        {
                            user.setDraw(std::stoi(value));
                        }
                        else if (key == "isPlaying")
                        {
                            user.setIsPlaying(std::stoi(value));
                        }
                        else if (key == "messages")
                        {
                            // Parse messages
                            // cout << "parsing messages" << endl;
                            std::vector<Message> messages = user.parseMessage(value);
                            allUserMessages[username] = messages;
                        }
                        else if (key == "mail")
                        {
                            // Parse mail
                            // cout << "parsing mail" << endl;
                            std::vector<Mail> mail = user.parseMail(value);
                            allUserMails[username] = mail;
                        }
                        else if (key == "quietMode")
                        {
                            cout << "QM" << endl;
                            user.setQuietMode(std::stoi(value));
                        }
                        else if (key == "blockList")
                        {
                            // Parse blockList
                            // cout << "parsing block list" << endl;
                            std::vector<std::string> blockList = user.parseBlockList(value);
                            // user.setBlockList(blockList);
                        }
                        else if (key == "rank")
                        {
                            user.setRank(std::stoi(value));
                        }
                        else if (key == "points")
                        {
                            user.setPoints(std::stoi(value));
                        }
                        else if (key == "totalGames")
                        {
                            user.setTotalGames(std::stoi(value));
                        }
                        // Add more fields as needed
                    }
                }
                cout << "Sabai lines read garyo" << endl;
                user.setUsername(username);
                userFile.close();
                cout << "File close vayo hai " << endl;
                cout << "User name is " << username << endl;
                // Add user to allUsers

                cout << "User object heram: " << user.getUsername() << endl;
                cout << "User object heram password: " << user.getPassword() << endl;
                // cout << "User object heram password" << user.isVa() << endl;
                // allUsersInfo[username] = user;

                cout << "All user info ma insert gardai....." << endl;
                if (!user.username.empty())
                {
                    cout << "User still exists" << user.username << endl;
                }
                else
                {
                    cout << "user has moved" << endl;
                }
                allUsersInfo.insert(make_pair(username, std::move(user)));
                cout << "All user info ma insert gardai 222....." << endl;
                all_users.insert(username);
                cout << "insert ni garyo" << endl;
            }
            else
            {
                std::cerr << "Failed to open user file: " << filename << std::endl;
            }
            cout << "if else sakkyo" << endl;
        }
        cout << "Print garna baki xa" << endl;
        // Print out all users
        cout << "Printing all users:" << endl;
        cout << "for one it ended" << endl;
    }
}

void GameServer::start()
{
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
    // server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY);
    server_address.sin_addr.s_addr = INADDR_ANY;  // htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port); // Server port

    // Bind socket to the address
    int bind_result = ::bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result < 0)
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
        // cout << "again in the server\n";
        // select is destructive, so make a copy
        ready_sockets = all_sockets;
        int socket_count;
        if ((socket_count = select(maxfd + 1, &ready_sockets, NULL, NULL, NULL)) < 0)
        {
            // cout << "Select error" << endl;
            if (errno == EINTR)
            {
                printf("Saving all data\n");
                saveAllData();
                continue;
            }
            handleConnectionError("Select error!!");
        }

        // Make a temporary vector for iterating over the current fds
        const vector<int> temp_fds = {active_connections.begin(), active_connections.end()};
        // cout << "set size: " << active_connections.size() << endl;
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
    bool is_empty_msg = (received_data[0] == '\n' || received_data[0] == '\r');
    if (tokens.size() > 0)
    {

        string command = tokens[0];
        cout << "Command is: " << command << endl;
        // cout << "=============== " << endl;

        // Client tries to log in
        if (not_logged_in.find(client) != not_logged_in.end())
        {
            cout << "In login" << endl;
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
    }
    else if (is_empty_msg)
    {
        cout << "empty message" << endl;
        string emptyCommand = "";
        handleLogin(client, is_empty_msg, tokens, emptyCommand, received_data);
    }

    // handle the cases where the client is the registererd user
    return true;
}

void GameServer::handleLogin(int &client, bool &is_empty_msg, vector<string> &tokens,
                             string &command, string &received_data)
{
    cout << "Handle login called...\n";
    string user = not_logged_in[client];
    cout << "recieved data is " << received_data << endl;

    for (auto it : all_users)
    {
        cout << it << endl;
    }
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
        string usr_name = not_logged_in[client];
        not_logged_in.erase(client);
        User &usr = allUsersInfo[usr_name];
        cout << "User password stored is " << usr.getPassword() << endl;
        if (trim(usr.getPassword()) == trim(received_data))
        {
            user_socket_map[user] = client;
            socket_user_map[client] = user;
            ostringstream oss;
            oss << "Successfully logged in..\n <" << usr_name << ">:";
            string msg = oss.str();
            sendMsg(client, msg);

            int unreadMsg = 0;
            vector<Mail> userMails = allUserMails[usr_name];

            for(auto mail: userMails){
                if(mail.status == "unread"){
                    unreadMsg +=1;
                }
            }



            msg = "User Info: " + usr.info + "\nYou have "+to_string(unreadMsg)+" unread messages..";
            sendMsg(client, msg);
            sendEmptyMsg(client);
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
    cout << "The command is :" << command << endl;
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
        int register_vayota = registerUser(username, password, true);
        cout << "Register vayo........" << endl;
        string msg = "";
        if (register_vayota == 0)
        {
            cout << "User registration failed \n";
            msg = "\tUser registration failed \n";
        }
        else
        {
            cout << "User registered \n";
            msg = "User registered \n";
            // socket_user_map[client] = username;
        }
        sendMsg(client, msg);
        // sendEmptyMsg(client);
        // handle user regsitration
    }
    else
    {
        string message = "\tYou are logged in as a guest.\n"
                         "\tClient can only register new user\n"
                         "\tCommand: register username password\n";

        sendMsg(client, message);
        sendEmptyMsg(client);
    }
}

string getTimeNow()
{
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Convert the current time to a string
    const int bufferSize = 80; // Adjust buffer size as needed
    char buffer[bufferSize];
    std::strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return buffer;
}
int getIndex(const std::string &input)
{
    // Mapping of input strings to index
    static const std::map<std::string, int> indexMap = {
        {"A1", 0}, {"A2", 1}, {"A3", 2}, {"B1", 3}, {"B2", 4}, {"B3", 5}, {"C1", 6}, {"C2", 7}, {"C3", 8}};

    // Find the input string in the map
    auto it = indexMap.find(input);
    if (it != indexMap.end())
    {
        return it->second;
    }

    // Return -1 if input string is not found
    return -1;
}
void GameServer::handleRegisteredUser(int &client, bool &is_empty_msg, vector<string> &tokens,
                                      string &command, string &received_data)
{
    ostringstream oss;
    string user_name = socket_user_map[client];
    User &user = allUsersInfo[user_name];
    if (command == "who")
    {
        // list all online users
        string username = socket_user_map[client];
        string allOnlineUsers = getOnlineUsers();
        sendMsg(client, allOnlineUsers);
        sendEmptyMsg(client);
    }
    else if (command == "exit" || command == "quit")
    {
        string msg = "Bye Bye...\n";
        handleClientExit(client, msg);
    }
    else if (command == "stats")
    {
        string userNameToView = socket_user_map[client];
        if (tokens.size() > 1)
        {
            userNameToView = tokens[1];
        }
        User &user = allUsersInfo[userNameToView];
        string username = socket_user_map[client];
        std::ostringstream oss;
        oss << "Stats for: " << userNameToView << ": \n"
            << "\nInfo: " << user.info
            << "\nWins: " << user.getWins()
            << "\nLoss: " << user.getLoss()
            << "\nDraws: " << user.getDraw()
            << "\nPoints: " << user.getPoints()
            << "\nRank: " << user.getRank();
        std::string msg = oss.str();
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "game")
    {
        cout << "game command\n";
        string msg = "All games:";
        for (auto it : all_games)
        {
            TicTacToe &game = it.second;

            msg += "\n Game ID: " + to_string(game.id);
            msg += "\n Player 1: " + game.user1;
            msg += "\n Player 2: " + game.user2;
            msg += "\n Player to move: " + game.next_move;
        }
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "observe")
    {
        // handle tokens <=1
        string gameToObserve = tokens[1];
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        // handle string to int conversion error
        int gameNumber = stoi(gameToObserve);
        user.gameObserving = gameNumber;
        TicTacToe &game = all_games[gameNumber];
        game.observerSet.insert(client);
        // game.observers.push_back(client);
        string msg = "Oberving game: " + gameToObserve + ". You will now receive updates when moves are made or someone comments";
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "unobserve")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        TicTacToe &game = all_games[user.gameObserving];
        if (game.observerSet.find(client) != game.observerSet.end())
        {
            game.observerSet.erase(client);
            user.gameObserving = 0;
            string msg = "Removed game from observing";
            sendMsg(client, msg);
            sendEmptyMsg(client);
        }
        else
        {
            sendEmptyMsg(client);
        }
    }
    else if (command == "match")
    {
        string opponent_name = tokens[1];
        string blackOrWhite = tokens[2];
        string timeStr = tokens[3];
        // int gameTime = stoi(timeStr);

        User &opponent = allUsersInfo[opponent_name];
        int opponet_fd = user_socket_map[opponent_name];

        if (user.getIsPlaying())
        {
            oss << "You are currently playing a game..\nComplete it to send new match request..\n<" << user_name << ">:";
            string msg = oss.str();
            sendMsg(client, msg);
        }

        else if (!isUserOnline(opponent_name))
        {
            oss << "User " << opponent_name << " is offline..\n<" << user_name << ">:";
            string msg = oss.str();
            sendMsg(client, msg);
        }
        else if (opponent.getIsPlaying())
        {
            oss << "User " << opponent_name << " is currently playing..\n<" << user_name << ">:";
            string msg = oss.str();
            sendMsg(client, msg);
        }
        else
        {

            cout << "opponent name is " << opponent_name << endl;

            auto it = match_requests.find(user_name);
            bool malaiRequestAkoXa = it != match_requests.end();

            if (malaiRequestAkoXa && match_requests[user_name] == opponent_name)
            {
                // The opponent has already sent match request
                // User is acceptig the request
                cout << "pailai dekhin xa request --------" << endl;
                string gameK = user_name + ":" + opponent_name;
                string gameSetting = game_settings[gameK];
                vector<string> settings = tokenize(gameSetting, ' ');
                bool blackOrWhiteMatch = settings[0] != tokens[2];
                bool timeMatch = settings[1] == tokens[3];
                string opponentBlackOrW = tokens[2] == "b" ? "w" : "b";

                if (!blackOrWhiteMatch || !timeMatch)
                {
                    cout << "match vaena requests" << endl;
                    // game settings do not match. dont start the match. send new request to them instead.
                    string msg = user_name + " has requested a game with new settings. Type match " + user_name + " " + opponentBlackOrW + " " + tokens[3];
                    int opponentClient = user_socket_map[opponent_name];
                    sendMsg(opponentClient, msg);
                    sendEmptyMsg(opponentClient);
                    sendEmptyMsg(client);

                    // also remove the old request and insert the new one
                    match_requests.erase(user_name);
                    game_settings.erase(gameK);

                    // insert the new one
                    match_requests[opponent_name] = user_name;
                    string newGameKey = opponent_name + ":" + user_name;
                    string newGameSetting = tokens[2] + " " + tokens[3];
                    game_settings[newGameKey] = newGameSetting;
                }
                else
                {
                    cout << "match vayo request" << endl;
                    // match requests match. Now start a match
                    cout << "Username is " << user_name << endl;
                    match_requests.erase(user_name);
                    // user.request_from.erase(opponent_name);
                    // for (const auto it : allUsersInfo)
                    // {
                    //     if (it.first == opponent_name)
                    //     {
                    //     }
                    //     else if (it.first == user_name)
                    //     {
                    //     }
                    // }

                    user.setIsPlaying(true);
                    opponent.setIsPlaying(true);
                    user.opponent = opponent_name;
                    opponent.opponent = user_name;

                    cout << "Game init" << endl;
                    // create a new instance of game
                    TicTacToe game = TicTacToe(user_name, opponent_name);
                    game.user1Time = stod(settings[1]);
                    game.user2Time = stod(settings[1]);

                    // time_t t1 = time(0);
                    game.previousMoveTime = time(0);

                    user.currentGameId = game.id;
                    user.moveName = "X";
                    cout << "Eta samma" << endl;
                    opponent.currentGameId = game.id;
                    cout << "1" << endl;
                    opponent.moveName = "O";
                    cout << "2" << endl;

                    int id = game.id;
                    cout << "3" << endl;

                    // by reference use graya xa??
                    all_games[id] = game;
                    cout << "4" << endl;

                    cout << "Before displaying board" << endl;
                    string board = game.displayBoard();
                    cout << "After displaying board" << board << endl;

                    oss << "Game started! User to move: " << user_name << endl;
                    oss << board << endl;
                    string msg = oss.str();
                    cout << "msg is " << msg << endl;
                    sendMsg(opponet_fd, msg);
                    sendEmptyMsg(opponet_fd);
                    sendMsg(client, msg);
                    sendEmptyMsg(client);
                }
            }
            else
            {
                cout << "Inserting to set" << endl;
                // The user is sending match request to the opponent_name for the first time
                // if user already has a key and the opponent name matches with the value then start a game and delete the entry

                // malai match request ako xa ta
                auto it = match_requests.find(user_name);
                bool malaiRequestAkoXa = it != match_requests.end();
                if (malaiRequestAkoXa && match_requests[user_name] == opponent_name)
                {
                    // start the game
                    cout << "pailai dekhin xa request" << endl;
                }
                //  haina vane chai send request to opponent
                match_requests[opponent_name] = user_name;
                // also add the game settings to map
                // use key as opponent:user = b|w t
                string gameKey = opponent_name + ":" + user_name;
                string gameValue = blackOrWhite + " " + timeStr;
                game_settings[gameKey] = gameValue;
                string opponentBlackOrWhite = blackOrWhite == "b" ? "w" : "b";
                // opponent.request_from.insert(user_name);
                oss << user_name << " has invited you for a match. To accept type command: match " << user_name << " " << opponentBlackOrWhite << " " << timeStr << "\n<" << opponent_name << ">:";
                string msg = oss.str();
                sendMsg(opponet_fd, msg);
                oss.str("");
                oss << "Sent match request to " << opponent_name << "\n<" << user_name << ">:";
                string msg1 = oss.str();
                sendMsg(client, msg1);
            }
        }
    }
    else if (command == "resign")
    {
        // check if user is playing
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        string msg = "";
        if (!user.getIsPlaying())
        {
            msg += "You are not currently playing a match.";
            sendMsg(client, msg);
            sendEmptyMsg(client);
        }
        else
        {
            int gameId = user.currentGameId;
            TicTacToe &game = all_games[gameId];

            string winner = game.user1;

            if (game.user1 == username)
            {
                // current player is user 1
                // get the other user
                winner = game.user2;
            }
            User &opponent = allUsersInfo[winner];
            int opponentClient = user_socket_map[winner];

            string observerMsg = user.getUsername() + " has resigned the game!";
            for (auto it : game.observers)
            {
                sendMsg(it, observerMsg);
                sendEmptyMsg(it);
            }

            gameWon(winner, game, opponentClient, client, user, opponent);

            string userMsg = "You have lost the game!";
            sendMsg(client, userMsg);
            sendEmptyMsg(client);

            string opponentMsg = user.getUsername() + " has resigned the game. You have won!";
            sendMsg(opponentClient, opponentMsg);
            sendEmptyMsg(opponentClient);

            // alert all observers too

            // remove the game
            all_games.erase(gameId);
        }
    }
    else if (command == "refresh")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        // current game observing
        int currentGame = user.gameObserving;
        if (currentGame == 0)
        {
            string msg = "You are not observing any games at the moment";
            sendMsg(client, msg);
            sendEmptyMsg(client);
        }
        else
        {
            TicTacToe &game = all_games[currentGame];
            string msg = "Game refreshed! \n";
            msg += game.displayBoard();
            sendMsg(client, msg);
            sendEmptyMsg(client);
        }
    }
    else if (command == "shout")
    {
        string usrname = socket_user_map[client];
        string msg = usrname + " shouted a message \n";
        for (auto it : tokens)
        {
            if (it == command)
            {
                continue;
            }
            msg += it + " ";
        }

        for (const auto it : socket_user_map)
        {
            int clientId = it.first;
            string username = socket_user_map[clientId];
            if (clientId == client)
            {
                continue;
            }
            User &usr = allUsersInfo[username];
            vector<string> userBlockList;
            for (const auto it : userBlockList)
            {
                cout << "Block list: " << it << endl;
            }
            bool isUserBlocked = isItemInSet(usrname, usr.blockListSet);
            if (!usr.getQuietMode() && !isUserBlocked)
            {
                // only send message when the user is not in quiet mode and has not blocked the user
                sendMsg(clientId, msg);
                sendEmptyMsg(clientId);
            }
            // sendEmptyMsg(client);
        }
        string msg4 = "Shouted to everyone!! \n<" + usrname + ">";
        sendMsg(client, msg4);
    }
    else if (command == "tell")
    {
        string messageTo = tokens[1];
        string message = "";
        for (auto it : tokens)
        {
            if (it == command || it == messageTo)
            {
                continue;
            }
            message += it;
            message += " ";
        }
        string userFrom = socket_user_map[client];

        User &user = allUsersInfo[messageTo];
        // int randomId = generateRandomNumber(1000, 9999);
        Message msg = Message(userFrom, message, "read", getTimeNow());
        vector<Message> userMessages = allUserMessages[user.username];
        // vector<Message> userMessages =  user.getMessages();
        bool isUserInQuietMode = user.getQuietMode();
        userMessages.push_back(msg);
        allUserMessages[user.username] = userMessages;
        // user.setMessages(userMessages);

        if (!isItemInSet(userFrom, user.blockListSet) && !isUserInQuietMode)
        {
            // dont send message if in block list
            string msgTo = "You have recieved a new message from " + userFrom + "\n Message: " + message + "\t Time: " + msg.getTime() + "\n <" + user.getUsername() + ">";
            int messageToClient = user_socket_map[messageTo];
            string usern = socket_user_map[client];
            sendMsg(messageToClient, msgTo);
            string meroChak = "\n<" + usern + ">";
            sendMsg(client, meroChak);
        }
        else
        {
            sendEmptyMsg(client);
        }
    }
    else if (command == "kibitz" || command == "'")
    {
        User &user = allUsersInfo[socket_user_map[client]];
        int gameId = user.gameObserving;
        TicTacToe &game = all_games[gameId];
        string comment = "";
        for (const auto it : tokens)
        {
            if (it == command)
            {
                continue;
            }
            comment += it + " ";
        }
        string msg = user.getUsername() + " commented: " + comment;
        for (auto it : game.observers)
        {
            sendMsg(it, msg);
            sendEmptyMsg(it);
        }
        User &user1 = allUsersInfo[game.user1];
        int user1Client = user_socket_map[game.user1];
        User &user2 = allUsersInfo[game.user2];
        int user2Client = user_socket_map[game.user2];

        sendMsg(user1Client, msg);
        sendEmptyMsg(user1Client);

        sendMsg(user2Client, msg);
        sendEmptyMsg(user2Client);

        sendEmptyMsg(client);

        game.comments.push_back(msg);
    }
    else if (command == "quiet")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        user.setQuietMode(true);
        string msg = "Quiet mode has been set on! \n <" + username + ">:";
        sendMsg(client, msg);
    }
    else if (command == "nonquiet")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        user.setQuietMode(false);
        string msg = "Quiet mode has been set off! \n <" + username + ">:";
        sendMsg(client, msg);
    }
    else if (command == "block")
    {
        string username = socket_user_map[client];
        string userToBlock = tokens[1];
        User &user = allUsersInfo[username];
        cout << "Username is " << user.getUsername() << endl;

        user.blockListSet.insert(userToBlock);
        // vector<string> blockList = user.getBlockList();
        // blockList.push_back(userToBlock);
        // user.setBlockList(blockList);
        for (auto it : user.blockListSet)
        {
            cout << "Blocked user:" << it << endl;
        }
        cout << "eta samma" << endl;
        string msg = userToBlock + " has been added to block list! \n<" + username + ">:";
        sendMsg(client, msg);
    }
    else if (command == "unblock")
    {
        string username = socket_user_map[client];
        string userToUnblock = tokens[1];
        User &user = allUsersInfo[username];
        string msg = "";
        if (isItemInSet(userToUnblock, user.blockListSet))
        {
            user.blockListSet.erase(userToUnblock);
            msg += userToUnblock + " has been removed from the block list";
        }
        else
        {
            msg += userToUnblock + " is not in the block list";
        }
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "listmail")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        vector<Mail> mails = allUserMails[username];
        // vector<Mail> mails = user.getMail();
        if (mails.size() == 0)
        {
            string msg1 = "You have no mails!";
            sendMsg(client, msg1);
            sendEmptyMsg(client);
        }
        else
        {
            cout << "Email size: " << mails.size() << endl;
            string msg = "Headers: \n";
            for (auto it : mails)
            {
                cout << "ID: " << to_string(it.getId()) << endl;
                cout << "Get header: " << it.getHeaders() << endl;
                cout << "Time: " << it.getTime() << endl;

                msg += to_string(it.getId()) + "\t" + it.getHeaders() + "\t" + it.getTime() + "\n";
                cout << "email :" << msg << endl;
            }
            msg += "<" + username + ">: ";
            sendMsg(client, msg);
        }
    }
    else if (command == "readmail")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        string idToMatch = tokens[1];
        vector<Mail> mails = allUserMails[username];
        cout << "in read mail: \n";
        string msg = "======Reading Mail=====";
        for (auto &it : mails)
        {
            if (it.getId() == stoi(idToMatch))
            {
                it.status = "read";
                msg = msg + "\nFrom: " + it.from + "\nID: " + to_string(it.getId()) + "\nHeader: " + it.getHeaders();
                msg = msg + "\nMessage: " + it.getMsg() + "\nTime: " + it.getTime() + "\n";
                break;
                // msg += to_string(it.getId()) + "\t" + it.getHeaders() + "\t" + it.getMsg() + "\t" + it.getTime() + "\n";
            }
        }
        msg += "<" + username + ">: ";
        sendMsg(client, msg);
    }
    else if (command == "deletemail")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        string idToMatch = tokens[1];
        vector<Mail> mails = allUserMails[username];

        auto it = std::find_if(mails.begin(), mails.end(), [&](const Mail &mail)
                               { return mail.getId() == stoi(idToMatch); });

        string msg = "";
        if (it != mails.end())
        {
            mails.erase(it);
            // user.setMail(mails);
            allUserMails[username] = mails;
            allUsersInfo[username] = user;
            msg += "Message deleted successfully. \n";
        }
        else
        {
            msg += "Message with ID " + idToMatch + " not found. \n";
        }
        msg += "<" + username + ">: ";
        sendMsg(client, msg);
    }
    else if (command == "mail")
    {
        string mailTo = tokens[1];
        string header = "";
        for (auto it : tokens)
        {
            if (it == mailTo || it == command)
            {
                continue;
            }
            header += it;
            header += " ";
        }
        string userFrom = socket_user_map[client];
        User &user = allUsersInfo[userFrom];
        user.mailTo = mailTo;
        user.setIsSendingMsg();
        int randomId = generateRandomNumber();
        // Mail mail = Mail(randomId, userFrom, "", "unread", getTimeNow(), header);
        // mail.to = mailTo;
        string msg = "Enter the message below. After you're done send a new line with only '.' to send the mail\n";
        user.draftHeader = header;
        cout << "Drft header:: " << user.draftHeader << endl;
        sendMsg(client, msg);
    }
    else if (command == "." && tokens.size() == 1)
    {
        // send the mail now
        User &user = allUsersInfo[socket_user_map[client]];
        cout << "Draft: " << user.draftMessage << endl;
        //
        int randomId = generateRandomNumber();
        string userFrom = socket_user_map[client];
        cout << "From: " << userFrom << endl;
        Mail mail = Mail(randomId, userFrom, user.draftMessage, "unread", getTimeNow(), user.draftHeader);
        // Mail &mail = user.draft;
        // mail.setMsg(draftMsg);
        cout << "Email details before send: " << endl;
        cout << "ID: " << to_string(mail.id) << endl;
        cout << "Get header: " << mail.header << endl;
        cout << "Time: " << mail.time << endl;

        int toClient = user_socket_map[user.mailTo];
        cout << "User.mailto: " << user.mailTo << endl;
        User &toUser = allUsersInfo[user.mailTo];

        cout << "TO: " << toUser.getUsername();

        string msg = "You have received a new mail from: " + user.getUsername() + "\n" + mail.getHeaders() + "\t" + mail.getTime();
        vector<Mail> userMails = allUserMails[toUser.username];
        // vector<Mail> userMails = toUser.getMail();
        userMails.push_back(mail);
        allUserMails[toUser.username] = userMails;
        // toUser.setMail(userMails);

        sendMsg(toClient, msg);
        sendEmptyMsg(toClient);

        string userMsg = "Mail sent!";
        sendMsg(client, userMsg);
        sendEmptyMsg(client);

        draftMsg = "";
        user.setIsSendingMsg();
    }
    else if (command == "info")
    {
        string info = "";
        for (auto it : tokens)
        {
            if (it == command)
                continue;
            info += it + " ";
        }
        User &user = allUsersInfo[socket_user_map[client]];

        user.info = info;
        string msg = "Info updated!";
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "passwd")
    {
        string username = socket_user_map[client];
        User &user = allUsersInfo[username];
        user.setPassword(tokens[1]);
        string msg = "Password changed successfully! \n <" + username + ">: ";
        sendMsg(client, msg);
        sendEmptyMsg(client);
    }
    else if (command == "help" || command == "?")
    {
        cout << "Command is help!!\n\n";
        sendMsg(client, manual);
        sendEmptyMsg(client);
    }
    else if (is_empty_msg && !user.getIsSendingMsg())
    {
        oss << "<" << user_name << ">:";
        string msg = oss.str();
        sendMsg(client, msg);
    }
    else if (
        command == "A1" || command == "A2" || command == "A3" ||
        command == "B1" || command == "B2" || command == "B3" ||
        command == "C1" || command == "C2" || command == "C3"

    )
    {
        string currentUser = socket_user_map[client];
        User &user = allUsersInfo[currentUser];
        string msg = "";
        TicTacToe &game = all_games[user.currentGameId];
        string user1 = game.user1;
        string user2 = game.user2;
        User &usr1 = allUsersInfo[user1];
        // cout << "User1 playing is "<< usr1.getUsername() << " "<<usr1.getIsPlaying() << endl;

        User &usr2 = allUsersInfo[user2];
        // cout << "User2 playing is "<< usr2.getUsername() << " "<<usr2.getIsPlaying() << endl;

        if (!user.getIsPlaying())
        {
            msg += "Invalid command! You are not currently playing a game";
            sendMsg(client, msg);
            sendEmptyMsg(client);
            return;
        }
        cout << "Next move is " << game.next_move << endl;
        cout << "current user name is " << currentUser << endl;

        if (game.next_move != currentUser)
        {
            string msg1 = "It is not your turn. Wait for your turn";
            sendMsg(client, msg1);
            sendEmptyMsg(client);
            return;
        }

        int indexToUpdate = getIndex(tokens[0]);
        if (game.board[indexToUpdate] != " ")
        {
            string errorMsg = "You cannot make this move. It has already been made before! Try another move";
            sendMsg(client, errorMsg);
            sendEmptyMsg(client);
            return;
        }

        string userToMove = user1;
        int opponentClient = user_socket_map[userToMove];

        time_t currentTime = time(0);
        double time_diff = difftime(currentTime, game.previousMoveTime);
        game.previousMoveTime = currentTime;

        double rem_tim = 0.0;
        if (currentUser == user1)
        {

            cout << "============User to move changed...........\n";
            userToMove = user2;
            opponentClient = user_socket_map[userToMove];

            if (time_diff > game.user1Time)
            {
                cout << "Your timer has already been expired....You lost the game...from if";

                cout << "USR2 Client: " << client;

                string mmssgg = currentUser + " time has been expired\n";

                sendMsg(client, mmssgg);
                // call fuction to end the game
                gameWon(user2, game, user_socket_map[user2], client, usr1, usr2);
                // gameWon(userToMove, game, opponentClient, client, usr2, usr1);
                return;
            }
            else
            {
                game.user1Time = game.user1Time - time_diff;
                rem_tim = game.user2Time;
            }
        }
        else
        {
            //
            if (time_diff > game.user2Time)
            {
                cout << "Your timer has already been expired....You lost the game...from else";

                string mmssgg = currentUser + " time has been expired\n";
                sendMsg(client, mmssgg);
                gameWon(user1, game, user_socket_map[user1], client, usr2, usr1);
                // gameWon(userToMove, game, client, opponentClient, usr2, usr1);
                return;
                // call fuction to end the game
            }
            else
            {
                game.user2Time = game.user2Time - time_diff;
                rem_tim = game.user1Time;
            }
        }

        game.board[indexToUpdate] = user.moveName;

        bool isGameWon = game.checkGameWon(user.moveName);
        bool isGameDraw = true;
        for (auto it : game.board)
        {
            // loop through the game board. If it finds any empty slot game can still continue
            if (it == " ")
            {
                isGameDraw = false;
                break;
            }
        }
        cout << "game won is " << isGameWon << endl;
        string boardOnly = "\nboard: ";
        for (auto it : game.board)
        {
            boardOnly += it + ",";
        }
        boardOnly.pop_back();
        boardOnly += "\n";
        if (isGameWon)
        {
            gameWon(currentUser, game, client, opponentClient, usr1, usr2);
            sendMsg(client, boardOnly);
            sendMsg(opponentClient, boardOnly);
            sendEmptyMsg(client);
            sendEmptyMsg(opponentClient);
            return;
        }
        else if (isGameDraw)
        {
            string msg2 = "\nGame has ended in a draw!";
            cout << msg << endl;
            msg += game.displayBoard();
            sendMsg(client, msg2);
            sendMsg(opponentClient, msg2);
            sendEmptyMsg(client);
            sendEmptyMsg(opponentClient);
            for (auto it : game.observerSet)
            {
                sendMsg(it, msg2);
                sendEmptyMsg(it);
            }
            usr1.setPoints(usr1.getPoints() + 1);
            usr1.setDraw(usr1.getDraw() + 1);
            usr2.setPoints(usr2.getPoints() + 1);
            usr2.setDraw(usr2.getDraw() + 1);
            usr1.setIsPlaying(false);
            usr1.opponent = "";

            usr2.setIsPlaying(false);
            usr2.opponent = "";

            usr2.setTotalGames(usr2.getTotalGames() + 1);
            usr1.setTotalGames(usr1.getTotalGames() + 1);

            sendMsg(opponentClient, boardOnly);
            sendEmptyMsg(opponentClient);
            sendMsg(client, boardOnly);
            sendEmptyMsg(client);

            cout << "\n\nErase aghai..." << endl;
            all_games.erase(usr1.currentGameId);
            cout << "\n\nErase paxi..." << endl;
            updateRank();
            saveAllData();
            return;
        }
        else
        {
            string message = "\nUser to move: " + userToMove + "\n";
            message = message +userToMove+ " remaining time: "+ to_string(rem_tim) + "\n";

            string updatedBoard = game.displayBoard();
            message += "\n" + updatedBoard;

            sendMsg(client, message);
            sendMsg(client, boardOnly);
            sendMsg(opponentClient, boardOnly);

            sendEmptyMsg(client);
            sendMsg(opponentClient, message);
            sendEmptyMsg(opponentClient);
        }
        // update the next move
        game.next_move = game.next_move == user1 ? user2 : user1;

        // send update to all the observers too
        string msg3 = "\n" + currentUser + " moved: " + command + "\n";
        msg3 += game.displayBoard();
        for (auto it : game.observerSet)
        {
            sendMsg(it, msg3);
            sendEmptyMsg(it);
        }
    }
    else
    {
        cout << "In last else\n\n"
             << endl;
        User &user = allUsersInfo[socket_user_map[client]];
        if (user.getIsSendingMsg())
        {
            // do nothing. user is typing the message
            cout << "Reeived data: " << received_data << endl;
            user.draftMessage += received_data;
            cout << "draft: " << user.draftMessage << endl;
        }
        else
        {
            string msg = "Command not suppported!!";
            sendMsg(client, msg);
            sendEmptyMsg(client);
        }
    }
}

void GameServer::gameWon(std::string &user_name, TicTacToe &game, int &client, int &opponentClient, User &usr1, User &usr2)
{
    string msg = "\n" + user_name + " has won the game =============== \n";
    cout << msg << endl;
    msg += game.displayBoard();
    sendMsg(client, msg);
    sendMsg(opponentClient, msg);
    sendEmptyMsg(client);
    sendEmptyMsg(opponentClient);

    for (auto it : game.observerSet)
    {
        sendMsg(it, msg);
        sendEmptyMsg(it);
    }
    if (user_name == usr1.getUsername())
    {
        // Usr1 has won
        usr1.setWins(usr1.getWins() + 1);
        usr1.setPoints(usr1.getPoints() + 3);
        usr2.setLoss(usr2.getLoss() + 1);
    }
    else
    {
        // Usr2 has won
        usr2.setWins(usr2.getWins() + 1);
        usr2.setPoints(usr2.getPoints() + 3);
        usr1.setLoss(usr1.getLoss() + 1);
    }
    usr1.setIsPlaying(false);
    usr1.opponent = "";

    usr2.setIsPlaying(false);
    usr2.opponent = "";

    usr2.setTotalGames(usr2.getTotalGames() + 1);
    usr1.setTotalGames(usr1.getTotalGames() + 1);

    cout << "\n\nErase aghai..." << endl;
    all_games.erase(usr1.currentGameId);
    cout << "\n\nErase paxi..." << endl;

    updateRank();
    saveAllData();
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
                     "\tThe only command that you can use is 'register username password' and 'quit/exit'.\n";
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

    if (active_guests.find(client) != active_guests.end())
    {
        active_guests.erase(client);
    }

    else if (not_logged_in.find(client) != not_logged_in.end())
    {
        not_logged_in.erase(client);
    }
    // User shoud be logged user
    else
    {
        string user = socket_user_map[client];
        socket_user_map.erase(client);
        user_socket_map.erase(user);
    }

    close(client);
    // Clear it from the socket
    FD_CLR(client, &all_sockets);
    cout << "Closed, AC size: " << active_connections.size() << endl;
    maxfd = getMaxSet(active_connections);
    cout << "successfully exited: " << endl;
}

void GameServer::handleConnectionError(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void GameServer::sendMsg(int &client, string &msg)
{
    if (active_guests.find(client) != active_guests.end())
    {
        msg += "<guest: >";
    }
    if (send(client, msg.c_str(), msg.length(), 0) < 0)
    {
        std::cerr << "Send failed" << std::endl;
    }
    cout << "sent.." << endl;
}

void GameServer::sendEmptyMsg(int &client)
{
    string username = socket_user_map[client];
    if (username == "")
    {
        username = "guest";
    }
    string msg = "\n<" + username + ">: ";
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

// vector<string> GameServer::displayAllUsers() {} // who

// vector<string> GameServer::displayAllGames() {}

// User GameServer::getUser() {} // stats [name]

// User GameServer::getUser(string username) {} // stats [name]

int GameServer::registerUser(string username, string password, bool isGuest)
{
    if (isGuest == false)
    {
        cout << "Only guest can register as new user" << endl;
        return 0;
    }
    User usr = User();
    std::string filename = "users/" + username + ".txt";
    std::ofstream userFile(filename);
    if (!userFile.is_open())
    {
        std::cerr << "Error creating user file." << std::endl;
        return 0;
    }
    // Write user data to file
    userFile << "username: " << username << "\n";
    userFile << "password: " << password << "\n";
    userFile << "isPlaying: 0\n";
    userFile << "wins: 0\n";
    userFile << "loss: 0\n";
    userFile << "draw: 0\n";
    userFile << "message: {}\n";
    userFile << "mail: {}\n";
    userFile << "quietMode: 0\n";
    userFile << "blockList: {}\n";
    userFile << "rank: 0\n";
    userFile << "points: 0\n";
    userFile << "totalGames: 0\n";
    userFile.close();

    // usr.setUsername(username);
    // usr.setPassword(password);
    usr.username = username;
    usr.password = password;
    allUserMails[username] = std::vector<Mail>();
    allUserMessages[username] = std::vector<Message>();
    cout << "===========Laamoooo===========" << endl;
    all_users.insert(username);
    cout << "===========Laamoooo===========111111====" << username << endl;
    allUsersInfo.insert(make_pair(username, usr));
    cout << "===========Laamoooo===========222222====" << endl;

    // cout << "Is online: " << isUserOnline(username) << endl;
    cout << "Aba ta register vayo hai" << endl;
    return 1;
    // return usr.registerUser(username, password);
}

// bool GameServer::loginUser(string username, string password) {}

bool GameServer::isUserOnline(string &user)
{
    if (user_socket_map.find(user) != user_socket_map.end())
    {
        return true;
    }
    return false;
}

bool GameServer::isUserRegistered(string &user)
{
    if (all_users.find(user) != all_users.end())
    {
        return true;
    }
    return false;
}

string putQuotes(string msg)
{
    return "\"" + msg + "\"";
}

void GameServer::saveAllData()
{
    for (auto &userPair : allUsersInfo)
    {
        string username = userPair.first;
        User &user = userPair.second;

        // Construct the file path for the user's file
        std::string filePath = "users/" + username + ".txt";

        // Open the file for writing
        std::ofstream outputFile(filePath, ofstream::trunc);

        // Check if the file was opened successfully
        if (outputFile.is_open())
        {
            // Write user data to the file
            outputFile << "username: " << user.getUsername() << std::endl;
            outputFile << "password: " << user.getPassword() << std::endl;
            outputFile << "wins: " << user.getWins() << endl;
            outputFile << "loss: " << user.getLoss() << endl;
            outputFile << "draw: " << user.getDraw() << endl;
            outputFile << "isPlaying: " << user.getIsPlaying() << endl;
            string messages = "";
            // Message::Message(const std::__1::string &from, const std::__1::string &msg, const std::__1::string &status, const std::__1::string &time)
            // for (Message it : user.getMessages())
            for (Message it : allUserMessages[user.username])
            {
                cout << "put quotes" << putQuotes(it.from) << endl;
                messages += "{" + putQuotes(it.from) + "," + putQuotes(it.msg) + "," + putQuotes(it.status) + "," + putQuotes(it.time) + "},";
            }
            string msgToSave = "{}";
            if (!messages.empty())
            {
                messages.pop_back();
                msgToSave = "{" + messages + "}";
            }
            cout << "msg to save is :" << msgToSave << endl;

            outputFile << "message: " << msgToSave << endl;

            string mails = "";
            // int m_id, std::__1::string m_from, std::__1::string m_msg, std::__1::string m_status, std::__1::string m_time, std::__1::string m_header)
            // for (auto it : user.getMail())
            for (auto it : allUserMails[user.username])
            {
                cout << "it msg" << it.msg << endl;
                // string msgr = it.msg.replace('\n',' ');
                it.msg.replace(it.msg.begin(), it.msg.end(), '\n', ' ');
                mails += "{" + to_string(it.id) + "," + putQuotes(it.from) + "," + putQuotes(it.msg) + "," + putQuotes(it.status) + "," + putQuotes(it.time) + "," + putQuotes(it.header) + "},";
            }
            string mailToSave = "{}";
            if (!mails.empty())
            {
                mails.pop_back();
                mailToSave = "{" + mails + "}";
            }
            cout << "mail to save is :" << mailToSave << endl;

            outputFile << "mail: " << mailToSave << endl;
            outputFile << "quietMode: " << user.getQuietMode() << endl;
            string blockedUsers = "";
            for (auto b : user.blockListSet)
            {
                blockedUsers += putQuotes(b) + ",";
            }
            string blockedUsersToSave = "{}";
            if (!blockedUsers.empty())
            {
                blockedUsers.pop_back();
                blockedUsersToSave = "{" + blockedUsers + "}";
            }
            cout << "block to save is :" << blockedUsersToSave << endl;
            outputFile << "blockList: " << blockedUsersToSave << endl;
            outputFile << "rank: " << user.getRank() << endl;
            outputFile << "points: " << user.getPoints() << endl;
            outputFile << "totalGames: " << user.getTotalGames() << endl;

            // Close the file
            outputFile.close();
        }
        else
        {
            // Handle file open error
            std::cerr << "Error opening file: " << filePath << std::endl;
        }
    }
    cout << "\n\nAll data pani save vayo hai\n\n";
}

void GameServer::updateRank()
{
    std::vector<std::pair<int, std::string>> userPoints;

    for (auto &userPair : allUsersInfo)
    {
        userPoints.emplace_back(userPair.second.getPoints(), userPair.first);
    }

    std::sort(userPoints.begin(), userPoints.end(), std::greater<>());

    int rank = 1;
    for (const auto &[points, username] : userPoints)
    {
        allUsersInfo[username].setRank(rank++);
    }

    std::string filePath = "rank.txt";
    std::ofstream outputFile(filePath, std::ofstream::trunc);
    if (outputFile.is_open())
    {
        for (const auto &[points, username] : userPoints)
        {
            outputFile << "Username: " << username << ", Points: " << points << ", Rank: " << allUsersInfo[username].getRank() << std::endl;
        }
        outputFile.close();
    }
    else
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    cout << "\n\nRank pani save vayo hai\n\n";
}