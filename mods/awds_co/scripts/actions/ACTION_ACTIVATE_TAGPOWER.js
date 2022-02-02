var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if ((co0 !== null) && co0.canUseSuperpower() &&
            (co1 !== null) && co1.canUseSuperpower())
        {
            return true;
        }
        return false;
    };
    this.getActionText = function(map)
    {
        return qsTr("Tagpower");
    };
    this.getIcon = function(map)
    {
        return "tagpower";
    };
    this.perform = function(action, map)
    {
        map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Tagpower);
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
}


Constructor.prototype = ACTION_ACTIVATE_TAGPOWER;
var ACTION_ACTIVATE_TAGPOWER = new Constructor();
