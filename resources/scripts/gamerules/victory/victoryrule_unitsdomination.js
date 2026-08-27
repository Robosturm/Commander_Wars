var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        return qsTr("A player loses when he has less than the given units after surpassing that amount in case he has less at the start of the match.")
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        if (itemNumber === 0)
        {
            return qsTr("Unit domination");
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
        return ["spinbox", "checkbox"]
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return 0;
        }
        else
        {
            return 0;
        }
    };
    this.getInfiniteValue = function(itemNumber)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
        var playerCount = map.getPlayerCount();
        var variables = rule.getVariables();
        var noWatermines = VICTORYRULE_UNITSDOMINATION.getRuleValue(rule, 1, map);
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
            variable.writeDataInt32(unitCount);
        }
    };
    this.checkUnitCount = function(rule, player, map)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var oldUnitCount = variable.readDataInt32();
        var noWatermines = VICTORYRULE_UNITSDOMINATION.getRuleValue(rule, 1, map);
        var targetUnitCount = VICTORYRULE_UNITSDOMINATION.getRuleValue(rule, 0, map);
        var newUnitCount = player.getUnitCount();
        if (noWatermines)
        {
            newUnitCount -= player.getUnitCount("WATERMINE");
        }
        var value = false
        if (oldUnitCount >= targetUnitCount && newUnitCount < targetUnitCount)
        {
            value = true;
        }
        return value;
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        if (VICTORYRULE_UNITSDOMINATION.checkUnitCount(rule, player, map))
        {
            return GameEnums.DefeatType_Defeated;
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map, item)
    {
        var unitCount = player.getUnitCount();
        var noWatermines = VICTORYRULE_UNITSDOMINATION.getRuleValue(rule, 1, map);
        if (noWatermines)
        {
            unitCount -= player.getUnitCount("WATERMINE");
        }
        return unitCount;
    };
    this.getRuleCategoryId = function(rule, map)
    {
        return "UNITS";
    };
    this.getRuleOrder = function(rule, map)
    {
        return 3;
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_UNITSDOMINATION = new Constructor();

