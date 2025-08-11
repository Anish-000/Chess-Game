#include "Piece.h"
#include "Board.h"
#include <cstdlib>

// Helper function to check if path is clear (for pieces that move in straight lines)
bool isPathClear(const Position& from, const Position& to, const Board& board) {
    int rowStep = (to.row - from.row) ? (to.row - from.row) / abs(to.row - from.row) : 0;
    int colStep = (to.col - from.col) ? (to.col - from.col) / abs(to.col - from.col) : 0;
    
    Position current(from.row + rowStep, from.col + colStep);
    while (current != to) {
        if (board.getPiece(current) != nullptr) return false;
        current.row += rowStep;
        current.col += colStep;
    }
    return true;
}

// Pawn movement
bool Pawn::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int direction = white ? 1 : -1;
    int rowDiff = to.row - from.row;
    int colDiff = abs(to.col - from.col);
    
    // Forward movement
    if (colDiff == 0) {
        // Single square forward
        if (rowDiff == direction && !board.getPiece(to)) return true;
        
        // Two squares forward from starting position
        if (!hasMoved && rowDiff == 2 * direction && 
            !board.getPiece(to) && 
            !board.getPiece(Position(from.row + direction, from.col))) {
            return true;
        }
    }
    // Capture diagonally
    else if (colDiff == 1 && rowDiff == direction) {
        const auto& targetPiece = board.getPiece(to);
        return targetPiece && targetPiece->isWhite() != white;
    }
    
    return false;
}

// Rook movement
bool Rook::isValidMove(const Position& from, const Position& to, const Board& board) const {
    if (from.row != to.row && from.col != to.col) return false;
    
    const auto& targetPiece = board.getPiece(to);
    if (targetPiece && targetPiece->isWhite() == white) return false;
    
    return isPathClear(from, to, board);
}

// Knight movement
bool Knight::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) return false;
    
    const auto& targetPiece = board.getPiece(to);
    return !targetPiece || targetPiece->isWhite() != white;
}

// Bishop movement
bool Bishop::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    if (rowDiff != colDiff) return false;
    
    const auto& targetPiece = board.getPiece(to);
    if (targetPiece && targetPiece->isWhite() == white) return false;
    
    return isPathClear(from, to, board);
}

// Queen movement
bool Queen::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    // Move like rook or bishop
    if (!((from.row == to.row || from.col == to.col) || (rowDiff == colDiff))) return false;
    
    const auto& targetPiece = board.getPiece(to);
    if (targetPiece && targetPiece->isWhite() == white) return false;
    
    return isPathClear(from, to, board);
}

// King movement
bool King::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = abs(to.row - from.row);
    int colDiff = abs(to.col - from.col);
    
    // Normal one square movement
    if (rowDiff <= 1 && colDiff <= 1) {
        const auto& targetPiece = board.getPiece(to);
        return !targetPiece || targetPiece->isWhite() != white;
    }
    
    // Castling
    if (!hasMoved() && rowDiff == 0 && colDiff == 2) {
        // Check if it's a valid castling move (implemented in Board class)
        return board.canCastle(from, to);
    }
    
    return false;
}