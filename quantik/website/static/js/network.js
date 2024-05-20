/**
 * Initializes new network utility object
 */
Network = function (game, context) {
	this.game = game;
	const data = { username: "example" };
};

/**
 * Ask the server for infos about the current board setting, shown as a hint to the player
 * 
 * May discard a late response from the server, if the timestamp received does not match the timestamp of the last
 * request sent
 */
Network.prototype.requestMoveInfo = function () {
	window.lastRequest = new Date().getTime();
	
	// Compute canonical encodings of all possible following states
	this.encoding_set = new Set();
	for (let moves of window.game.possibleMoves){
		for (let move of moves) {
			let new_board = window.game.board.map((arr) => arr.slice());
			new_board[move.target_row][move.target_col] = move.piece_type;

			let encoding = window.game.encode(compute_canonical(new_board));
			move.encoding = encoding;
			this.encoding_set.add(encoding);
		}
	}

	$.post('/moveinfo', {
		'encodings[]': Array.from(this.encoding_set),
		timestamp: window.lastRequest,
	}).done(
		$.proxy(function (data, textStatus, jqXHR) {
			
			// Check if response belongs to last request, otherwise don't show it
			if (data.timestamp == window.lastRequest) {
				if (data.error) {
					if (this.game.showMoveInfos) {
						window.output.showError(data.error);
					}
				} else if (data.wait) {
					window.output.showError("Server is under high load<br/>", $('<a>').html(
						"Click to retry the move info request").click($.proxy(function () {
							this.requestMoveInfo();
						}, this)));
				} else {

					let enc_code_map = new Map();
					let it = this.encoding_set.values();
					for (let i = 0; i < data.codes.length; i++){
						enc_code_map.set(it.next().value, data.codes[i] + 1);
					}

					for (let i = 0; i < window.game.possibleMoves.length; i++){
						for (let j = 0; j < window.game.possibleMoves[i].length; j++) {
							window.game.possibleMoves[i][j].eval = enc_code_map.get(window.game.possibleMoves[i][j].encoding);
						}
					}

					if (window.game.waitingForMove) {
						window.game.takeAiMove();
					} else {
						drawBoard();
					}
				}
			}
		}, this)).fail($.proxy(function (jqXHR, textStatus, errorThrown) {
			window.output.showError(String.format("Request failed, no network?"));
		}, this));
};
