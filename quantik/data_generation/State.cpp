#include "State.hpp"
#include "PieceManager.hpp"

// omitted for performance TONOTDO: bool ret val if legal
void State::make_move(Move move)
{
    this->board[move.row_index][move.col_index] = move.piece;
    this->placed_pieces[move.piece].push_back({move.row_index, move.col_index});
    this->black_turn = !this->black_turn;
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
    encoding enc = 0;
    size_t mult = 1;
    for (char i = 15; i >= 0; i--)
    {
        enc += ((char)*(&(this->board[0][0]) + i)) * mult;
        mult *= 9;
    }
    return enc;
}

State State::decode(encoding enc)
{
    State state{};

    for (int i = 0; i < 16; ++i)
    {
        uint64_t remainder = enc % 9;
        enc /= 9;
        *(&(state.board[3][3]) - i) = (PieceType)remainder;
    }

    state.update_placed_pieces();

    return state;
}

void State::update_placed_pieces()
{
    this->placed_pieces = pieces_map{};

    char white_pieces = 0;
    char black_pieces = 0;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (this->board[i][j] != PieceType::EMPTY)
            {
                this->placed_pieces[this->board[i][j]].push_back({i, j});
                if (PieceManager::is_black(this->board[i][j]))
                {
                    black_pieces++;
                }
                else
                {
                    white_pieces++;
                }
            }
        }
    }

    black_turn = black_pieces < white_pieces;
}

State State::mirror()
{
    State transformed_state;
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
    return transformed_state;
}

State State::rotate_90()
{
    State transformed_state;
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
    return transformed_state;
}

State State::rotate_180()
{
    State transformed_state;
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
    return transformed_state;
}

State State::rotate_270()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_rows_0_1()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_rows_2_3()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_rows_both()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_cols_0_1()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_cols_2_3()
{
    State transformed_state;
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
    return transformed_state;
}

State State::swap_cols_both()
{
    State transformed_state;
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
    return transformed_state;
}

void State::fix_shape_order()
{
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

            this->board[i][j] = (PieceType)order_mapping[value];
        }
    }
}

bool State::is_legal_move(Move &move)
{
    // Try to place empty Piece
    if (move.piece == PieceType::EMPTY)
        return false;

    // Try to place outside of board
    if (move.col_index >= 4 || move.row_index >= 4)
        return false;

    // Try to place piece of wrong color
    if (PieceManager::is_black(move.piece) != this->black_turn)
        return false;

    // Try to place on top of another piece
    if (this->board[move.row_index][move.col_index] != PieceType::EMPTY)
        return false;

    // Try to place a third of a shape
    if (this->placed_pieces[move.piece].size() >= 2)
        return false;

    // Try to place in row, col or sector where opponent has one of same shape
    PieceType counterpart = PieceManager::get_counterpart_piece(move.piece);
    for (auto pos : this->placed_pieces[counterpart])
    {   
        // Row
        if (pos.first == move.row_index)
            return false;
        
        // Col
        if (pos.second == move.col_index)
            return false;

        // Sector
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

    // Try to play on a final board
    // if (is_winning_state()) // TONOTDO: this check was removed for performance!
    //     return false;

    return true;
}

bool State::is_winning_state()
{
    for (char i = 0; i < 4; i++)
    {
        std::set<PieceShape> shapes_in_row;
        std::set<PieceShape> shapes_in_col;
        for (char j = 0; j < 4; j++)
        {
            PieceShape shape_1 = PieceManager::get_piece_shape(this->board[i][j]);
            PieceShape shape_2 = PieceManager::get_piece_shape(this->board[j][i]);

            if (shape_1 != PieceShape::NONE) shapes_in_row.insert(shape_1);
            if (shape_2 != PieceShape::NONE) shapes_in_row.insert(shape_2);
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

state_map State::compute_following_states()
{
    state_map following_states{};

    for (Move &move : Move::get_all_moves(this->black_turn))
    {
        if (this->is_legal_move(move))
        {
            State new_state{*this};
            new_state.make_move(move);
            State canonical = new_state.compute_canonical();
            following_states.emplace(canonical.encode(), canonical);
        }
    }

    return following_states;
}

State State::compute_canonical()
{
    return CanonicalGenerator::compute_canonical(*this);
}