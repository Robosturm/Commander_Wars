var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        if (rule !== null)
        {
            var victoryTeam = VICTORYRULE_TURNLIMIT.getRuleValue(rule, 1, map);
            if (victoryTeam > 0)
            {
                var text = qsTr("Team %0 wins when the time is up.");
                text = replaceTextArgs(text, [victoryTeam]);
                return text
            }
            else
            {
                return qsTr("All player lose when the time is up.");
            }
        }
        else
        {
            if (itemNumber === 0)
            {
                return qsTr("All player lose when the time is up.");
            }
            else
            {
                return qsTr("The Team winning the game if the time is up. Infinite for all player losing the game.");
            }
        }
    };
    this.getRuleName = function(rule, itemNumber, map)
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
    this.checkDefeat = function(rule, player, map)
    {
        var turn = map.getCurrentDay();
        var turnLimit = VICTORYRULE_TURNLIMIT.getRuleValue(rule, 0, map);
        var victoryTeam = VICTORYRULE_TURNLIMIT.getRuleValue(rule, 1, map);
        if (turn <= turnLimit ||
            player.getTeam() === victoryTeam - 1)
		{
			return GameEnums.DefeatType_Alive;
		}
		// all players loose if the time limit is exceeded
		return GameEnums.DefeatType_Defeated;
    };
    this.getRuleProgress = function(rule, player, map)
    {
        return map.getCurrentDay();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_TURNLIMIT = new Constructor();
