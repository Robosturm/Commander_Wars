ACTION_FLARE.canBePerformed = function(action)
{
    var unit = action.getTargetUnit();
    var actionTargetField = action.getActionTarget();
    var targetField = action.getTarget();
    if (unit.getHasMoved() === true)
    {
        return false;
    }
    if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y))
    {
        if (unit.hasAmmo1())
        {
            return true;
        }
    }
    return false;
}
