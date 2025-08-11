#include "Game.h"
#include <iostream>
#include <cctype>
#include <string>

Game::Game() {}

void Game::play() {
    std::string moveStr;
    displayBoard();

    while (!isGameOver()) {
        std::cout << (board.isWhiteTurn ? "White" : "Black") << "'s turn.\n";
        std::cout << "Enter move (e.g., e2e4): ";
        std::getline(std::cin, moveStr);

        if (moveStr == "quit") {
            break;
        }

        if (!makeMove(moveStr)) {
            std::cout << "Invalid move! Try again.\n";
            continue;
        }

        displayBoard();

        if (board.isInCheck(board.isWhiteTurn)) {
            std::cout << "Check!\n";
        }
    }

    std::cout << "Game Over! " << getGameResult() << "\n";
}

void Game::displayBoard() const {
    std::cout << board.toString() << std::endl;
}

bool Game::executePlayerMove() {
    std::string move;
    std::cout << "Enter move (e.g. e2e4): ";
    std::cin >> move;
    
    if (move.length() != 4) {
        return false;
    }
    
    // Convert input to positions
    Position from(move[1] - '1', move[0] - 'a');
    Position to(move[3] - '1', move[2] - 'a');
    
    // Try to execute the move
    return board.makeMove(from, to);
}

bool Game::makeMove(const std::string& moveStr) {
    if (!isValidMoveString(moveStr)) {
        return false;
    }

    Position from, to;
    if (!parseMove(moveStr, from, to)) {
        return false;
    }

    return board.makeMove(from, to);
}

bool Game::isGameOver() const {
    if (board.isCheckmate(board.isWhiteTurn)) { 
        std::cout << (!board.isWhiteTurn ? "White" : "Black") << " wins by checkmate!" << std::endl; 
        return true;
    } else if (board.isStalemate(board.isWhiteTurn)) { 
        std::cout << "Game ends in stalemate!" << std::endl; 
        return true;
    } 
    return false;
}

std::string Game::getGameResult() const {
    if (board.isCheckmate(board.isWhiteTurn)) {
        return (board.isWhiteTurn ? "Black" : "White") + std::string(" wins by checkmate!");
    }
    if (board.isStalemate(board.isWhiteTurn)) {
        return "Game is drawn by stalemate.";
    }
    return "Game in progress.";
}

bool Game::parseMove(const std::string& moveStr, Position& from, Position& to) {
    if (moveStr.length() != 4) {
        return false;
    }

    from = stringToPosition(moveStr.substr(0, 2));
    to = stringToPosition(moveStr.substr(2, 2));

    return from.row != -1 && from.col != -1 && 
           to.row != -1 && to.col != -1;
}

Position Game::stringToPosition(const std::string& pos) const {
    if (!isValidPositionString(pos)) {
        return Position(-1, -1);
    }

    int col = pos[0] - 'a';
    int row = pos[1] - '1';

    return Position(row, col);
}

std::string Game::positionToString(const Position& pos) const {
    if (!board.isValidPosition(pos)) {
        return "";
    }

    std::string result;
    result += static_cast<char>('a' + pos.col);
    result += static_cast<char>('1' + pos.row);
    return result;
}

bool Game::isValidMoveString(const std::string& moveStr) const {
    return moveStr.length() == 4 && 
           isValidPositionString(moveStr.substr(0, 2)) && 
           isValidPositionString(moveStr.substr(2, 2));
}

bool Game::isValidPositionString(const std::string& pos) const {
    return pos.length() == 2 && 
           isValidFile(pos[0]) && 
           isValidRank(pos[1]);
}

bool Game::isValidFile(char file) const {
    return file >= 'a' && file <= 'h';
}

bool Game::isValidRank(char rank) const {
    return rank >= '1' && rank <= '8';
}

void Game::handlePawnPromotion(const Position& pos) {
    // Default promotion to Queen is handled in Board class
    board.promotePawn(pos, 'Q');
}