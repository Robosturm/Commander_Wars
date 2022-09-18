var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        var type = itemNumber;
        if (rule !== null)
        {
            var teamVictory = VICTORYRULE_ELIMINATION.getRuleValue(rule, 1, map);
            if (teamVictory)
            {
                type = 2;
            }
        }
        if (type === 0)
        {
            return qsTr("A player wins after killing the given amount of units by attacking or explosion damage from black bombs or Black Hole Buildings.")
        }
        else if (type === 1)
        {
            return qsTr("If checked the Team needs to kill the given number of units else a single player needs to kill them.");
        }
        else
        {
            return qsTr("A team wins after killing the given amount of units by attacking or explosion damage from black bombs or Black Hole Buildings.")
        }
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        var type = itemNumber;
        if (rule !== null)
        {
            var teamVictory = VICTORYRULE_ELIMINATION.getRuleValue(rule, 1, map);
            if (teamVictory)
            {
                type = 2;
            }
        }
        if (type === 0)
        {
            return qsTr("Elimination");
        }
        else if (type === 1)
        {
            return qsTr("Team Counter");
        }
        else
        {
            return qsTr("Team elimination");
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
    this.getKilledUnits = function(rule, player, map)
    {
        var count = map.getGameRecorder().getDestroyedUnits(player.getPlayerID());
        var players = map.getPlayerCount();
        var teamVictory = VICTORYRULE_ELIMINATION.getRuleValue(rule, 1, map);
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
    this.checkDefeat = function(rule, player, map)
    {
        var count 		= VICTORYRULE_ELIMINATION.getKilledUnits(rule, player, map);
        var killLimit 	= VICTORYRULE_ELIMINATION.getRuleValue(rule, 0, map);
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
    this.getRuleProgress = function(rule, player, map)
    {
        return VICTORYRULE_ELIMINATION.getKilledUnits(rule, player, map);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_ELIMINATION = new Constructor();
