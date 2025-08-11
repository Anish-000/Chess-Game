#include <iostream>
#include <Windows.h>
#include "Game.h"
#include "Board.h"

int main() {

    try {
        // Create and start the chess game
        Game game;
        
        // Main game loop
        while (!game.isGameOver()) {
            // Display the current board state
            game.displayBoard();
            
            // Show whose turn it is
            std::cout << (game.isWhiteTurn() ? "White" : "Black") << "'s turn." << std::endl;
            
            // Get and execute the player's move
            if (!game.executePlayerMove()) {
                std::cout << "Invalid move. Please try again." << std::endl;
                continue;
            }
        }
        
        // Display final board state
        game.displayBoard();
        
        // Show game result
        // if (_board.isCheckmate(_board.isWhiteTurn())) {
        //     std::cout << (!game.isWhiteTurn() ? "White" : "Black") << " wins by checkmate!" << std::endl;
        // } else if (_board.isStalemate(_board.isWhiteTurn())) {
        //     std::cout << "Game ends in stalemate!" << std::endl;
        // }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}