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