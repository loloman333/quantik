#pragma once

#include "quantik.hpp"

class Move
{
    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Members ---

    char row_index{};
    char col_index{};
    PieceType piece{};

    // --- Constructors ---

    Move(char row, char col, PieceType piece);

    // --- Methods ---

    string get_print_string();

    // -------------
    // Static Stuff
    // -------------
    private:

    static vector<Move> all_black_moves;
    static vector<Move> all_white_moves;
    static void compute_all_moves();
    
    public: 
    
    static vector<Move>& get_all_moves(bool for_black);
    
};
