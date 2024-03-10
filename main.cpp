#include <string>
#include <iostream>
#include "classes.cpp"
#include "game.cpp"

using namespace std;

int main(){
    GameServer gameServer;

    // Register a new user
    // User newUser = gameServer.registerUser("saransh", "password123", true);

    TicTacToe game;
    cout << "Enter commands like A1, C2 to make a move" << endl;
    cout << "Initial State of the Board\n";
    game.displayBoard();

    string userInput;
    char column;
    int row;
    char symbol;

    int moveNumber = 0;
    string currentPlayer = "Player 1";
    while (true) {
        ++moveNumber;

        cout << currentPlayer << " turn. Move number: " << moveNumber << ". ";
        cin >> userInput;

        // Validate the input format
        if (userInput.length() != 2) {
            cout << "Invalid input format. Try again.\n";
            --moveNumber; 
            continue;
        }

        column = std::toupper(userInput[0]); 
        row = userInput[1] - '0'; 

        if (column < 'A' || column > 'C' || row < 1 || row > 3) {
            cout << "Invalid column or row. Try again.\n";
            --moveNumber; 
            continue;
        }

        symbol = (moveNumber % 2 == 1) ? 'X' : 'O';

        if (!game.makeMove(column, row, symbol)) {
            cout << "Invalid move. Try again.\n";
            --moveNumber; 
            continue;
        }
        game.displayBoard();
        if (game.checkGameWon('X')) {
            cout << "Player 1 has won the game \n";
            break;
        } else if (game.checkGameWon('O')) {
            cout << "Player 2 has won the game!\n";
            break;
        } else if (moveNumber == 9) {
            cout << "It's a draw!\n";
            break;
        }
    }


    return 0;
}