var Constructor = function()
{
    this.getRuleDescription = function()
    {
        return qsTr("A player wins after killing the given amount of units by attacking or explosion damage from black bombs or Black Hole Buildings.")
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
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        var count 		= map.getGameRecorder().getDestroyedUnits(player);
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
        return map.getGameRecorder().getDestroyedUnits(player);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_ELIMINATION = new Constructor();
