var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        return qsTr("A player loses when he loses his last unit.")
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        if (itemNumber === 0)
        {
            return qsTr("No units");
        }
        else
        {
            return qsTr("Ignore watermines");
        }
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["checkbox", "checkbox"]
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
        var playerCount = map.getPlayerCount();
        var variables = rule.getVariables();
        var noWatermines = VICTORYRULE_NOUNITS.getRuleValue(rule, 1, map);
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName = "Active" + playerID.toString();
            var variable = variables.createVariable(variableName);
            var unitCount = player.getUnitCount();
            if (noWatermines)
            {
                unitCount -= player.getUnitCount("WATERMINE");
            }

            if (unitCount > 0)
            {
                variable.writeDataBool(true);
            }
            else
            {
                variable.writeDataBool(false);
            }
        }
    };
    this.checkUnitCount = function(rule, player, map)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var value = variable.readDataBool();
        var noWatermines = VICTORYRULE_NOUNITS.getRuleValue(rule, 1, map);
        if (value === false)
        {
            var unitCount = player.getUnitCount();
            if (noWatermines)
            {
                unitCount -= player.getUnitCount("WATERMINE");
            }
            if (unitCount > 0)
            {
                variable.writeDataBool(true);
                value = true;
            }
        }
        return value;
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        if (VICTORYRULE_NOUNITS.checkUnitCount(rule, player, map))
        {
            var noWatermines = VICTORYRULE_NOUNITS.getRuleValue(rule, 1, map);
            var unitCount = player.getUnitCount();
            if (noWatermines)
            {
                unitCount -= player.getUnitCount("WATERMINE");
            }
            if (unitCount <= 0)
            {
                return GameEnums.DefeatType_Defeated;
            }
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map, item)
    {
        return player.getUnitCount();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_NOUNITS = new Constructor();
