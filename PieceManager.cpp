#include "PieceManager.hpp"

unordered_map<PieceType, string> PieceManager::_piece_type_to_str
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

unordered_map<std::string, PieceType> PieceManager::_piece_str_to_type {
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

vector<PieceType> PieceManager::_piece_types
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

PieceType PieceManager::get_counterpart_piece(PieceType piece)
{
    if (piece == PieceType::EMPTY) return piece;
    PieceType counterpart = (PieceType) (is_black(piece) ? (int) piece - 1 : (int) piece + 1);
    return counterpart;
}

bool PieceManager::is_black(PieceType piece)
{
    return ((int) piece % 2) == 0;
}

