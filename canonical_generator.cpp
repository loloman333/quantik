//#include "definitions.hpp"
#include "state.cpp"


// TODO: Am I doing this right with references ???
void add_mirror_states(vector<State>& states)
{
    vector<State> newStates;

    for (State& state : states)
    {
        // TODO: add check
        newStates.push_back(state.mirror());
    }

    states.insert(states.end(), newStates.begin(), newStates.end());
}

// TODO: Am I doing this right with references ???
void add_rotate_states(vector<State>& states)
{
    vector<State> newStates;

    for (State& state : states)
    {
        // TODO: add check
        newStates.push_back(state.rotate_90());
        newStates.push_back(state.rotate_180());
        newStates.push_back(state.rotate_270());
    }

    states.insert(states.end(), newStates.begin(), newStates.end());
}

// TODO: Am I doing this right with references ???
void add_swap_cols_states(vector<State>& states)
{
    vector<State> newStates;

    for (State& state : states)
    {
        // TODO: add check
        State first = state.swap_rows_or_cols(SwapType::COLUMNS, 0, 1);
        State second = state.swap_rows_or_cols(SwapType::COLUMNS, 2, 3);

        newStates.push_back(first);
        newStates.push_back(second);
        newStates.push_back(first.swap_rows_or_cols(SwapType::COLUMNS, 2, 3));
    }

    states.insert(states.end(), newStates.begin(), newStates.end());
}

// TODO: Am I doing this right with references ???
void add_swap_rows_states(vector<State>& states)
{
    vector<State> newStates;

    for (State& state : states)
    {
        // TODO: add check
        State first = state.swap_rows_or_cols(SwapType::ROWS, 0, 1);
        State second = state.swap_rows_or_cols(SwapType::ROWS, 2, 3);
        
        newStates.push_back(first);
        newStates.push_back(second);
        newStates.push_back(first.swap_rows_or_cols(SwapType::ROWS, 2, 3));
    }

    states.insert(states.end(), newStates.begin(), newStates.end());
}

// TODO: Benchmark list vs vector
State compute_canonical(State& state)
{
    vector<State> states{state};

    // mirror if halfs are not the same (binary)
    add_mirror_states(states);

    // rotate (if rotated quarters are not the same)
    add_rotate_states(states);

    // swap cols if not the same
    add_swap_cols_states(states);

    // swap rows if not the same or smaller
    add_swap_rows_states(states);

    // fix shape order
    for (State& state : states) state = state.fix_shape_order();

    return states[0];
}