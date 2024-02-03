var VICTORYRULE =
{
    getRuleDescription : function(itemNumber, map)
    {
        return "";
    },

    getRuleProgress : function(rule, player, map, item)
    {
        return 0;
    },

    // shown name in the game
    getRuleName : function(itemNumber, map)
    {
        return "";
    },
    // the type how the rule will be represented in the map selection ui
    getRuleType : function()
    {
        // for now checkbox or spinbox
        return ["checkbox"];
    },
    // defines the default value during map selection for this rule
    getDefaultRuleValue : function(itemNumber, map)
    {
        return 0;
    },
    getInfiniteValue : function(itemNumber, map)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    },
    getMaxValue : function(itemNumber)
    {
        return 99999;
    },
    getRuleTargetValue  : function(rule, map, itemNumber, player)
    {
        var ruleTypes = rule.getRuleType();
        if (ruleTypes[0] === "checkbox")
        {
            return 0;
        }
        else
        {
            return rule.getRuleValue(0);
        }
    },
    getRuleTargetCount : function(rule, map)
    {
        return 1;
    },
    // create and initialize the variables for this rule
    init : function(rule, map)
    {
    },
    // sets the rule value of the spinbox
    setRuleValue : function(rule, value, item = 0, map)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        variable.writeDataInt32(value);
    },
    // called from the engine when the player changes the value in the selection screen
    getRuleValue : function(rule, item = 0, map)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        return variable.readDataInt32();
    },

    // checks if the selected player is declared defeated by this rule
    checkDefeat : function(rule, player, map)
    {
        return GameEnums.DefeatType_Alive;
    },
}
