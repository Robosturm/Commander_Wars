var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var building = action.getTargetBuilding();
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        var firecount = building.getFireCount();
        if ((firecount === 1) &&
             map.onMap(x, y) && map.getTerrain(x, y).getUnit() === null &&
             ACTION_PRODUCE_OOZIUM_FREE.canBuildUnit(action, map))
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
        return qsTr("Produce Oozium");
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
    this.buildlist = ["HOELLIUM"];

    this.canBuildUnit = function(action, map)
    {
        var building = action.getTargetBuilding();
        var owner = building.getOwner();
        var unitLimit = map.getGameRules().getUnitLimit();
        var unitCount = owner.getUnitCount();
        if ((unitLimit <= 0 ||
             unitCount < unitLimit))
        {
            var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
            var x = building.getX() + offset.x;
            var y = building.getY() + offset.y;
            var buildlist = owner.getBuildList();
            for (var i = 0; i < ACTION_PRODUCE_OOZIUM_FREE.buildlist.length; i++)
            {
                // check all units if they can move over this terrain
                if (buildlist.includes(ACTION_PRODUCE_OOZIUM_FREE.buildlist[i]) &&
                    Global[Global[ACTION_PRODUCE_OOZIUM_FREE.buildlist[i]].getMovementType()].getMovementpoints(map.getTerrain(x, y), null, map.getTerrain(x, y), true, map) > 0 &&
                    ACTION_PRODUCE_OOZIUM_FREE.isCOSpecificUnit(ACTION_PRODUCE_OOZIUM_FREE.buildlist[i]) === false)
                {
                    return true;
                }
            }
        }
        return false;
    };

    this.isCOSpecificUnit = function(unitId)
    {
        if (Global[unitId].getCOSpecificUnit !== null &&
            Global[unitId].getCOSpecificUnit())
        {
            return true;
        }
        return false;
    };

    this.getStepData = function(action, data, map)
    {
        var building = action.getTargetBuilding();
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        var units = map.getAllUnitIDs();
        var terrain = map.getTerrain(x, y);
        for (var i = 0; i < units.length; i++)
        {
            // check all units if they can move over this terrain
            if (ACTION_PRODUCE_OOZIUM_FREE.buildlist.includes(units[i]) &&
                Global[Global[units[i]].getMovementType()].getMovementpoints(terrain, null, terrain, true, map) > 0 &&
                ACTION_PRODUCE_OOZIUM_FREE.isCOSpecificUnit(units[i]) === false)
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
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        building.setFireCount(building.getFireCount() - 1);
        // spawn the unit
        var unit = map.spawnUnit(x, y, unitID, player);
        map.getGameRecorder().buildUnit(player.getPlayerID(), unitID, player.getPlayerID());
        player.buildedUnit(unit);

        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_BUILD_UNIT.unitProduced(unitID);
        }
    };

    this.getIsBaseProductionAction = function(map)
    {
        return true;
    };

    this.getName = function()
    {
        return qsTr("Produce Oozium");
    };

    this.getDescription = function()
    {
        return qsTr("Orders a the black bolt to spit out a new oozium.");
    };
}

Constructor.prototype = ACTION;
var ACTION_PRODUCE_OOZIUM_FREE = new Constructor();
