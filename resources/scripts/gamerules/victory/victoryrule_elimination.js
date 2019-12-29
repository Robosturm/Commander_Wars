var Constructor = function()
{
    this.getRuleDescription = function()
    {
        return qsTr("A player wins after killing the given amount of units by attacking or explosion damage from black bombs or Black Hole Buildings. Units getting destroyed by counter attacks don't count.")
    };
    this.getRuleName = function()
    {
        return qsTr("Elimination");
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
		return 0;
	};
    // create and initialize the variables for this rule
    this.init = function(rule)
    {
        var playerCount = map.getPlayerCount();
        var variables = rule.getVariables();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName1 = "OwnedUnits" + playerID.toString();
			var variableName2 = "KilledUnits" + playerID.toString();            
            var variable1 = variables.createVariable(variableName1);
			var variable2 = variables.createVariable(variableName2);
            var count = player.getUnitCount();
            variable1.writeDataInt32(count);
            variable2.writeDataInt32(0);
        }
    };
    this.checkKilledUnitCount = function(rule, player)
    {
        // setup read curremt progress
        var currentPlayer = map.getCurrentPlayer();
        var currentPlayerID =  player.getPlayerID();
        var variableName2 = "KilledUnits" + currentPlayerID;
        var playerCount = map.getPlayerCount();
        var variables = rule.getVariables();
        var ret = variables.createVariable(variableName2);
        // only update data for the current player
        if (player === currentPlayer)
        {
            var killed = ret.readDataInt32();
            // loop through players and find out if units got killed by the last action
            for (var i = 0; i < playerCount; i++)
            {
                var updatePlayer = map.getPlayer(i);
                var playerID = updatePlayer.getPlayerID();
                var variableName1 = "OwnedUnits" + playerID.toString();
                var variable1 = variables.createVariable(variableName1);
                var count = updatePlayer.getUnitCount();
                // enemy player?
                if (player.getTeam() !== updatePlayer.getTeam())
                {
                    // last detect unit count is smaller?
                    var value = variable1.readDataInt32();
                    if (value > count)
                    {
                        killed += (value - count);
                    }
                }
                // store current count
                variable1.writeDataInt32(count);
            }
            ret.writeDataInt32(killed);
        }
        return ret.readDataInt32();
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
		var count 		= VICTORYRULE_ELIMINATION.checkKilledUnitCount(rule, player);
		var killLimit 	= VICTORYRULE_ELIMINATION.getRuleValue(rule);
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
		var playerID = player.getPlayerID();
		var variableName2 = "KilledUnits" + playerID.toString();
		var variables = rule.getVariables();
		var killedUnits = variables.createVariable(variableName2);
        return killedUnits.readDataInt32();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_ELIMINATION = new Constructor();
