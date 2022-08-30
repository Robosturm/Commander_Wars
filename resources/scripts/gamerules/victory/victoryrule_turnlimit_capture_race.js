var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        var type = itemNumber;
        if (rule !== null)
        {
            var teamVictory = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getRuleValue(rule, 1, map);
            if (teamVictory)
            {
                type = 2;
            }
        }
        if (type === 0)
        {
            return qsTr("The player with the most captured buildings wins after the time is up.");
        }
        else if (type === 1)
        {
            return qsTr("If checked the Team with the most captured buildings wins.");
        }
        else
        {
            return qsTr("The team with the most captured buildings wins after the time is up.");
        }
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        var type = itemNumber;
        if (rule !== null)
        {
            var teamVictory = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getRuleValue(rule, 1, map);
            if (teamVictory)
            {
                type = 2;
            }
        }
        if (type === 0)
        {
            return qsTr("Capture race");
        }
        else if (type === 1)
        {
            return qsTr("Team Counter");
        }
        else
        {
            return qsTr("Team capture race");
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
        // disable value of the rule for spinboxes. :)
        return 0;
    };

    // create and initialize the variables for this rule
    this.init = function(rule)
    {
    };
    this.getWinnerTeam = function(rule, player, map)
    {
        var winnerTeam = -1;
        var maxBuildings = 0;
        var players = map.getPlayerCount();
        for (var i = 0; i < players; i++)
        {
            var curPlayer = map.getPlayer(i);
            if (!curPlayer.getIsDefeated())
            {
                var count = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getBuildingCount(rule, curPlayer);
                if (count > maxBuildings)
                {
                    winnerTeam = curPlayer.getTeam();
                    maxBuildings = count;
                }
                else if (count === maxBuildings)
                {
                    winnerTeam = -1;
                }
            }
        }
        return winnerTeam;
    };
    this.getBuildingCount = function(rule, player, map)
    {
        var count = player.getBuildingListCount(["TEMPORARY_AIRPORT", "TEMPORARY_HARBOUR"], false);
        var teamVictory = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getRuleValue(rule, 1, map);
        if (teamVictory === 1)
        {
            var players = map.getPlayerCount();
            for (var i = 0; i < players; i++)
            {
                var ally = map.getPlayer(i);
                if ((player !== ally) &&
                    !ally.getIsDefeated() &&
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
        var turn = map.getCurrentDay();
        var turnLimit = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getRuleValue(rule, 0, map);
        var victoryTeam = VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getWinnerTeam(rule, player, map);
        if (turn <= turnLimit ||
            player.getTeam() === victoryTeam)
        {
            return GameEnums.DefeatType_Alive;
        }
        // all players loose if the time limit is exceeded
        return GameEnums.DefeatType_Defeated;
    };
    this.getRuleProgress = function(rule, player, map)
    {
        return VICTORYRULE_TURNLIMIT_CAPTURE_RACE.getBuildingCount(rule, player, map);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_TURNLIMIT_CAPTURE_RACE = new Constructor();
