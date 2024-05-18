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

    static State compute_canonical(State& state);

};