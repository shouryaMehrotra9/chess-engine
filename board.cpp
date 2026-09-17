#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include "board.h"
#include "pawn.h"
#include "knight.h"
#include "rook.h"

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

// Helper to parse a square like "e2" into row (6) and col (4)
static bool parse_square(const std::string& sq, int& row, int& col) {
    if (sq.length() < 2) return false;
    char file = static_cast<char>(tolower(static_cast<unsigned char>(sq[0])));
    char rank = sq[1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return false;
    }
    col = file - 'a';
    row = '8' - rank;
    return true;
}

// Helper to parse user input like "e2 e4" or "e2e4"
static bool parse_user_move(const std::string& line, Move& move) {
    std::istringstream iss(line);
    std::string first, second;
    iss >> first;
    if (first.empty()) return false;

    // Two tokens: e.g. "e2 e4"
    if (iss >> second) {
        int fr, fc, tr, tc;
        if (!parse_square(first, fr, fc) || !parse_square(second, tr, tc)) {
            return false;
        }
        move = Move(fr, fc, tr, tc);
        return true;
    }

    // Single token 4 characters: e.g. "e2e4"
    if (first.length() == 4) {
        int fr, fc, tr, tc;
        if (!parse_square(first.substr(0, 2), fr, fc) || !parse_square(first.substr(2, 2), tr, tc)) {
            return false;
        }
        move = Move(fr, fc, tr, tc);
        return true;
    }

    return false;
}

// Validates whether the move is legal for the active player's turn
static bool is_valid_turn_move(const Move& move, const Board& board, bool is_white_turn) {
    if (!board.is_inside(move.from_row, move.from_col) || !board.is_inside(move.to_row, move.to_col)) {
        std::cout << "[!] Move is outside the board (a1 to h8).\n";
        return false;
    }

    if (move.from_row == move.to_row && move.from_col == move.to_col) {
        std::cout << "[!] Start and destination square cannot be the same.\n";
        return false;
    }

    const piece& p = board.grid[move.from_row][move.from_col];
    if (p.empty()) {
        std::cout << "[!] No piece at " << Move::square_to_algebraic(move.from_row, move.from_col) << ".\n";
        return false;
    }

    if (p.is_white != is_white_turn) {
        std::cout << "[!] That piece belongs to " << (p.is_white ? "White" : "Black")
                  << ", but it is " << (is_white_turn ? "White" : "Black") << "'s turn!\n";
        return false;
    }

    char type = static_cast<char>(toupper(static_cast<unsigned char>(p.type)));
    bool valid = false;

    if (type == 'P') {
        valid = is_valid_pawn_move(move, board);
    } else if (type == 'N') {
        valid = is_valid_knight_move(move, board);
    } else if (type == 'R') {
        valid = is_valid_rook_move(move, board);
    } else {
        // Fallback for pieces whose move generation is not implemented yet (B, Q, K)
        const piece& dest = board.grid[move.to_row][move.to_col];
        if (dest.empty() || dest.is_white != p.is_white) {
            std::cout << "[i] Note: full move rules for '" << p.symbol() << "' not yet written. Move allowed.\n";
            valid = true;
        }
    }

    if (!valid) {
        std::cout << "[!] Illegal move for " << (p.is_white ? "White" : "Black") << " " << p.symbol() << "!\n";
    }

    return valid;
}

// Display all recorded moves in standard numbered history format
static void print_move_history(const std::vector<Move>& history) {
    if (history.empty()) {
        std::cout << "\nNo moves have been recorded yet.\n\n";
        return;
    }

    std::cout << "\n================ Recorded Move History ================\n";
    for (size_t i = 0; i < history.size(); i++) {
        if (i % 2 == 0) {
            std::cout << std::setw(3) << (i / 2 + 1) << ". White: " << history[i].to_string();
        } else {
            std::cout << "   |   Black: " << history[i].to_string() << "\n";
        }
    }
    if (history.size() % 2 != 0) {
        std::cout << "\n";
    }
    std::cout << "=======================================================\n\n";
}

int main() {
    Board board;
    bool is_white_turn = true;
    std::vector<Move> move_history;

    std::cout << "=======================================================\n";
    std::cout << "              INTERACTIVE CHESS ENGINE                 \n";
    std::cout << "=======================================================\n";
    std::cout << "Commands:\n";
    std::cout << " - Enter moves like: 'e2 e4' or 'e2e4'\n";
    std::cout << " - Type 'history' to view all recorded moves\n";
    std::cout << " - Type 'board'   to re-print the board\n";
    std::cout << " - Type 'quit'    to exit the game\n";
    std::cout << "=======================================================\n";

    board.print();

    while (true) {
        int turn_number = static_cast<int>(move_history.size() / 2 + 1);
        std::cout << "[Turn " << turn_number << "] "
                  << (is_white_turn ? "WHITE's turn: " : "BLACK's turn: ");

        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }

        // Trim leading whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            continue;
        }
        line = line.substr(start);

        // Remove trailing carriage returns/spaces
        size_t end = line.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
        }

        if (line == "quit" || line == "exit") {
            std::cout << "Game exited by user.\n";
            break;
        }

        if (line == "history") {
            print_move_history(move_history);
            continue;
        }

        if (line == "board") {
            board.print();
            continue;
        }

        Move move;
        if (!parse_user_move(line, move)) {
            std::cout << "[!] Invalid input format. Use 'e2 e4' or 'e2e4'.\n\n";
            continue;
        }

        if (!is_valid_turn_move(move, board, is_white_turn)) {
            std::cout << "Please try a different move.\n\n";
            continue;
        }

        // Execute the move
        board.make_move(move);

        // Record the move in history
        move_history.push_back(move);
        std::cout << ">> Move recorded: " << (is_white_turn ? "White" : "Black")
                  << " played " << move.to_string() << "\n";

        // Display updated board
        board.print();

        // Alternate player turn
        is_white_turn = !is_white_turn;
    }

    print_move_history(move_history);
    return 0;
}
