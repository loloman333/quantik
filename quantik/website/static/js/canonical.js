function mirror(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][0] = board[3][0];
    transformed_board[0][1] = board[3][1];
    transformed_board[0][2] = board[3][2];
    transformed_board[0][3] = board[3][3];
    transformed_board[1][0] = board[2][0];
    transformed_board[1][1] = board[2][1];
    transformed_board[1][2] = board[2][2];
    transformed_board[1][3] = board[2][3];
    transformed_board[2][0] = board[1][0];
    transformed_board[2][1] = board[1][1];
    transformed_board[2][2] = board[1][2];
    transformed_board[2][3] = board[1][3];
    transformed_board[3][0] = board[0][0];
    transformed_board[3][1] = board[0][1];
    transformed_board[3][2] = board[0][2];
    transformed_board[3][3] = board[0][3];
    return transformed_board;
}

function rotate_90(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][3] = board[0][0];
    transformed_board[1][3] = board[0][1];
    transformed_board[2][3] = board[0][2];
    transformed_board[3][3] = board[0][3];
    transformed_board[0][2] = board[1][0];
    transformed_board[1][2] = board[1][1];
    transformed_board[2][2] = board[1][2];
    transformed_board[3][2] = board[1][3];
    transformed_board[0][1] = board[2][0];
    transformed_board[1][1] = board[2][1];
    transformed_board[2][1] = board[2][2];
    transformed_board[3][1] = board[2][3];
    transformed_board[0][0] = board[3][0];
    transformed_board[1][0] = board[3][1];
    transformed_board[2][0] = board[3][2];
    transformed_board[3][0] = board[3][3];
    return transformed_board;
}

function rotate_180(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[3][3] = board[0][0];
    transformed_board[3][2] = board[0][1];
    transformed_board[3][1] = board[0][2];
    transformed_board[3][0] = board[0][3];
    transformed_board[2][3] = board[1][0];
    transformed_board[2][2] = board[1][1];
    transformed_board[2][1] = board[1][2];
    transformed_board[2][0] = board[1][3];
    transformed_board[1][3] = board[2][0];
    transformed_board[1][2] = board[2][1];
    transformed_board[1][1] = board[2][2];
    transformed_board[1][0] = board[2][3];
    transformed_board[0][3] = board[3][0];
    transformed_board[0][2] = board[3][1];
    transformed_board[0][1] = board[3][2];
    transformed_board[0][0] = board[3][3];
    return transformed_board;
}

function swap_rows_0_1(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][0] = board[1][0];
    transformed_board[0][1] = board[1][1];
    transformed_board[0][2] = board[1][2];
    transformed_board[0][3] = board[1][3];
    transformed_board[1][0] = board[0][0];
    transformed_board[1][1] = board[0][1];
    transformed_board[1][2] = board[0][2];
    transformed_board[1][3] = board[0][3];
    transformed_board[2][0] = board[2][0];
    transformed_board[2][1] = board[2][1];
    transformed_board[2][2] = board[2][2];
    transformed_board[2][3] = board[2][3];
    transformed_board[3][0] = board[3][0];
    transformed_board[3][1] = board[3][1];
    transformed_board[3][2] = board[3][2];
    transformed_board[3][3] = board[3][3];
    return transformed_board;
}

function swap_rows_2_3(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][0] = board[0][0];
    transformed_board[0][1] = board[0][1];
    transformed_board[0][2] = board[0][2];
    transformed_board[0][3] = board[0][3];
    transformed_board[1][0] = board[1][0];
    transformed_board[1][1] = board[1][1];
    transformed_board[1][2] = board[1][2];
    transformed_board[1][3] = board[1][3];
    transformed_board[2][0] = board[3][0];
    transformed_board[2][1] = board[3][1];
    transformed_board[2][2] = board[3][2];
    transformed_board[2][3] = board[3][3];
    transformed_board[3][0] = board[2][0];
    transformed_board[3][1] = board[2][1];
    transformed_board[3][2] = board[2][2];
    transformed_board[3][3] = board[2][3];
    return transformed_board;
}

function swap_cols_0_1(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][0] = board[0][1];
    transformed_board[0][1] = board[0][0];
    transformed_board[0][2] = board[0][2];
    transformed_board[0][3] = board[0][3];
    transformed_board[1][0] = board[1][1];
    transformed_board[1][1] = board[1][0];
    transformed_board[1][2] = board[1][2];
    transformed_board[1][3] = board[1][3];
    transformed_board[2][0] = board[2][1];
    transformed_board[2][1] = board[2][0];
    transformed_board[2][2] = board[2][2];
    transformed_board[2][3] = board[2][3];
    transformed_board[3][0] = board[3][1];
    transformed_board[3][1] = board[3][0];
    transformed_board[3][2] = board[3][2];
    transformed_board[3][3] = board[3][3];
    return transformed_board;
}

function swap_cols_2_3(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    transformed_board[0][0] = board[0][0];
    transformed_board[0][1] = board[0][1];
    transformed_board[0][2] = board[0][3];
    transformed_board[0][3] = board[0][2];
    transformed_board[1][0] = board[1][0];
    transformed_board[1][1] = board[1][1];
    transformed_board[1][2] = board[1][3];
    transformed_board[1][3] = board[1][2];
    transformed_board[2][0] = board[2][0];
    transformed_board[2][1] = board[2][1];
    transformed_board[2][2] = board[2][3];
    transformed_board[2][3] = board[2][2];
    transformed_board[3][0] = board[3][0];
    transformed_board[3][1] = board[3][1];
    transformed_board[3][2] = board[3][3];
    transformed_board[3][3] = board[3][2];
    return transformed_board;
}

function fix_shape_order(board) {
    let transformed_board = [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]];
    let order_mapping = [
        PieceType.EMPTY, PieceType.EMPTY, PieceType.EMPTY, 
        PieceType.EMPTY, PieceType.EMPTY, PieceType.EMPTY, 
        PieceType.EMPTY, PieceType.EMPTY, PieceType.EMPTY
    ];
    let next_shape = 1;

    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            let value = board[i][j];
            if (value != PieceType.EMPTY && order_mapping[value] == PieceType.EMPTY) {
                let index = value - (1 - value % 2);
                order_mapping[index] = next_shape;
                order_mapping[index + 1] = next_shape + 1;
                next_shape += 2;
            }

            transformed_board[i][j] = order_mapping[value];
        }
    }
    return transformed_board;
}

function add_transformation_states(states, transformation) {
    let newStates = new Map();
    for (let board of states) {
        let new_board = transformation(board[1]);
        newStates.set(window.game.encode(new_board), new_board)
    }
    newStates.forEach((value, key) => states.set(key, value));
}

function compute_canonical(board) {
    let states = new Map();
    states.set(window.game.encode(board), board)

    // Mirror
    add_transformation_states(states, mirror);

    // Rotate
    add_transformation_states(states, rotate_90);
    add_transformation_states(states, rotate_180);

    // Swap Columns
    add_transformation_states(states, swap_cols_0_1);
    add_transformation_states(states, swap_cols_2_3);

    // Swap Rows
    add_transformation_states(states, swap_rows_0_1);
    add_transformation_states(states, swap_rows_2_3);

    // Fix shape order and select max
    let max_encoding = 0;
    let canonicalState;
    for (let state of states) {
        state = fix_shape_order(state[1]);
        let enc = window.game.encode(state);
        if (enc >= max_encoding) {
            max_encoding = enc;
            canonicalState = state;
        }
    }

    return canonicalState;
}
