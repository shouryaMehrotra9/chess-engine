#pragma once
#include<vector>
#include "board.h"



std::vector<Move> get_knight_moves(int row, int col, const Board& board);
bool is_valid_knight_move(const Move& move, const Board& board);