/**
 * Initializes new output utility object
 */
function Output(game) {
	this.game = game;
};

/**
 * Shows the moves that can be "redone" as a history list
 */
Output.prototype.showHistoryRedoList = function() {
	$('#historyForward').html("");
	for ( var i = 0; i < this.game.historyForward.length; ++i) {
		$('#historyForward').append(this.game.historyForward[this.game.historyForward.length - i - 1].toHistoryLi(i % 2 +1));
	}
	if (window.game.showMoveInfos) {
		$('.histvalue').show();
	} else {
		$('.histvalue').hide();
	}
};

/**
 * Shows the moves that can be "undone"/"reverted" as a history list
 */
Output.prototype.showHistoryList = function() {
	$('#history').html("");
	for ( var i = 0; i < this.game.history.length; ++i) {
		$('#history').append(this.game.history[i].toHistoryLi(i % 2 +1));
	}
	if (window.game.showMoveInfos) {
		$('.histvalue').show();
	} else {
		$('.histvalue').hide();
	}
};

/**
 * Shows which player's turn it is
 */
Output.prototype.showTurn = function() {
	if (this.game.possibleMoves.length == 0) {
	    let player_won = !(this.game.turn == 2) + 1;
		$('#gameinfo').html(
				String.format("Player {0} has won!", player_won));
		window.lastRequest = new Date().getTime();
	}
	else if (window.game.waitingForMove)
	{
	      $('#gameinfo').html(
                String.format("Player {0} waiting for AI move!", this.game.turn));
	}
	else
	{
        if (this.game.activeMove.source_row == -1)
		{
			$('#gameinfo').html(
				String.format("Player {0} please select a piece to move!", this.game.turn));
		}else
		{
			$('#gameinfo').html(
				String.format("Player {0} please select a field to move to!",
				this.game.turn));
		}
	}
};

/**
 * Shows a #error message, therefore clearing the #info message; and the current turn.
 * May include a retry link to append to the error message
 */
Output.prototype.showError = function(message, elementAppend) {
	$('#messages').hide();
	$('#error').html(message);
	if (elementAppend)
		$('#error').append(elementAppend);
	$('#info').html("");
	if (message.length > 0)
		$('#messages').show();
	this.showTurn();
};

/**
 * Shows a #info message, therefore clearing the #error message; and the current turn
 */
Output.prototype.showInfo = function(message) {
	$('#messages').hide();
	$('#error').html("");
	$('#info').html(message);
	if (message.length > 0)
		$('#messages').show();
	this.showTurn();
};
