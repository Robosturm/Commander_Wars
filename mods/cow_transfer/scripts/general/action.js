ACTION.getCoWTransferUnitEmptyFieldActions = ACTION.getEmptyFieldActions;
ACTION.getEmptyFieldActions = function()
{
	return "ACTION_TRANSFER_UNIT," + ACTION.getCoWTransferUnitEmptyFieldActions();
};