#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <map>

using 
    std::string, 
    std::cout,
    std::endl;

/*
    0 -> empty
    1 -> ■
    2 -> □
    3 -> ▲
    4 -> △
    5 -> ●
    6 -> ◯
    7 -> ◆
    8 -> ◇
*/
string pieces[9] = {"-", "■", "□", "▲", "△", "●", "◯", "◆", "◇"};

string get_piece_from_index(char i)
{
    return pieces[i];
}

char get_index_from_piece(string piece)
{
    char i = 0;
    for (string p : pieces)
    {
        if (p == piece)
        {
            return i;
        }
        i++;
    }
    assert(false);
}

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

// TODO: check for only valid characters
bool is_valid_input(string input)
{
    if (input.length() != 3)
    {
        return false;
    }
    return true;
}

using std::map;

int main()
{
    State state{};
    char turn = 0;
    std::map<string, string> player_1_mapping = {{"R", "■"}, {"T", "▲"}, {"C", "●"}, {"D", "◆"}};
    std::map<string, string> player_2_mapping = {{"R", "□"}, {"T", "△"}, {"C", "◯"}, {"D", "◇"}};
    std::map<string, string> piece_mapping[2] = {player_1_mapping, player_2_mapping};
    
    while (true) // TODO: Check for win
    {
        cout << "Player " << std::to_string(turn) << "'s turn:"  << endl;
        cout << "Enter move like: [row][col][shape]" << endl;
        
        cout << "Shapes: ";
        if (turn == 0) cout << "■ = R; ▲ = T; ● = C; ◆ = D"; 
        else cout << "□ = R; △ = T; ◯ = C; ◇ = D"; 
        cout << endl;
        
        cout << state.get_string() << endl << "Input: ";

        string input;
        std::cin >> input;

        if (! is_valid_input(input))
        {
           cout << endl << "Invalid Input, please try again!" << endl << endl;
           continue;
        }

        char row_index = std::stoi(input.substr(0, 1)) - 1;
        char col_index = std::stoi(input.substr(1, 1)) - 1;
        string shape = input.substr(2, 1);

        state.board[row_index][col_index] = get_index_from_piece(piece_mapping[turn][shape]);
        turn = turn == 0 ? 1 : 0;
    }
}