#include "Move.hpp"
#include "PieceManager.hpp"

Move::Move(char row, char col, PieceType piece)
{
    this->row_index = row;
    this->col_index = col;
    this->piece = piece;
}

string Move::get_print_string() 
{
    string result = 
        std::to_string(this->row_index) + 
        std::to_string(this->col_index) +
        PieceManager::_piece_type_to_str[this->piece];
    return result;
}

vector<Move> Move::all_black_moves{};
vector<Move> Move::all_white_moves{};

void Move::compute_all_moves()
{
    for (char row = 0; row < 4; row++)
    {
        for (char col = 0; col < 4; col++)
        {
            for (PieceType type : PieceManager::_piece_types)
            {
                bool is_black = ((int) type % 2) == 0;
                if (is_black) Move::all_black_moves.push_back(Move{row, col, type});
                else Move::all_white_moves.push_back(Move{row, col, type});
            }
        }
    }
};

vector<Move>& Move::get_all_moves(bool for_black)
{
    vector<Move>& moves = for_black ? Move::all_black_moves : Move::all_white_moves;
    if (moves.size() == 0) Move::compute_all_moves();
    return moves;
}