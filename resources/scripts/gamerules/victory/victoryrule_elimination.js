var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("A player wins after killing the given amount of units by attacking or explosion damage from black bombs or Black Hole Buildings.")
        }
        else
        {
            return qsTr("If checked the Team needs to kill the given number of units else a single player needs to kill them.");
        }
    };
    this.getRuleName = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("Elimination");
        }
        else
        {
            return qsTr("Team Counter");
        }
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["spinbox", "checkbox"];
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        return 0;
    };
    this.getInfiniteValue = function(itemNumber)
    {
		return 0;
	};
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
    };
    this.getKilledUnits = function(rule, player)
    {
        var count = map.getGameRecorder().getDestroyedUnits(player.getPlayerID());
        var players = map.getPlayerCount();
        var teamVictory = VICTORYRULE_ELIMINATION.getRuleValue(rule, 1);
        if (teamVictory === 1)
        {
            for (var i = 0; i < players; i++)
            {
                var ally = map.getPlayer(i);
                if ((player !== ally) &&
                    (player.getTeam() === ally.getTeam()))
                {
                    count += map.getGameRecorder().getDestroyedUnits(i);
                }
            }
        }
        return count;
    };

    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        var count 		= VICTORYRULE_ELIMINATION.getKilledUnits(rule, player);
        var killLimit 	= VICTORYRULE_ELIMINATION.getRuleValue(rule, 0);
        if (count >= killLimit)
		{
            // defeat all other player
            var players = map.getPlayerCount();
            for (var i = 0; i < players; i++)
            {
                var defeatPlayer = map.getPlayer(i);
                if (player.getTeam() !== defeatPlayer.getTeam())
                {
                    defeatPlayer.setIsDefeated(true);
                }
            }
		}
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player)
    {
        return VICTORYRULE_ELIMINATION.getKilledUnits(rule, player);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_ELIMINATION = new Constructor();
