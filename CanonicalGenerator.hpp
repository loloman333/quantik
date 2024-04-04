#pragma once

#include "State.hpp"
#include "quantik.hpp"

#define PRINT if (CanonicalGenerator::DEBUG_PRINTS)

class CanonicalGenerator
{
    public: 

    static bool DEBUG_PRINTS;

    // -------------
    // Static Stuff
    // -------------

    // TODO: maybe transform all the add_... functions to one which takes a function pointer to transformation
    static void add_mirror_states(state_map& states);
    static void add_rotate_states(state_map& states);
    static void add_swap_cols_states(state_map& states);
    static void add_swap_rows_states(state_map& states);

    static State compute_canonical(State& state);

};