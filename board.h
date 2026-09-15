#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

// Represents a single chess piece on the board
class piece {
public:
    char type;      // 'P' (Pawn), 'R' (Rook), 'N' (Knight), 'B' (Bishop), 'Q' (Queen), 'K' (King), ' ' (Empty)
    bool is_white;  // true for white, false for black

    piece(char t = ' ', bool w = false) : type(t), is_white(w) {}

    bool empty() const {
        return type == ' ';
    }

    char symbol() const {
        if (empty()) return '.';
        return is_white ? static_cast<char>(toupper(static_cast<unsigned char>(type)))
                        : static_cast<char>(tolower(static_cast<unsigned char>(type)));
    }
};

// Represents a chess move from one square to another
struct Move {
    int from_row, from_col;
    int to_row, to_col;
    char promotion; // ' ' if no promotion, or 'Q', 'R', 'B', 'N'

    Move(int fr = 0, int fc = 0, int tr = 0, int tc = 0, char promo = ' ')
        : from_row(fr), from_col(fc), to_row(tr), to_col(tc), promotion(promo) {}

    // Helper to format coordinates to algebraic notation (e.g. row 6, col 4 -> "e2")
    static std::string square_to_algebraic(int r, int c) {
        char file = static_cast<char>('a' + c);
        char rank = static_cast<char>('8' - r);
        return std::string(1, file) + rank;
    }

    std::string to_string() const {
        std::string res = square_to_algebraic(from_row, from_col) + " -> " + square_to_algebraic(to_row, to_col);
        if (promotion != ' ') {
            res += " (=";
            res += promotion;
            res += ")";
        }
        return res;
    }
};

// Represents the 8x8 chess board
class Board {
public:
    piece grid[8][8];

    Board();
    void reset();
    bool is_inside(int r, int c) const;
    void print() const;
    bool make_move(const Move& m);
};