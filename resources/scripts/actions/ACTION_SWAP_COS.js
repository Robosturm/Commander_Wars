var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        if (!map.getGameRules().getParallelCos())
        {
            var player = map.getCurrentPlayer();
            var co0 = player.getCO(0);
            var co1 = player.getCO(1);
            if (co0 !== null && co1 !== null)
            {
                if (co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
                    co1.getPowerMode() === GameEnums.PowerMode_Tagpower)
                {
                    return false;
                }
                return true;
            }
        }
        return false;
    };
    this.getActionText = function(map)
    {
        return qsTr("Swap CO's");
    };
    this.getIcon = function(map)
    {
        return "swap";
    };
    this.perform = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if (co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
            co1.getPowerMode() === GameEnums.PowerMode_Off)
        {
            // start another movement turn by reenabling all units :)
            player.swapCOs();
            map.enableUnits(player);
        }
        else
        {
            player.swapCOs();
            map.nextTurn();
        }
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getName = function(map)
    {
        return qsTr("Swap CO's");
    };
    this.getDescription = function()
    {
        return qsTr("Swaps the position of CO 1 and 2 and ends your turn.");
    };
}


Constructor.prototype = ACTION;
var ACTION_SWAP_COS = new Constructor();
