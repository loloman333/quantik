#include "state.cpp"
#include "move.cpp"

// TODO: make vector only once
vector<Move> compute_all_moves()
{
    vector<Move> all_moves{};
    for (char row = 0; row < 4; row++)
    {
        for (char col = 0; col < 4; col++)
        {
            for (PieceType type : _piece_types)
            {
                all_moves.push_back(Move{row, col, type});
            }
        }

    }
    return all_moves;
};

vector<Move> compute_possible_moves(State& state)
{
    vector<Move> all_moves = compute_all_moves();
    vector<Move> legal_moves{};

    for (Move& move : all_moves)
    {
        if (is_legal_move(move, state))
        {
            legal_moves.push_back(move);
        }   
    }    

    return legal_moves;
}

state_map compute_following_states(State& state)
{
    state_map following_states{};

    vector<Move> legal_moves = compute_possible_moves(state);
    for (Move& move : legal_moves)
    {
        State new_state{state};
        new_state.make_move(move);
        following_states.emplace(new_state.encode_base_9(), new_state);
    }

    return following_states;
}

bool is_legal_move(Move& move, State& state)
{
    if (move.piece == PieceType::EMPTY) return false;
    if (move.col_index >= 4 || move.row_index >= 4) return false;

    if (state.board[move.row_index][move.col_index] != PieceType::EMPTY) return false;
    if (state.placed_pieces[move.piece].size() >= 2) return false;

    PieceType counterpart = get_counterpart_piece(move.piece);
    for (auto pos : state.placed_pieces[counterpart])
    {
        if (pos.first == move.row_index || pos.second == move.col_index) return false;
    }

    return true;
}