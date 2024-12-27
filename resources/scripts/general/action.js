// this is the base class for terrain
var ACTION =
{
    canBePerformed : function(action, map)
	{
		return false;
    },
    canBePerformed : null,
    perform : function(action, map)
    {
    },
    perform : null,
    //
    addItemSelection : function(itemVector, menue, step)
    {
    },
    addItemSelection : null,
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
    getIcon : null,
    isFinalStep : function(action, map)
    {
        // make the action infinite
        return false;
    },
    isFinalStep : null,
    emptyFieldActions : ["ACTION_TRANSFER_UNIT", "ACTION_SELL_UNIT", "ACTION_INFO",
                         "ACTION_NICKNAME_UNIT", "ACTION_DELETE_UNIT", "ACTION_SURRENDER", "ACTION_DRAWVOTE", "ACTION_FOCUSMAP",
                         "ACTION_OPTIONS", "ACTION_MOVEMENTPLANNER", "ACTION_EXITGAME", "ACTION_LOADGAME", "ACTION_SAVEGAME",
                         "ACTION_ACTIVATE_POWER_CO_0", "ACTION_ACTIVATE_POWER_CO_1", "ACTION_ACTIVATE_SUPERPOWER_CO_0",
                         "ACTION_ACTIVATE_SUPERPOWER_CO_1", "ACTION_ACTIVATE_TAGPOWER",
                         "ACTION_NEXT_PLAYER", "ACTION_SWAP_COS"],
    getEmptyFieldActions : function()
    {
        return ACTION.emptyFieldActions;
    },
    viewplayerActionList : ["ACTION_INFO", "ACTION_OPTIONS"],
    getViewplayerActionList : function()
    {
        return ACTION.viewplayerActionList;
    },

    getStepInputType : function(action, map)
    {
        // supported types are MENU and FIELD
        return "";
    },
    getStepInputType : null,

    getStepCursor : function(action, cursorData, map)
    {
        return "cursor+default";
    },

    getStepData : function(action, data, map)
    {
        // type of the data object depends on the StepInputType
    },
    getStepData : null,

    getDescription : function()
    {
        return "";
    },
    getDescription : null,

    getRequiresEmptyField : function(action, map)
    {
        return true;
    },
    getBannedByDefault : function()
    {
        return false;
    },
    getBannedByDefault : null,
    getSubWikiInfoCount : function()
    {
        return 0;
    },
    getSubWikiInfoCount : null,
    getSubWikiInfoIcon : function(item)
    {
        return "";
    },
    getSubWikiInfoIcon : null,
    getSubWikiInfoName : function(item)
    {
        return "";
    },
    getSubWikiInfoName : null,
    getSubWikiInfoDescription = function(item)
    {
        return "";
    },
    getSubWikiInfoDescription : null,
    getSubWikiInfoId : function(item)
    {
        return "";
    },
    getSubWikiInfoId : null,

    isEmptyFieldAndHasNotMoved : function(action, unit, actionTargetField, targetField, map)
    {
        if (unit.getHasMoved() === false &&
            ((actionTargetField.x === targetField.x && actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null && unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) > 0)))
        {
            return true;
        }
        else
        {
            return false;
        }
    },
};
