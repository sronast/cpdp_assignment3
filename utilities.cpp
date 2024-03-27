#include "utilities.hpp"

int getRandomInt(){
    return rand();
}

string trim(const string &str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos || end == string::npos)
    {
        return ""; // String contains only whitespace
    }

    return str.substr(start, end - start + 1);
}
int getMaxSet(unordered_set<int> &mySet){
    int maxElement = *(mySet.begin()); // Initialize maxElement with the first element

    for (int elem : mySet) {
        if (elem > maxElement) {
            maxElement = elem;
        }
    }
    return maxElement;
}

vector<string> tokenize(string &data, const char &delimeter)
{
    vector<string> tokens;
    stringstream iss(data);
    string token;

    while (getline(iss, token, delimeter))
    {
        if (!token.empty())
        {
            // cout<<"Token: "<<token;
            token = trim(token);
            tokens.push_back(token);
        }
    }
    return tokens;
}