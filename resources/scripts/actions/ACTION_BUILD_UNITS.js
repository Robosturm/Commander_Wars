var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var building = action.getTargetBuilding();
        if ((unit === null) && building !== null)
		{
			return true;
		}
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
        var unit = map.spawnUnit(action.getTarget().x, action.getTarget().y, unitID, map.getCurrentPlayer());
        // pay for the unit
        map.getCurrentPlayer().addFonds(-action.getCosts());
        unit.setHasMoved(true);
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
        var units = Global[building.getBuildingID()].getConstructionList();
        for (i = 0; i < units.length; i++)
        {
            var name = Global[units[i]].getName();
            var costs = Global[units[i]].getBaseCost();
            // todo modify costs
            var enabled = false;
            var fonds = map.getCurrentPlayer().getFonds();
            if (costs <= fonds)
            {
                enabled = true;
            }
            data.addData(name + " " + costs.toString(), units[i], units[i], costs, enabled);
        }
    };
}

Constructor.prototype = ACTION;
var ACTION_BUILD_UNITS = new Constructor();
