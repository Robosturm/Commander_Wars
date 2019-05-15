// this is the base class for terrain
var ACTION =
{
    canBePerformed : function(action)
	{
		return false;
    },
    perform : function(action)
    {
    },
    //
    addItemSelection : function(itemVector, menue, step)
    {
    },
    getActionText : function()
    {
        return qsTr("Unknown");
    },
    getIcon : function()
    {
        return "";
    },
    isFinalStep : function(action)
    {
        // make the action infinite
        return false;
    },

    getActions : function()
    {
        // returns a string id list of the actions this building can perform
        return "";
    },

    getEmptyFieldActions : function()
    {
        return "ACTION_ACTIVATE_POWER_CO_0,ACTION_ACTIVATE_POWER_CO_1,ACTION_ACTIVATE_SUPERPOWER_CO_0," +
               "ACTION_ACTIVATE_SUPERPOWER_CO_1,ACTION_NEXT_PLAYER,ACTION_COINFO,ACTION_VICTORYINFO,ACTION_SAVEGAME,ACTION_EXITGAME";
    },

    getStepInputType : function(action)
    {
        // supported types are MENU and FIELD
        return "";
    },

    getStepCursor : function(action, cursorData)
    {
        return "cursor+default";
    },

    getStepData : function(action, data)
    {
        // type of the data object depends on the StepInputType
    },
};
