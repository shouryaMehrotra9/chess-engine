#include <vector>
#include <cctype>
#include <iostream>
#include "rook.h"

std::vector<Move> get_rook_moves(int row, int col, const Board& board) {
    std::vector<Move> moves;

    // Boundary check
    if (!board.is_inside(row, col)) {
        return moves;
    }

    const piece& p = board.grid[row][col];

    // Ensure the piece is a rook
    if (p.empty() || toupper(static_cast<unsigned char>(p.type)) != 'R') {
        return moves;
    }

    // 1. Move RIGHT (increasing column)
    for (int j = col + 1; j <= 7; j++) {
        const piece& target = board.grid[row][j];
        if (target.empty()) {
            moves.push_back(Move(row, col, row, j));
        } else {
            if (target.is_white != p.is_white) {
                moves.push_back(Move(row, col, row, j)); // capture
            }
            break; // blocked
        }
    }

    // 2. Move LEFT (decreasing column)
    for (int j = col - 1; j >= 0; j--) {
        const piece& target = board.grid[row][j];
        if (target.empty()) {
            moves.push_back(Move(row, col, row, j));
        } else {
            if (target.is_white != p.is_white) {
                moves.push_back(Move(row, col, row, j)); // capture
            }
            break; // blocked
        }
    }

    // 3. Move DOWN (increasing row)
    for (int i = row + 1; i <= 7; i++) {
        const piece& target = board.grid[i][col];
        if (target.empty()) {
            moves.push_back(Move(row, col, i, col));
        } else {
            if (target.is_white != p.is_white) {
                moves.push_back(Move(row, col, i, col)); // capture
            }
            break; // blocked
        }
    }

    // 4. Move UP (decreasing row)
    for (int i = row - 1; i >= 0; i--) {
        const piece& target = board.grid[i][col];
        if (target.empty()) {
            moves.push_back(Move(row, col, i, col));
        } else {
            if (target.is_white != p.is_white) {
                moves.push_back(Move(row, col, i, col)); // capture
            }
            break; // blocked
        }
    }

    return moves;
}

bool is_valid_rook_move(const Move& move, const Board& board) {
    std::vector<Move> legal_moves = get_rook_moves(move.from_row, move.from_col, board);
    for (const auto& m : legal_moves) {
        if (m.to_row == move.to_row && m.to_col == move.to_col) {
            return true;
        }
    }
    return false;
}