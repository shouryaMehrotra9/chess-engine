#pragma once
#include <vector>
#include "board.h"

// Generate all legal/pseudo-legal moves for a pawn at (row, col) on the board
std::vector<Move> get_pawn_moves(int row, int col, const Board& board);

// Validate if moving a pawn from (from_row, from_col) to (to_row, to_col) is legal
bool is_valid_pawn_move(const Move& move, const Board& board);
