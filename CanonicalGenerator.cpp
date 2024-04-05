#include "CanonicalGenerator.hpp"

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
        State newStateFirst = pair.second.swap_rows_or_cols(SwapType::COLUMNS, 0, 1);
        State newStateSecond = pair.second.swap_rows_or_cols(SwapType::COLUMNS, 2, 3);
        State newStateBoth = newStateFirst.swap_rows_or_cols(SwapType::COLUMNS, 2, 3);

        newStates.emplace(newStateFirst.encode(), newStateFirst);
        newStates.emplace(newStateSecond.encode(), newStateSecond);
        newStates.emplace(newStateBoth.encode(), newStateBoth);
    }

    states.insert(newStates.begin(), newStates.end());
}

void CanonicalGenerator::add_swap_rows_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newStateFirst = pair.second.swap_rows_or_cols(SwapType::ROWS, 0, 1);
        State newStateSecond = pair.second.swap_rows_or_cols(SwapType::ROWS, 2, 3);
        State newStateBoth = newStateFirst.swap_rows_or_cols(SwapType::ROWS, 2, 3);

        newStates.emplace(newStateFirst.encode(), newStateFirst);
        newStates.emplace(newStateSecond.encode(), newStateSecond);
        newStates.emplace(newStateBoth.encode(), newStateBoth);
    }

    states.insert(newStates.begin(), newStates.end());
}

// TODO: ???
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

State CanonicalGenerator::compute_canonical(State& state)
{
    DBGMSG(DBG_CANONICAL_GENERATOR, "Begin computing the canonical...\n");
    state_map states{{state.encode(), state}};

    // Mirror
    add_mirror_states(states);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount after mirroring: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Rotate
    add_rotate_states(states);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount after rotating: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Swap Columns
    add_swap_cols_states(states);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount swapping cols: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    // Swap Rows
    add_swap_rows_states(states);
    DBGMSG(DBG_CANONICAL_GENERATOR, "Amount swapping rows: " + STR(states.size()) + "\n");
    DBGMSG(DBG_CANONICAL_GENERATOR_DETAILED, get_state_map_str(states));

    DBGMSG(DBG_CANONICAL_GENERATOR, "Fixing shapes and looking for the largest...\n");

    // Fix shape order and select max
    encoding max_encoding = 0;
    State canonicalState{};
    for (auto& pair : states) 
    {
        State fixedState = pair.second.fix_shape_order();
        encoding enc = fixedState.encode();
        if (enc > max_encoding)
        {
            max_encoding = enc;
            canonicalState = fixedState;
        }
    }

    DBGMSG(DBG_CANONICAL_GENERATOR, "Found the canonical:" + canonicalState.get_print_string());

    return canonicalState;
}
