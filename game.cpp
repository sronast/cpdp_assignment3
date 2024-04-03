#include "game.hpp"
#include "utilities.hpp"

TicTacToe::TicTacToe() {}
TicTacToe::TicTacToe(string usr1, string usr2)
{
    resetBoard();
    user1 = usr1;
    user2 = usr2;
    next_move = usr1;
    id = generateRandomNumber();
    cout << "Game Id cpppp: " << id << endl;
}
TicTacToe::TicTacToe(const TicTacToe &other)
{
    user1 = other.user1;
    user2 = other.user2;
    user1Time = other.user1Time;
    user2Time = other.user2Time;
    next_move = other.next_move;
    board = other.board;
    boardSize = other.boardSize;
    id = other.id;
    columnMap = other.columnMap;
    observers = other.observers;
    comments = other.comments;
    observerSet = other.observerSet;
}

void TicTacToe::resetBoard()
{
    board = {" ", " ", " ", " ", " ", " ", " ", " ", " "};
}


std::string TicTacToe::displayBoard() const
{
    std::string boardStr;
    boardStr += "  1   2   3\n";
    boardStr += "A ";
    boardStr += board[0];
    boardStr += " | ";
    boardStr += board[1];
    boardStr += " | ";
    boardStr += board[2];
    boardStr += "\n";
    boardStr += " ---|---|---\n";
    boardStr += "B ";
    boardStr += board[3];
    boardStr += " | ";
    boardStr += board[4];
    boardStr += " | ";
    boardStr += board[5];
    boardStr += "\n";
    boardStr += " ---|---|---\n";
    boardStr += "C ";
    boardStr += board[6];
    boardStr += " | ";
    boardStr += board[7];
    boardStr += " | ";
    boardStr += board[8];
    boardStr += "\n";

    return boardStr;
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

bool TicTacToe::checkGameWon(string symbol)
{
    // Check rows and columns
    for (int i = 0; i < 3; ++i)
    {
        if ((board[i] == symbol && board[i + 3] == symbol && board[i + 6] == symbol) ||           // Check columns
            (board[i * 3] == symbol && board[i * 3 + 1] == symbol && board[i * 3 + 2] == symbol)) // Check rows
        {
            return true; // Found a winning pattern
        }
    }

    // Check diagonals
    if ((board[0] == symbol && board[4] == symbol && board[8] == symbol) || // Top-left to bottom-right
        (board[2] == symbol && board[4] == symbol && board[6] == symbol))   // Top-right to bottom-left
    {
        return true; // Found a winning pattern
    }

    return false; // No winning pattern found
}
TicTacToe::~TicTacToe() {cout<<"Destructor game...\n";}

// TicTacToe& TicTacToe::operator=(const TicTacToe& other)
// {
//     if (this != &other) // Check for self-assignment
//     {
//         // Copy the board
//         for (int i = 0; i < boardSize; ++i)
//         {
//             for (int j = 0; j < boardSize; ++j)
//             {
//                 board[i][j] = other.board[i][j];
//             }
//         }

//         // Copy other members
//         user1 = other.user1;
//         user2 = other.user2;
//         next_move = other.next_move;
//         id = other.id;
//     }
//     return *this;
// }