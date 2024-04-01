#include "definitions.hpp"
#include "move.cpp"

struct State
{

    State() = default;

    // TOOD: needed??? !!!???!?!??!?
    State(const State& other) 
    {
        memcpy(board, other.board, sizeof(board));
        update_placed_pieces();
    }

    // Members

    pieces_map placed_pieces;
    PieceType board[4][4];
    bool whiteTurn;

    // Methods

    void make_move(Move move) { State::make_move(*this, move); }
    static void make_move(State& state, Move move)
    {
        // TODO: check if legal???
        state.board[move.row_index][move.col_index] = move.piece;
        state.placed_pieces[move.piece].push_back({move.row_index, move.col_index});
    }

    string get_print_string() { return State::get_print_string(*this); }
    static string get_print_string(State &state)
    {
        string result = "\n  | 1 2 3 4\n–––––––––––\n";

        for (char i = 0; i < 4; i++)
        {
            result += std::to_string(i + 1) + " | ";
            for (char j = 0; j < 4; j++)
            {
                result += _piece_type_to_str[state.board[i][j]];
                result += " ";
            }
            result += "\n";
        }

        return result;
    }

    encoding encode_base_9() { return State::encode_base_9(*this); }
    static encoding encode_base_9(State &state)
    {
        encoding code = 0;
        size_t mult = 1;
        for (char i = 15; i >= 0; i--)
        {
            code += ((char) *(&(state.board[0][0]) + i)) * mult;
            mult *= 9;
        }
        return code;
    }

    static State decode_base_9(int encoding)
    {
        State state{};

        int remainder;
        for (int i = 0; i < 16; ++i)
        {
            remainder = encoding % 9;
            encoding /= 9;
            state.board[i / 4][i % 4] = (PieceType)remainder;
            // TODO: cast fine? 
        }

        state.update_placed_pieces();
        return state;
    }

    void update_placed_pieces() { State::update_placed_pieces(*this); }
    static void update_placed_pieces(State& state) 
    {
        state.placed_pieces = pieces_map{};

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (state.board[i][j] != PieceType::EMPTY)
                {
                    state.placed_pieces[state.board[i][j]].push_back({i, j});
                }
            }
        }
    }


    // TODO: "hardcode" symmetries
    State mirror() { return State::mirror(*this); }
    static State mirror(State &state)
    {
        State mirrored_state{};
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                mirrored_state.board[i][j] = state.board[3 - i][j];
            }
        }
        mirrored_state.update_placed_pieces();
        return mirrored_state;
    }

    // TODO: "hardcode" rotation
    State rotate_90() { return State::rotate_90(*this); }
    static State rotate_90(State &state)
    {
        State rotatedState;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                rotatedState.board[j][3 - i] = state.board[i][j];
            }
        }
        rotatedState.update_placed_pieces();
        return rotatedState;
    }

    State rotate_180() { return State::rotate_180(*this); }
    static State rotate_180(State &state) { return state.rotate_90().rotate_90(); }

    State rotate_270() { return State::rotate_270(*this); }
    static State rotate_270(State &state) { return state.rotate_90().rotate_90().rotate_90(); }

    State fix_shape_order() { return State::fix_shape_order(*this); }
    static State fix_shape_order(State &state)
    {
        State fixed_state;
        char order_mapping[9]{};
        char next_shape = 1;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {

                char value = (char) state.board[i][j];
                if (value != 0 && order_mapping[value] == 0)
                {
                    char index = value - (1 - value % 2);
                    order_mapping[index] = next_shape;
                    order_mapping[index + 1] = next_shape + 1;
                    next_shape += 2;
                }

                fixed_state.board[i][j] = (PieceType) order_mapping[value];
            }
        }

        fixed_state.update_placed_pieces();
        return fixed_state;
    }

    // TODO: "hardcode" swapping
    State swap_rows_or_cols(SwapType type, char index1, char index2) {return State::swap_rows_or_cols(*this, type, index1, index2); }
    static State swap_rows_or_cols(State &state, SwapType type, char index1, char index2) {
        State swapped_state;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                char row_index = i;
                char col_index = j;

                if (type == SwapType::ROWS) {
                    row_index = (i == index1) ? index2 : (i == index2) ? index1 : i;
                } else if (type == SwapType::COLUMNS) {
                    col_index = (j == index1) ? index2 : (j == index2) ? index1 : j;
                }

                swapped_state.board[i][j] = state.board[row_index][col_index];
            }
        }

        swapped_state.update_placed_pieces();
        return swapped_state;
    }

};