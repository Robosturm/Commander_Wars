ACTION.getCoWResellEmptyFieldActions = ACTION.getEmptyFieldActions;
ACTION.getEmptyFieldActions = function()
{
	return "ACTION_SELL_UNIT," + ACTION.getCoWResellEmptyFieldActions();
};