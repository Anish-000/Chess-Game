#ifndef POSITION_H
#define POSITION_H

struct Position {
    int row;    // 0-7 for ranks 1-8
    int col;    // 0-7 for files a-h
    
    // Default constructor
    Position() : row(0), col(0) {}
    
    // Constructor with parameters
    Position(int r, int c) : row(r), col(c) {}
    
    // Equality operators
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
    
    // Check if position is within board bounds
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

#endif // POSITION_H