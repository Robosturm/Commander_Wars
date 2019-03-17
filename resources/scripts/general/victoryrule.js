var VICTORYRULE =
{
    // the type how the rule will be represented in the map selection ui
    getRuleType : function()
    {
        // for now checkbox or spinbox
        return "checkbox"
    },
    // defines the default value during map selection for this rule
    getDefaultRuleValue : function()
    {
        return false;
    },
    // create and initialize the variables for this rule
    init : function(rule)
    {
    },
    // checks if the selected player is declared defeated by this rule
    checkDefeat : function(rule, player)
    {
        return GameEnums.DefeatType_Alive;
    },
}
