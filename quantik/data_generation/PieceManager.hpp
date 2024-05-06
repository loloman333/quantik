#pragma once

#include "quantik.hpp"

class PieceManager
{
    public: 
    // -------------
    // Static Stuff
    // -------------

    // --- Members ---
    static unordered_map<PieceType, string> piece_type_to_str;
    static unordered_map<std::string, PieceType> piece_str_to_type;
    static vector<PieceType> piece_types;

    // --- Methods ---
    static PieceShape get_piece_shape(PieceType piece);
    static PieceType get_counterpart_piece(PieceType piece);
    static bool is_black(PieceType piece);
};