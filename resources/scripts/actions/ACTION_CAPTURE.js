var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0) ||
            (unit.getOwner().getFieldVisibleType(actionTargetField.x, actionTargetField.y) === GameEnums.VisionType_Shrouded))
        {
            return false;
        }
        var capturableBuildings = ACTION_CAPTURE.getCapturableBuildings();
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null))
        {
            var building = action.getMovementBuilding();
            if (building !== null)
            {
                var alliance = unit.getOwner().checkAlliance(building.getOwner());
                if ((alliance === GameEnums.Alliance_Enemy) &&
                    (capturableBuildings.indexOf(building.getBuildingID()) >= 0))
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

    this.getCapturableBuildings = function()
    {
        return ["AIRPORT", "FACTORY", "HARBOUR", "HQ", "LABOR", "MINE",
                "PIPESTATION", "RADAR", "TOWER", "TOWN", "TEMPORARY_AIRPORT",
                "TEMPORARY_HARBOUR", "OILRIG"];
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
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_CAPTURE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        ACTION_CAPTURE.postAnimationUnit = unit;
        ACTION_CAPTURE.postAnimationBuilding = action.getMovementBuilding();
        ACTION_CAPTURE.postAnimationTarget = action.getActionTarget();
    };
    this.postAnimationUnit = null;
    this.postAnimationBuilding = null;
    this.postAnimationTarget = null;
    this.performPostAnimation = function(postAnimation)
    {
        var maxCapturePoints = 20;
        // capture the building
        var capturePoints = ACTION_CAPTURE.postAnimationUnit.getCapturePoints();
        var building = ACTION_CAPTURE.postAnimationBuilding;
        ACTION_CAPTURE.postAnimationUnit.increaseCapturePoints(ACTION_CAPTURE.postAnimationTarget);
        var captured = false;
        // check if the capture points are high enough
        if (ACTION_CAPTURE.postAnimationUnit.getCapturePoints() >= maxCapturePoints)
        {
            captured = true;
            ACTION_CAPTURE.postAnimationUnit.setCapturePoints(0);
        }
        var targetX = ACTION_CAPTURE.postAnimationTarget.x;
        var targetY = ACTION_CAPTURE.postAnimationTarget.y;
        var viewPlayer = map.getCurrentViewPlayer();
        if (viewPlayer === ACTION_CAPTURE.postAnimationUnit.getOwner() || viewPlayer.getFieldVisible(targetX, targetY))
        {
            var x = targetX * map.getImageSize() - 10;
            var y = targetY * map.getImageSize() - 30;
            var captureAnimation = GameAnimationFactory.createGameAnimationCapture(x , y, capturePoints, ACTION_CAPTURE.postAnimationUnit.getCapturePoints(), maxCapturePoints);
            captureAnimation.addBackgroundSprite("capture_background");
            var armyName = ACTION_CAPTURE.postAnimationUnit.getOwner().getArmy().toLowerCase();
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
            var color;
            if (building.getOwner() === null)
            {
                color = "#FFFFFF";
            }
            else
            {
                color = building.getOwner().getColor();
            }
            Global[building.getBuildingID()].addCaptureAnimationBuilding(captureAnimation, building, color, ACTION_CAPTURE.postAnimationUnit.getOwner().getColor());
            captureAnimation.addSoldierSprite("soldier+" + armyName + "+mask" , ACTION_CAPTURE.postAnimationUnit.getOwner().getColor(), true);
            captureAnimation.addSoldierSprite("soldier+" + armyName , ACTION_CAPTURE.postAnimationUnit.getOwner().getColor(), false);
        }

        if (captured)
        {
            if (building.getBuildingID() === "HQ")
            {
                map.getGameRecorder().addSpecialEvent(ACTION_CAPTURE.postAnimationUnit.getOwner().getPlayerID(),
                                                      GameEnums.GameRecord_SpecialEvents_HQCaptured);
                if (building.getOwner() !== null)
                {
                    map.getGameRecorder().addSpecialEvent(building.getOwner().getPlayerID(),
                                                          GameEnums.GameRecord_SpecialEvents_HQLost);
                }
            }
            building.setUnitOwner(ACTION_CAPTURE.postAnimationUnit);
        }
        // disable unit commandments for this turn
        ACTION_CAPTURE.postAnimationUnit.setHasMoved(true);
        ACTION_CAPTURE.postAnimationUnit = null;
        ACTION_CAPTURE.postAnimationBuilding = null;
        ACTION_CAPTURE.postAnimationTarget = null;
    };
    this.isFinalStep = function(action)
    {

        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_CAPTURE = new Constructor();
