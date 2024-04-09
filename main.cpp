#include "quantik.hpp"
#include "State.hpp"
#include "GameTree.hpp"
#include "TransformationGenerator.hpp"

char ascii_1 = 49;
char ascii_4 = 52;
char ascii_C = 67;
char ascii_D = 68;
char ascii_R = 82;
char ascii_T = 84;

bool is_valid_input(string input)
{
    if (input.length() != 3)
        return false;

    char row = input[0];
    if (row < ascii_1 || row > ascii_4)
        return false;

    char col = input[1];
    if (col < ascii_1 || col > ascii_4)
        return false;

    char shape = toupper(input[2]);
    if (shape != ascii_C && shape != ascii_D && shape != ascii_R && shape != 84)
        return false;

    return true;
}

// TODO: repair and check for win
void manual_play()
{
    State state{};
    char turn = 0;

    map<char, PieceType> player_1_mapping {
        {ascii_R, PieceType::WHITE_SQUARE}, 
        {ascii_T,PieceType::WHITE_TRIANGLE}, 
        {ascii_C, PieceType::WHITE_CIRCLE}, 
        {ascii_D, PieceType::WHITE_DIAMOND}
    };
    map<char, PieceType> player_2_mapping {
        {ascii_R, PieceType::BLACK_SQUARE}, 
        {ascii_T,PieceType::BLACK_TRIANGLE}, 
        {ascii_C, PieceType::BLACK_CIRCLE}, 
        {ascii_D, PieceType::BLACK_DIAMOND}
    };
    map<char, PieceType> piece_mapping[2] = {player_1_mapping, player_2_mapping};

    while (true)
    {
        cout << "Player " << std::to_string(turn) << "'s turn:" << endl;
        cout << "Enter move like: [row][col][shape]" << endl;

        cout << "Shapes: ";
        if (turn == 0)
            cout << "■ = R; ▲ = T; ● = C; ◆ = D";
        else
            cout << "□ = R; △ = T; ◯ = C; ◇ = D";
        cout << endl;

        cout << state.get_print_string() << endl;

        cout << "Canonical: \n" << state.compute_canonical().get_print_string() << endl;
        cout << "Input: " << endl;

        string input;
        std::cin >> input;

        if (!is_valid_input(input))
        {
            cout << endl
                 << "Invalid Input, please try again!" << endl
                 << endl;
            continue;
        }

        char row_index = std::stoi(input.substr(0, 1)) - 1;
        char col_index = std::stoi(input.substr(1, 1)) - 1;

        // if (state.board[row_index][col_index] != PieceType::EMPTY)
        // {
        //     cout << endl
        //          << "Please choose an empty square!" << endl
        //          << endl;
        //     continue;
        // }

        // TODO: check max 2 of each shape
        // Done in "is_legal_move" -> use this function

        char shape = toupper(input[2]);

        state.make_move(Move{row_index, col_index, piece_mapping[turn][shape]});
        turn = turn == 0 ? 1 : 0;
    }
}

void generate_all_transformation_functions()
{
    cout << TransformationGenerator::generate_mirror_function() << endl;

    cout << TransformationGenerator::generate_rotate_90_function() << endl;
    cout << TransformationGenerator::generate_rotate_180_function() << endl;
    cout << TransformationGenerator::generate_rotate_270_function() << endl;

    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::ROWS, 0, 1) << endl;
    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::ROWS, 2, 3) << endl;
    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::ROWS, 0, 1, true, 2, 3) << endl;

    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::COLUMNS, 0, 1) << endl;
    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::COLUMNS, 2, 3) << endl;
    cout << TransformationGenerator::generate_swap_rows_or_cols_function(SwapType::COLUMNS, 0, 1, true, 2, 3) << endl;
}

int main()
{
    GameTree* tree{GameTree::compute_tree(2)};
    delete tree;

    // generate_all_transforamtion_functions();
}