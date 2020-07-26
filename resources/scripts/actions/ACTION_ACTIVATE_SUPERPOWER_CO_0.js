var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var co = map.getCurrentPlayer().getCO(0);
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
        var text = qsTr("Superpower of ");
        if (map !== null &&
            map.getCurrentPlayer() !== null &&
            map.getCurrentPlayer().getCO(0) !== null)
        {
            text = text + map.getCurrentPlayer().getCO(0).getCOName();
        }
        return text;
    };
    this.getIcon = function()
    {
        return "superpower";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Superpower);
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_SuperPower);
    };
    this.isFinalStep = function(action)
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Activates the superpower of your first CO. Read the CO's superpower for more information.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_SUPERPOWER_CO_0 = new Constructor();
