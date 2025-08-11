#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "position.h"
#include <memory>
#include <vector>
#include <string>

class Board {
private:
    static const int BOARD_SIZE = 8;
    std::unique_ptr<Piece> board[BOARD_SIZE][BOARD_SIZE];
    
    Position lastMove[2];  // Store last move's [from, to] positions for en passant

    bool isPathClear(const Position& from, const Position& to) const;
    void movePiece(const Position& from, const Position& to);
    bool wouldBeInCheck(const Position& from, const Position& to, bool isWhite) const;

public:
    bool isWhiteTurn;

    Board();
    Board(const Board& other);
    ~Board() = default;

    // Disable copying to prevent multiple boards
    // Board(const Board&) = delete;
    // Board& operator=(const Board&) = delete;

    bool isValidPosition(const Position& pos) const;

    // Core game functions
    bool makeMove(const Position& from, const Position& to);
    bool isValidMove(const Position& from, const Position& to) const;
    bool isInCheck(bool isWhite) const;
    bool isCheckmate(bool isWhite) const;
    bool isStalemate(bool isWhite) const;

    // Special moves
    bool canCastle(const Position& from, const Position& to) const;
    bool isEnPassantMove(const Position& from, const Position& to) const;
    void promotePawn(const Position& pos, char promotionPiece);

    // Getters
    const Piece* getPiece(const Position& pos) const;
    bool isWhitesTurn() const { return isWhiteTurn; }
    std::vector<Position> getValidMoves(const Position& pos) const;

    // Board representation
    std::string toString() const;
};

#endif // BOARD_H