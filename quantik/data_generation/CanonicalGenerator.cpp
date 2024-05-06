#include "CanonicalGenerator.hpp"

string get_state_map_str(state_map& map)
{
    std::stringstream ss;
    for (auto& pair : map)
    {
        ss << "Encoding: " << pair.first << endl;
        ss << "State " << pair.second.get_print_string() << endl;
    }

    return ss.str();
}

void CanonicalGenerator::add_mirror_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newState = pair.second.mirror();
        newStates.emplace(newState.encode(), newState);
    }

    states.insert(newStates.begin(), newStates.end());
}

void CanonicalGenerator::add_rotate_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newState90 = pair.second.rotate_90();
        State newState180 = pair.second.rotate_180();
        State newState270 = pair.second.rotate_270();

        newStates.emplace(newState90.encode(), newState90);
        newStates.emplace(newState180.encode(), newState180);
        newStates.emplace(newState270.encode(), newState270);
    }

    states.insert(newStates.begin(), newStates.end());
}

void CanonicalGenerator::add_swap_cols_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newStateFirst = pair.second.swap_cols_0_1();
        State newStateSecond = pair.second.swap_cols_2_3();
        State newStateBoth = pair.second.swap_cols_both();

        newStates.emplace(newStateFirst.encode(), newStateFirst);
        newStates.emplace(newStateSecond.encode(), newStateSecond);
        newStates.emplace(newStateBoth.encode(), newStateBoth);
    }

    states.insert(newStates.begin(), newStates.end());
}

void CanonicalGenerator::add_transformation_states(state_map& states, State (State::*transformation)())
{
    state_map newStates;
    for (auto& pair : states)
    {
        State transformed_state = (pair.second.*transformation)();
        newStates.emplace(transformed_state.encode(), transformed_state);
    }
    states.insert(newStates.begin(), newStates.end());
}

void CanonicalGenerator::add_swap_rows_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newStateFirst = pair.second.swap_rows_0_1();
        State newStateSecond = pair.second.swap_rows_2_3();
        State newStateBoth = pair.second.swap_rows_both();

        newStates.emplace(newStateFirst.encode(), newStateFirst);
        newStates.emplace(newStateSecond.encode(), newStateSecond);
        newStates.emplace(newStateBoth.encode(), newStateBoth);
    }

    states.insert(newStates.begin(), newStates.end());
}

State CanonicalGenerator::compute_canonical(State& state)
{
    DBGMSG(DBG_CANONICAL_GENERATOR, "Begin computing the canonical of:\n");
    DBGMSG(DBG_CANONICAL_GENERATOR, state.get_print_string());
    state_map states{{state.encode(), state}};

    // Mirror
    add_transformation_states(states, &State::mirror);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount after mirroring: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Rotate
    add_transformation_states(states, &State::rotate_90);
    add_transformation_states(states, &State::rotate_180);
    // add_transformation_states(states, &State::rotate_270);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount after rotating: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Swap Columns
    add_transformation_states(states, &State::swap_cols_0_1);
    add_transformation_states(states, &State::swap_cols_2_3);
    // add_transformation_states(states, &State::swap_cols_both);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount swapping cols: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Swap Rows
    add_transformation_states(states, &State::swap_rows_0_1);
    add_transformation_states(states, &State::swap_rows_2_3);
    // add_transformation_states(states, &State::swap_rows_both);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount swapping rows: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));  

    DBGMSG(DBG_CANONICAL_GENERATOR, "Fixing shapes and looking for the largest...\n");

    // Fix shape order and select max
    encoding max_encoding = 0;
    State canonicalState{};
    for (auto& pair : states) 
    {
       pair.second.fix_shape_order();
        encoding enc = pair.second.encode();
        if (enc > max_encoding)
        {
            max_encoding = enc;
            canonicalState = pair.second;
        }
    }

    DBGMSG(DBG_CANONICAL_GENERATOR, "Found the canonical:" + canonicalState.get_print_string());

    // Set correct turn and update placed pieces
    canonicalState.update_placed_pieces();
    canonicalState.black_turn = state.black_turn;

    return canonicalState;
}