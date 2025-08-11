#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>

class Game {
public:
    Game();
    
    // Main game loop
    void play();
    
    // Display current board state
    void displayBoard() const;

    bool executePlayerMove();
    
    // Process a move in algebraic notation (e.g., "e2e4")
    bool makeMove(const std::string& moveStr);
    
    // Check game state
    bool isGameOver() const;
    std::string getGameResult() const;
    
    // Get current player's turn
    bool isWhiteTurn() const { return board.isWhiteTurn; }
    
 private:
    Board board;
    
    // Helper methods
    bool parseMove(const std::string& moveStr, Position& from, Position& to);
    Position stringToPosition(const std::string& pos) const;
    std::string positionToString(const Position& pos) const;
    bool isValidMoveString(const std::string& moveStr) const;
    void handlePawnPromotion(const Position& pos);
    
    // Input validation
    bool isValidPositionString(const std::string& pos) const;
    bool isValidFile(char file) const;
    bool isValidRank(char rank) const;
};

#endif // GAME_H