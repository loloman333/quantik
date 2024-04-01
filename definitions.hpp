// Includes

#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <map>
#include <cctype>
#include <unordered_map>
#include <cstring>

// Namespace useages
using 
    std::string, 
    std::cout,
    std::endl,
    std::map,
    std::unordered_map,
    std::vector;

struct State;

// Typedefs
typedef uint64_t encoding;
typedef unordered_map<encoding, State> state_map;
typedef unordered_map<PieceType, vector<std::pair<char, char>>> pieces_map; 

// Predefs
// TODO: add more / all?
struct State;
enum class PieceType;

// Enums
enum class SwapType { ROWS, COLUMNS };
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

PieceType get_counterpart_piece(PieceType piece)
{
    if (piece == PieceType::EMPTY) return piece;

    bool is_white = ((int) piece % 2) == 1;
    PieceType counterpart = (PieceType) (is_white ? (int) piece + 1 : (int) piece - 1);
    
    return counterpart;
}

// Globals

bool DEBUG = false;

unordered_map<PieceType, string> _piece_type_to_str
{
    {PieceType::EMPTY, "-"},
    {PieceType::WHITE_SQUARE, "■"},
    {PieceType::BLACK_SQUARE, "□"},
    {PieceType::WHITE_TRIANGLE, "▲"},
    {PieceType::BLACK_TRIANGLE, "△"},
    {PieceType::WHITE_CIRCLE, "●"},
    {PieceType::BLACK_CIRCLE, "◯"},
    {PieceType::WHITE_DIAMOND, "◆"},
    {PieceType::BLACK_DIAMOND, "◇"},
};
unordered_map<std::string, PieceType> _piece_str_to_type {
    {"-", PieceType::EMPTY},
    {"■", PieceType::WHITE_SQUARE},
    {"□", PieceType::BLACK_SQUARE},
    {"▲", PieceType::WHITE_TRIANGLE},
    {"△", PieceType::BLACK_TRIANGLE},
    {"●", PieceType::WHITE_CIRCLE},
    {"◯", PieceType::BLACK_CIRCLE},
    {"◆", PieceType::WHITE_DIAMOND},
    {"◇", PieceType::BLACK_DIAMOND}
};

vector<PieceType> _piece_types
{ 
    PieceType::WHITE_SQUARE, 
    PieceType::BLACK_SQUARE, 
    PieceType::WHITE_TRIANGLE, 
    PieceType::BLACK_TRIANGLE, 
    PieceType::WHITE_CIRCLE, 
    PieceType::BLACK_CIRCLE, 
    PieceType::WHITE_DIAMOND, 
    PieceType::BLACK_DIAMOND
};