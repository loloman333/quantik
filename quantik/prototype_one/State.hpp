#pragma once

#include "quantik.hpp"

#include "Move.hpp"
#include "CanonicalGenerator.hpp"

class State
{
    friend class CanonicalGenerator;
    
    // -------------
    // Private Stuff
    // -------------
    private: 

    // --- Members ---

    pieces_map placed_pieces{};
    PieceType board[4][4]{};
    bool black_turn{};

    // --- Methods ---

    void update_placed_pieces();

    // -------------
    // Public Stuff
    // -------------
    public: 

    // --- Constructors ---

    State() = default;
    State(const State& other) = default;

    // --- Methods ---

    void make_move(Move move);
    bool is_legal_move(Move& move); 
    bool is_winning_state();

    state_map compute_following_states();

    vector<vector<PieceType*>> get_sectors();

    string get_print_string();

    encoding encode();

    // Transformation functions do not keep "placed_pieces" up to date
    State mirror();
    State rotate_90();
    State rotate_180();
    State rotate_270();
    State swap_rows_0_1();
    State swap_rows_2_3();
    State swap_rows_both();
    State swap_cols_0_1();
    State swap_cols_2_3();
    State swap_cols_both();
    void fix_shape_order();

    State compute_canonical();

    // -------------
    // Static Stuff
    // -------------
    
    static State decode(int encoding);
};