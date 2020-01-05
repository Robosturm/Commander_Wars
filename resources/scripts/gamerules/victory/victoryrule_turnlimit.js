var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("All player loose when the time is up.");
        }
        else
        {
            return qsTr("The Team winning the game if the time is up. ∞ for all player loose the game.");
        }
    };
    this.getRuleName = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("Turn Limit");
        }
        else
        {
            return qsTr("Winner Team");
        }
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["spinbox", "spinbox"];
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        return 0;
    };
    this.getInfiniteValue = function(itemNumber)
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
        var victoryTeam = VICTORYRULE_TURNLIMIT.getRuleValue(rule, 1);
        if (turn <= turnLimit ||
            player.getTeam() === victoryTeam - 1)
		{
			return GameEnums.DefeatType_Alive;
		}
		// all players loose if the time limit is exceeded
		return GameEnums.DefeatType_Defeated;
    };
    this.getRuleProgress = function(rule, player)
    {
        return map.getCurrentDay();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_TURNLIMIT = new Constructor();
