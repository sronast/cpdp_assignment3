#ifndef GAME_HPP
#define GAME_HPP

#include<iostream>
#include<vector>
#include<map>
#include<string>

#include "utilities.hpp"

using namespace std;

class TicTacToe {
public:
    int id;
    vector<string> board;
    int boardSize = 3;  
    std::map<char, int> columnMap{{'A', 0}, {'B', 1}, {'C', 2}}; 
    vector<string> move_history;
    string user1;
    string user2;
    string next_move;
    vector<int> observers;
    unordered_set<int> observerSet;
    vector<string> comments;
    int user1Time;
    int user2Time;


    TicTacToe();
    TicTacToe(string usr1, string usr2);
    TicTacToe(const TicTacToe& other);
    // TicTacToe& TicTacToe::operator=(TicTacToe& other);

    void resetBoard();

    string displayBoard() const;

    bool makeMove(char column, int row, char symbol);

    bool checkGameWon(string symbol);
};

#endif