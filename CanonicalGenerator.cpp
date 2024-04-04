#include "CanonicalGenerator.hpp"

bool CanonicalGenerator::DEBUG_PRINTS = false;

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
void print_state_map(state_map& map)
{
    for (auto& pair : map)
    {
        cout << "Encoding: " << pair.first << endl;
        cout << "State " << pair.second.get_print_string() << endl;
    }
}

State CanonicalGenerator::compute_canonical(State& state)
{
    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Begin computing the canonical..." << endl;
    state_map states{{state.encode(), state}};

    // Mirror
    add_mirror_states(states);
    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Amount after mirroring: " << states.size() << endl;
    if (CanonicalGenerator::DEBUG_PRINTS) print_state_map(states);

    // Rotate
    add_rotate_states(states);
    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Amount after rotating: " << states.size() << endl;
    if (CanonicalGenerator::DEBUG_PRINTS) print_state_map(states);

    // Swap Columns
    add_swap_cols_states(states);
    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Amount after swapping cols: " << states.size() << endl;
    if (CanonicalGenerator::DEBUG_PRINTS) print_state_map(states);

    // Swap Rows
    add_swap_rows_states(states);
    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Amount after swapping rows: " << states.size() << endl;
    if (CanonicalGenerator::DEBUG_PRINTS) print_state_map(states);

    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Fixing shapes and looking for the largest..." << endl;

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

    if (CanonicalGenerator::DEBUG_PRINTS) cout << "Found the canonical!" << endl;

    return canonicalState;
}
