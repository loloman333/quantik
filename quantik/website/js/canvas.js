/**
 * Consts
 */
const PIECE_SIZE = 90

/**
 * Reset and resize board, if sizes changed
 */
function resizeBoard(){
    // window.canvas.height = PIECE_SIZE * rows;
    // window.canvas.width = PIECE_SIZE * cols;
    drawBoard();
}

/**
 * Draws all pawns
 */
// function drawPawns(){
//     var cols = document.getElementById("cols").value;
//     var rows = document.getElementById("rows").value;
//     for(var i = 0; i < cols; i++){
//         drawPawn(white_pawn, PIECE_SIZE * i, (rows - 1) * PIECE_SIZE, "null");
//     }
//     for(var i = 0; i < cols; i++){
//         drawPawn(black_pawn, PIECE_SIZE * i, 0, "null");
//     }
// }

/**
 * Initializes all pawns (should only be called once)
 */
function initPieces(){
    // var cols = document.getElementById("cols").value;
    // var rows = document.getElementById("rows").value;

    window.piece_images = {};

    window.piece_images[PieceType.WHITE_SQUARE] = new Image();
    window.piece_images[PieceType.WHITE_SQUARE].src = "images/white_square.png";

    window.piece_images[PieceType.WHITE_CIRCLE] = new Image();
    window.piece_images[PieceType.WHITE_CIRCLE].src = "images/white_circle.png";

    window.piece_images[PieceType.WHITE_TRIANGLE] = new Image();
    window.piece_images[PieceType.WHITE_TRIANGLE].src = "images/white_triangle.png";

    window.piece_images[PieceType.WHITE_DIAMOND] = new Image();
    window.piece_images[PieceType.WHITE_DIAMOND].src = "images/white_diamond.png";

    window.piece_images[PieceType.BLACK_SQUARE] = new Image();
    window.piece_images[PieceType.BLACK_SQUARE].src = "images/black_square.png";

    window.piece_images[PieceType.BLACK_CIRCLE] = new Image();
    window.piece_images[PieceType.BLACK_CIRCLE].src = "images/black_circle.png";

    window.piece_images[PieceType.BLACK_TRIANGLE] = new Image();
    window.piece_images[PieceType.BLACK_TRIANGLE].src = "images/black_triangle.png";

    window.piece_images[PieceType.BLACK_DIAMOND] = new Image();
    window.piece_images[PieceType.BLACK_DIAMOND].src = "images/black_diamond.png";
}

/**
 * Draw one pawn at position with backgroundcolor
 */
var drawPawn = function(image, x, y, color) {
    if(color !== "null"){
        window.context.fillStyle = color;
        window.context.fillRect(x, y, PIECE_SIZE, PIECE_SIZE);
    }
    window.context.drawImage(image, x, y);
};

/**
 * Draw one classification at position
 */
var drawEval = function(position, eval) {
    let x = position.x + PIECE_SIZE * 0.5;
    let y = position.y + PIECE_SIZE * 0.55;

    if (eval.includes("x"))
    {
        window.context.fillStyle = "grey";
    }   
    else if (window.game.recommendedMoveIndex !== -1)
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
    else
    {
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

/**
 * Draw  the board on the canvas
 */
var drawBoard = function() {
    var activePlayer = window.game.turn;
    window.context.clearRect(0, 0, window.canvas.width, window.canvas.height);
    // if (window.game.activeMove.target_row !== -1)
    // {

    for (let [_, val] of Object.entries(PieceType))
    {
        if (val == PieceType.EMPTY) continue;
        let pos = getSpareSquarePosition(val);
        let image = window.piece_images[val];
        drawPawn(image, pos.x, pos.y, BackgroundColor.INACTIVE);
        drawEval(pos, "x2");
    }

    for (var i = 0; i < window.game.cols; i++) {
        for(var j = 0; j < window.game.rows; j++){
            if (window.game.board[i][j] !== PieceType.EMPTY)
            {
                var position = getSquarePosition(j, i);
                var image = window.piece_images[window.game.board[i][j]];
                drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
            }
        }
    }
    // var position = getNodePosition(window.game.activeMove.target_row, window.game.activeMove.target_col);
    // var image = window.piece_images[window.game.board[window.game.activeMove.target_row][window.game.activeMove.target_col]];
    // drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
    // }
    // else if(window.game.activeMove.source_row !== -1)
    // {
    //     for (var i = 0; i < window.game.cols; i++) {
    //         for(var j = 0; j < window.game.rows; j++){
    //             if (window.game.board[i][j] !== NodeColor.EMPTY)
    //             {
    //                 var position = getNodePosition(j, i);
    //                 var image = window.game.board[i][j] == NodeColor.WHITE ? white_pawn : black_pawn;
    //                 drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
    //             }
    //             if ((i == window.game.mouseOverCol) && (j == window.game.mouseOverRow) &&
    //                 (window.game.isMouseOverLegalMove(j, i)))
    //             {
    //                 var position = getNodePosition(j, i);
    //                 var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
    //                 drawPawn(image, position.x, position.y, BackgroundColor.POSSIBLE);
    //             }
    //         }
    //     }
    //     var position = getNodePosition(window.game.activeMove.source_row, window.game.activeMove.source_col);
    //     var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
    //     drawPawn(image, position.x, position.y, BackgroundColor.ACTIVE);
    // }
    // else
    // {
    //     for (var i = 0; i < window.game.cols; i++) {
    //         for(var j = 0; j < window.game.rows; j++) {
    //             if (window.game.board[i][j] !== NodeColor.EMPTY)
    //             {
    //                 var position = getNodePosition(j, i);
    //                 var image = game.board[i][j] == NodeColor.WHITE ? white_pawn : black_pawn;
    //                 var backgroundColor = BackgroundColor.INACTIVE;
    //                 if  (window.game.isMouseOverLegalMove(j, i)){
    //                     backgroundColor = BackgroundColor.POSSIBLE;
    //                 }
    //                 drawPawn(image, position.x, position.y, backgroundColor);
    //             }
    //             if ((i == window.game.mouseOverCol) && (j == window.game.mouseOverRow) &&
    //                 (window.game.isMouseOverLegalMove(j, i)))
    //             {
    //                 var position = getNodePosition(j, i);
    //                 var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
    //                 drawPawn(image, position.x, position.y, BackgroundColor.ACTIVE);
    //             }
    //         }
    //     }
    // }
    // drawEvals();
};

/**
 * Draw all classifications
 */
var drawEvals = function() {
    if (((!window.game.showMoveInfos) || (window.game.waitingForMove)) && (window.game.recommendedMoveIndex == -1) ||
        (window.game.activeMove.target_row !== -1)) return;

    var bestEval = [];
    for(var i = 0; i < window.game.cols; i++){
        var col = []
        for(var j = 0; j < window.game.rows; j++){
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
        for (var i = 0; i < window.game.cols; i++)
        {
            for(var j = 0; j < window.game.rows; j++){
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
        for (var i = 0; i < window.game.cols; i++)
        {
            for(var j = 0; j < window.game.rows; j++){
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
    var node = getNode(e.offsetX, e.offsetY);
    window.game.mouseOverRow = node.row;
    window.game.mouseOverCol = node.col;
    drawBoard();
};

/**
 * react on mouseClick on canvas
 */
var mouseClick = function(e) {
    if (window.game.waitingForMove) return;
    var node = getNode(e.offsetX, e.offsetY);
    window.game.fieldClicked(node.row, node.col);
    drawBoard();
};

/**
 * returns the index of the node at the coordinates
 */
function getNode(x_pos, y_pos) {
    for (var i = 0; i < window.game.cols; i++) {
        for(var j = 0; j < window.game.rows; j++){
            var position = getSquarePosition(j, i);
            var top = position.y;
            var bottom = position.y + PIECE_SIZE;
            var left = position.x;
            var right = position.x + PIECE_SIZE;
            if ((x_pos >= left) && (x_pos <= right) && (y_pos >= top) && (y_pos <= bottom))
            {
                return {
                    row: j,
                    col: i,
                };
            }
        }
    }
    return {
        row: -1,
        col: -1,
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
        x: isBlack ? PIECE_SIZE / 4 : PIECE_SIZE * window.game.cols + PIECE_SIZE * 1.75,
        y: posIndex * PIECE_SIZE + PIECE_SIZE / 4
    }
}

/**
 * returns the position of the square with index
 */
function getSquarePosition(row, col) {
    return {
        x: (PIECE_SIZE + PIECE_SIZE / 2) + col * PIECE_SIZE,
        y: (PIECE_SIZE / 4) + row * PIECE_SIZE,
    };
}