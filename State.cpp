#include "State.hpp"
#include "PieceManager.hpp"

void State::make_move(Move move)
{
    // TODO: check if legal???

    // cout << "BEFORE:" << get_print_string() << endl;

    // cout << "MOVE: " << move.get_print_string() << endl;

    this->board[move.row_index][move.col_index] = move.piece;
    this->placed_pieces[move.piece].push_back({move.row_index, move.col_index});
    this->black_turn = !this->black_turn;

    // cout << "AFTER:" << get_print_string() << endl;
}

string State::get_print_string()
{
    string result = "\n  | 1 2 3 4\n–––––––––––\n";

    for (char i = 0; i < 4; i++)
    {
        result += std::to_string(i + 1) + " | ";
        for (char j = 0; j < 4; j++)
        {
            result += PieceManager::piece_type_to_str[this->board[i][j]];
            result += " ";
        }
        result += "\n";
    }

    return result;
}

encoding State::encode()
{
    encoding code = 0;
    size_t mult = 1;
    for (char i = 15; i >= 0; i--)
    {
        code += ((char)*(&(this->board[0][0]) + i)) * mult;
        mult *= 9;
    }
    return code;
}

State State::decode(int encoding)
{
    State state{};

    int remainder;
    for (int i = 0; i < 16; ++i)
    {
        remainder = encoding % 9;
        encoding /= 9;
        state.board[i / 4][i % 4] = (PieceType)remainder;
    }

    state.update_placed_pieces();
    return state;
}

void State::update_placed_pieces()
{
    this->placed_pieces = pieces_map{};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (this->board[i][j] != PieceType::EMPTY)
            {
                this->placed_pieces[this->board[i][j]].push_back({i, j});
            }
        }
    }
}

State old_swap_rows_or_cols(State& state, SwapType type, char index1, char index2)
{
    State swapped_state;

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

            swapped_state.board[i][j] = state.board[row_index][col_index];
        }
    }

    return swapped_state;
}

State State::old_mirror()
{
    State transformed_state{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            transformed_state.board[i][j] = this->board[3 - i][j];
        }
    }
    return transformed_state;
}

State State::mirror()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[3][0];
    transformed_state.board[0][1] = this->board[3][1];
    transformed_state.board[0][2] = this->board[3][2];
    transformed_state.board[0][3] = this->board[3][3];
    transformed_state.board[1][0] = this->board[2][0];
    transformed_state.board[1][1] = this->board[2][1];
    transformed_state.board[1][2] = this->board[2][2];
    transformed_state.board[1][3] = this->board[2][3];
    transformed_state.board[2][0] = this->board[1][0];
    transformed_state.board[2][1] = this->board[1][1];
    transformed_state.board[2][2] = this->board[1][2];
    transformed_state.board[2][3] = this->board[1][3];
    transformed_state.board[3][0] = this->board[0][0];
    transformed_state.board[3][1] = this->board[0][1];
    transformed_state.board[3][2] = this->board[0][2];
    transformed_state.board[3][3] = this->board[0][3];
    State cmp = old_mirror();
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::rotate_90()
{
    State transformed_state{};
    transformed_state.board[0][3] = this->board[0][0];
    transformed_state.board[1][3] = this->board[0][1];
    transformed_state.board[2][3] = this->board[0][2];
    transformed_state.board[3][3] = this->board[0][3];
    transformed_state.board[0][2] = this->board[1][0];
    transformed_state.board[1][2] = this->board[1][1];
    transformed_state.board[2][2] = this->board[1][2];
    transformed_state.board[3][2] = this->board[1][3];
    transformed_state.board[0][1] = this->board[2][0];
    transformed_state.board[1][1] = this->board[2][1];
    transformed_state.board[2][1] = this->board[2][2];
    transformed_state.board[3][1] = this->board[2][3];
    transformed_state.board[0][0] = this->board[3][0];
    transformed_state.board[1][0] = this->board[3][1];
    transformed_state.board[2][0] = this->board[3][2];
    transformed_state.board[3][0] = this->board[3][3];
    State cmp = old_rotate_90();
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::old_rotate_90()
{
    State rotatedState;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            rotatedState.board[j][3 - i] = this->board[i][j];
        }
    }
    return rotatedState;
}

State State::old_rotate_180()
{
    State rotatedState;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            rotatedState.board[3 - i][3 - j] = this->board[i][j];
        }
    }
    return rotatedState;
}

State State::old_rotate_270()
{
    State rotatedState;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            rotatedState.board[3 - j][i] = this->board[i][j];
        }
    }
    return rotatedState;
}

State State::rotate_180()
{
    State transformed_state{};
    transformed_state.board[3][3] = this->board[0][0];
    transformed_state.board[3][2] = this->board[0][1];
    transformed_state.board[3][1] = this->board[0][2];
    transformed_state.board[3][0] = this->board[0][3];
    transformed_state.board[2][3] = this->board[1][0];
    transformed_state.board[2][2] = this->board[1][1];
    transformed_state.board[2][1] = this->board[1][2];
    transformed_state.board[2][0] = this->board[1][3];
    transformed_state.board[1][3] = this->board[2][0];
    transformed_state.board[1][2] = this->board[2][1];
    transformed_state.board[1][1] = this->board[2][2];
    transformed_state.board[1][0] = this->board[2][3];
    transformed_state.board[0][3] = this->board[3][0];
    transformed_state.board[0][2] = this->board[3][1];
    transformed_state.board[0][1] = this->board[3][2];
    transformed_state.board[0][0] = this->board[3][3];
    State cmp = old_rotate_180();
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::rotate_270()
{
    State transformed_state{};
    transformed_state.board[3][0] = this->board[0][0];
    transformed_state.board[2][0] = this->board[0][1];
    transformed_state.board[1][0] = this->board[0][2];
    transformed_state.board[0][0] = this->board[0][3];
    transformed_state.board[3][1] = this->board[1][0];
    transformed_state.board[2][1] = this->board[1][1];
    transformed_state.board[1][1] = this->board[1][2];
    transformed_state.board[0][1] = this->board[1][3];
    transformed_state.board[3][2] = this->board[2][0];
    transformed_state.board[2][2] = this->board[2][1];
    transformed_state.board[1][2] = this->board[2][2];
    transformed_state.board[0][2] = this->board[2][3];
    transformed_state.board[3][3] = this->board[3][0];
    transformed_state.board[2][3] = this->board[3][1];
    transformed_state.board[1][3] = this->board[3][2];
    transformed_state.board[0][3] = this->board[3][3];
    State cmp = old_rotate_270();
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::swap_rows_0_1()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[1][0];
    transformed_state.board[0][1] = this->board[1][1];
    transformed_state.board[0][2] = this->board[1][2];
    transformed_state.board[0][3] = this->board[1][3];
    transformed_state.board[1][0] = this->board[0][0];
    transformed_state.board[1][1] = this->board[0][1];
    transformed_state.board[1][2] = this->board[0][2];
    transformed_state.board[1][3] = this->board[0][3];
    transformed_state.board[2][0] = this->board[2][0];
    transformed_state.board[2][1] = this->board[2][1];
    transformed_state.board[2][2] = this->board[2][2];
    transformed_state.board[2][3] = this->board[2][3];
    transformed_state.board[3][0] = this->board[3][0];
    transformed_state.board[3][1] = this->board[3][1];
    transformed_state.board[3][2] = this->board[3][2];
    transformed_state.board[3][3] = this->board[3][3];
    State cmp = old_swap_rows_or_cols(*this, SwapType::ROWS, 0, 1);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::swap_rows_2_3()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[0][0];
    transformed_state.board[0][1] = this->board[0][1];
    transformed_state.board[0][2] = this->board[0][2];
    transformed_state.board[0][3] = this->board[0][3];
    transformed_state.board[1][0] = this->board[1][0];
    transformed_state.board[1][1] = this->board[1][1];
    transformed_state.board[1][2] = this->board[1][2];
    transformed_state.board[1][3] = this->board[1][3];
    transformed_state.board[2][0] = this->board[3][0];
    transformed_state.board[2][1] = this->board[3][1];
    transformed_state.board[2][2] = this->board[3][2];
    transformed_state.board[2][3] = this->board[3][3];
    transformed_state.board[3][0] = this->board[2][0];
    transformed_state.board[3][1] = this->board[2][1];
    transformed_state.board[3][2] = this->board[2][2];
    transformed_state.board[3][3] = this->board[2][3];
    State cmp = old_swap_rows_or_cols(*this, SwapType::ROWS, 2, 3);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::swap_rows_both()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[1][0];
    transformed_state.board[0][1] = this->board[1][1];
    transformed_state.board[0][2] = this->board[1][2];
    transformed_state.board[0][3] = this->board[1][3];
    transformed_state.board[1][0] = this->board[0][0];
    transformed_state.board[1][1] = this->board[0][1];
    transformed_state.board[1][2] = this->board[0][2];
    transformed_state.board[1][3] = this->board[0][3];
    transformed_state.board[2][0] = this->board[3][0];
    transformed_state.board[2][1] = this->board[3][1];
    transformed_state.board[2][2] = this->board[3][2];
    transformed_state.board[2][3] = this->board[3][3];
    transformed_state.board[3][0] = this->board[2][0];
    transformed_state.board[3][1] = this->board[2][1];
    transformed_state.board[3][2] = this->board[2][2];
    transformed_state.board[3][3] = this->board[2][3];
    State cmp = old_swap_rows_or_cols(*this, SwapType::ROWS, 0, 1);
    cmp = old_swap_rows_or_cols(cmp, SwapType::ROWS, 2, 3);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    
    return transformed_state;
}

State State::swap_cols_0_1()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[0][1];
    transformed_state.board[0][1] = this->board[0][0];
    transformed_state.board[0][2] = this->board[0][2];
    transformed_state.board[0][3] = this->board[0][3];
    transformed_state.board[1][0] = this->board[1][1];
    transformed_state.board[1][1] = this->board[1][0];
    transformed_state.board[1][2] = this->board[1][2];
    transformed_state.board[1][3] = this->board[1][3];
    transformed_state.board[2][0] = this->board[2][1];
    transformed_state.board[2][1] = this->board[2][0];
    transformed_state.board[2][2] = this->board[2][2];
    transformed_state.board[2][3] = this->board[2][3];
    transformed_state.board[3][0] = this->board[3][1];
    transformed_state.board[3][1] = this->board[3][0];
    transformed_state.board[3][2] = this->board[3][2];
    transformed_state.board[3][3] = this->board[3][3];
    State cmp = old_swap_rows_or_cols(*this, SwapType::COLUMNS, 0, 1);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::swap_cols_2_3()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[0][0];
    transformed_state.board[0][1] = this->board[0][1];
    transformed_state.board[0][2] = this->board[0][3];
    transformed_state.board[0][3] = this->board[0][2];
    transformed_state.board[1][0] = this->board[1][0];
    transformed_state.board[1][1] = this->board[1][1];
    transformed_state.board[1][2] = this->board[1][3];
    transformed_state.board[1][3] = this->board[1][2];
    transformed_state.board[2][0] = this->board[2][0];
    transformed_state.board[2][1] = this->board[2][1];
    transformed_state.board[2][2] = this->board[2][3];
    transformed_state.board[2][3] = this->board[2][2];
    transformed_state.board[3][0] = this->board[3][0];
    transformed_state.board[3][1] = this->board[3][1];
    transformed_state.board[3][2] = this->board[3][3];
    transformed_state.board[3][3] = this->board[3][2];
    State cmp = old_swap_rows_or_cols(*this, SwapType::COLUMNS, 2, 3);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::swap_cols_both()
{
    State transformed_state{};
    transformed_state.board[0][0] = this->board[0][1];
    transformed_state.board[0][1] = this->board[0][0];
    transformed_state.board[0][2] = this->board[0][3];
    transformed_state.board[0][3] = this->board[0][2];
    transformed_state.board[1][0] = this->board[1][1];
    transformed_state.board[1][1] = this->board[1][0];
    transformed_state.board[1][2] = this->board[1][3];
    transformed_state.board[1][3] = this->board[1][2];
    transformed_state.board[2][0] = this->board[2][1];
    transformed_state.board[2][1] = this->board[2][0];
    transformed_state.board[2][2] = this->board[2][3];
    transformed_state.board[2][3] = this->board[2][2];
    transformed_state.board[3][0] = this->board[3][1];
    transformed_state.board[3][1] = this->board[3][0];
    transformed_state.board[3][2] = this->board[3][3];
    transformed_state.board[3][3] = this->board[3][2];
    State cmp = old_swap_rows_or_cols(*this, SwapType::COLUMNS, 0, 1);
    cmp = old_swap_rows_or_cols(cmp, SwapType::COLUMNS, 2, 3);
    assert(std::memcmp(transformed_state.board, cmp.board, sizeof(transformed_state.board)) == 0);
    return transformed_state;
}

State State::fix_shape_order()
{
    State fixed_state;
    char order_mapping[9]{};
    char next_shape = 1;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {

            char value = (char)this->board[i][j];
            if (value != 0 && order_mapping[value] == 0)
            {
                char index = value - (1 - value % 2);
                order_mapping[index] = next_shape;
                order_mapping[index + 1] = next_shape + 1;
                next_shape += 2;
            }

            fixed_state.board[i][j] = (PieceType)order_mapping[value];
        }
    }

    return fixed_state;
}

bool State::is_legal_move(Move &move)
{
    if (move.piece == PieceType::EMPTY)
        return false;
    if (move.col_index >= 4 || move.row_index >= 4)
        return false;
    if (PieceManager::is_black(move.piece) != this->black_turn)
        return false;

    if (this->board[move.row_index][move.col_index] != PieceType::EMPTY)
        return false;
    if (this->placed_pieces[move.piece].size() >= 2)
        return false;

    PieceType counterpart = PieceManager::get_counterpart_piece(move.piece);
    for (auto pos : this->placed_pieces[counterpart])
    {
        if (pos.first == move.row_index)
            return false;
        if (pos.second == move.col_index)
            return false;
        // TODO: improve performance of this check
        for (auto &sector : get_sectors())
        {
            bool correct_sector = false;
            bool has_counterpart = false;
            for (PieceType *piece_ptr : sector)
            {
                if (piece_ptr == &(this->board[move.row_index][move.col_index]))
                    correct_sector = true;
                if (*piece_ptr == counterpart)
                    has_counterpart = true;
            }
            if (correct_sector && has_counterpart)
                return false;
        }
    }

    if (is_final_state())
        return false;

    return true;
}

bool State::is_final_state()
{
    for (char i = 0; i < 4; i++)
    {
        std::set<PieceShape> shapes_in_row;
        std::set<PieceShape> shapes_in_col;
        for (char j = 0; j < 4; j++)
        {
            PieceShape shape = PieceManager::get_piece_shape(this->board[i][j]);
            if (shape != PieceShape::NONE)
            {
                shapes_in_row.insert(shape);
                shapes_in_col.insert(shape);
            }
        }
        if (shapes_in_row.size() == 4)
            return true;
        if (shapes_in_col.size() == 4)
            return true;
    }

    for (auto &sector : get_sectors())
    {
        std::set<PieceShape> shapes_in_sector;
        for (PieceType *piece_ptr : sector)
        {
            PieceShape shape = PieceManager::get_piece_shape(*piece_ptr);
            if (shape != PieceShape::NONE)
            {
                shapes_in_sector.insert(shape);
            }
        }
        if (shapes_in_sector.size() == 4)
            return true;
    }

    return false;
}

vector<vector<PieceType *>> State::get_sectors()
{
    return {
        vector<PieceType *>{&(this->board[0][0]), &(this->board[1][0]), &(this->board[0][1]), &(this->board[1][1])},
        vector<PieceType *>{&(this->board[2][0]), &(this->board[3][0]), &(this->board[2][1]), &(this->board[3][1])},
        vector<PieceType *>{&(this->board[0][2]), &(this->board[1][2]), &(this->board[0][3]), &(this->board[1][3])},
        vector<PieceType *>{&(this->board[2][2]), &(this->board[3][2]), &(this->board[2][3]), &(this->board[3][3])},
    };
}

vector<State> State::compute_following_states()
{
    vector<State> following_states{};

    for (Move &move : Move::get_all_moves(this->black_turn))
    {
        if (this->is_legal_move(move))
        {
            State new_state{*this};
            new_state.make_move(move);
            following_states.push_back(new_state);
        }
    }

    return following_states;
}

State State::compute_canonical()
{
    return CanonicalGenerator::compute_canonical(*this);
}