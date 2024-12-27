var GAMERULE =
{
    getRuleDescription : function(itemNumber)
    {
        return "";
    },
    getRuleDescription : null,

    // shown name in the game
    getRuleName : function(itemNumber)
    {
        return "";
    },
    getRuleName : null,

    // the type how the rule will be represented in the map selection ui
    getRuleType : function()
    {
        // for now checkbox or spinbox
        return ["checkbox"];
    },
    // defines the default value during map selection for this rule
    getDefaultRuleValue : function(itemNumber)
    {
        return 0;
    },
    getDefaultRuleValue : null,
    getInfiniteValue : function(itemNumber)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    },
    getInfiniteValue : null,
	// create and initialize the variables for this rule
    init : function(rule)
    {
    },
    init : null,
	// called from the engine when the player changes the value in the selection screen
	setRuleValue : function(rule, value, item = 0)
    {

        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        variable.writeDataInt32(value);
    },
	// called from the engine to determine the current start value in the selection screen
    getRuleValue : function(rule, item = 0)
    {
        var variableName = "SpinboxValue" + item.toString();
        var variables = rule.getVariables();
        var variable = variables.createVariable(variableName);
        return variable.readDataInt32();
    },
}
