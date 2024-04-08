#include "quantik.hpp"

string generate_mirror_function()
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

string generate_rotate_90_function()
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

string generate_rotate_180_function()
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

string generate_rotate_270_function()
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

string generate_swap_rows_or_cols_function(SwapType type, char index1, char index2)
{
    std::stringstream ss;

    string type_str = type == SwapType::COLUMNS ? "cols" : "rows";

    ss << "State State::swap_" << type_str << "_" << index1 << "_" << index2 << "(){";
    ss << "State transformed_state{};";

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            char row_index = i;
            char col_index = j;

            if (type == SwapType::ROWS)
            {
                row_index = (i == index1) ? index2 : (i == index2) ? index1 : i;
            }
            else if (type == SwapType::COLUMNS)
            {
                col_index = (j == index1) ? index2 : (j == index2) ? index1 : j;
            }

            ss << "transformed_state.board[" << (i) << "][" << (j) << "] = this->board[" << (row_index) << "][" << (col_index) << "];";
        }
    }
    ss << "}";

    return ss.str();
}