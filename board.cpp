#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>
using namespace std;

class piece {
public:
    char type;
    bool is_white;

    piece(char t = ' ', bool w = false) : type(t), is_white(w) {}

    bool empty() const {
        return type == ' ';
    }

    char symbol() const {
        if (empty())
            return '.';
        return is_white ? toupper(static_cast<unsigned char>(type)) : tolower(static_cast<unsigned char>(type));
    }
};

int main() {
    piece board[8][8] = {
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
            cout << board[i][j].symbol() << " ";
        }
        cout << endl;
    }

    return 0;
}



