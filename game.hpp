#ifndef GAME_HPP
#define GAME_HPP

#include<iostream>
#include<vector>
#include<map>

class TicTacToe {
private:
    std::vector<std::vector<char>> board;
    const int boardSize = 3;  
    std::map<char, int> columnMap{{'A', 0}, {'B', 1}, {'C', 2}}; 

public:
    TicTacToe();
    void resetBoard();

    void displayBoard() const;

    bool makeMove(char column, int row, char symbol);

    bool checkGameWon(char symbol) const;
};

#endif