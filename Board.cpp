#include "Board.h"
#include <stdexcept>
#include <sstream>
#include <string>
#include <cctype>
#include <iostream>

Board::Board() : isWhiteTurn(true) {
    // Initialize empty board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = nullptr;
        }
    }

    // Set up white pieces
    board[0][0] = std::make_unique<Rook>(true);
    board[0][1] = std::make_unique<Knight>(true);
    board[0][2] = std::make_unique<Bishop>(true);
    board[0][3] = std::make_unique<Queen>(true);
    board[0][4] = std::make_unique<King>(true);
    board[0][5] = std::make_unique<Bishop>(true);
    board[0][6] = std::make_unique<Knight>(true);
    board[0][7] = std::make_unique<Rook>(true);
    for (int j = 0; j < BOARD_SIZE; j++) {
        board[1][j] = std::make_unique<Pawn>(true);
    }

    // Set up black pieces
    board[7][0] = std::make_unique<Rook>(false);
    board[7][1] = std::make_unique<Knight>(false);
    board[7][2] = std::make_unique<Bishop>(false);
    board[7][3] = std::make_unique<Queen>(false);
    board[7][4] = std::make_unique<King>(false);
    board[7][5] = std::make_unique<Bishop>(false);
    board[7][6] = std::make_unique<Knight>(false);
    board[7][7] = std::make_unique<Rook>(false);
    for (int j = 0; j < BOARD_SIZE; j++) {
        board[6][j] = std::make_unique<Pawn>(false);
    }
}

Board::Board(const Board& other) : isWhiteTurn(other.isWhiteTurn) {
    // Deep copy the board array
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (other.board[row][col]) {
                board[row][col] = other.board[row][col]->clone();
            } else {
                board[row][col] = nullptr;
            }
        }
    }
}

bool Board::isValidPosition(const Position& pos) const {
    return pos.row >= 0 && pos.row < BOARD_SIZE && 
           pos.col >= 0 && pos.col < BOARD_SIZE;
}

bool Board::isPathClear(const Position& from, const Position& to) const {
    int rowStep = (to.row - from.row) ? (to.row - from.row) / abs(to.row - from.row) : 0;
    int colStep = (to.col - from.col) ? (to.col - from.col) / abs(to.col - from.col) : 0;

    Position current{from.row + rowStep, from.col + colStep};
    while (current.row != to.row || current.col != to.col) {
        if (board[current.row][current.col] != nullptr) {
            return false;
        }
        current.row += rowStep;
        current.col += colStep;
    }
    return true;
}

bool Board::isValidMove(const Position& from, const Position& to) const {
    if (!isValidPosition(from) || !isValidPosition(to)) {
        return false;
    }

    const Piece* piece = board[from.row][from.col].get();
    if (!piece || piece->isWhite() != isWhiteTurn) {
        return false;
    }

    const Piece* targetPiece = board[to.row][to.col].get();
    if (targetPiece && targetPiece->isWhite() == piece->isWhite()) {
        return false;
    }

    if (!piece->isValidMove(from, to, *this)) {
        return false;
    }

    // Special moves check
    if (dynamic_cast<const King*>(piece)) {
        if (abs(to.col - from.col) == 2) {
            return canCastle(from, to);
        }
    } else if (dynamic_cast<const Pawn*>(piece)) {
        if (from.col != to.col && !targetPiece) {
            return isEnPassantMove(from, to);
        }
    }

    return isPathClear(from, to) && !wouldBeInCheck(from, to, piece->isWhite());
}

void Board::movePiece(const Position& from, const Position& to) {
    if (board[from.row][from.col]) {
        // Handle en passant capture
        if (dynamic_cast<Pawn*>(board[from.row][from.col].get()) && 
            from.col != to.col && 
            !board[to.row][to.col]) {
            board[from.row][to.col] = nullptr; // Capture en passant pawn
        }

        // Handle castling
        if (dynamic_cast<King*>(board[from.row][from.col].get()) && 
            abs(to.col - from.col) == 2) {
            int rookFromCol = (to.col > from.col) ? 7 : 0;
            int rookToCol = (to.col > from.col) ? 5 : 3;
            board[to.row][rookToCol] = std::move(board[to.row][rookFromCol]);
        }

        board[to.row][to.col] = std::move(board[from.row][from.col]);
        lastMove[0] = from;
        lastMove[1] = to;
    }
}

bool Board::makeMove(const Position& from, const Position& to) {
    if (!isValidMove(from, to)) {
        return false;
    }

    movePiece(from, to);
    isWhiteTurn = !isWhiteTurn;
    return true;
}

bool Board::isInCheck(bool isWhite) const {
    Position kingPos{-1, -1};

    // Find the king
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const Piece* piece = board[i][j].get();
            if (piece && dynamic_cast<const King*>(piece) && piece->isWhite() == isWhite) {
                kingPos = Position{i, j};
                break;
            }
        }
        if (kingPos.row != -1) break;
    }

    // Check if any opponent piece can capture the king
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const Piece* piece = board[i][j].get();
            if (piece && piece->isWhite() != isWhite) {
                Position from{i, j};
                if (piece->isValidMove(from, kingPos, *this) && isPathClear(from, kingPos)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::wouldBeInCheck(const Position& from, const Position& to, bool isWhite) const {
    // Create a temporary board to simulate the move
    Board tempBoard = *this;
    tempBoard.movePiece(from, to);
    return tempBoard.isInCheck(isWhite);
}

bool Board::canCastle(const Position& from, const Position& to) const {
    const King* king = dynamic_cast<const King*>(board[from.row][from.col].get());
    if (!king || king->hasMoved()) {
        return false;
    }

    int rookCol = (to.col > from.col) ? 7 : 0;
    const Rook* rook = dynamic_cast<const Rook*>(board[from.row][rookCol].get());
    if (!rook || rook->hasMoved()) {
        return false;
    }

    // Check if path is clear and king is not in check during castling
    if (isInCheck(king->isWhite())) {
        return false;
    }

    int step = (to.col > from.col) ? 1 : -1;
    for (int col = from.col + step; col != rookCol; col += step) {
        if (board[from.row][col] != nullptr) {
            return false;
        }
        Position intermediate{from.row, col};
        if (wouldBeInCheck(from, intermediate, king->isWhite())) {
            return false;
        }
    }

    return true;
}

bool Board::isEnPassantMove(const Position& from, const Position& to) const {
    const Pawn* pawn = dynamic_cast<const Pawn*>(board[from.row][from.col].get());
    if (!pawn) return false;

    // Check if the last move was a two-square pawn advance
    const Position& lastFrom = lastMove[0];
    const Position& lastTo = lastMove[1];
    const Piece* lastPiece = board[lastTo.row][lastTo.col].get();

    return lastPiece && dynamic_cast<const Pawn*>(lastPiece) &&
           abs(lastTo.row - lastFrom.row) == 2 &&
           lastTo.col == to.col &&
           abs(from.col - to.col) == 1 &&
           ((pawn->isWhite() && from.row == 4) || (!pawn->isWhite() && from.row == 3));
}

void Board::promotePawn(const Position& pos, char promotionPiece) {
    if (!isValidPosition(pos) || !board[pos.row][pos.col]) {
        return;
    }

    const Pawn* pawn = dynamic_cast<Pawn*>(board[pos.row][pos.col].get());
    if (!pawn) {
        return;
    }

    bool isWhite = pawn->isWhite();
    switch (promotionPiece) {
        case 'Q': board[pos.row][pos.col] = std::make_unique<Queen>(isWhite); break;
        case 'R': board[pos.row][pos.col] = std::make_unique<Rook>(isWhite); break;
        case 'B': board[pos.row][pos.col] = std::make_unique<Bishop>(isWhite); break;
        case 'N': board[pos.row][pos.col] = std::make_unique<Knight>(isWhite); break;
        default: board[pos.row][pos.col] = std::make_unique<Queen>(isWhite); break;
    }
}

const Piece* Board::getPiece(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return nullptr;
    }
    return board[pos.row][pos.col].get();
}

std::vector<Position> Board::getValidMoves(const Position& pos) const {
    std::vector<Position> validMoves;
    if (!isValidPosition(pos) || !board[pos.row][pos.col]) {
        return validMoves;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Position to{i, j};
            if (isValidMove(pos, to)) {
                validMoves.push_back(to);
            }
        }
    }
    return validMoves;
}

std::string Board::toString() const {
    std::stringstream ss;
    printf("\n");
    const std::string files = "  a   b   c   d   e   f   g   h";

    auto printBorder = [&]() {
        ss << "  +---+---+---+---+---+---+---+---+\n";
    };

    ss << "    " << files << "\n";
    printBorder();

    for (int i = BOARD_SIZE - 1; i >= 0; i--) {
        ss << (i + 1) << " |";
        for (int j = 0; j < BOARD_SIZE; j++) {
            const Piece* piece = board[i][j].get();
            char symbol = ' ';
            if (piece) {
                char p = piece->getSymbol();
                symbol = piece->isWhite() ? static_cast<char>(toupper(p)) 
                                          : static_cast<char>(tolower(p));
            } else {
                symbol = '.';
            }
            ss << " " << symbol << " |";
        }
        ss << " " << (i + 1) << "\n";
        printBorder();
    }

    ss << "    " << files << "\n";

    return ss.str();
}

bool Board::isCheckmate(bool isWhite) const {
    if (!isInCheck(isWhite)) {
        return false;
    }

    // Check if any move can get out of check
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const Piece* piece = board[i][j].get();
            if (piece && piece->isWhite() == isWhite) {
                Position from{i, j};
                std::vector<Position> moves = getValidMoves(from);
                if (!moves.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Board::isStalemate(bool isWhite) const {
    if (isInCheck(isWhite)) {
        return false;
    }

    // Check if any legal move exists
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const Piece* piece = board[i][j].get();
            if (piece && piece->isWhite() == isWhite) {
                Position from{i, j};
                std::vector<Position> moves = getValidMoves(from);
                if (!moves.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}