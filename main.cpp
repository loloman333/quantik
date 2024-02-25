#include "state.cpp"

char ascii_1 = 49;
char ascii_4 = 52;
char ascii_C = 67;
char ascii_D = 68;
char ascii_R = 82;
char ascii_T = 84;

bool is_valid_input(string input)
{
    if (input.length() != 3) return false;

    char row = input[0];
    if (row < ascii_1 || row > ascii_4) return false;

    char col = input[1];
    if (col < ascii_1 || col > ascii_4) return false;

    char shape = toupper(input[2]);
    if (shape != ascii_C && shape != ascii_D && shape != ascii_R && shape != 84) return false;

    return true;
}

int main()
{
    State state{};
    char turn = 0;

    map<char, string> player_1_mapping = {{ascii_R, "■"}, {ascii_T, "▲"}, {ascii_C, "●"}, {ascii_D, "◆"}};
    map<char, string> player_2_mapping = {{ascii_R, "□"}, {ascii_T, "△"}, {ascii_C, "◯"}, {ascii_D, "◇"}};
    map<char, string> piece_mapping[2] = {player_1_mapping, player_2_mapping};
    
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
        
        if (state.board[row_index][col_index] != 0 )
        {      
           cout << endl << "Please choose and empty sqaure!" << endl << endl;
           continue;
        }

        // TODO: check max 2 of each shape

        char shape = toupper(input[2]);

        state.board[row_index][col_index] = get_index_from_piece(piece_mapping[turn][shape]);
        turn = turn == 0 ? 1 : 0;
    }
}