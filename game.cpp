#include "game.hpp"



TicTacToe::TicTacToe()
{
    resetBoard();
}

void TicTacToe::resetBoard()
{
    board = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
}

void TicTacToe::displayBoard() const
{
    std::cout << "  A   B   C\n";
    for (int i = 0; i < boardSize; ++i)
    {
        std::cout << i + 1 << " ";
        for (int j = 0; j < boardSize; ++j)
        {
            std::cout << board[i][j];
            if (j < boardSize - 1)
            {
                std::cout << " | ";
            }
        }
        std::cout << "\n";
        if (i < boardSize - 1)
        {
            std::cout << " ---|---|---\n";
        }
    }
}

bool TicTacToe::makeMove(char column, int row, char symbol)
{
    if (row < 1 || row > boardSize || columnMap.find(column) == columnMap.end())
    {
        std::cout << "Invalid move!\n";
        return false;
    }

    int columnIndex = columnMap[column];
    int rowIndex = row - 1;

    if (board[rowIndex][columnIndex] != ' ')
    {
        std::cout << "Cell already occupied!\n";
        return false;
    }

    board[rowIndex][columnIndex] = symbol;
    return true;
}

bool TicTacToe::checkGameWon(char symbol) const
{
    for (int i = 0; i < boardSize; ++i)
    {
        if ((board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) ||
            (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol))
        {
            return true;
        }
    }

    if ((board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) ||
        (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol))
    {
        return true;
    }

    return false;
}