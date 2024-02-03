var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        var type = itemNumber;
        if (rule !== null)
        {
            var teamVictory = VICTORYRULE_BUILDINGLIMIT.getRuleValue(rule, 1, map);
            if (teamVictory)
            {
                type = 2;
            }
        }
        if (type === 0)
        {
            return qsTr("The player who reaches the number of buildings wins.")
        }
        else if (type === 1)
        {
            return qsTr("If checked the Team needs to capture the given number of buildings else a single player needs to capture them.");
        }
        else
        {
            return qsTr("The team which reaches the number of buildings wins.")
        }
    };

    this.getRuleName = function(rule, itemNumber, map)
    {
        if (itemNumber === 0)
        {
            return qsTr("Capture victory");
        }
        else
        {
            return qsTr("Team counter");
        }
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["spinbox", "checkbox"];
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber, map)
    {
        if (itemNumber === 0)
        {
            return VICTORYRULE_BUILDINGLIMIT.getMinimumCount(map);
        }
        else
        {
            return 0;
        }
    };
    this.getInfiniteValue = function(itemNumber, map)
    {
        if (itemNumber === 0)
        {
            // disable value of the rule for spinboxes. :)
            return VICTORYRULE_BUILDINGLIMIT.getMinimumCount(map);
        }
        else
        {
            return 0;
        }
    };

    this.getMinimumCount = function(map)
    {
        var players = map.getPlayerCount();
        var buildingCount = 0;
        for (var i = 0; i < players; i++)
        {
            var count = map.getPlayer(i).getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false);
            if (count > buildingCount)
            {
                buildingCount = count;
            }
        }
        return buildingCount;
    };
	
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
    };

    this.getBuildings = function(rule, player, map)
    {
        var count = player.getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false);
        var players = map.getPlayerCount();
        var teamVictory = VICTORYRULE_BUILDINGLIMIT.getRuleValue(rule, 1, map);
        if (teamVictory === 1)
        {
            for (var i = 0; i < players; i++)
            {
                var ally = map.getPlayer(i);
                if ((player !== ally) &&
                    (player.getTeam() === ally.getTeam()))
                {
                    count += ally.getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false);
                }
            }
        }
        return count;
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        var buildings = VICTORYRULE_BUILDINGLIMIT.getBuildings(rule, player, map);
        var buildingLimit = VICTORYRULE_BUILDINGLIMIT.getRuleValue(rule, 0, map);
        if (buildings >= buildingLimit)
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
    this.getRuleProgress = function(rule, player, map, item)
    {
        return VICTORYRULE_BUILDINGLIMIT.getBuildings(rule, player, map);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_BUILDINGLIMIT = new Constructor();
