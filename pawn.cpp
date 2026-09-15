#include <vector>
#include "pawn.h"

// Helper function to add a move, taking pawn promotion into account
static void add_move_with_promotions(int from_r, int from_c, int to_r, int to_c, int promo_row, std::vector<Move>& moves) {
    if (to_r == promo_row) {
        // Pawn reached the other side: promote to Queen, Rook, Bishop, or Knight
        moves.push_back(Move(from_r, from_c, to_r, to_c, 'Q'));
        moves.push_back(Move(from_r, from_c, to_r, to_c, 'R'));
        moves.push_back(Move(from_r, from_c, to_r, to_c, 'B'));
        moves.push_back(Move(from_r, from_c, to_r, to_c, 'N'));
    } else {
        moves.push_back(Move(from_r, from_c, to_r, to_c, ' '));
    }
}

std::vector<Move> get_pawn_moves(int row, int col, const Board& board) {
    std::vector<Move> moves;

    // 1. Boundary check
    if (!board.is_inside(row, col)) {
        return moves;
    }

    const piece& p = board.grid[row][col];

    // 2. Ensure this piece is actually a pawn
    if (p.empty() || toupper(static_cast<unsigned char>(p.type)) != 'P') {
        return moves;
    }

    // 3. Direction and initial ranks:
    // White pawns move UP the board (decreasing row index: -1) starting at row 6 (rank 2)
    // Black pawns move DOWN the board (increasing row index: +1) starting at row 1 (rank 7)
    int direction = p.is_white ? -1 : 1;
    int start_row = p.is_white ? 6 : 1;
    int promo_row = p.is_white ? 0 : 7;

    // ----------------------------------------------------
    // Rule A: Single Step Forward
    // ----------------------------------------------------
    int one_step_row = row + direction;
    if (board.is_inside(one_step_row, col) && board.grid[one_step_row][col].empty()) {
        add_move_with_promotions(row, col, one_step_row, col, promo_row, moves);

        // ----------------------------------------------------
        // Rule B: Two Steps Forward (only from starting rank)
        // ----------------------------------------------------
        int two_step_row = row + (2 * direction);
        if (row == start_row && board.is_inside(two_step_row, col) && board.grid[two_step_row][col].empty()) {
            moves.push_back(Move(row, col, two_step_row, col, ' '));
        }
    }

    // ----------------------------------------------------
    // Rule C: Diagonal Captures (left and right)
    // ----------------------------------------------------
    int capture_cols[2] = {col - 1, col + 1};
    for (int next_col : capture_cols) {
        if (board.is_inside(one_step_row, next_col)) {
            const piece& target = board.grid[one_step_row][next_col];
            // Must have an enemy piece on the diagonal
            if (!target.empty() && target.is_white != p.is_white) {
                add_move_with_promotions(row, col, one_step_row, next_col, promo_row, moves);
            }
        }
    }

    // ----------------------------------------------------
    // Rule D: En Passant (can be added here when tracking en-passant square)
    // ----------------------------------------------------

    return moves;
}

bool is_valid_pawn_move(const Move& move, const Board& board) {
    std::vector<Move> legal_moves = get_pawn_moves(move.from_row, move.from_col, board);
    for (const auto& m : legal_moves) {
        if (m.to_row == move.to_row && m.to_col == move.to_col) {
            return true;
        }
    }
    return false;
}
