var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var building = action.getTargetBuilding();
        var x = building.getX() - 2;
        var y = building.getY() + 1;
        var firecount = building.getFireCount();
        if ((firecount === 1 || firecount === 3 || firecount === 5 || firecount === 7) &&
                map.onMap(x, y) && map.getTerrain(x, y).getUnit() === null &&
                ACTION_BLACKHOLEFACTORY_DOOR1.canBuildUnit(action))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Build Door 1");
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

    this.getStepInputType = function(action)
    {
        // supported types are MENU and FIELD
        if (action.getInputStep() === 0)
        {
            return "MENU";
        }
        return "";
    };

    this.canBuildUnit = function(action)
    {
        var building = action.getTargetBuilding();
        var x = building.getX() - 2;
        var y = building.getY() + 1;
        var buildlist = building.getOwner().getBuildList();
        var units = map.getAllUnitIDs();
        for (var i = 0; i < units.length; i++)
        {
            // check all units if they can move over this terrain
            if (buildlist.includes(units[i]) &&
                Global[Global[units[i]].getMovementType()].getMovementpoints(map.getTerrain(x, y), null, map.getTerrain(x, y), true) > 0 &&
                Global[units[i]].getCOSpecificUnit() === false)
            {
                return true;
            }
        }
        return false;
    };

    this.getStepData = function(action, data)
    {
        var building = action.getTargetBuilding();
        var x = building.getX() - 2;
        var y = building.getY() + 1;
        var units = map.getAllUnitIDs();
        var buildlist = building.getOwner().getBuildList();
        for (var i = 0; i < units.length; i++)
        {
            // check all units if they can move over this terrain
            if (buildlist.includes(units[i]) &&
                Global[Global[units[i]].getMovementType()].getMovementpoints(map.getTerrain(x, y), null, map.getTerrain(x, y), true) > 0 &&
                Global[units[i]].getCOSpecificUnit() === false)
            {
                var name = Global[units[i]].getName();
                data.addData(name, units[i], units[i], 0, true);
            }
        }
    }

    this.perform = function(action)
    {
        action.startReading();
        var unitID = action.readDataString();
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        var player = building.getOwner();
        var x = building.getX() - 2;
        var y = building.getY() + 1;
        building.setFireCount(building.getFireCount() - 1);
        // spawn the unit
        var unit = map.spawnUnit(x, y, unitID, player);
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID);
        player.buildedUnit(unit);

        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_BUILD_UNIT.unitProduced(unitID);
        }
    };
    this.getDescription = function()
    {
        return qsTr("Orders a Black Hole Factory to produce a unit at the first door. ") +
               qsTr("The unit is able to move immediatly.");
    };
}

Constructor.prototype = ACTION;
var ACTION_BLACKHOLEFACTORY_DOOR1 = new Constructor();
