#ifndef PIECE_H
#define PIECE_H

#include "position.h"
#include <memory>

class Board; // Forward declaration

class Piece {
protected:
    bool white;      // true for white pieces, false for black
    char symbol;     // P=pawn, R=rook, N=knight, B=bishop, Q=queen, K=king

public:
    Piece(bool isWhite, char sym) : white(isWhite), symbol(sym) {}
    virtual ~Piece() = default;
    
    bool isWhite() const { return white; }
    char getSymbol() const { return symbol; }
    
    virtual bool isValidMove(const Position& from, const Position& to, const Board& board) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;  // Add this line
};

class Pawn : public Piece {
private:
    bool hasMoved;

public:
    Pawn(bool isWhite) : Piece(isWhite, 'P'), hasMoved(false) {}
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Pawn>(*this); }
};

class Rook : public Piece {
    private:
        bool _hasMoved;
    public:
        Rook(bool isWhite) : Piece(isWhite, 'R') {}
        bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
        bool hasMoved() const { return _hasMoved; }
        std::unique_ptr<Piece> clone() const override { return std::make_unique<Rook>(*this); }
};

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite, 'N') {}
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Knight>(*this); }
};

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite, 'B') {}
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Bishop>(*this); }
};

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite, 'Q') {}
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Queen>(*this); }
};

class King : public Piece {
private:
    bool _hasMoved;

public:
    King(bool isWhite) : Piece(isWhite, 'K'), _hasMoved(false) {}
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    bool hasMoved() const { return _hasMoved; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<King>(*this); }
};

#endif // PIECE_H