ACTION_TRAP.isTrap = function(action, moveUnit, targetFieldUnit, targetX, targetY, previousX, previousY, moveCost, map)
{
    // used to determine if a trap is in the move path.
    // the engine takes care of checking the path in the correct order and cutting the path.
    if (targetFieldUnit !== null)
    {
        var moveUnitType = moveUnit.getUnitType();
        var targetFieldUnitType = targetFieldUnit.getUnitType();
        if ((targetFieldUnit.isStealthed(moveUnit.getOwner()) &&
             ((moveUnitType === targetFieldUnitType) || (moveUnitType !== GameEnums.UnitType_Air && targetFieldUnitType !== GameEnums.UnitType_Air)) &&
             !targetFieldUnit.getIgnoreUnitCollision()) ||
             moveCost < 0)
        {
            return true;
        }
    }
    return false;
};
