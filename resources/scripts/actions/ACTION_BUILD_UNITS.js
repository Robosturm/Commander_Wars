var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var building = action.getTargetBuilding();
        var unitLimit = map.getGameRules().getUnitLimit();
        var units = building.getConstructionList();
        if ((unit === null) &&
            (building !== null) &&
            (units.length > 0))
		{
            var unitCount = building.getOwner().getUnitCount();
            if ((unitLimit <= 0) ||
                (unitCount < unitLimit))
            {
                return true;
            }
		}
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Build");
    };
    this.getIcon = function()
    {
        return "build";
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    };


    this.perform = function(action)
    {
        action.startReading();
        var unitID = action.readDataString();
        var player = map.getCurrentPlayer();
        var unit = map.spawnUnit(action.getTarget().x, action.getTarget().y, unitID, player);
        // pay for the unit
        map.getCurrentPlayer().addFunds(-action.getCosts());
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID);
        unit.setHasMoved(true);
        player.buildedUnit(unit);
        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_BUILD_UNIT.unitProduced(unitID);
        }
    };

    this.getStepInputType = function(action)
    {
        // supported types are MENU and FIELD
        if (action.getInputStep() === 0)
        {
            return "MENU";
        }
        return "";
    };

    this.getStepData = function(action, data)
    {
        var building = action.getTargetBuilding();
        var units = building.getConstructionList();
        var funds = map.getCurrentPlayer().getFunds();
        for (i = 0; i < units.length; i++)
        {
            var name = Global[units[i]].getName();
            var costs = map.getCurrentPlayer().getCosts(units[i]);
            var enabled = false;
            if (costs <= funds)
            {
                enabled = true;
            }
            data.addData(name + " " + costs.toString(), units[i], units[i], costs, enabled);
        }
    };
}

Constructor.prototype = ACTION;
var ACTION_BUILD_UNITS = new Constructor();
