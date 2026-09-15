#include <vector>
#include <cctype>
#include "knight.h"

std::vector<Move> get_knight_moves(int row, int col, const Board& board) {
    std::vector<Move> moves;

    // boundary check
    if (!board.is_inside(row, col)) {
        return moves;
    }

    const piece& p = board.grid[row][col];

    // ensure the piece is a knight
    if (p.empty() || toupper(static_cast<unsigned char>(p.type)) != 'N') {
        return moves;
    }

    // Check all squares in a 5x5 window centered on the knight
    for (int i = row - 2; i <= row + 2; i++) {
        for (int j = col - 2; j <= col + 2; j++) {
            int dr = i - row;
            int dc = j - col;

            // A valid knight leap satisfies dr^2 + dc^2 == 5 (e.g. (+-1, +-2) or (+-2, +-1))
            if (dr * dr + dc * dc == 5) {
                if (board.is_inside(i, j)) {
                    const piece& target = board.grid[i][j];
                    // Can move to empty square or capture enemy piece
                    if (target.empty() || target.is_white != p.is_white) {
                        moves.push_back(Move(row, col, i, j, ' '));
                    }
                }
            }
        }
    }

    return moves;
}

bool is_valid_knight_move(const Move& move, const Board& board) {
    std::vector<Move> legal_moves = get_knight_moves(move.from_row, move.from_col, board);
    for (const auto& m : legal_moves) {
        if (m.to_row == move.to_row && m.to_col == move.to_col) {
            return true;
        }
    }
    return false;
}
