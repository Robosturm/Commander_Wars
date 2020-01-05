var VICTORYRULE =
{
    getRuleDescription : function(itemNumber)
    {
        return "";
    },

    getRuleProgress : function(rule, player)
    {
        return 0;
    },

    // shown name in the game
    getRuleName : function(itemNumber)
    {
        return "";
    },
    // the type how the rule will be represented in the map selection ui
    getRuleType : function()
    {
        // for now checkbox or spinbox
        return "checkbox";
    },
    // defines the default value during map selection for this rule
    getDefaultRuleValue : function(itemNumber)
    {
        return 0;
    },
    getInfiniteValue : function(itemNumber)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    },
    // create and initialize the variables for this rule
    init : function(rule)
    {
    },
    // sets the rule value of the spinbox
    setRuleValue : function(rule, value, item = 0)
    {

        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        variable.writeDataInt32(value);
    },

    getRuleValue : function(rule, item = 0)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        return variable.readDataInt32();
    },

    // checks if the selected player is declared defeated by this rule
    checkDefeat : function(rule, player)
    {
        return GameEnums.DefeatType_Alive;
    },
}
