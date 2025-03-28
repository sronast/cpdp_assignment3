#include "classes.hpp"

// Message

// Message Message::createNewMessage(string from, string to, string msg)
// {
//     Message message = Message();
//     // message.id = "m_" + to_string(getRandomInt());
//     message.from = from;
//     // message.to = to;
//     message.msg = msg;
//     message.status = "init";
//     message.time = "";
//     return message;
// }

// Constructor
User::User() :username(""),password(""),wins(0), loss(0), draw(0), isPlaying(0),\
  quietMode(0), rank(0), points(0), totalGames(0), isSendingMessage(0)\
  , currentGameId(0), moveName(""), gameObserving(0), info(""), draftMessage(""),\
  draftHeader(""), mailTo(""){
    blockListSet;
  }
User::User(const User& other) {
    username = other.username;
    password = other.password;
    wins = other.wins;
    loss = other.loss;
    draw = other.draw;
    isPlaying = other.isPlaying;
    quietMode = other.quietMode;
    rank = other.rank;
    points = other.points;
    totalGames = other.totalGames;
    isSendingMessage = other.isSendingMessage;
    opponent = other.opponent;
    currentGameId = other.currentGameId;
    moveName = other.moveName;
    gameObserving = other.gameObserving;
    info = other.info;

    draftMessage = other.draftMessage;
    draftHeader = other.draftHeader;
    mailTo = other.mailTo;
    blockListSet = other.blockListSet;
}

User::~User() {
    cout << "destructing..."<<endl;
    // No dynamic memory allocation in this class, so nothing to delete
}
// Getters and setters
void User::setUsername(const std::string &username) { this->username = username; }
std::string User::getUsername() const { return username; }

void User::setPassword(const std::string &password) { this->password = password; }
std::string User::getPassword() const { return password; }

void User::setWins(int wins) { this->wins = wins; }
int User::getWins() const { return wins; }

void User::setLoss(int loss) { this->loss = loss; }
int User::getLoss() const { return loss; }

void User::setIsSendingMsg()
{
    isSendingMessage = !isSendingMessage;
}
int User::getIsSendingMsg() const
{
    return isSendingMessage;
}

void User::setDraw(int draw) { this->draw = draw; }
int User::getDraw() const { return draw; }

void User::setIsPlaying(int isPlaying) { 

    this->isPlaying = isPlaying; }
int User::getIsPlaying() const { return isPlaying; }

// void User::setMail(const std::vector<Mail> &mail) { this->mail = mail; }
// std::vector<Mail> User::getMail() const { return mail; }

void User::setQuietMode(int quietMode) { 

    this->quietMode = quietMode; }
int User::getQuietMode() const { return quietMode; }


void User::setRank(int rank) { this->rank = rank; }
int User::getRank() const { return rank; }

void User::setPoints(int points) { this->points = points; }
int User::getPoints() const { return points; }

void User::setTotalGames(int totalGames) { this->totalGames = totalGames; }
int User::getTotalGames() const { return totalGames; }

std::vector<Message> User::parseMessage(const std::string &messageStr)
{
    std::vector<Message> messages;
    if (messageStr.front() != '{' || messageStr.back() != '}')
    {
        return messages;
    }
    std::string messagesContent = messageStr.substr(1, messageStr.size() - 2);
    // cout << "message content" << messagesContent << endl;
    if (messagesContent.empty()) return {};
    messagesContent = messagesContent.substr(1);
    messagesContent.pop_back();
    std::stringstream ss(messagesContent);
    std::string item;
    std::vector<std::string> parts;
    while (std::getline(ss, item, ','))
    {
        item.erase(0, item.find_first_not_of(" \t\n\r\f\v"));
        item.erase(item.find_last_not_of(" \t\n\r\f\v") + 1);

        if (item.front() != '"' || item.back() != '"')
        {
            continue;
        }
        std::string msg = item.substr(1, item.size() - 2);
        parts.push_back(msg);
    }
    if (parts.size() == 4)
    {
        messages.push_back(Message(parts[0], parts[3], parts[2], parts[1]));
    }
    return messages;
}

std::vector<Mail> User::parseMail(const std::string &mailStr)
{
    std::vector<Mail> mails = {};
    if (mailStr.front() != '{' || mailStr.back() != '}')
    {
        cout << "Black hole" << endl;
        return mails;
    }
    std::string mailContent = mailStr.substr(1, mailStr.size() - 2);
    if(mailContent.empty()){
        return mails;
    }
    mailContent = mailContent.substr(1);
    mailContent.pop_back();

    cout << "MC: "<<mailContent << endl;

    std::stringstream ss(mailContent);
    std::string item;
    std::vector<std::string> parts;
    while (std::getline(ss, item, ','))
    {
        item.erase(0, item.find_first_not_of(" \t\n\r\f\v"));
        item.erase(item.find_last_not_of(" \t\n\r\f\v") + 1);

        if (item.front() != '"' || item.back() != '"')
        {
            continue;
        }
        std::string msg = item.substr(1, item.size() - 2);
        parts.push_back(msg);
    }
    if (parts.size() == 6)
    {

        mails.push_back(Mail(stoi(parts[0]), parts[1], parts[5], parts[3], parts[2], parts[4]));
    }
    return mails;
}
std::vector<std::string> User::parseBlockList(const std::string &blockListStr)
{
    return {blockListStr};
}
// Constructor
Message::Message(const std::string &from, const std::string &msg, const std::string &status, const std::string &time)
    : from(from), msg(msg), status(status), time(time) {}

// Getter and setter for 'from' field
Message::~Message() {
    cout << "destructing msg..."<<endl;
    // No dynamic memory allocation in this class, so nothing to delete
}
std::string Message::getFrom() const
{
    return from;
}

void Message::setFrom(const std::string &from)
{
    this->from = from;
}

// Getter and setter for 'msg' field
std::string Message::getMsg() const
{
    return msg;
}

void Message::setMsg(const std::string &msg)
{
    this->msg = msg;
}

// Getter and setter for 'status' field
std::string Message::getStatus() const
{
    return status;
}

void Message::setStatus(const std::string &status)
{
    this->status = status;
}

// Getter and setter for 'time' field
std::string Message::getTime() const
{
    return time;
}

void Message::setTime(const std::string &time)
{
    this->time = time;
}

Mail::Mail(){}

Mail::Mail(int m_id,  string m_from,  string m_msg,  string m_status,  string m_time, string m_header) {
    // randomId, userFrom, user.draftMessage, "unread", getTimeNow(), user.draftHeader
    
    id = m_id;
    from = m_from;
    msg = m_msg;
    status = m_status;
    time = m_time;
    header = m_header;
    cout<<"In mail constructor....\n\n";
}

Mail::~Mail() {
    cout << "destructing mail..."<<endl;
    // No dynamic memory allocation in this class, so nothing to delete
}
// Getter and setter for 'from' field
std::string Mail::getFrom() const
{
    return from;
}

void Mail::setFrom(const std::string &from)
{
    this->from = from;
}

// Getter and setter for 'msg' field
std::string Mail::getMsg() const
{
    return msg;
}

void Mail::setMsg(const std::string &msg)
{
    this->msg = msg;
}

// Getter and setter for 'status' field
std::string Mail::getStatus() const
{
    return status;
}

void Mail::setStatus(const std::string &status)
{
    this->status = status;
}

// Getter and setter for 'time' field
std::string Mail::getTime() const
{
    return time;
}

void Mail::setTime(const std::string &time)
{
    this->time = time;
}

std::string Mail::getHeaders() const
{
    return header;
}
void Mail::setHeader(const std::string &header)
{
    this->header = header;
}

void Mail::setId(const int &id)
{
    this->id = id;
}
int Mail::getId() const
{
    return id;
}

// Ratings
void Ratings::updateRanking(string userId)
{
    // read from the file and update the ranking
}



// Game::
