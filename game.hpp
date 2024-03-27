#ifndef GAME_HPP
#define GAME_HPP

#include<iostream>
#include<vector>
#include<map>
#include<string>

using namespace std;

class TicTacToe {
public:
    std::vector<std::vector<char>> board;
    const int boardSize = 3;  
    std::map<char, int> columnMap{{'A', 0}, {'B', 1}, {'C', 2}}; 
    string user1;
    string user2;
    string next_move;

    TicTacToe(string usr1, string usr2){
        user1 = usr1;
        user2 = usr2;
        next_move = usr1;
        
    };

    void resetBoard();

    void displayBoard() const;

    bool makeMove(char column, int row, char symbol);

    bool checkGameWon(char symbol) const;
};

#endif