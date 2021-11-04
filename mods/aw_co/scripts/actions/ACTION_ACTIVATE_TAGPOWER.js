var Constructor = function()
{
    this.canBePerformed = function(action)
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
    this.getActionText = function()
    {
        return qsTr("Tagpower");
    };
    this.getIcon = function()
    {
        return "tagpower";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Tagpower);
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION_ACTIVATE_TAGPOWER;
var ACTION_ACTIVATE_TAGPOWER = new Constructor();
