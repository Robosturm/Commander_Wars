var Constructor = function()
{
	// needs to be first in case of HQ replacing
    this.getRuleDescription = function(itemNumber)
    {
        return qsTr("A player loses when he lost his last HQ. All buildings will be owned by the current player. A captured HQ and neutral HQ's will be converted into a town.")
    };
    this.getRuleName = function(itemNumber)
    {
        return qsTr("No HQ Single HQ");
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return "checkbox"
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        return 0;
    };
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
        var playerCount = map.getPlayerCount();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName = "Active" + playerID.toString();
            var variables = rule.getVariables();
            var variable = variables.createVariable(variableName);
            if (player.getBuildingCount("HQ") > 0)
            {
                variable.writeDataBool(true);
            }
            else
            {
                variable.writeDataBool(false);
            }
            var variableHqXsName = "HqXs" + playerID.toString();
            var variableHqYsName = "HqYs" + playerID.toString();
            var variableHqXs = variables.createVariable(variableHqXsName);
            var variableHqYs = variables.createVariable(variableHqYsName);
            var buildings = player.getBuildings();
            var pointsX = [];
            var pointsY = [];
            for (var i2 = 0; i2 < buildings.size(); i2++)
            {
                var building = buildings.at(i2);
                if (building.getBuildingID() === "HQ")
                {
                    pointsX.push(building.getX());
                    pointsY.push(building.getY());
                }
            }
            variableHqXs.writeDataListInt32(pointsX);
            variableHqYs.writeDataListInt32(pointsY);
            buildings.remove();
        }
    };
    this.checkHQCount = function(rule, player)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var variableHqXsName = "HqXs" + playerID.toString();
        var variableHqYsName = "HqYs" + playerID.toString();
        var variableHqXs = variables.createVariable(variableHqXsName);
        var variableHqYs = variables.createVariable(variableHqYsName);
        var pointsX = variableHqXs.readDataListInt32();
        var pointsY = variableHqYs.readDataListInt32();
        var buildings = player.getBuildings();
        for (var i2 = 0; i2 < buildings.size(); i2++)
        {
            var building = buildings.at(i2);
            if (building.getBuildingID() === "HQ")
            {
                var found = false;
                var x = building.getX();
                var y = building.getY();
                for (var i = 0; i < pointsX.length; i++)
                {
                    if (pointsX[i] === x &&
                        pointsY[i] === y)
                    {
                        found = true;
                        break;
                    }
                }
                if (found === false)
                {
                    var terrain = building.getTerrain();
                    terrain.loadBuilding("TOWN");
                    terrain.getBuilding().setOwner(player);
                }
            }
        }
        buildings.remove();
        var value = variable.readDataBool();
        if (value === false)
        {
            if (player.getBuildingCount("HQ") > 0)
            {
                variable.writeDataBool(true);
                value = true;
            }
        }
        return value;
    };
    this.replaceNeutrals = function()
    {
        var buildings = map.getBuildings(null);
        for (var i = 0; i < buildings.size(); i++)
        {
            var building = buildings.at(i);
            if (building.getBuildingID() === "HQ")
            {
                var terrain = building.getTerrain();
                terrain.loadBuilding("TOWN");
            }
        }
        buildings.remove();
    }
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        if (player.getPlayerID() === 0)
        {
            VICTORYRULE_1NOHQ_TOWN_REPLACE.replaceNeutrals()
        }
        if (VICTORYRULE_1NOHQ_TOWN_REPLACE.checkHQCount(rule, player))
        {
            if (player.getBuildingCount("HQ") <= 0)
            {
                return GameEnums.DefeatType_ByCurrentPlayer;
            }
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player)
    {
        return player.getBuildingCount("HQ");
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_1NOHQ_TOWN_REPLACE = new Constructor();
