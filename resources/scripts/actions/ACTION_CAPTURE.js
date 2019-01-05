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
            var building = action.getMovementBuilding();
            if (building !== null)
            {

                var alliance = unit.getOwner().checkAlliance(building.getOwner());
                if (alliance === PLAYER.Alliance_Enemy)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Capture");
    };
    this.getIcon = function()
    {
        return "capture";
    };
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        // move unit to target position
        unit.moveUnitAction(action);
        // capture the building
        unit.increaseCapturePoints();
        // check if the capture points are high enough
        if (unit.capturePoints >= 20)
        {
            var building = action.getMovementBuilding();
            building.setUnitOwner(unit);
            unit.capturePoints = 0;
        }
        // disable unit commandments for this turn
        unit.setHasMoved(true);
    };
    this.isFinalStep = function(action)
    {

        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_CAPTURE = new Constructor();
