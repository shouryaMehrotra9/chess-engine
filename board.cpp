#include <iostream>
#include <iomanip>
#include "board.h"
#include "pawn.h"

Board::Board() {
    reset();
}

void Board::reset() {
    // Standard initial chess setup
    piece initial[8][8] = {
        {piece('R', false), piece('N', false), piece('B', false), piece('Q', false), piece('K', false), piece('B', false), piece('N', false), piece('R', false)},
        {piece('P', false), piece('P', false), piece('P', false), piece('P', false), piece('P', false), piece('P', false), piece('P', false), piece('P', false)},
        {piece(), piece(), piece(), piece(), piece(), piece(), piece(), piece()},
        {piece(), piece(), piece(), piece(), piece(), piece(), piece(), piece()},
        {piece(), piece(), piece(), piece(), piece(), piece(), piece(), piece()},
        {piece(), piece(), piece(), piece(), piece(), piece(), piece(), piece()},
        {piece('P', true), piece('P', true), piece('P', true), piece('P', true), piece('P', true), piece('P', true), piece('P', true), piece('P', true)},
        {piece('R', true), piece('N', true), piece('B', true), piece('Q', true), piece('K', true), piece('B', true), piece('N', true), piece('R', true)}
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = initial[i][j];
        }
    }
}

bool Board::is_inside(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

void Board::print() const {
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "  +-----------------+\n";
    for (int i = 0; i < 8; i++) {
        std::cout << (8 - i) << " | ";
        for (int j = 0; j < 8; j++) {
            std::cout << grid[i][j].symbol() << " ";
        }
        std::cout << "| " << (8 - i) << "\n";
    }
    std::cout << "  +-----------------+\n";
    std::cout << "    a b c d e f g h\n\n";
}

bool Board::make_move(const Move& m) {
    if (!is_inside(m.from_row, m.from_col) || !is_inside(m.to_row, m.to_col)) {
        return false;
    }

    piece p = grid[m.from_row][m.from_col];
    grid[m.from_row][m.from_col] = piece(); // empty old square

    // Handle promotion if applicable
    if (m.promotion != ' ') {
        grid[m.to_row][m.to_col] = piece(m.promotion, p.is_white);
    } else {
        grid[m.to_row][m.to_col] = p;
    }

    return true;
}

int main() {
    Board board;

    std::cout << "=== Initial Chess Board ===" << std::endl;
    board.print();

    // Demonstrate pawn moves for White pawn at e2 (row 6, col 4)
    std::cout << "Available moves for White pawn at e2 (row 6, col 4):" << std::endl;
    std::vector<Move> white_moves = get_pawn_moves(6, 4, board);
    for (const auto& m : white_moves) {
        std::cout << " - " << m.to_string() << std::endl;
    }

    // Demonstrate pawn moves for Black pawn at d7 (row 1, col 3)
    std::cout << "\nAvailable moves for Black pawn at d7 (row 1, col 3):" << std::endl;
    std::vector<Move> black_moves = get_pawn_moves(1, 3, board);
    for (const auto& m : black_moves) {
        std::cout << " - " << m.to_string() << std::endl;
    }

    // Play 1. e2 -> e4
    if (!white_moves.empty()) {
        Move e2_e4 = white_moves.back(); // 2-step jump
        std::cout << "\nPlaying move: " << e2_e4.to_string() << std::endl;
        board.make_move(e2_e4);
        board.print();
    }

    return 0;
}
