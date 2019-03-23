var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null))
        {
            if (unit.getHidden() === true)
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Unstealth");
    };
    this.getIcon = function()
    {
        return "unhide";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        Global[unit.getUnitID()].doWalkingAnimation(action);
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        unit.setHidden(false);
    };
}

Constructor.prototype = ACTION;
var ACTION_UNSTEALTH = new Constructor();
