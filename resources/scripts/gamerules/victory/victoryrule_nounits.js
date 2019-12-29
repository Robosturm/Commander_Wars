var Constructor = function()
{
    this.getRuleDescription = function()
    {
        return qsTr("A player looses when he looses his last unit.")
    };
    this.getRuleName = function()
    {
        return qsTr("No Units");
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return "checkbox"
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function()
    {
        return 1;
    };
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
        var playerCount = map.getPlayerCount();
        var variables = rule.getVariables();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName = "Active" + playerID.toString();
            var variable = variables.createVariable(variableName);
            if (player.getUnitCount() > 0)
            {
                variable.writeDataBool(true);
            }
            else
            {
                variable.writeDataBool(false);
            }
        }
    };
    this.checkUnitCount = function(rule, player)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var value = variable.readDataBool();
        if (value === false)
        {
            if (player.getUnitCount() > 0)
            {
                variable.writeDataBool(true);
                value = true;
            }
        }
        return value;
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        if (VICTORYRULE_NOUNITS.checkUnitCount(rule, player))
        {
            if (player.getUnitCount() <= 0)
            {
                return GameEnums.DefeatType_Defeated;
            }
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player)
    {
        return player.getUnitCount();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_NOUNITS = new Constructor();
