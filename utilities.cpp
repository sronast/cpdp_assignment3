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

bool isItemInSet(string &item, unordered_set<string> &mySet){
    if (mySet.find(item) != mySet.end())
    {
        return true;
    }
    return false;
}

int generateRandomNumber() {
    int min=0;
    int max=9999;
    // Get the current time as a timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // Use the timestamp as a seed for the random number engine
    std::mt19937 gen(static_cast<unsigned>(timestamp));

    // Create a uniform distribution for the range
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate a random number
    return distribution(gen);
}