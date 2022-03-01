var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("The player with the highest combined tech and power score wins after the time is up.");
        }
        else
        {
            return qsTr("If checked the Team with the highest average score wins.");
        }
    };
    this.getRuleName = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return qsTr("Score race");
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
        var maxScore = 0;
        var players = map.getPlayerCount();
        for (var i = 0; i < players; i++)
        {
            var curPlayer = map.getPlayer(i);
            if (!curPlayer.getIsDefeated())
            {
                var count = VICTORYRULE_TURNLIMIT_SCORE_RACE.getScore(rule, curPlayer, map);
                if (count > maxScore)
                {
                    winnerTeam = curPlayer.getTeam();
                    maxScore = count;
                }
                else if (count === maxScore)
                {
                    winnerTeam = -1;
                }
            }
        }
        return winnerTeam;
    };
    this.getScore = function(rule, player, map)
    {
        var recorder = map.getGameRecorder();
        var score = recorder.getScoreValues(player.getPlayerID());
        var count = score.y + score.width;
        var teamVictory = VICTORYRULE_TURNLIMIT_SCORE_RACE.getRuleValue(rule, 1, map);
        var teamCount = 1;
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
                    score = recorder.getScoreValues(ally.getPlayerID());
                    count += score.y + score.width;
                    teamCount += 1;
                }
            }
        }
        return count / teamCount;
    };

    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        var turn = map.getCurrentDay();
        var turnLimit = VICTORYRULE_TURNLIMIT_SCORE_RACE.getRuleValue(rule, 0, map);
        var victoryTeam = VICTORYRULE_TURNLIMIT_SCORE_RACE.getWinnerTeam(rule, player, map);
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
        return VICTORYRULE_TURNLIMIT_SCORE_RACE.getScore(rule, player, map);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_TURNLIMIT_SCORE_RACE = new Constructor();
