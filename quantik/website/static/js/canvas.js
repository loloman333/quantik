/**
 * Consts
 */
const PIECE_SIZE = 90

const SPARE_BLACKS_X = PIECE_SIZE / 4;
const SPARE_BLACKS_Y = PIECE_SIZE / 4;

const SPARE_WHITES_X = PIECE_SIZE * 5.75;
const SPARE_WHITES_Y = PIECE_SIZE / 4;

const BOARD_X = PIECE_SIZE * 1.5;
const BOARD_Y = PIECE_SIZE / 4;


/**
 * Initializes all pieces (should only be called once)
 */
function initPieces(){
    window.piece_images = {};

    window.piece_images[PieceType.EMPTY] = new Image();

    window.piece_images[PieceType.WHITE_SQUARE] = new Image();
    window.piece_images[PieceType.WHITE_SQUARE].src = "/static/images/white_square.png";

    window.piece_images[PieceType.WHITE_CIRCLE] = new Image();
    window.piece_images[PieceType.WHITE_CIRCLE].src = "/static/images/white_circle.png";

    window.piece_images[PieceType.WHITE_TRIANGLE] = new Image();
    window.piece_images[PieceType.WHITE_TRIANGLE].src = "/static/images/white_triangle.png";

    window.piece_images[PieceType.WHITE_DIAMOND] = new Image();
    window.piece_images[PieceType.WHITE_DIAMOND].src = "/static/images/white_diamond.png";

    window.piece_images[PieceType.BLACK_SQUARE] = new Image();
    window.piece_images[PieceType.BLACK_SQUARE].src = "/static/images/black_square.png";

    window.piece_images[PieceType.BLACK_CIRCLE] = new Image();
    window.piece_images[PieceType.BLACK_CIRCLE].src = "/static/images/black_circle.png";

    window.piece_images[PieceType.BLACK_TRIANGLE] = new Image();
    window.piece_images[PieceType.BLACK_TRIANGLE].src = "/static/images/black_triangle.png";

    window.piece_images[PieceType.BLACK_DIAMOND] = new Image();
    window.piece_images[PieceType.BLACK_DIAMOND].src = "/static/images/black_diamond.png";
}

initPieces();

/**
 * Draw one piece at position with backgroundcolor
 */
var drawSquare = function(image, x, y, color, radii = 0, borders = true) {
    window.context.fillStyle = color;
    window.context.strokeStyle = "#827162";
    window.context.beginPath();
    window.context.roundRect(x, y, PIECE_SIZE, PIECE_SIZE, radii);
    window.context.fill();
    if (borders) window.context.stroke();
    window.context.drawImage(image, x, y);
};

/**
 * Draw one classification at position
 */
var drawEval = function(position, eval) {

    let x = position.x + PIECE_SIZE * 0.15;
    let y = position.y + PIECE_SIZE * 0.25;

    if (window.game.recommendedMoveIndex !== -1)
    {
        eval = "!";
        window.context.fillStyle = "red";
        window.context.font = "bold 20px Arial";
        window.context.textAlign = "center";
        window.context.fillText(eval, x, y);
        return;
    }
    else if (eval == -1)
    {
        window.context.fillStyle = "blue";
        eval = "?";
    }
    else if (eval == 21){
        window.context.fillStyle = "blue";
        if (eval == 21) eval = "D";
    } else {
        if (eval % 2 == 0)
        {
            window.context.fillStyle = "red";
        }
        else
        {
            window.context.fillStyle = "green";
        }
    }
    if (eval == 1) eval = "Win";
    window.context.font = "bold 16px Arial";
    window.context.textAlign = "center";
    window.context.fillText(eval, x, y);
};

function getCornerRadii(first_index, second_index = false){
    if (second_index === false){
        return [
            (first_index == 0) ? 10 : 0,
            (first_index == 0) ? 10 : 0,
            (first_index == 3) ? 10 : 0,
            (first_index == 3) ? 10 : 0,
        ]
    }

    return [
        (first_index == 0 && second_index == 0) ? 10 : 0,
        (first_index == 0 && second_index == window.game.COLS - 1) ? 10 : 0,
        (first_index == window.game.ROWS - 1 && second_index == window.game.COLS - 1) ? 10 : 0,
        (first_index == window.game.ROWS - 1 && second_index == 0) ? 10 : 0,
    ]
}

/**
 * Draw  the board on the canvas
 */
var drawBoard = function() {

    // Clear old
    // window.context.clearRect(0, 0, window.canvas.width, window.canvas.height); // TODO: needed?

    let activeMove = window.game.activeMove;

    let mouseOverType = window.game.mouseOverType;
    let mouseOverRow= window.game.mouseOverRow;
    let mouseOverCol = window.game.mouseOverCol;

    // Background
    window.context.fillStyle = "#bbada0";
    window.context.strokeStyle = "#827162";

    window.context.beginPath();
    window.context.roundRect(SPARE_BLACKS_X, SPARE_BLACKS_Y, PIECE_SIZE, PIECE_SIZE * 4, 10);
    window.context.fill();
    window.context.stroke();

    window.context.beginPath();
    window.context.roundRect(SPARE_WHITES_X, SPARE_WHITES_Y, PIECE_SIZE, PIECE_SIZE * 4, 10);
    window.context.fill();
    window.context.stroke();

    window.context.beginPath();
    window.context.roundRect(BOARD_X, BOARD_Y, PIECE_SIZE * 4, PIECE_SIZE * 4, 10);
    window.context.fill();
    window.context.stroke();

    // Spare Pieces
    for (let [_, val] of Object.entries(PieceType))
    {
        if (val == PieceType.EMPTY) continue;
        
        let pos = getSpareSquarePosition(val);
        let image = window.piece_images[val];
        let radii = getCornerRadii(Math.ceil(val - (val / 2)) - 1);
        
        let color = BackgroundColor.INACTIVE;
        if (activeMove.piece_type == val){
            color = BackgroundColor.ACTIVE;
        } else if (activeMove.piece_type == PieceType.EMPTY && window.game.turn == getPieceColor(val) && window.game.pieceCounter[val] < 2){
            if (mouseOverType == val) color = BackgroundColor.ACTIVE;
            else color = BackgroundColor.POSSIBLE;
        }

        drawSquare(image, pos.x, pos.y, color, radii);

        window.context.fillStyle = "grey";
        window.context.textAlign = "center";
        window.context.font = "bold 16px Arial";
        window.context.fillText(
            "x" + (2 - window.game.pieceCounter[val]), 
            pos.x + PIECE_SIZE * 0.5, 
            pos.y + PIECE_SIZE * 0.55
        );

        let best_move_with_piece = window.game.getBestMove(val);
        if (best_move_with_piece !== undefined) drawEval(pos, best_move_with_piece.eval)
    }

    // Board
    for (let i = 0; i < window.game.ROWS; i++) {
        for(let j = 0; j < window.game.COLS; j++){
            let position = getSquarePosition(i, j);
            let image = window.piece_images[window.game.board[i][j]];
            let radii = getCornerRadii(i, j);
            drawSquare(image, position.x, position.y, BackgroundColor.INACTIVE, radii);
        }
    }

    // Possible Moves
    for (let move of window.game.possibleMoves[activeMove.piece_type])
    {   
        let isMouseOver = (mouseOverRow == move.target_row) && (mouseOverCol == move.target_col);

        let position = getSquarePosition(move.target_row, move.target_col);
        let image = isMouseOver ? window.piece_images[activeMove.piece_type] : window.piece_images[PieceType.EMPTY];
        let color = isMouseOver ?  BackgroundColor.ACTIVE : BackgroundColor.POSSIBLE;
        let radii = getCornerRadii(move.target_row, move.target_col);

        drawSquare(image, position.x, position.y, color, radii);
        drawEval(position, move.eval)
    }
};

/**
 * Draw all classifications
 */
var drawEvals = function() {
    if (((!window.game.showMoveInfos) || (window.game.waitingForMove)) && (window.game.recommendedMoveIndex == -1) ||
        (window.game.activeMove.target_row !== -1)) return;

    var bestEval = [];
    for(var i = 0; i < window.game.COLS; i++){
        var col = []
        for(var j = 0; j < window.game.ROWS; j++){
            col.push(-2);
        }
        bestEval.push(col);
    }

    if(window.game.activeMove.source_row !== -1){
        for (const move of window.game.possibleMoves)
        {
            if ((move.source_row == window.game.activeMove.source_row) && (move.source_col == window.game.activeMove.source_col) && 
                isBetterEval(move.eval, bestEval[move.target_col][move.target_row]))
            {
                bestEval[move.target_col][move.target_row] = move.eval;
            }
        }
        for (var i = 0; i < window.game.COLS; i++)
        {
            for(var j = 0; j < window.game.ROWS; j++){
                if (bestEval[i][j] !== -2)
                {
                    if ((window.game.recommendedMoveIndex == -1) ||
                        ((window.game.possibleMoves[window.game.recommendedMoveIndex].target_col == i) && 
                        (window.game.possibleMoves[window.game.recommendedMoveIndex].target_row == j)))
                    {
                        var position = getSquarePosition(j, i);
                        drawEval(position, bestEval[i][j]);
                    }
                }
            }
        }
    }
    else
    {
        for (const move of window.game.possibleMoves)
        {
            if (isBetterEval(move.eval, bestEval[move.source_col][move.source_row]))
            {
                bestEval[move.source_col][move.source_row] = move.eval;
            }
        }
        for (var i = 0; i < window.game.COLS; i++)
        {
            for(var j = 0; j < window.game.ROWS; j++){
                if (bestEval[i][j] !== -2)
                {
                    if ((window.game.recommendedMoveIndex == -1) ||
                        ((window.game.possibleMoves[window.game.recommendedMoveIndex].target_col == i) && 
                        (window.game.possibleMoves[window.game.recommendedMoveIndex].target_row == j)))
                    {
                        var position = getSquarePosition(j, i);
                        drawEval(position, bestEval[i][j]);
                    }
                }
            }
        }
    }
};

/**
 * returns if new_eval is better than eval
 */
var isBetterEval = function(new_eval, eval) {
    if (new_eval == -1 ) return true;
    if (eval == -2 ) return true;
    if (eval % 2 == 0)
    {
        return (( new_eval > eval) || (new_eval % 2 == 1));
    }
    else
    {
        return (( new_eval < eval) && (new_eval % 2 == 1));
    }
};

/**
 * react on mouseMove over canvas
 */
var mouseMove = function(e) {
    if (window.game.waitingForMove) return;
    var node = getSquareInfo(e.offsetX, e.offsetY);
    window.game.mouseOverRow = node.row;
    window.game.mouseOverCol = node.col;
    window.game.mouseOverType = node.type;
    drawBoard();
};

/**
 * react on mouseClick on canvas
 */
var mouseClick = function(e) {
    if (window.game.waitingForMove) return;
    window.game.squareClicked(window.game.mouseOverRow, window.game.mouseOverCol, window.game.mouseOverType);
    drawBoard();
};

/**
 * checks if a positon in in the bounds of a square
 */
function isPositionOverSquare(mouse_x, mouse_y, square_x, square_y)
{
    let top = square_y;
    let bottom = square_y + PIECE_SIZE;
    let left = square_x;
    let right = square_x + PIECE_SIZE;
    return (mouse_x >= left) && (mouse_x <= right) && (mouse_y >= top) && (mouse_y <= bottom)
}

/**
 * returns information of the node at the coordinates
 */
function getSquareInfo(x_pos, y_pos) {
    for (let i = 0; i < window.game.COLS; i++) {
        for(let j = 0; j < window.game.ROWS; j++){

            let square_position = getSquarePosition(j, i);
            if (isPositionOverSquare(x_pos, y_pos, square_position.x, square_position.y))
            {
                return {
                    row: j,
                    col: i,
                    type: PieceType.EMPTY // not used so far
                };
            }
        }
    }

    for (let i = 0; i < window.game.ROWS; i++)
    {
        if (isPositionOverSquare(x_pos, y_pos, SPARE_BLACKS_X, SPARE_BLACKS_Y + i * PIECE_SIZE)){
            return {
                row: -1,
                col: -1,
                type: (i + 1) * 2
            }
        }

        if (isPositionOverSquare(x_pos, y_pos, SPARE_WHITES_X, SPARE_WHITES_Y + i * PIECE_SIZE)){
            return {
                row: -1,
                col: -1,
                type: (i + 1) * 2 - 1
            }
        }
    }

    return {
        row: -1,
        col: -1,
        type: PieceType.EMPTY
    };
}

/**
 * returns the position of the spare piece with type
 */
function getSpareSquarePosition(type) {
    if (type == PieceType.EMPTY) return;

    let isBlack = (type % 2) == 0;
    let posIndex = Math.ceil(type - (type / 2)) - 1;

    return {
        x: isBlack ? SPARE_BLACKS_X : SPARE_WHITES_X,
        y: posIndex * PIECE_SIZE + SPARE_BLACKS_Y
    }
}

/**
 * returns the position of the square with index
 */
function getSquarePosition(row, col) {
    return {
        x: BOARD_X + col * PIECE_SIZE,
        y: BOARD_Y + row * PIECE_SIZE,
    };
}