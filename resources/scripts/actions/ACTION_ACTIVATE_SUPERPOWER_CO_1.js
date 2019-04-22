var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var co = map.getCurrentPlayer().getCO(1);
        if ((co !== null) && co.canUseSuperpower())
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Superpower of CO 2");
    };
    this.getIcon = function()
    {
        return "superpower";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(1).activateSuperpower();
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_SUPERPOWER_CO_1 = new Constructor();
