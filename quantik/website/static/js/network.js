/**
 * Initializes new network utility object
 */
Network = function (game, context) {
	this.game = game;
	const data = { username: "example" };
	// fetch('http://localhost:5000', {
	// 	method: 'POST', 
	// 	headers: {
	// 		"Content-Type": "application/json",
	// 	},
	// 	body: JSON.stringify(data),
	// }).then(response => response.json()).then(text => console.log(text))
};

/**
 * Ask the server for infos about the current board setting, shown as a hint to the player
 * 
 * May discard a late response from the server, if the timestamp received does not match the timestamp of the last
 * request sent
 */
Network.prototype.requestMoveInfo = function () {
	window.lastRequest = new Date().getTime();
	console.log("ahaj")
	$.post('/moveinfo', {
		encoding: this.game.encode(),
		timestamp: window.lastRequest,
	}).done(
		$.proxy(function (data, textStatus, jqXHR) {
			console.log("return")
			
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

					console.log("moveinfo", window.lastRequest, data.code)

					// var i = 0;
					// $.each(data.moveInfos, $.proxy(function (index, value) {
					// 	this.game.possibleMoves[index].eval = value;
					// }, this));

					// if (window.game.waitingForMove) {
					// 	window.game.takeAiMove();
					// } else {
					// 	drawBoard();
					// }
				}
			}
		}, this)).fail($.proxy(function (jqXHR, textStatus, errorThrown) {
			window.output.showError(String.format("Request failed, no network?"));
		}, this));
};
