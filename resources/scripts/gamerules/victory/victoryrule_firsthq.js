var Constructor = function()
{
    this.getRuleDescription = function(rule, itemNumber, map)
    {
        return qsTr("A player looses when he looses his first HQ. All buildings will be owned by the current player.")
    };
    this.getRuleName = function(rule, itemNumber, map)
    {
        return qsTr("First HQ Loose");
    };
    // the type how the rule will be represented in the map selection ui
    this.getRuleType = function()
    {
        // for now checkbox or spinbox
        return "checkbox"
    };
    // defines the default value during map selection for this rule
    this.getDefaultRuleValue = function(itemNumber)
    {
        return 0;
    };
    // create and initialize the variables for this rule
    this.init = function(rule, map)
    {
        var playerCount = map.getPlayerCount();
        for (var i = 0; i < playerCount; i++)
        {
            var player = map.getPlayer(i);
            var playerID = player.getPlayerID();
            var variableName = "Active" + playerID.toString();
            var variables = rule.getVariables();
            var variable = variables.createVariable(variableName);
            var hqCount = player.getBuildingListCount(BUILDING.hqIds);
            variable.writeDataInt32(hqCount);
        }
    };
    this.checkHQCount = function(rule, player, map)
    {
        var playerID = player.getPlayerID();
        var variableName = "Active" + playerID.toString();
        var variables = rule.getVariables();
        var variable = variables.getVariable(variableName);
        var hqCount = variable.readDataInt32();
        var newHqCount = player.getBuildingListCount(BUILDING.hqIds);
        if (hqCount > 0 && newHqCount < hqCount)
        {
            return true;
        }
        else
        {
            variable.writeDataInt32(newHqCount);
            return false;
        }
    };
    // checks if the selected player is declared defeated by this rule
    this.checkDefeat = function(rule, player, map)
    {
        if (VICTORYRULE_FIRSTHQ.checkHQCount(rule, player, map))
        {
            return GameEnums.DefeatType_ByCurrentPlayer;
        }
        return GameEnums.DefeatType_Alive;
    };
    this.getRuleProgress = function(rule, player, map, item)
    {
        return player.getBuildingListCount(BUILDING.hqIds);
    };
};

Constructor.prototype = VICTORYRULE;
var VICTORYRULE_FIRSTHQ = new Constructor();
