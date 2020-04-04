ACTION.getEmptyFieldActions2 = ACTION.getEmptyFieldActions;
ACTION.getEmptyFieldActions = function()
{
	return "ACTION_SELL_UNIT," + ACTION.getEmptyFieldActions2();
};