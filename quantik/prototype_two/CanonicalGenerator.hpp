#pragma once

#include "State.hpp"
#include "quantik.hpp"
class CanonicalGenerator
{
    public: 

    // -------------
    // Static Stuff
    // -------------

    static void add_transformation_states(state_map& states, State (State::*transformation)());

    static void add_mirror_states(state_map& states);
    static void add_rotate_states(state_map& states);
    static void add_swap_cols_states(state_map& states);
    static void add_swap_rows_states(state_map& states);

    static State compute_canonical(State& state);

};