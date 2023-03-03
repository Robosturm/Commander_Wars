var Constructor = function()
{
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["gs", "gs"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (unit.getOwner().getFieldVisibleType(actionTargetField.x, actionTargetField.y) === GameEnums.VisionType_Shrouded)
        {
            return false;
        }
        var capturableBuildings = ACTION_CAPTURE.getCapturableBuildings();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
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
        return false;
    };

    this.getCapturableBuildings = function()
    {
        return ["AIRPORT", "FACTORY", "HARBOUR", "HQ", "LABOR", "MINE",
                "PIPESTATION", "RADAR", "TOWER", "TOWN", "TEMPORARY_AIRPORT",
                "TEMPORARY_HARBOUR", "OILRIG", "POWERPLANT", "AMPHIBIOUSFACTORY"];
    };

    this.getActionText = function(map)
    {
        return qsTr("Capture");
    };
    this.getIcon = function(map)
    {
        return "capture";
    };
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_CAPTURE", "performPostAnimation");
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        ACTION_CAPTURE.postAnimationUnit = unit;
        ACTION_CAPTURE.postAnimationBuilding = action.getMovementBuilding();
        ACTION_CAPTURE.postAnimationTarget = action.getActionTarget();
    };
    this.postAnimationUnit = null;
    this.postAnimationBuilding = null;
    this.postAnimationTarget = null;
    this.performPostAnimation = function(postAnimation, map)
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
            var captureAnimation = GameAnimationFactory.createGameAnimationCapture(map, x , y, capturePoints, ACTION_CAPTURE.postAnimationUnit.getCapturePoints(), maxCapturePoints);
            captureAnimation.addBackgroundSprite("capture_background");
            var armyName = Global.getArmyNameFromPlayerTable(ACTION_CAPTURE.postAnimationUnit.getOwner(), ACTION_CAPTURE.armyData);
            Global[building.getBuildingID()].addCaptureAnimationBuilding(captureAnimation, building, building.getOwner(), ACTION_CAPTURE.postAnimationUnit.getOwner());
            captureAnimation.addSoldierSprite("soldier+" + armyName + "+mask" , ACTION_CAPTURE.postAnimationUnit.getOwner(), GameEnums.Recoloring_Matrix);
            captureAnimation.addSoldierSprite("soldier+" + armyName, ACTION_CAPTURE.postAnimationUnit.getOwner(), GameEnums.Recoloring_None);
        }

        if (captured)
        {
            Global[building.getBuildingID()].onCaptured(building);
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
            // achievements
            var player = ACTION_CAPTURE.postAnimationUnit.getOwner();
            if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
            {
                ACHIEVEMENT_CAPTURED_BUILDING.buildingCaptured(building.getBuildingID());
            }
        }
        // disable unit commandments for this turn
        ACTION_CAPTURE.postAnimationUnit.setHasMoved(true);
        ACTION_CAPTURE.postAnimationUnit = null;
        ACTION_CAPTURE.postAnimationBuilding = null;
        ACTION_CAPTURE.postAnimationTarget = null;
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getName = function()
    {
        return qsTr("Capture");
    };
    this.getDescription = function()
    {
        return qsTr("Orders a unit to capture a building. Once the resistant is reduced to 0 the building  will generate effects for your army based on the captured building.");
    };
}


Constructor.prototype = ACTION;
var ACTION_CAPTURE = new Constructor();
