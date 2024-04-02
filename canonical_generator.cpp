#include "move_generator.cpp"

void print_state_map(state_map& map)
{
    for (auto& pair : map)
    {
        cout << "Encoding: " << pair.first << endl;
        cout << "State " << pair.second.get_print_string() << endl;
    }
}

// TODO: maybe transform all the add_... functions to one which takes a function pointer to transformation
void add_mirror_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newState = pair.second.mirror();
        newStates.insert({newState.encode_base_9(), newState});
    }

    states.insert(newStates.begin(), newStates.end());
}

void add_rotate_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newState90 = pair.second.rotate_90();
        State newState180 = pair.second.rotate_180();
        State newState270 = pair.second.rotate_270();

        newStates.insert({newState90.encode_base_9(), newState90});
        newStates.insert({newState180.encode_base_9(), newState180});
        newStates.insert({newState270.encode_base_9(), newState270});
    }

    states.insert(newStates.begin(), newStates.end());
}

void add_swap_cols_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newStateFirst = pair.second.swap_rows_or_cols(SwapType::COLUMNS, 0, 1);
        State newStateSecond = pair.second.swap_rows_or_cols(SwapType::COLUMNS, 2, 3);
        State newStateBoth = newStateFirst.swap_rows_or_cols(SwapType::COLUMNS, 2, 3);

        newStates.insert({newStateFirst.encode_base_9(), newStateFirst});
        newStates.insert({newStateSecond.encode_base_9(), newStateSecond});
        newStates.insert({newStateBoth.encode_base_9(), newStateBoth});
    }

    states.insert(newStates.begin(), newStates.end());
}

void add_swap_rows_states(state_map& states)
{
    state_map newStates;

    for (auto& pair : states)
    {
        State newStateFirst = pair.second.swap_rows_or_cols(SwapType::ROWS, 0, 1);
        State newStateSecond = pair.second.swap_rows_or_cols(SwapType::ROWS, 2, 3);
        State newStateBoth = newStateFirst.swap_rows_or_cols(SwapType::ROWS, 2, 3);

        newStates.insert({newStateFirst.encode_base_9(), newStateFirst});
        newStates.insert({newStateSecond.encode_base_9(), newStateSecond});
        newStates.insert({newStateBoth.encode_base_9(), newStateBoth});
    }

    states.insert(newStates.begin(), newStates.end());
}

State compute_canonical(State& state)
{
    if (DEBUG) cout << "Begin computing the canonical..." << endl;
    state_map states{{state.encode_base_9(), state}};

    // Mirror
    add_mirror_states(states);
    if (DEBUG) cout << "Amount after mirroring: " << states.size() << endl;
    if (DEBUG) print_state_map(states);

    // Rotate
    add_rotate_states(states);
    if (DEBUG) cout << "Amount after rotating: " << states.size() << endl;
    if (DEBUG) print_state_map(states);

    // Swap Columns
    add_swap_cols_states(states);
    if (DEBUG) cout << "Amount after swapping cols: " << states.size() << endl;
    if (DEBUG) print_state_map(states);

    // Swap Rows
    add_swap_rows_states(states);
    if (DEBUG) cout << "Amount after swapping rows: " << states.size() << endl;
    if (DEBUG) print_state_map(states);

    if (DEBUG) cout << "Fixing shapes and looking for the largest..." << endl;

    // Fix shape order and select max
    encoding max_encoding = 0;
    State canonicalState{};
    for (auto& pair : states) 
    {
        State fixedState = pair.second.fix_shape_order();
        encoding enc = fixedState.encode_base_9();
        if (enc > max_encoding)
        {
            max_encoding = enc;
            canonicalState = fixedState;
        }
    }

    if (DEBUG) cout << "Found the canonical!" << endl;

    return canonicalState;
}