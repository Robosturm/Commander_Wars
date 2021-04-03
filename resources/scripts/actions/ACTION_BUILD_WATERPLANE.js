var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var unitID = "WATERPLANE";
        var costs = map.getCurrentPlayer().getCosts(unitID);
        var funds = map.getCurrentPlayer().getFunds();

        var unitLimit = map.getGameRules().getUnitLimit();
        var unitCount = unit.getOwner().getUnitCount();


        if ((unit.getHasMoved() === true) ||
            (costs > funds) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0) ||
            (!unit.hasAmmo2()))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y))
        {
            if (unit.getLoadedUnitCount() < Global[unit.getUnitID()].getLoadingPlace())
            {
                if ((unitLimit <= 0) ||
                    (unitCount < unitLimit))
                {
                    return true;
                }
			}
        }
        return false;
        
    };


    this.getActionText = function()
    {
        var unitID = "WATERPLANE";
        var costs = Global[unitID].getBaseCost();
        if (map !== null &&
                map.getCurrentPlayer() !== null)
        {
            costs = map.getCurrentPlayer().getCosts(unitID);
        }
        var name = Global[unitID].getName();
        return qsTr(name + " " + costs.toString());
    };
    this.getIcon = function()
    {
        return "WATERPLANE";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };

    this.perform = function(action)
    {
        var unitID = "WATERPLANE";
        // we need to move the unit to the target position
        var player = map.getCurrentPlayer();
        var costs = player.getCosts(unitID);
        var unit = action.getTargetUnit();
        unit.reduceAmmo2(1);
        // pay for the unit
        player.addFunds(-costs);
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID);
        var spawnUnit = unit.spawnUnit(unitID);
        unit.setHasMoved(true);
        spawnUnit.setHasMoved(true);
        player.buildedUnit(spawnUnit);
        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_BUILD_UNIT.unitProduced(unitID);
        }
    };    
    this.getDescription = function()
    {
        return qsTr("Orders an aircraft carrier to produce a waterplane which can be launched next turn.");
    };
}

Constructor.prototype = ACTION;
var ACTION_BUILD_WATERPLANE = new Constructor();
