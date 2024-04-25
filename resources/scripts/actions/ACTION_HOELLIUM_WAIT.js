var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
		var targetUnit = action.getMovementTarget();
        if (unit.getHasMoved() === false &&
           (actionTargetField.x === targetField.x && actionTargetField.y === targetField.y) ||
           ((targetUnit === null || unit.getOwner().isEnemyUnit(targetUnit)) && unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) > 0))
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
        var targetUnit = action.getMovementTarget();
        if ((targetUnit !== null) && (targetUnit !== unit))
        {
            targetUnit.killUnit();
        }
        Global[unit.getUnitID()].doWalkingAnimation(action, map);
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
    };
    this.getName = function()
    {
        return qsTr("Oozium Wait");
    };
    this.getDescription = function()
    {
        return qsTr("Orders a oozium to wait at the current position and end all actions for this turn. Ooziums can wait on enemy units, destroying the enemy unit in the process.");
    };
    this.getRequiresEmptyField = function(action)
    {
        return false;
    };
}

Constructor.prototype = ACTION;
var ACTION_HOELLIUM_WAIT = new Constructor();
