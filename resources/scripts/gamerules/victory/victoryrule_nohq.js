var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        return qsTr("A player loses when he loses his last HQ. All buildings will be owned by the current player.")
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        return qsTr("No HQ's");
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
        return 1;
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
        var playerCount = map.getPlayerCount();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName = "Active" + playerID.toString();
            var variables = rule.getVariables();
            var variable = variables.createVariable(variableName);
            if (player.getBuildingListCount(["HQ", "FORTHQ"]) > 0)
            {
                variable.writeDataBool(true);
            }
            else
            {
                variable.writeDataBool(false);
            }
        }
    };
    this.checkHQCount = function(rule, player, map)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var value = variable.readDataBool();
        if (value === false)
        {
            if (player.getBuildingListCount(["HQ", "FORTHQ"]) > 0)
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
        if (VICTORYRULE_NOHQ.checkHQCount(rule, player, map))
        {
            if (player.getBuildingListCount(["HQ", "FORTHQ"]) <= 0)
            {
                return GameEnums.DefeatType_ByCurrentPlayer;
            }
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map)
    {
        return player.getBuildingListCount(["HQ", "FORTHQ"]);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_NOHQ = new Constructor();
