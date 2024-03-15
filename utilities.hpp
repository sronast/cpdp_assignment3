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

using namespace std;


int getRandomInt();
vector<string> tokenize(string &command, const char &delimeter);
string trim(const string& str);

#endif