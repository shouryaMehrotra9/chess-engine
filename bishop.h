#pragma once
#include<vector>
#include "board.h"
std::vector<Move> get_bishop_moves(int row, int col, const Board& board);
bool is_valid_bishop_move(const Move& move, const Board& board);