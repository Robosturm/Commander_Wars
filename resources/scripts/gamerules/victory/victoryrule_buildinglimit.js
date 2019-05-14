var Constructor = function()
{
    this.getRuleDescription = function()
    {
        return qsTr("The player who reaches the number of buildings wins.")
    };

    this.getRuleName = function()
    {
        return qsTr("Capture Victory");
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
        return VICTORYRULE_BUILDINGLIMIT.getMinimumCount();
    };
	this.getInfiniteValue = function()
    {
        // disable value of the rule for spinboxes. :)
        return VICTORYRULE_BUILDINGLIMIT.getMinimumCount();
    };

    this.getMinimumCount = function()
    {
        var players = map.getPlayerCount();
        var buildingCount = 0;
        for (var i = 0; i < players; i++)
        {
            var count = map.getPlayer(i).getBuildingCount();
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
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player)
    {
        var buildings = player.getBuildingCount();
        var buildingLimit = VICTORYRULE_BUILDINGLIMIT.getRuleValue(rule);
        if (buildings >= buildingLimit)
		{
            // defeat all other player
            var players = map.getPlayerCount();
            var buildingCount = 0;
            for (var i = 0; i < players; i++)
            {
                map.getPlayer(i).setIsDefeated(true);
            }
		}
		// all players loose if the time limit is exceeded
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(player)
    {
        return player.getBuildingCount();
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_BUILDINGLIMIT = new Constructor();
