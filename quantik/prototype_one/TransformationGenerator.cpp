#include "TransformationGenerator.hpp"
#include "quantik.hpp"

string TransformationGenerator::generate_mirror_function()
{   
    std::stringstream ss;
    ss << "State State::mirror(){";
    ss << "State transformed_state{};";
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ss << "transformed_state.board[" << (i) << "][" << (j) << "] = this->board[" << (3 - i) << "][" << (j) << "];";
        }
    }
    ss << "}";
    return ss.str();
}

string TransformationGenerator::generate_rotate_90_function()
{   
    std::stringstream ss;
    ss << "State State::rotate_90(){";
    ss << "State transformed_state{};";
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ss << "transformed_state.board[" << (j) << "][" << (3 - i) << "] = this->board[" << (i) << "][" << (j) << "];";
        }
    }
    ss << "}";
    return ss.str();
}

string TransformationGenerator::generate_rotate_180_function()
{   
    std::stringstream ss;
    ss << "State State::rotate_180(){";
    ss << "State transformed_state{};";
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ss << "transformed_state.board[" << (3 - i) << "][" << (3 - j) << "] = this->board[" << (i) << "][" << (j) << "];";
        }
    }
    ss << "}";
    return ss.str();
}

string TransformationGenerator::generate_rotate_270_function()
{   
    std::stringstream ss;
    ss << "State State::rotate_270(){";
    ss << "State transformed_state{};";
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ss << "transformed_state.board[" << (3 - j) << "][" << (i) << "] = this->board[" << (i) << "][" << (j) << "];";
        }
    }
    ss << "}";
    return ss.str();
}


string TransformationGenerator::generate_swap_rows_or_cols_function(SwapType type, int index1, int index2, bool two_swaps, int second_index1, int second_index2)
{
    string type_str = type == SwapType::COLUMNS ? "cols" : "rows";

    std::stringstream ss;
    ss << "State State::swap_" << type_str << "_";

    if (two_swaps)
    {
        ss << "both";
    }
    else 
    {
        ss << index1 << "_" << index2;
    }

    ss << "(){";
    ss << "State transformed_state{};";

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int row_index = i;
            int col_index = j;

            if (type == SwapType::ROWS)
            {
                row_index = (i == index1) ? index2 : (i == index2) ? index1 : row_index;
                if (two_swaps)
                {
                    row_index = (i == second_index1) ? second_index2 : (i == second_index2) ? second_index1 : row_index;
                }
            }
            else if (type == SwapType::COLUMNS)
            {
                col_index = (j == index1) ? index2 : (j == index2) ? index1 : col_index;
                if (two_swaps)
                {
                    col_index = (j == second_index1) ? second_index2 : (j == second_index2) ? second_index1 : col_index;  
                }
            }

            ss << "transformed_state.board[" << (i) << "][" << (j) << "] = this->board[" << (row_index) << "][" << (col_index) << "];";
        }
    }
    ss << "return transformed_state;}";

    return ss.str();
}