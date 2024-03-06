#include "definitions.hpp"

struct State
{

    // Members

    char board[4][4];

    // Methods

    string get_print_string() { return State::get_print_string(*this); }
    static string get_print_string(State &state)
    {
        string result = "\n  | 1 2 3 4\n–––––––––––\n";

        for (char i = 0; i < 4; i++)
        {
            result += std::to_string(i + 1) + " | ";
            for (char j = 0; j < 4; j++)
            {
                assert(state.board[i][j] >= 0 && state.board[i][j] <= 8);
                result += get_piece_from_index(state.board[i][j]);
                result += " ";
            }
            result += "\n";
        }

        return result;
    }

    int encode_base_9() { return State::encode_base_9(*this); }
    static int encode_base_9(State &state)
    {
        int code = 0;
        int mult = 1;
        for (char i = 15; i >= 0; i--)
        {
            code += *(&(state.board[0][0]) + i) * mult;
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
            state.board[i / 4][i % 4] = remainder;
        }

        return state;
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
        return rotatedState;
    }

    State rotate_180() { return State::rotate_180(*this); }
    static State rotate_180(State &state) { return state.rotate_90().rotate_90(); }

    State rotate_270() { return State::rotate_270(*this); }
    static State rotate_270(State &state) { return state.rotate_90().rotate_90().rotate_90(); }

    State fix_shape_order() { return State::fix_shape_order(*this); }
    static State fix_shape_order(State &state)
    {
        State fixedState;
        char order_mapping[9]{};
        char next_shape = 1;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {

                char value = state.board[i][j];
                if (value != 0 && order_mapping[value] == 0)
                {
                    char index = value - (1 - value % 2);
                    order_mapping[index] = next_shape;
                    order_mapping[index + 1] = next_shape + 1;
                    next_shape += 2;
                }

                fixedState.board[i][j] = order_mapping[value];
            }
        }

        return fixedState;
    }

    // TODO: "hardcode" swapping
    State swap_rows_or_cols(SwapType type, char index1, char index2) {return State::swap_rows_or_cols(*this, type, index1, index2); }
    static State swap_rows_or_cols(State &state, SwapType type, char index1, char index2) {
        State swappedState;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                char row_index = i;
                char col_index = j;

                if (type == SwapType::ROWS) {
                    row_index = (i == index1) ? index2 : (i == index2) ? index1 : i;
                } else if (type == SwapType::COLUMNS) {
                    col_index = (j == index1) ? index2 : (j == index2) ? index1 : j;
                }

                swappedState.board[i][j] = state.board[row_index][col_index];
            }
        }

        return swappedState;
    }
};