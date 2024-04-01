#include "state.cpp"
#include "move.cpp"

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

    for (Move& move : all_moves)
    {
        
    }    
}

state_map compute_following_states(State& state)
{
    // TODO
}