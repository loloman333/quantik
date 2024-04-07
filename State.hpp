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

    vector<State> compute_following_states();

    string get_print_string();

    encoding encode();

    // TODO: "hardcode" symmetries
    State mirror();
    State rotate_90();
    State rotate_180() { return this->rotate_90().rotate_90(); }
    State rotate_270() { return this->rotate_90().rotate_90().rotate_90(); }
    State fix_shape_order();
    State swap_rows_or_cols(SwapType type, char index1, char index2);

    State compute_canonical();

    // -------------
    // Static Stuff
    // -------------
    
    static State decode(int encoding);
};