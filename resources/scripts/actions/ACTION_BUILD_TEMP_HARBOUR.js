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
            if (terrain.getID() === "BEACH")
            {
                return true;
            }
        }
        return false;
    };

    this.getActionText = function()
    {
        return qsTr("Build Harbour");
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
        var armyName = unit.getOwner().getArmy().toLowerCase();
        // bh and bg have the same sprites
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "os") &&
            (armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        Global["TEMPORARY_HARBOUR"].addCaptureAnimationBuilding(captureAnimation, building, null, unit.getOwner());
        captureAnimation.addSoldierSprite("soldier+" + armyName + "+mask" , unit.getOwner(), GameEnums.Recoloring_Matrix);

        animation.queueAnimation(captureAnimation);

        if (captured)
        {
            var terrain = map.getTerrain(actionTargetField.x, actionTargetField.y);
            terrain.loadBuilding("TEMPORARY_HARBOUR");
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
        return qsTr("Orders an apc to build a harbour on a beach which can be used to resupply sea units. The harbour will reduce the material of the apc by 1.");
    };
}


Constructor.prototype = ACTION;
var ACTION_BUILD_TEMP_HARBOUR = new Constructor();
