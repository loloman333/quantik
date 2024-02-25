#include "definitions.hpp"

struct State
{
    char board[4][4];

    string get_string()
    {
        string result = "\n  | 1 2 3 4\n–––––––––––\n";

        for (char i = 0; i < 4; i++)
        {
            result += std::to_string(i + 1) + " | ";
            for (char j = 0; j < 4; j++)
            {
                assert(board[i][j] >= 0 && board[i][j] <= 8);
                result += get_piece_from_index(board[i][j]);
                result += " ";
            }
            result += "\n";
        }

        return result;
    }

    int encode_base_9()
    {
        int code = 0;
        int mult = 1;
        for (char i = 15; i >= 0; i--)
        {
            code += *(&board[0][0] + i) * mult;
            mult *= 9;
        }
        return code;
    }

    State mirror()
    {
        State state{};
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                state.board[i][j] = board[3 - i][j];
            }
        }
        return state;
    }


};