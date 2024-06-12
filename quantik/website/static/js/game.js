/**
 * Object to store a single move by a player (e.g. for history)
 */
function Move(target_row, target_col, piece_type, eval = -1, encoding = -1) {
    this.piece_type = piece_type;
    this.target_row = target_row;
    this.target_col = target_col;
    this.eval = eval;
    this.encoding = encoding;
};

Move.prototype.compute_all_moves = function () {
    let moves = [];
    for (let row = 0; row < 4; row++) {
        for (let col = 0; col < 4; col++) {
            for (let type = 1; type < Object.keys(PieceType).length; type++) {
                let move = new Move(row, col, type);
                moves.push(move);
            }
        }
    }

    return moves;
};

/**
 * Shows information about the move (player, col, row, value of move) in a list element for history
 */
Move.prototype.toHistoryLi = function (player) {
    var str = String.format("Player {0}", player);

    let color_string = Object.keys(PieceColor).find(key => PieceColor[key] === getPieceColor(this.piece_type));
    let shape_string = Object.keys(PieceShape).find(key => PieceShape[key] === getPieceShape(this.piece_type));

    str = str + String.format(" placed a {0} {1} on x:{2} y:{3}.", color_string, shape_string, this.target_col + 1, this.target_row + 1);

    if (this.eval !== -1) {
        str = str + String.format("<span class='histvalue'> ({0})</span>", this.getStringForEval(this.eval));
    }
    else {
        str = str + String.format("<span class='histvalue'> ({0})</span>", "The server didn't respond in time!");
    }

    return $('<li>').html(str);
};

/**
 * EVAL TEXT
 */
Move.prototype.getStringForEval = function (value) {
    if (value == 21) return "Draw";
    if (value == 0) return "Win!";
    if (value % 2 == 1) return "Win in " + value;
    return "Loose in " + value;
}

/**
 * Store information about the current game
 */
function Game() {
    /**
     * Holds indices of squares that are currently part of a winning position
     */
    this.win_squares = [];
    /**
     * Number of rows on board
     */
    this.ROWS = 4;
    /**
     * Number of cols on board
     */
    this.COLS = 4;
    /**
     * Indicated if a move has already been done
     */
    this.started = false;
    /**
     * List of the Pieces of all squares
     */
    this.board = [];
    /**
     * List of the Pieces of all squares
     */
    this.pieceCounter = [16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,]
    /**
     * Row of the square the mouse is pointing to
     */
    this.mouseOverRow = -1;
    /**
     * Col of the square the mouse is pointing to
     */
    this.mouseOverCol = -1;
    /**
     * Type of the piece the mouse is pointing to
     */
    this.mouseOverType = PieceType.EMPTY;
    /**
     * Index of the recommended move
     */
    this.recommendedMove = false;
    /**
     * The active move
     */
    this.activeMove = new Move(-1, -1, PieceType.EMPTY, -1);
    /**
     * All moves
     */
    this.allMoves = Move.prototype.compute_all_moves();
    /**
     * Array for all possible moves
     */
    this.possibleMoves = [];
    /**
     * Player to make the next move (we are starting with A)
     */
    this.turn = PieceColor.WHITE;
    /**
     * List of Move objects to revert
     */
    this.history = [];
    /**
     * List of Move objects to redo (moves previously undone from history)
     */
    this.historyForward = [];
    /**
     * Whether to show the move infos on the board (and in history list)
     */
    this.showMoveInfos = true;
    /**
     * Whether the first player (A) should be controlled by AI
     */
    this.aiA = false;
    /**
     * Level of the AI for the first player (A)
     */
    this.aiAlevel = LevelEnum.Perfect;
    /**
     * Whether the second player (B) should be controlled by AI
     */
    this.aiB = false;
    /**
     * Level of the AI for the second player (B)
     */
    this.aiBlevel = LevelEnum.Perfect;
    /**
     * Whether we are waiting for the AI to make a move
     */
    this.waitingForMove = false;

    window.output = new Output(this);
    this.storage = new Storage(this);
    this.network = new Network(this);
};

/**
 * Initializes an empty board
 */
Game.prototype.init = function () {
    this.initBoard();
    this.calcMoves();
    this.turn = PieceColor.WHITE;
    this.history = [];
    this.historyForward = [];
    this.activeMove = new Move(-1, -1, PieceType.EMPTY, -1);
    this.mouseOverRow = -1;
    this.mouseOverCol = -1;

    // this.network.requestMoveInfo();
    window.output.showHistoryList();
    window.output.showHistoryRedoList();
    window.output.showInfo("");

    var d = new Date();
    $('#gamedate').html(
        String.format("Game has started at {0.4}-{1.2}-{2.2} {3.2}:{4.2}:{5.2}", d.getFullYear(), d.getMonth(), d
            .getDate(), d.getHours(), d.getMinutes(), d.getSeconds()));

};

/**
 * initializes the board to match the chosen row and col and sets pieces to their starting position
 */
Game.prototype.initBoard = function () {
    this.board = [];
    for (var i = 0; i < this.ROWS; i++) {
        var row = [];
        for (var j = 0; j < this.COLS; j++) {
            row.push(PieceType.EMPTY);
        }
        this.board.push(row);
    }
}

/**
 * Returns if the move is a legal move
 */
Game.prototype.getBoardSector = function (rowIndex, colIndex) {
    let startRow = Math.floor(rowIndex / 2) * 2;
    let startCol = Math.floor(colIndex / 2) * 2;

    return [
        {x: startRow , y: startCol},
        {x: startRow + 1 , y: startCol},
        {x: startRow , y: startCol + 1},
        {x: startRow + 1 , y: startCol + 1}
    ];
}

/**
 * Returns if the move is a legal move
 */
Game.prototype.isLegalMove = function (move) {
    // Try to place empty Piece
    if (move.piece_type == PieceType.EMPTY) return false;

    // Try to place outside of board
    if (move.target_col >= 4 || move.target_row >= 4) return false;
    if (move.target_col <= -1 || move.target_row <= -1) return false;

    // Try to place piece of wrong color
    let piece_color = getPieceColor(move.piece_type);
    if (this.turn != piece_color) return false;

    // Try to place on top of another piece
    if (this.board[move.target_row][move.target_col] != PieceType.EMPTY) return false;

    // Try to place a third of a shape
    if (this.pieceCounter[move.piece_type] >= 2) return false;

    // Try to place in row, col or sector where opponent has one of same shape
    let counterpart = (piece_color == PieceColor.BLACK ? move.piece_type - 1 : move.piece_type + 1);

    for (var row = 0; row < this.ROWS; row++) {
        for (var col = 0; col < this.COLS; col++) {

            if (this.board[row][col] != counterpart) continue;

            // Row
            if (row == move.target_row) return false;

            // Col
            if (col == move.target_col) return false;
        }
    }

    // Sector
    for (let square of this.getBoardSector(move.target_row, move.target_col)) {
        if (square == counterpart) return false;
    }

    // Try to play on a final board
    if (this.isFinished()) return false;

    return true;
}

/**
 * Takes a piece and returns its shape
 */
Game.prototype.getPieceShape = function (piece) {
    return Math.ceil(piece / 2);
}

/**
 * Checks if one player has already won
 */
Game.prototype.isFinished = function () {
    if (this.win_squares.length != 0) return true;

    for (let i = 0; i < this.ROWS; i++) {
        let shapes_in_row = new Set();
        let shapes_in_col = new Set();

        for (let j = 0; j < this.COLS; j++) {

            let shape_1 = this.getPieceShape(this.board[i][j]);
            let shape_2 = this.getPieceShape(this.board[j][i]);

            if (shape_1 != PieceShape.NONE) shapes_in_row.add(shape_1);
            if (shape_2 != PieceShape.NONE) shapes_in_col.add(shape_2);
        }

        if (shapes_in_col.size == 4){
            this.win_squares.push(
                {x: i, y: 0},
                {x: i, y: 1},
                {x: i, y: 2},
                {x: i, y: 3},
            );
        }

        if (shapes_in_row.size == 4) {
            this.win_squares.push(
                {x: 0, y: i},
                {x: 1, y: i},
                {x: 2, y: i},
                {x: 3, y: i},
            );
        }
    }

    let sectors = [this.getBoardSector(0, 0), this.getBoardSector(3, 0), this.getBoardSector(0, 3), this.getBoardSector(3, 3)]
    for (let sector of sectors) {
        let shapes_in_sector = new Set();

        for (let indices of sector) {
            
            let shape = this.getPieceShape(this.board[indices.x][indices.y]);
            if (shape != PieceShape.NONE) shapes_in_sector.add(shape);
        }

        if (shapes_in_sector.size == 4){
            this.win_squares.push.apply(this.win_squares, sector);
        }
    }

    return this.win_squares.length != 0;
}

/**
 * Calculate all possible moves and puts them in this.possibleMoves
 */
Game.prototype.calcMoves = function () {
    var activePlayer = this.turn;

    var moves = [];
    for (let _ = 0; _ < Object.keys(PieceType).length; _++) moves.push([]);

    // Win Cond
    if (this.isFinished()) {
        this.possibleMoves = moves;
        return;
    }

    for (let move of this.allMoves) {
        if (this.isLegalMove(move)) moves[move.piece_type].push(move);
    }

    this.possibleMoves = moves;
}

/**
 * Takes the move in this.activeMove
 */
Game.prototype.takeMove = function (isRedoMove = false) {
    this.recommendedMove = false;

    this.started = true;
    let move = this.activeMove;

    this.board[move.target_row][move.target_col] = move.piece_type;
    this.pieceCounter[PieceType.EMPTY]--;
    this.pieceCounter[move.piece_type]++;

    this.history.push(move);
    if (!isRedoMove) this.historyForward = [];

    this.nextPlayer();
}

/**
 * Reverts the last move
 */
Game.prototype.revertMove = function () {
    var move = this.history.pop();
    this.win_squares = [];

    this.board[move.target_row][move.target_col] = PieceType.EMPTY;
    this.pieceCounter[move.piece_type] -= 1;

    this.historyForward.push(move);
    this.nextPlayer();
}

/**
 * Reacts to a square being clicked. Expands this.activeMove, takes the move if it is a full possible move
 */
Game.prototype.squareClicked = function (row, col, type) {

    if (row == -1) {
        let shape = this.activeMove.piece_type == PieceType.EMPTY ? false : this.activeMove.piece_type;
        let best_move_with_piece = this.getBestMove(shape);

        if (this.activeMove.piece_type == type) this.activeMove.piece_type = PieceType.EMPTY;
        else if (best_move_with_piece !== undefined &&
            getPieceColor(best_move_with_piece.piece_type) == getPieceColor(type) &&
            this.pieceCounter[type] < 2) this.activeMove.piece_type = type;

        if (this.recommendedMove) {
            this.recommendedMove = best_move_with_piece;
        }
    } else {
        if (this.activeMove.piece_type != PieceType.EMPTY) {
            this.activeMove.target_row = row;
            this.activeMove.target_col = col;
        }
    }

    if (this.isLegalMove(this.activeMove)) {
        let eval = this.possibleMoves[this.activeMove.piece_type].find(move => move.target_row == row && move.target_col == col).eval;
        this.activeMove.eval = eval;

        window.game.takeMove();
        window.output.showInfo("");
    }
    window.output.showTurn();
}

/**
 * Give the player a hint, which column to choose for her next move
 */
Game.prototype.moveRecommender = function () {
    if (this.recommendedMove) {
        this.recommendedMove = false;
    } else {
        let shape = this.activeMove.piece_type == PieceType.EMPTY ? false : this.activeMove.piece_type
        this.recommendedMove = this.getBestMove(shape);
    }
    drawBoard();
};

/**
 * Hands the turn to the next player, may be AI based. advance = false: Just refresh the AI request
 */
Game.prototype.nextPlayer = function (advance = true) {
    if (advance) {
        this.turn = this.turn == PieceColor.BLACK ? PieceColor.WHITE : PieceColor.BLACK
        this.calcMoves();
    }

    // AI to move ?
    this.waitingForMove = (this.aiA && this.turn == PieceColor.WHITE) || (this.aiB && this.turn == PieceColor.BLACK);

    this.activeMove = new Move(-1, -1, PieceType.EMPTY, -1);
    drawBoard();
    this.network.requestMoveInfo();
    window.output.showHistoryList();
    window.output.showHistoryRedoList();
    window.output.showInfo("");
};

/**
 * lets the AI take a move
 */
Game.prototype.takeAiMove = function () {
    var aiLevel = this.turn == PieceColor.WHITE ? this.aiAlevel : this.aiBlevel;
    if (aiLevel == LevelEnum.Random) {
        let possibleMoves = [];
        for (let moves of this.possibleMoves) {
            possibleMoves.push.apply(possibleMoves, moves);
        }
        var move = possibleMoves[Math.floor(Math.random() * possibleMoves.length)]
    }
    else if (aiLevel == LevelEnum.Perfect) {
        var move = this.getBestMove(false, false);
    }
    else {
        var move = this.getBestMove(false, true);
    }
    this.activeMove = move;
    this.takeMove();
};

/**
 * returns the best move (or a random winning move is just_win = true)
 */
Game.prototype.getBestMove = function (piece_type = false, just_win = false) {
    let winMoves = [];
    let loseMoves = [];
    let noEval = [];
    let drawMoves = [];

    let best_winning_eval = 17
    let best_loosing_eval = 0

    for (let moves of this.possibleMoves) {
        for (let move of moves) {

            if (piece_type !== false && move.piece_type != piece_type) continue;

            if (move.eval == 21) {
                drawMoves.push(move)
            } else if (move.eval == -1) {
                noEval.push(move);
            } else if (move.eval % 2 == 1) {
                if (move.eval <= best_winning_eval) {
                    best_winning_eval = move.eval;
                }
                winMoves.push(move);
            }
            else {
                if (move.eval >= best_loosing_eval) {
                    best_loosing_eval = move.eval;
                }
                loseMoves.push(move);
            }
        }
    }

    if (winMoves.length > 0) {
        if (just_win) {
            return winMoves[Math.floor(Math.random() * winMoves.length)];
        }
        for (let move of winMoves) {
            if (move.eval == best_winning_eval) {
                return move
            }
        }
    }

    for (let move of drawMoves) {
        return move
    }

    for (let move of loseMoves) {
        if (move.eval == best_loosing_eval) {
            return move;
        }
    }

    return noEval[0];
};

/**
 * Tries to revert the last move (the last entry in the history list)
 */
Game.prototype.historyRevert = function () {
    this.recommendedMove = false;
    if (this.history.length == 0) {
        window.output.showError("No move to revert");
        return;
    }
    this.aiA = false;
    this.aiB = false;
    this.revertMove();
    window.output.showInfo("Move reverted");
};

/**
 * Tries to redo a move (the last entry in the history-forward list)
 */
Game.prototype.historyRedo = function () {
    this.recommendedMove = false;
    if (this.historyForward.length == 0) {
        window.output.showError("No move to redo");
        return;
    }
    this.activeMove = this.historyForward.pop();
    this.takeMove(true);
    window.output.showInfo("Move redone");
};

/**
 * Encodes the board into an integer representing the state
 */
Game.prototype.encode = function (board = this.board) {
    let enc = 0;
    let mult = 1;
    for (let i = this.ROWS - 1; i >= 0; i--) {
        for (let j = this.COLS - 1; j >= 0; j--) {
            enc += board[i][j] * mult;
            mult *= 9;
        }
    }
    return enc;
}