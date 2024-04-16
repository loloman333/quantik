/**
 * Consts
 */
const PIECE_SIZE = 90

/**
 * Reset and resize board, if sizes changed
 */
function resizeBoard(){
    window.canvas.height = 90 * rows;
    window.canvas.width = 90 * cols;
    window.game.base = 4 * window.game.nCr(rows, 2) + 2 * rows + 1
    drawBoard();
}

/**
 * Draws all pawns
 */
function drawPawns(){
    var cols = document.getElementById("cols").value;
    var rows = document.getElementById("rows").value;
    for(var i = 0; i < cols; i++){
        drawPawn(white_pawn, PIECE_SIZE * i, (rows - 1) * PIECE_SIZE, "null");
    }
    for(var i = 0; i < cols; i++){
        drawPawn(black_pawn, PIECE_SIZE * i, 0, "null");
    }
}

/**
 * Initializes all pawns (should only be called once)
 */
function initPawns(){
    var cols = document.getElementById("cols").value;
    var rows = document.getElementById("rows").value;
    window.white_pawn = new Image();
    window.white_pawn.src = "images/Bauer_Weiss.png";
    window.white_pawn.onload = function(){
        for(var i = 0; i < cols; i++){
            drawPawn(window.white_pawn, PIECE_SIZE * i, (rows - 1) * PIECE_SIZE, "null");
        }
    }
    window.black_pawn = new Image();
    window.black_pawn.src = "images/Bauer_Schwarz.png";
    window.black_pawn.onload = function(){
        for(var i = 0; i < cols; i++){
            drawPawn(window.black_pawn, PIECE_SIZE * i, 0, "null");
        }
    }
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
    if (window.game.recommendedMoveIndex !== -1)
    {
        eval = "!";
        window.context.fillStyle = "red";
        window.context.font = "bold 20px Arial";
        window.context.textAlign = "center";
        window.context.fillText(eval, position.x+10, position.y + 16);
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
    window.context.font = "bold 12px Arial";
    window.context.textAlign = "center";
    window.context.fillText(eval, position.x + 10, position.y + 16);
};

/**
 * Draw  the board on the canvas
 */
var drawBoard = function() {
    var activePlayer = window.game.turn;
    window.context.clearRect(0, 0, window.canvas.width, window.canvas.height);
    if (window.game.activeMove.target_row !== -1)
    {
        for (var i = 0; i < window.game.cols; i++) {
            for(var j = 0; j < window.game.rows; j++){
                if (window.game.board[i][j] !== NodeColor.EMPTY)
                {
                    var position = getNodePosition(j, i);
                    var image = window.game.board[i][j] == NodeColor.WHITE ? white_pawn : black_pawn;
                    drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
                }
            }
        }
        var position = getNodePosition(window.game.activeMove.target_row, window.game.activeMove.target_col);
        var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
        drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
    }
    else if(window.game.activeMove.source_row !== -1)
    {
        for (var i = 0; i < window.game.cols; i++) {
            for(var j = 0; j < window.game.rows; j++){
                if (window.game.board[i][j] !== NodeColor.EMPTY)
                {
                    var position = getNodePosition(j, i);
                    var image = window.game.board[i][j] == NodeColor.WHITE ? white_pawn : black_pawn;
                    drawPawn(image, position.x, position.y, BackgroundColor.INACTIVE);
                }
                if ((i == window.game.mouseOverCol) && (j == window.game.mouseOverRow) &&
                    (window.game.isMouseOverLegalMove(j, i)))
                {
                    var position = getNodePosition(j, i);
                    var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
                    drawPawn(image, position.x, position.y, BackgroundColor.POSSIBLE);
                }
            }
        }
        var position = getNodePosition(window.game.activeMove.source_row, window.game.activeMove.source_col);
        var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
        drawPawn(image, position.x, position.y, BackgroundColor.ACTIVE);
    }
    else
    {
        for (var i = 0; i < window.game.cols; i++) {
            for(var j = 0; j < window.game.rows; j++) {
                if (window.game.board[i][j] !== NodeColor.EMPTY)
                {
                    var position = getNodePosition(j, i);
                    var image = game.board[i][j] == NodeColor.WHITE ? white_pawn : black_pawn;
                    var backgroundColor = BackgroundColor.INACTIVE;
                    if  (window.game.isMouseOverLegalMove(j, i)){
                        backgroundColor = BackgroundColor.POSSIBLE;
                    }
                    drawPawn(image, position.x, position.y, backgroundColor);
                }
                if ((i == window.game.mouseOverCol) && (j == window.game.mouseOverRow) &&
                    (window.game.isMouseOverLegalMove(j, i)))
                {
                    var position = getNodePosition(j, i);
                    var image = activePlayer == NodeColor.WHITE ? white_pawn : black_pawn;
                    drawPawn(image, position.x, position.y, BackgroundColor.ACTIVE);
                }
            }
        }
    }
    drawEvals();
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
                        var position = getNodePosition(j, i);
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
                        var position = getNodePosition(j, i);
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
            var position = getNodePosition(j, i);
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
 * returns the position of the node with index
 */
function getNodePosition(row, col) {
    return {
        x: col * 90,
        y: row * 90,
    };
}