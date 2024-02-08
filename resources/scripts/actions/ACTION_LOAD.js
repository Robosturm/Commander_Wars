var Constructor = function ()
{
    
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var targetUnit = action.getMovementTarget();
        var transportTerrain = action.getMovementTerrain();
        if (unit.getHasMoved() === false &&
            targetUnit !== null &&
            unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) > 0 &&
            (actionTargetField.x !== targetField.x || actionTargetField.y !== targetField.y))
        {
            if ((targetUnit.getOwner() === unit.getOwner()) &&
                (targetUnit.getTransportUnits().indexOf(unit.getUnitID()) >= 0) &&
                (targetUnit.getLoadedUnitCount() < targetUnit.getLoadingPlace()))
            {
                return ACTION_LOAD.isLoadingTerrain(targetUnit, transportTerrain);
            }
        }
        return false;
    };

    this.isLoadingTerrain = function(transporter, terrain)
    {
        var unitID = transporter.getUnitID();
        var terrainId = terrain.getID();
        if (unitID === "TRANSPORTPLANE")
        {
            if (terrainId === "AIRPORT" ||
                terrainId === "TEMPORARY_AIRPORT" ||
                terrainId === "SNOW_STREET" ||
                terrainId === "STREET" ||
                terrainId === "STREET1" ||
                terrainId === "BRIDGE" ||
                terrainId === "BRIDGE1" ||
                terrainId === "BRIDGE2")
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (unitID === "LANDER" ||
                 unitID === "BLACK_BOAT" ||
                 unitID === "CANNONBOAT")
        {
            if ((terrainId === "BRIDGE") ||
                (terrainId === "BRIDGE1") ||
                (terrainId === "BRIDGE2"))
            {
                return false;
            }
        }
        return true;
    };

    this.getActionText = function(map)
    {
        return qsTr("Load");
    };
    this.getIcon = function(map)
    {
        return "load";
    };
    this.isFinalStep = function (action, map)
    {
        return true;
    };
    this.postAnimationTargetUnit = null;
    this.postAnimationUnit = null;
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        ACTION_LOAD.postAnimationUnit = action.getTargetUnit();
        ACTION_LOAD.postAnimationTargetUnit = action.getMovementTarget();
        if (map.getGameRules().getMoveVision())
        {
            ACTION_LOAD.postAnimationUnit.createMoveVisionFromAction(action);
        }
        var animation = Global[ACTION_LOAD.postAnimationUnit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_LOAD", "performPostAnimation");
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        ACTION_LOAD.postAnimationUnit.setHasMoved(true);
        ACTION_LOAD.postAnimationUnit.setCapturePoints(0);
        ACTION_LOAD.postAnimationTargetUnit.loadUnit(ACTION_LOAD.postAnimationUnit);
        audio.playSound("load.wav");
        ACTION_LOAD.postAnimationTargetUnit = null;
        ACTION_LOAD.postAnimationUnit = null;
    };
    this.getName = function()
    {
        return qsTr("Load");
    };
    this.getDescription = function()
    {
        return qsTr("Loads a unit to a transporters. This makes the unit untargetable for most effects. However the units gets lost when the transporter gets destroyed.");
    };    
}

Constructor.prototype = ACTION;
var ACTION_LOAD = new Constructor();
