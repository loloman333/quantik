#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <map>
#include <cctype>

using 
    std::string, 
    std::cout,
    std::endl,
    std::map,
    std::vector;

enum class SwapType { ROWS, COLUMNS };

/*
    0 -> empty
    1 -> ■
    2 -> □
    3 -> ▲
    4 -> △
    5 -> ●
    6 -> ◯
    7 -> ◆
    8 -> ◇
*/
string pieces[9] = {"-", "■", "□", "▲", "△", "●", "◯", "◆", "◇"};

string get_piece_from_index(char i)
{
    return pieces[i];
}

char get_index_from_piece(string piece)
{
    char i = 0;
    for (string p : pieces)
    {
        if (p == piece)
        {
            return i;
        }
        i++;
    }
    assert(false);
}
