var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var co = map.getCurrentPlayer().getCO(1);
        if ((co !== null) && co.canUsePower())
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        var text = qsTr("Power of ");
        if (map !== null &&
            map.getCurrentPlayer() !== null &&
            map.getCurrentPlayer().getCO(0) !== null)
        {
            text = text + map.getCurrentPlayer().getCO(1).getCOName();
        }
        return text;
    };
    this.getIcon = function(map)
    {
        return "power";
    };
    this.perform = function(action, map)
    {
        var player = map.getCurrentPlayer();
        var co = player.getCO(1);
        co.activatePower();
        map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                              GameEnums.GameRecord_SpecialEvents_Power);
        // achievements
        if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
        {
            ACHIEVEMENT_POWERS.powerUsed("POWER");
        }
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Activates the Power of your second CO. Read the CO's power for more information.");
    };
}


Constructor.prototype = ACTION;
var ACTION_ACTIVATE_POWER_CO_1 = new Constructor();
