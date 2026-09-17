#pragma once
#include <vector>
#include "board.h"

std::vector<Move> get_rook_moves(int row, int col, const Board& board);
bool is_valid_rook_move(const Move& move, const Board& board);