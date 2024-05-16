/**
 * Simple String formatting with {0} and {1}, ...
 * based on http://stackoverflow.com/a/4673436
 */
if (!String.format) {
	String.format = function (format) {
		var args = Array.prototype.slice.call(arguments, 1);
		return format.replace(/{(\d+)(?:\.(\d+))?}/g, function (match, number, intFormat) {
			if (args[number] != 'undefined') {
				if (intFormat != undefined && intFormat.length > 0) {
					return ('0' + args[number].toString(10)).substr(-intFormat);
				}
				return args[number];
			}
			return match;
		});
	};
};

/**
 * New unique ID for this client, to detect old requests from the same client on the server
 */
var uuid = (function () {
	/**
	 * Generates a string to look like a globally unique identifier,
	 * based on http://stackoverflow.com/a/105074
	 */
	function s4() {
		return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
	}
	return s4() + s4() + '-' + s4() + '-' + s4() + '-' + s4() + '-' + s4() + s4() + s4();
})();

/**
 * Enum for the type of all pieces
 */
var PieceType = {
	EMPTY: 0,
	WHITE_SQUARE: 1,
	BLACK_SQUARE: 2,
	WHITE_TRIANGLE: 3,
	BLACK_TRIANGLE: 4,
	WHITE_CIRCLE: 5,
	BLACK_CIRCLE: 6,
	WHITE_DIAMOND: 7,
	BLACK_DIAMOND: 8
};

/**
 * Enum for the color of a piece
 */
var PieceColor = {
	NONE: 0,
	WHITE: 1,
	BLACK: 2
};

/**
 * Enum for the color of a piece
 */
var PieceShape = {
	NONE: 0,
	SQUARE: 1,
	TRIANGLE: 2,
	CIRCLE: 3,
	DIAMOND: 4
};

function getPieceColor(piece_type){
	if (piece_type == PieceType.EMPTY) return PieceColor.NONE;
	return piece_type % 2 == 1 ? 1 : 2;
}

/**
 * Enum for the color of the boarders of the pieces
 */
var BackgroundColor = {
	INACTIVE: "transparent",
	ACTIVE: "grey",
	// HOVER: "gainsboro",
	POSSIBLE: "yellow"
};

/**
 * Enum for level of AI:
 * Perfect: Always makes the best move available (random only if several moves have the same value)
 * JustWin: Makes a random winning move, no matter the value of it (or draw or best loosing move)
 * Random:  Don't look at the values, just choose a non-empty column (does not ask the server)
 */
var LevelEnum = {
	Perfect: "perfect",
	JustWin: "justwin",
	Random: "random"
};