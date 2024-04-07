#pragma once

#include "quantik.hpp"

class PieceManager
{

    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    // --- Methods ---

    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Constructors ---

    // --- Methods ---

    // -------------
    // Static Stuff
    // -------------

    // --- Members ---
    // TODO rename those
    static unordered_map<PieceType, string> _piece_type_to_str;
    static unordered_map<std::string, PieceType> _piece_str_to_type;
    static vector<PieceType> _piece_types;

    // --- Methods ---
    static PieceType get_counterpart_piece(PieceType piece);
    static bool is_black(PieceType piece);
};