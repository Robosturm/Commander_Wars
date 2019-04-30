var Constructor = function()
{
    this.getRuleName = function()
    {
        return qsTr("Turn Limit");
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return "spinbox"
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function()
    {
        return 0;
    };
	this.getInfiniteValue = function()
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    };
	
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        var turn = map.getCurrentDay();
		var turnLimit = VICTORYRULE_TURNLIMIT.getRuleValue(rule);
        if (turn <= turnLimit)
		{
			return GameEnums.DefeatType_Alive;
		}
		// all players loose if the time limit is exceeded
		return GameEnums.DefeatType_Defeated;
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_TURNLIMIT = new Constructor();
