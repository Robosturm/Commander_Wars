var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if (co0 !== null && co1 !== null)
        {
            if   (co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
                  co1.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return false;
            }
            return true;
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
            map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Tagpower);
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
}


Constructor.prototype = ACTION_SWAP_COS;
var ACTION_SWAP_COS = new Constructor();
