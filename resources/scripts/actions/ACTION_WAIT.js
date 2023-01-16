var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        return qsTr("Wait");
    };
    this.getIcon = function(map)
    {
        return "wait";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        Global[unit.getUnitID()].doWalkingAnimation(action, map);
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
    };
    this.getName = function()
    {
        return qsTr("Wait");
    };
    this.getDescription = function()
    {
        return qsTr("Orders a unit to wait at the current position and end all actions for this turn.");
    };
}

Constructor.prototype = ACTION;
var ACTION_WAIT = new Constructor();
