/**
 * Object to store a single move by a player (e.g. for history)
 */
function Move(source_row, source_col, target_row, target_col, eval = -1)
{
	this.source_row = source_row;
	this.source_col = source_col;
	this.target_row = target_row;
	this.target_col = target_col;
	this.eval = eval;
};

/**
 * Shows information about the move (player, col, row, value of move) in a list element for history
 */
Move.prototype.toHistoryLi = function(player) {
	var str = String.format("Player {0}", player);
	str = str + String.format(" moved from {0}{1} to {2}{3}.", String.fromCharCode(65 + this.source_col), window.game.rows - this.source_row, 
                                                             String.fromCharCode(65 + this.target_col), window.game.rows - this.target_row);
	if (this.eval !== -1) {
		str = str + String.format("<span class='histvalue'> ({0})</span>", this.getStringForEval(this.eval));
	}
	else
	{
			str = str + String.format("<span class='histvalue'> ({0})</span>", "The server didn't respond in time!");
	}
	return $('<li>').html(str);
};

/**
 * EVAL TEXT
 */
Move.prototype.getStringForEval = function(value) {
  if (value == 0)	return "Draw";
  value--
  if (value == 0)	return "Win!";
  if (value % 2 == 1) return "Lose in " + value;
  return "Win in " + value;
}

/**
 * Store information about the current game
 */
function Game() {
	/**
	 * Number of rows on board
	 */
  this.rows = 4;
	/**
	 * Number of cols on board
	 */
  this.cols = 4;
	/**
	 * Indicated if a move has already been done
	 */
  this.started = false;
	/**
	 * List of the Color of all nodes
	 */
	this.board = [];
  /**
   * Row of the field the mouse is pointing to
   */
  this.mouseOverRow = -1;
  /**
   * Col of the field the mouse is pointing to
   */
  this.mouseOverCol = -1;
  /**
   * Index of the recommended move
   */
  this.recommendedMoveIndex = -1;
  /**
   * The active move
   */
  this.activeMove = new Move(-1, -1, -1, -1);
  /**
   * Array for all possible moves
   */
  this.possibleMoves = [];
  /**
   * Player to make the next move (we are starting with A)
   */
  this.turn = NodeColor.WHITE;
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
Game.prototype.init = function() {
  this.initBoard();
	this.calcMoves();
	this.turn = NodeColor.WHITE;
	this.history = [];
	this.historyForward = [];
  this.activeMove = new Move(-1, -1, -1, -1);
  this.mouseOverRow = -1;
  this.mouseOverCol = -1;

	this.network.requestMoveInfo();
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
Game.prototype.initBoard = function(){
  this.board = [];
  for(var i = 0; i < window.cols; i++){
    var col = [NodeColor.BLACK];
    for(var j = 0; j < window.rows - 2; j++){
      col.push(NodeColor.EMPTY);
    }
    col.push(NodeColor.WHITE);
    this.board.push(col);
  }
}

/**
 * Calculate all possible moves and puts them in this.possibleMoves
 */
Game.prototype.calcMoves = function()
{
  var activePlayer = this.turn;
  var passivePlayer = NodeColor.WHITE;
  if (activePlayer == NodeColor.WHITE)
  {
    passivePlayer = NodeColor.BLACK;
  }
  var moves = [];

  for(var col = 0; col < this.cols; col++){
    if(this.board[col][0] == NodeColor.WHITE || this.board[col][this.rows - 1] == NodeColor.BLACK){
      this.possibleMoves = moves;
      return;
    }
  }

  for(var col = 0; col < this.cols; col++){
    for(var row = 0; row < this.rows; row++){
      if(this.board[col][row] == activePlayer){
        if(activePlayer == NodeColor.WHITE){
          if(col > 0 && this.board[col - 1][row - 1] == passivePlayer)
          {
            var move = new Move(row, col, row - 1, col - 1);
            moves.push(move);
          }
          if(this.board[col][row - 1] == 0)
          {
            var move = new Move(row, col, row - 1, col);
            moves.push(move);
          }
          if(col < this.cols - 1 && this.board[col + 1][row - 1] == passivePlayer)
          {
            var move = new Move(row, col, row - 1, col + 1);
            moves.push(move);
          }
        }
        else
        {
          if(col > 0 && this.board[col - 1][row + 1] == passivePlayer)
          {
            var move = new Move(row, col, row + 1, col - 1);
            moves.push(move);
          }
          if(this.board[col][row + 1] == 0)
          {
            var move = new Move(row, col, row + 1, col);
            moves.push(move);
          }
          if(col < this.cols - 1 && this.board[col + 1][row + 1] == passivePlayer)
          {
            var move = new Move(row, col, row + 1, col + 1);
            moves.push(move);
          }
        }
      }
    }
  }
  this.possibleMoves = moves;
}

/**
 * Takes the move in this.activeMove
 */
Game.prototype.takeMove = function(isNotRedoMove = true)
{
  this.started = true;
  var move = null;
  if (!isNotRedoMove)
  {
    move = this.activeMove;
  }
  else
  {
    for (const possibleMove of this.possibleMoves)
    {
      if ((this.activeMove.source_row == possibleMove.source_row) && (this.activeMove.source_col == possibleMove.source_col) &&
          (this.activeMove.target_row == possibleMove.target_row) && (this.activeMove.target_col == possibleMove.target_col))
      {
        move = possibleMove;
      }
    }
    if(move == null){
      return;
    }
  }

  this.board[move.target_col][move.target_row] = this.turn;
  this.board[move.source_col][move.source_row] = NodeColor.EMPTY;

  this.history.push(move);
  if (isNotRedoMove) this.historyForward = [];

  this.nextPlayer();
}

/**
 * Reverts the last move
 */
Game.prototype.revertMove = function()
{
  var move = this.history.pop();

  var passivePlayer = this.turn;
  var activePlayer = NodeColor.WHITE;
  if (passivePlayer == NodeColor.WHITE)
  {
    activePlayer = NodeColor.BLACK;
  }

  this.board[move.source_col][move.source_row] = activePlayer;
  if(move.target_col != move.source_col){
    this.board[move.target_col][move.target_row] = passivePlayer;
  }else{
    this.board[move.target_col][move.target_row] = NodeColor.EMPTY;
  }

  this.historyForward.push(move);
  this.nextPlayer();
}

/**
 * Reacts to a node being clicked. Expands this.activeMove, takes the move if it is a full possible move
 */
Game.prototype.fieldClicked = function(row, col){
  var newMove = new Move(this.activeMove.source_row, this.activeMove.source_col, this.activeMove.target_row, this.activeMove.target_col)
  if (newMove.source_row == -1)
  {
    newMove.source_row = row;
    newMove.source_col = col;
  }
  else if (newMove.source_row == row && newMove.source_col == col)
  {
    this.activeMove = new Move(-1, -1, -1, -1);
    this.mouseOverIndex = -1;
    window.output.showInfo("");
    return;
  }
  else
  {
    newMove.target_row = row;
    newMove.target_col = col;
  }
  if (this.isLegalMove(newMove))
  {
    this.activeMove = newMove;
    this.recommendedMoveIndex = -1;
    window.game.takeMove();
    window.output.showInfo("");
  }
}
/**
 * Returns if the move is a legal move
 */
Game.prototype.isMouseOverLegalMove = function(row, col){
  var newMove = new Move(this.activeMove.source_row, this.activeMove.source_col, this.activeMove.target_row, this.activeMove.target_col)
  if (newMove.source_row == -1)
  {
    newMove.source_row = row;
    newMove.source_col = col;
  }
  else
  {
    newMove.target_row = row;
    newMove.target_col = col;
  }
  return (this.isLegalMove(newMove))
}

/**
 * Returns if the move is a legal move
 */
Game.prototype.isLegalMove = function(move){
  for (const possMove of this.possibleMoves)
  {
    if ((possMove.source_row == move.source_row) && (possMove.source_col == move.source_col) && (move.target_row == -1)) return true;
    if ((possMove.source_row == move.source_row) && (possMove.source_col == move.source_col) &&
        (possMove.target_row == move.target_row) && (possMove.target_col == move.target_col)) return true;
  }
  return false
}

/**
 * Returns if the recommended move is still possible
 */
Game.prototype.recommendationPossible= function(){
  return (((this.activeMove.source_row == -1) ||
          ((this.activeMove.source_row == this.possibleMoves[this.recommendedMoveIndex].source_row) &&
           (this.activeMove.source_col == this.possibleMoves[this.recommendedMoveIndex].source_col))) &&
          ((this.activeMove.target_row == -1) ||
          ((this.activeMove.target_row == this.possibleMoves[this.recommendedMoveIndex].target_row) &&
           (this.activeMove.target_col == this.possibleMoves[this.recommendedMoveIndex].target_col))));
}

/**
 * Give the player a hint, which column to choose for her next move
 */
Game.prototype.moveRecommender = function() {
  if (this.recommendedMoveIndex == -1)
  {
    this.nextPlayer(false);
    var move = this.getBestMove(false);
    for (var i = 0; i < this.possibleMoves.length; i++) {
      var possibleMove = this.possibleMoves[i]
      if ((move.source_row == possibleMove.source_row) && (move.source_col == possibleMove.source_col) && 
          (move.target_row == possibleMove.target_row) && (move.target_col == possibleMove.target_col))
      {
        this.recommendedMoveIndex = i;
        drawBoard();
        return;
      }
    }
  }
};

/**
 * Hands the turn to the next player, may be AI based. advance = false: Just refresh the AI request
 */
Game.prototype.nextPlayer = function(advance = true) {
  if (advance)
  {
    if (this.turn == NodeColor.WHITE)
    {
      this.turn = NodeColor.BLACK;
    }else
    {
      this.turn = NodeColor.WHITE;
    }
    this.calcMoves();
  }
  if ((this.aiA && this.turn == NodeColor.WHITE) || (this.aiB && this.turn == NodeColor.BLACK))
  {
    // AI on the move
    this.waitingForMove = true;
  }
  else
  {
    this.waitingForMove = false;
  }

  this.recommendedMoveIndex = -1;
  this.activeMove =  new Move(-1, -1, -1, -1);
  this.mouseOverIndex = -1
  drawBoard();
  this.network.requestMoveInfo();
  window.output.showHistoryList();
  window.output.showHistoryRedoList();
  window.output.showInfo("");
};

/**
 * lets the AI take a move
 */
Game.prototype.takeAiMove = function() {
  var aiLevel = this.turn == NodeColor.WHITE ? this.aiAlevel : this.aiBlevel;
	if (aiLevel == LevelEnum.Random) {
	  var move =  this.possibleMoves[Math.floor(Math.random() * this.possibleMoves.length)]
  }
  else if (aiLevel == LevelEnum.Perfect)
  {
    var move = this.getBestMove(false);
  }
  else
  {
    var move = this.getBestMove(true);
  }
  this.activeMove = move;
  this.takeMove();
};

/**
 * returns the best move (or a random winning move is just_win = true)
 */
Game.prototype.getBestMove = function(just_win) {
  var winMoves = [];
  var loseMoves = [];
  var noEval = [];
  var best_winning_eval = 255
  var best_loosing_eval = 0
  for (move of this.possibleMoves)
  {
    if(move.eval == -1){
      noEval.push(move);
    }
    if (move.eval % 2 == 1)
    {
      if (move.eval <=best_winning_eval)
      {
        best_winning_eval = move.eval;
      }
      winMoves.push(move);
    }
    else
    {
      if (move.eval >= best_loosing_eval)
      {
        best_loosing_eval = move.eval;
      }
      loseMoves.push(move);
    }
  }
  if (winMoves.length > 0)
  {
    if (just_win)
    {
      return winMoves[Math.floor(Math.random() * winMoves.length)];
    }
    for (move of winMoves){
      if (move.eval == best_winning_eval)
      {
        return move
      }
    }
  }
  for (move of loseMoves){
    if (move.eval == best_loosing_eval)
    {
      return move;
    }
  }
  return noEval[0];
};

/**
 * Tries to revert the last move (the last entry in the history list)
 */
Game.prototype.historyRevert = function() {
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
Game.prototype.historyRedo = function() {
	if (this.historyForward.length == 0) {
		window.output.showError("No move to redo");
		return;
	}
	this.activeMove = this.historyForward.pop();
	this.takeMove(false);
	window.output.showInfo("Move redone");
};

/**
 * Encodes the board into an integer representing the state
 */
Game.prototype.encode = function(state) {
  // var result = 0;
  // for(i = 0; i < cols; i++){
  //   var found = false;
  //   var start = 0;
  //   for(j = 0; j < rows; j++) {
  //     if (this.board[i][j] == 1) {
  //       for (k = 0; k < rows - j - 1; k++) {
  //         if (this.board[i][j + k + 1] == 1) {
  //           result += (start + k + 1 + 2 * rows) * Math.pow(this.base, cols - 1 - i);
  //           found = true;
  //           break;
  //         }
  //         if (this.board[i][j + k + 1] == 2) {
  //           result += (start + k + 1 + 2 * rows + this.nCr(rows, 2)) * Math.pow(this.base, cols - 1 - i);
  //           found = true;
  //           break;
  //         }
  //       }
  //       if (!found) {
  //         result += (j + 1) * Math.pow(this.base, cols - 1 - i);
  //       }
  //       break;
  //     } else if (this.board[i][j] == 2) {
  //       for (k = 0; k < rows - j - 1; k++) {
  //         if (this.board[i][j + k + 1] == 1) {
  //           result += (start + k + 1 + 2 * rows + 2 * this.nCr(rows, 2)) * Math.pow(this.base, cols - 1 - i);
  //           found = true;
  //           break;
  //         }
  //         if (this.board[i][j + k + 1] == 2) {
  //           result += (start + k + 1 + 2 * rows + 3 * this.nCr(rows, 2)) * Math.pow(this.base, cols - 1 - i);
  //           found = true;
  //           break;
  //         }
  //       }
  //       if (!found) {
  //         result += (j + 1 + rows) * Math.pow(this.base, cols - 1 - i);
  //       }
  //       break;
  //     }
  //     start += (rows - j - 1);
  //   }
  // }
  // TODO
  return 100;
}

/**
 * Calculates n choose r
 */
Game.prototype.nCr = function(n, r) {
  return this.factorial(n) / (this.factorial(r) * this.factorial(n - r));
}

/**
 * Calculates n factorial
 */
Game.prototype.factorial = function(n) {
  var result = 1;
  for(i = 2; i <= n; i++){
    result *= i;
  }
  return result;
}
