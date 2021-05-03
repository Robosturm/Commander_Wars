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
    getName : function(id)
    {
        return Global[id].getActionText();
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
               "ACTION_ACTIVATE_SUPERPOWER_CO_1,ACTION_ACTIVATE_TAGPOWER,ACTION_INFO," +
               "ACTION_NICKNAME_UNIT,ACTION_DELETE_UNIT,ACTION_SURRENDER," +
               "ACTION_OPTIONS,ACTION_EXITGAME,ACTION_SAVEGAME,ACTION_NEXT_PLAYER,ACTION_SWAP_COS";
    },

    getViewplayerActionList : function()
    {
        return ["ACTION_INFO", "ACTION_OPTIONS"];
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

    getDescription : function()
    {
        return "";
    },

    getRequiresEmptyField : function(action)
    {
        return true;
    },
};
