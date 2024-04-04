#ifndef UTILITIES_HPP
#define UTILITIES_HPP


#include <iostream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include<set>
#include <chrono>
#include <random>
#include <ctime>


using namespace std;


int getRandomInt();
vector<string> tokenize(string &command, const char &delimeter);
string trim(const string& str);
int getMaxSet(set<int> &mySet);
bool isItemInSet(string &item, set<string> &mySet);
int generateRandomNumber();

#endif
// gameWon(user2, game, user_socket_map[user2], client, usr1, usr2);
//                 gameWon(user1, game, user_socket_map[user1], client, usr2, usr1);