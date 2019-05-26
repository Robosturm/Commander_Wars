var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        // disabled for normal gameplay :)
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


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_TAGPOWER = new Constructor();
