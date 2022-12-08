var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var unitID = "WATERPLANE";
        var costs = unit.getOwner().getCosts(unitID, targetField);
        var funds = unit.getOwner().getFunds();

        var unitLimit = map.getGameRules().getUnitLimit();
        var unitCount = unit.getOwner().getUnitCount();

        if ((unit.getHasMoved() === true) ||
            (costs > funds) ||
            (!unit.hasAmmo2()))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y))
        {
            if (unit.getLoadedUnitCount() < Global[unit.getUnitID()].getLoadingPlace())
            {
                if ((unitLimit <= 0 ||
                    unitCount < unitLimit))
                {
                    return true;
                }
			}
        }
        return false;
        
    };

    this.getActionText = function(map)
    {
        if (map &&
            map !== null)
        {
            var unitID = "WATERPLANE";
            var costs = Global[unitID].getBaseCost();
            if (map.getCurrentPlayer() !== null)
            {
                costs = map.getCurrentPlayer().getCosts(unitID, Qt.point(-1, -1));
            }
            var name = Global[unitID].getName();
            return name + " " + costs.toString();
        }
        else
        {
            return qsTr("Build waterplane")
        }
    };

    this.getIcon = function(map)
    {
        return "WATERPLANE";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };

    this.perform = function(action, map)
    {
        var unitID = "WATERPLANE";
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var player = unit.getOwner();
        var costs = player.getCosts(unitID, unit.getPosition());
        unit.reduceAmmo2(1);
        // pay for the unit
        player.addFunds(-costs);
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID, player.getPlayerID());
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
    this.getName = function()
    {
        return qsTr("Build Waterplane");
    };
    this.getDescription = function()
    {
        return qsTr("Orders an aircraft carrier to produce a waterplane which can be launched next turn.");
    };
}

Constructor.prototype = ACTION;
var ACTION_BUILD_WATERPLANE = new Constructor();
