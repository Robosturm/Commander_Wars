var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var building = action.getTargetBuilding();
        var x = building.getX();
        var y = building.getY();
        var firecount = building.getFireCount();
        if ((firecount === 1) &&
             map.getTerrain(x, y).getUnit() === null &&
             ACTION_NEST_FACTORY_DOOR.canBuildUnit(action, map))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        return qsTr("Build Unit");
    };
    this.getIcon = function(map)
    {
        return "build";
    };
    this.isFinalStep = function(action, map)
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

    this.getStepInputType = function(action, map)
    {
        // supported types are MENU and FIELD
        if (action.getInputStep() === 0)
        {
            return "MENU";
        }
        return "";
    };

    this.canBuildUnit = function(action, map)
    {
        var building = action.getTargetBuilding();
        var owner = building.getOwner();
        var unitLimit = map.getGameRules().getUnitLimit();
        var unitCount = owner.getUnitCount();
        if ((unitLimit <= 0 ||
             unitCount < unitLimit))
        {
            var buildlist = owner.getBuildList();
            var units = map.getAllUnitIDs();
            for (var i = 0; i < units.length; i++)
            {
                // check all units if they can move over this terrain
                if (buildlist.includes(units[i]) &&
                    Global[units[i]].getUnitType() !== GameEnums.UnitType_Naval &&
                    Global[units[i]].getCOSpecificUnit() === false)
                {
                    return true;
                }
            }
        }
        return false;
    };

    this.getStepData = function(action, data, map)
    {
        var building = action.getTargetBuilding();
        var units = map.getAllUnitIDs();
        var buildlist = building.getOwner().getBuildList();
        for (var i = 0; i < units.length; i++)
        {
            // check all units if they can move over this terrain
            if (buildlist.includes(units[i]) &&
                Global[units[i]].getUnitType() !== GameEnums.UnitType_Naval &&
                Global[units[i]].getCOSpecificUnit() === false)
            {
                var name = Global[units[i]].getName();
                data.addData(name, units[i], units[i], 0, true);
            }
        }
    }

    this.perform = function(action, map)
    {
        action.startReading();
        var unitID = action.readDataString();
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        var player = building.getOwner();
        var x = building.getX();
        var y = building.getY();
        building.setFireCount(building.getFireCount() - 1);
        // spawn the unit
        var unit = map.spawnUnit(x, y, unitID, player, 0, true);
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID, player.getPlayerID());
        player.buildedUnit(unit);

        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_BUILD_UNIT.unitProduced(unitID);
        }
    };
    this.getName = function(map)
    {
        return qsTr("Build unit with nest");
    };
    this.getDescription = function()
    {
        return qsTr("Orders the Nest to produce a none naval unit.  The unit is able to move immediately.");
    };
}

Constructor.prototype = ACTION;
var ACTION_NEST_FACTORY_DOOR = new Constructor();
