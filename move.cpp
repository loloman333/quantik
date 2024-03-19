#include "definitions.hpp"

struct Move
{
    Move(char row, char col, PieceType piece)
    {
        this->row_index = row;
        this->col_index = col;
        this->piece = piece;
    }

    // Members

    char row_index;
    char col_index;
    PieceType piece;

    // Methods

    string get_print_string() { return Move::get_print_string(*this); }
    static string get_print_string(Move &move)
    {
        string result = 
            std::to_string(move.row_index) + 
            std::to_string(move.col_index) +
            _pieces[(int) move.piece];
        return result;
    }
};