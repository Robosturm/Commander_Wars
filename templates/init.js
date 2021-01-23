// template for an cow automation script of any sort not all menu's and objects are supported here for now
var Init =
{
	main = function(menu)
	{
		menu.enterSingleplayer();
	},
	
	mapsSelection = function(menu)
	{
		menu.selectMap("maps/2_player/", "Agitated.map");
		menu.slotButtonNext();
		menu.slotButtonNext();
		var selection = menu.getPlayerSelection();
		selection.selectPlayerAi(0, 5);
		selection.selectPlayerAi(1, 5);
		menu.startGame();
	},
	
	onVictory = function(menu)
	{
		menu.exitMenue();
	},
}