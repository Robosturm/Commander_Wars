var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        if (itemNumber === 0)
        {
            var type = itemNumber;
            if (rule !== null)
            {
                var teamVictory = VICTORYRULE_DOMINATION.getRuleValue(rule, 0, map);
                if (teamVictory)
                {
                    type = 1;
                }
            }
            var gameDay = 0;
            if (map !== null)
            {
                gameDay = map.getCurrentDay();
            }
            if (gameDay > 0)
            {
                var unitValue = VICTORYRULE_DOMINATION.getRuleValue(rule, 1, map);
                var unitCount = VICTORYRULE_DOMINATION.getRuleValue(rule, 2, map);
                var income = VICTORYRULE_DOMINATION.getRuleValue(rule, 3, map);
                var text;
                if (type === 0)
                {
                    text = qsTr("A player looses if after the start day he has %0% of unit value, %1% unit count and %2% income less than the second weakest player");
                }
                else
                {
                    text = qsTr("A team looses if after the start day it has %0% of unit value, %1% unit count and %2% income less than the second weakest team");
                }
                text = replaceTextArgs(text, [unitValue, unitCount, income]);
                return text;
            }
            else
            {
                if (type === 0)
                {
                    return qsTr("A player looses if after the start day he has a percentage of unit value, unit count and income less than the second weakest player");
                }
                else
                {
                    return qsTr("A team looses if after the start day it has a percentage of unit value, unit count and income less than the second weakest team");
                }
            }
        }
        else if (itemNumber === 1)
        {
            return qsTr("The percentage the unit value needs to be below of that of all other players");
        }
        else if (itemNumber === 2)
        {
            return qsTr("The percentage the unit count needs to be below of that of all other players");
        }
        else if (itemNumber === 3)
        {
            return qsTr("The percentage the unit count needs to be below of that of all other players");
        }
        else if (itemNumber === 4)
        {
            return qsTr("Team if the values are compared between teams and a team looses if all values are below that of all other players.");
        }
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        if (itemNumber === 0)
        {
            return qsTr("Domination day");
        }
        else if (itemNumber === 1)
        {
            return qsTr("Unit value");
        }
        else if (itemNumber === 2)
        {
            return qsTr("Unit Count");
        }
        else if (itemNumber === 3)
        {
            return qsTr("Income");
        }
        else if (itemNumber === 4)
        {
            return qsTr("Team");
        }
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return ["spinbox", "spinbox", "spinbox", "spinbox", "checkbox"];
    };
    this.getInfiniteValue = function(itemNumber)
    {
        // disable value of the rule for spinboxes. :)
        return 0;
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        if (itemNumber === 0)
        {
            return 0;
        }
        else if (itemNumber === 1 ||
                 itemNumber === 2 ||
                 itemNumber === 3)
        {
            return 60;
        }
        else
        {
            return 0;
        }
    };
    this.getMaxValue = function(itemNumber)
    {
        return 100;
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
    };
    this.getRuleTargetValue = function(rule, map)
    {
        return 0;
    };
    this.getCurrentValues = function(player, map, team)
    {
        var unitValue = player.calculatePlayerStrength();
        var unitCount = player.getUnitCount();
        var income = player.calcIncome();

        if (team !== 0)
        {
            for (var i = 0; i < players; i++)
            {
                var ally = map.getPlayer(i);
                if ((player !== ally) &&
                    !ally.getIsDefeated() &&
                    (player.getTeam() === ally.getTeam()))
                {
                    unitValue = unitValue + player.calculatePlayerStrength();
                    unitCount = unitCount + player.getUnitCount();
                    income = income + player.calcIncome();
                }
            }
        }
        return Qt.vector3d(unitValue, unitCount, income);
    };

    this.getAliveCount = function(rule, player, map)
    {
        var startDay = VICTORYRULE_DOMINATION.getRuleValue(rule, 0, map);
        var currentDay = map.getCurrentDay();
        var unitValue = VICTORYRULE_DOMINATION.getRuleValue(rule, 1, map);
        var unitCount = VICTORYRULE_DOMINATION.getRuleValue(rule, 2, map);
        var income = VICTORYRULE_DOMINATION.getRuleValue(rule, 3, map);
        var team = VICTORYRULE_DOMINATION.getRuleValue(rule, 4, map);
        if (currentDay >= startDay)
        {
            var unitValueAlive = false;
            var unitCountAlive = false;
            var incomeAlive = false;

            var ownValues = VICTORYRULE_DOMINATION.getCurrentValues(player, map, team);
            var players = map.getPlayerCount();
            for (var i = 0; i < players; i++)
            {
                var enemy = map.getPlayer(i);
                if ((player !== enemy) &&
                    !enemy.getIsDefeated() &&
                    (player.getTeam() !== enemy.getTeam()))
                {
                    var enemyValues = VICTORYRULE_DOMINATION.getCurrentValues(player, map, team);

                    if (ownValues.x >= enemyValues.x * unitValue / 100)
                    {
                        unitValueAlive = true;
                    }
                    if (ownValues.y >= enemyValues.y * income / 100)
                    {
                        unitCountAlive = true;
                    }
                    if (ownValues.z >= enemyValues.z * unitValue / 100)
                    {
                        incomeAlive = true;
                    }
                }
            }
            var aliveCount = 0;
            if (unitValueAlive)
            {
                aliveCount = aliveCount + 1;
            }
            if (unitCountAlive)
            {
                aliveCount = aliveCount + 1;
            }
            if (incomeAlive)
            {
                aliveCount = aliveCount + 1;
            }
            return aliveCount;
        }
        else
        {
            return 3;
        }
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        var aliveCount = VICTORYRULE_DOMINATION.getAliveCount(rule, player, map);
        if (aliveCount <= 0)
        {
            return GameEnums.DefeatType_Alive;
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map)
    {
        var progress = VICTORYRULE_DOMINATION.getAliveCount(rule, player, map);
        return progress;
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_DOMINATION = new Constructor();
