#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <map>
#include <cctype>
#include <unordered_map>
#include <cstring>

using 
    std::string, 
    std::cout,
    std::endl,
    std::map,
    std::unordered_map,
    std::vector;

struct State;

typedef uint64_t encoding;
typedef unordered_map<encoding, State> state_map;

enum class SwapType { ROWS, COLUMNS };

bool DEBUG = false;

enum class PieceType {
    EMPTY, 
    WHITE_SQUARE, 
    BLACK_SQUARE, 
    WHITE_TRIANGLE, 
    BLACK_TRIANGLE, 
    WHITE_CIRCLE, 
    BLACK_CIRCLE, 
    WHITE_DIAMOND, 
    BLACK_DIAMOND
};
string _pieces[9] = {"-", "■", "□", "▲", "△", "●", "◯", "◆", "◇"};

string get_piece_from_index(char i)
{
    return _pieces[i];
}

char get_index_from_piece(string piece)
{
    char i = 0;
    for (string p : _pieces)
    {
        if (p == piece)
        {
            return i;
        }
        i++;
    }
    assert(false);
}
