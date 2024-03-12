#include "classes.hpp"

int getRandomInt()
{
    return rand();
}

// Message

Message Message::createNewMessage(string from, string to, string msg)
{
    Message message = Message();
    message.id = "m_" + to_string(getRandomInt());
    message.from = from;
    message.to = to;
    message.msg = msg;
    message.status = "init";
    return message;
}

string User::toJson(User usr)
{
    json jsonObj = {
        {"username", usr.username},
        {"id", usr.id},
        {"win", usr.win},
        {"loss", usr.loss},
        {"isGuest", usr.isGuest},
        {"quietMode", usr.quietMode},
        {"blocklist", accumulate(usr.blockList.begin(), usr.blockList.end(), string())},
        {"password", usr.password}};
    string jsonStr = jsonObj.dump(2);
    return jsonStr;
}
User User::registerUser(string username, string password)
{
    User usr = User();
    usr.id = "u_" + to_string(getRandomInt());
    usr.username = username;
    usr.password = password;
    usr.win = 0;
    usr.loss = 0;
    usr.isGuest = false;
    usr.blockList = {};
    usr.messages = {};

    // save user to file
    string usrJson = toJson(usr);
    ofstream userFile("users.txt", std::ios::app);
    if (userFile.is_open())
    {
        userFile << usrJson;
        userFile << "\n";
    }
    userFile.close();
    return usr;
}

// Ratings
void Ratings::updateRanking(string userId)
{
    // read from the file and update the ranking
}

// Game::



// GameServer
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
