var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if ((((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y)) ||
            (action.getMovementTarget() === null)) &&
             unit.hasAmmo1())
        {
            var terrain = map.getTerrain(actionTargetField.x, actionTargetField.y);
            if (terrain.getID() === "PLAINS")
            {
                return true;
            }
        }
        return false;
    };

    this.getActionText = function()
    {
        return qsTr("Build Airport");
    };
    this.getIcon = function()
    {
        return "build";
    };
    this.perform = function(action)
    {
        var maxCapturePoints = 20;
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        // move unit to target position
        unit.moveUnitAction(action);
        // capture the building
        var capturePoints = unit.getCapturePoints();
        var building = action.getMovementBuilding();
        unit.increaseCapturePoints(action.getActionTarget());
        var captured = false;
        // check if the capture points are high enough
        if (unit.getCapturePoints() >= maxCapturePoints)
        {
            captured = true;
            unit.setCapturePoints(0);
        }
        var x = action.getActionTarget().x * map.getImageSize() - 10;
        var y = action.getActionTarget().y * map.getImageSize() - 30;
        var captureAnimation = GameAnimationFactory.createGameAnimationCapture(x , y, capturePoints, unit.getCapturePoints(), maxCapturePoints);
        captureAnimation.addBackgroundSprite("capture_background");
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), ACTION_CAPTURE.armyData);
        Global["TEMPORARY_AIRPORT"].addCaptureAnimationBuilding(captureAnimation, building, null, unit.getOwner());
        captureAnimation.addSoldierSprite("soldier+" + armyName + "+mask" , unit.getOwner(), GameEnums.Recoloring_Matrix);
        captureAnimation.addSoldierSprite("soldier+" + armyName , unit.getOwner(), GameEnums.Recoloring_None);

        animation.queueAnimation(captureAnimation);

        if (captured)
        {
            var terrain = map.getTerrain(actionTargetField.x, actionTargetField.y);
            terrain.loadBuilding("TEMPORARY_AIRPORT");
            terrain.getBuilding().setUnitOwner(unit);
            unit.reduceAmmo1(1);
        }
        // disable unit commandments for this turn
        unit.setHasMoved(true);
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Orders an apc to build an airport on a plain which can be used to resupply air units. The airport will reduce the material of the apc by 1.");
    };
}


Constructor.prototype = ACTION;
var ACTION_BUILD_TEMP_AIRPORT = new Constructor();
