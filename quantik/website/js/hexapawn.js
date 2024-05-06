function changeDims() {
    window.cols = document.getElementById("cols").value;
    if (cols == 6) {
        var rows = document.getElementById("rows").value;
        if(rows > 6){
            document.getElementById("rows").value = 6;
        }
        $(".highRows").hide();
    } else if(cols < 6){
        $(".highRows").show();
    }
    window.rows = document.getElementById("rows").value;
    if(!window.game.started){
        window.game.rows = window.rows;
        window.game.cols = window.cols;
        window.game.init();
        resizeBoard();
    }
};

var hideAllActions = function(resetAI) {
	$('#cleargameList').hide();
	$('#cleargameList').html("");
	$('#cleargameList').parent().removeClass("highlight");

	$('#loadgameList').hide();
	$('#loadgameList').html("");
	$('#loadgameList').parent().removeClass("highlight");

	$('#savegameStart').show();
	$('#savegameName').hide();
	$('#savegameCancel').hide();
	$('#savegameConfirm').hide();
	$('#savegameStart').parent().removeClass("highlight");

	$('#board td').removeClass("chip-rec");

	if (resetAI) {
		$('#optionsaia').prop('checked', false);
		window.game.aiA = false;
		$('#optionsaib').prop('checked', false);
		window.game.aiB = false;
		window.game.nextPlayer(false);
		window.game.waitingForMove = false;
	}
};

/**
 * Initializes the client (game object) and sets click functions for all actions on the GUI
 */
$(document).ready(function() {
	window.canvas = document.getElementById("myCanvas");
    window.context = canvas.getContext("2d");
    canvas.onmousemove = mouseMove;
    canvas.onclick = mouseClick;
    window.rows = 3;
    window.cols = 2;
    // canvas.onmousemove = mouseMove;
    // canvas.onclick = mouseClick;
    $(".about").hide();
    // initPawns();
	window.game = new Game();
	window.game.init();
	hideAllActions(true);
	$('#optionsaialevel').val("perfect");
	$('#optionsaiblevel').val("perfect");

    $('#restart').click(function() {
	    var showInfo = window.game.showMoveInfos;
		window.game = new Game();
		window.game.init();
		window.game.showMoveInfos = showInfo;
		hideAllActions(true);
		window.game.aiA = $('#optionsaia').is(':checked');
		window.game.aiB = $('#optionsaib').is(':checked');
		window.game.aiAlevel = $('#optionsaialevel').find("option:selected").val();
		window.game.aiBlevel = $('#optionsaiblevel').find("option:selected").val();
		window.game.nextPlayer(false);
		resizeBoard();
		return false;
	});
    $('#togglemoveinfos').click(function() {
		hideAllActions(true);
		window.game.showMoveInfos = !window.game.showMoveInfos;
		if (window.game.showMoveInfos) {
			$('.histvalue').show();
		} else {
			$('.histvalue').hide();
		}
		drawBoard();
		window.game.network.requestMoveInfo();
		return false;
	});
    $('#moverecommender').click(function() {
		window.game.moveRecommender();
		return false;
	});
    $('#historyrevert').click(function() {
		hideAllActions(true);
		window.game.historyRevert();
		return false;
	});
	$('#historyredo').click(function() {
		hideAllActions(true);
		window.game.historyRedo();
		return false;
	});
	$('#savegameStart').click(function() {
		hideAllActions(true);
		window.game.storage.saveGame("start");
		return false;
	});
	$('#savegameConfirm').click(function() {
		window.game.storage.saveGame("confirm");
		return false;
	});
	$('#savegameCancel').click(function() {
		window.game.storage.saveGame("cancel");
		return false;
	});
	$('#loadgame').click(function() {
		hideAllActions(true);
		window.game.storage.loadGame();
		return false;
	});
	$('#cleargame').click(function() {
		window.game.storage.clearGame();
		return false;
	});
	$('#optionsaia').change(function() {
		window.game.aiA = this.checked;
		window.game.nextPlayer(false);
	});
	$('#optionsaialevel').change(function() {
		window.game.aiAlevel = $(this).find("option:selected").val();
		window.game.nextPlayer(false);
	});
	$('#optionsaib').change(function() {
		window.game.aiB = this.checked;
		window.game.nextPlayer(false);
	});
	$('#optionsaiblevel').change(function() {
		window.game.aiBlevel = $(this).find("option:selected").val();
		window.game.nextPlayer(false);
	});
	$('#rows').change(function() {
		changeDims();
	});
	$('#cols').change(function() {
		changeDims();
	});
	$('#aboutlink').click(function() {
		$('.about').toggle();
		return false;
	});
});