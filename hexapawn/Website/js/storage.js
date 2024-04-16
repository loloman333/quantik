/**
 * Initializes new storage utility object
 */
function Storage(game) {
	this.game = game;
};

/**
 * Returns whether local storage (HTML5) is supported by the browser
 */
Storage.prototype.isStorageSupported = function() {
	try {
		return ('localStorage' in window) && (window['localStorage'] !== null);
	} catch (e) {
		return false;
	}
};

/**
 * Saves the current game in HTML5 local storage
 */
Storage.prototype.saveGame = function(action) {
	if (!this.isStorageSupported()) {
		window.output.showError("Local storage is not supported by your browser");
		hideAllActions();
		return;
	}

	if (action == "start") {
		// Let the user enter a name for the game to save
		$('#savegameStart').hide();
		$('#savegameName').show();
		$('#savegameCancel').show();
		$('#savegameConfirm').show();
		$('#savegameStart').parent().addClass("highlight");

		var d = new Date();
		$('#savegameName').val(
				String.format("{0.4}-{1.2}-{2.2} {3.2}:{4.2}", d.getFullYear(), d.getMonth(), d.getDate(),
						d.getHours(), d.getMinutes()));
		return;
	} else if (action == "cancel") {
		// User has canceled the save action
		hideAllActions();
		return;
	} else if (action == "confirm") {
		// Really save the game (user has entered a name and confirmed)
		hideAllActions();

		var savedGames = this.getNumberSavedGames(1);
		var idx = this.getFreeIdx();
		if (idx == savedGames)
			savedGames++

		var gameName = $('#savegameName').val();
		if (gameName == undefined || gameName.length == 0) {
			gameName = "Game " + freeIdx;
		}

		localStorage["ninemensmorris.games." + idx + ".name"] = gameName;
		localStorage["ninemensmorris.games." + idx + ".rows"] = JSON.stringify(this.game.rows);
		localStorage["ninemensmorris.games." + idx + ".cols"] = JSON.stringify(this.game.cols);
		localStorage["ninemensmorris.games." + idx + ".started"] = JSON.stringify(this.game.started);
		localStorage["ninemensmorris.games." + idx + ".board"] = JSON.stringify(this.game.board);
        localStorage["ninemensmorris.games." + idx + ".turn"] = JSON.stringify(this.game.turn);
		localStorage["ninemensmorris.games." + idx + ".showMoveInfos"] = JSON.stringify(this.game.showMoveInfos);
		localStorage["ninemensmorris.games." + idx + ".history"] = JSON.stringify(this.game.history);
		localStorage["ninemensmorris.games." + idx + ".historyForward"] = JSON.stringify(this.game.historyForward);

		localStorage["ninemensmorris.nSavedGames"] = savedGames;

		window.output.showInfo(String.format("Game saved as '{0}'", gameName));
	}
};

/**
 * Returns the next free idx in the local storage
 */
Storage.prototype.getFreeIdx = function() {
	var maxIdx = this.getNumberSavedGames(1);

	for ( var idx = 1; idx <= maxIdx; idx++) {
		var gameName = localStorage["ninemensmorris.games." + idx + ".name"];
		if (gameName == undefined || gameName.length == 0) {
			return idx;
		}
	}
	return maxIdx;
};

/**
 * Reads number of saved games in local storage
 */
Storage.prototype.getNumberSavedGames = function(defaultVal) {
	var savedGamesStr = localStorage["ninemensmorris.nSavedGames"];
	return savedGamesStr == undefined ? defaultVal : parseInt(savedGamesStr, 10);
};

/**
 * Really deletes a single game with given index from the local storage
 */
Storage.prototype.clearSingleGameHandler = function(idx) {
	var gameName = localStorage["ninemensmorris.games." + idx + ".name"];
	localStorage["ninemensmorris.games." + idx + ".name"] = "";
	localStorage["ninemensmorris.games." + idx + ".rows"] = "";
	localStorage["ninemensmorris.games." + idx + ".cols"] = "";
	localStorage["ninemensmorris.games." + idx + ".started"] = "";
	localStorage["ninemensmorris.games." + idx + ".board"] = "";
	localStorage["ninemensmorris.games." + idx + ".turn"] = "";
	localStorage["ninemensmorris.games." + idx + ".showMoveInfos"] = "";
	localStorage["ninemensmorris.games." + idx + ".history"] = "";
	localStorage["ninemensmorris.games." + idx + ".historyForward"] = "";
	hideAllActions();
	window.output.showInfo(String.format("Game '{0}' deleted", gameName));
	return false;
};

/**
 * Lists stored games in HTML5 local storage, with onclick function to delete that game
 */
Storage.prototype.clearGame = function() {
	if (!this.isStorageSupported()) {
		window.output.showError("Local storage is not supported by your browser");
		hideAllActions();
		return;
	}

	// Toggle display of list of games
	if ($('#cleargameList').is(':visible')) {
		hideAllActions();
		return;
	}
	hideAllActions();

	var maxIdx = this.getNumberSavedGames(0);
	var nGames = 0;

	for ( var idx = 1; idx <= maxIdx; idx++) {
		var gameName = localStorage["ninemensmorris.games." + idx + ".name"];
		if (gameName == undefined || gameName.length == 0) {
			continue;
		}
		nGames++;
		var func = $.proxy(this.clearSingleGameHandler, this, idx);
		$('#cleargameList').append($('<li>').html($('<a>').html(gameName).attr("href", "#").click(func)));
	}

	if (maxIdx == 0 || nGames == 0) {
		window.output.showError("No game saved yet");
	} else {
		// Sort list of shown games on GUI
		$('#cleargameList').children("li").sort(function(a, b) {
			var upA = $(a).text().toUpperCase();
			var upB = $(b).text().toUpperCase();
			return (upA < upB) ? -1 : (upA > upB) ? 1 : 0;
		}).appendTo('#cleargameList');
		$('#cleargameList').parent().addClass("highlight");
		$('#cleargameList').show();
	}
}

/**
 * Really loads a single game with specified index from the local storage
 */
Storage.prototype.loadSingleGameHandler = function(idx) {
	var gameName = localStorage["ninemensmorris.games." + idx + ".name"];
	// this.game.rows = JSON.parse(localStorage["ninemensmorris.games." + idx + ".rows"]);
	// this.game.cols = JSON.parse(localStorage["ninemensmorris.games." + idx + ".cols"]);
	document.getElementById("rows").value = JSON.parse(localStorage["ninemensmorris.games." + idx + ".rows"]);
	document.getElementById("cols").value = JSON.parse(localStorage["ninemensmorris.games." + idx + ".cols"]);
	changeDims();

	this.game.started = JSON.parse(localStorage["ninemensmorris.games." + idx + ".started"]);
	this.game.board = JSON.parse(localStorage["ninemensmorris.games." + idx + ".board"]);
	this.game.turn = JSON.parse(localStorage["ninemensmorris.games." + idx + ".turn"]);
	this.game.showMoveInfos = localStorage["ninemensmorris.games." + idx + ".showMoveInfos"] == "true";
	this.game.aiA = false;
	this.game.aiAlevel = LevelEnum.Perfect;
	this.game.aiB = false;
	this.game.aiBlevel = LevelEnum.Perfect;

	var hist = JSON.parse(localStorage["ninemensmorris.games." + idx + ".history"]);
	var histRedo = JSON.parse(localStorage["ninemensmorris.games." + idx + ".historyForward"]);
	this.game.history = [];
	for ( var i = 0; i < hist.length; ++i) {
		this.game.history.push(new Move(hist[i].source_row, hist[i].source_col, hist[i].target_row, hist[i].target_col, hist[i].eval));
	}
	window.output.showHistoryList();
	this.game.historyForward = [];
	for ( var i = 0; i < histRedo.length; ++i) {
		this.game.historyForward.push(new Move(histRedo[i].source_row, histRedo[i].source_col, histRedo[i].target_row, histRedo[i].target_col, histRedo[i].eval));
	}
	window.output.showHistoryRedoList();

	hideAllActions();
	window.output.showInfo(String.format("Game '{0}' loaded", gameName));

	if (this.game.showMoveInfos) {
		$('.histvalue').show();
	} else {
		$('.histvalue').hide();
	}
	this.game.network.requestMoveInfo();
	this.game.calcMoves();
	drawBoard();
	return false;
};

/**
 * Lists stored games in HTML5 local storage, with onclick function to load that game
 */
Storage.prototype.loadGame = function() {
	if (!this.isStorageSupported()) {
		window.output.showError("Local storage is not supported by your browser");
		hideAllActions();
		return;
	}
	// Toggle display of list of games
	if ($('#loadgameList').is(':visible')) {
		hideAllActions();
		return;
	}
	hideAllActions();

	var maxIdx = this.getNumberSavedGames(0);
	var nGames = 0;

	for ( var idx = 1; idx <= maxIdx; idx++) {
		var gameName = localStorage["ninemensmorris.games." + idx + ".name"];
		if (gameName == undefined || gameName.length == 0) {
			continue;
		}
		nGames++;
		var func = $.proxy(this.loadSingleGameHandler, this, idx);
		$('#loadgameList').append($('<li>').html($('<a>').html(gameName).attr("href", "#").click(func)));
	}

	if (maxIdx == 0 || nGames == 0) {
		window.output.showError("No game saved yet");
	} else {
		$('#loadgameList').children("li").sort(function(a, b) {
			var upA = $(a).text().toUpperCase();
			var upB = $(b).text().toUpperCase();
			return (upA < upB) ? -1 : (upA > upB) ? 1 : 0;
		}).appendTo('#loadgameList');
		$('#loadgameList').parent().addClass("highlight");
		$('#loadgameList').show();
	}
};
